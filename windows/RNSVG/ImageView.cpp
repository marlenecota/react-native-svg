#include "pch.h"
#include "ImageView.h"
#if __has_include("ImageView.g.cpp")
#include "ImageView.g.cpp"
#endif

#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Web.Http.h>

#include "Utils.h"

#include <d2d1effects.h>
#include <shcore.h>

using namespace winrt::Microsoft::ReactNative;
using namespace winrt::Windows::Security::Cryptography;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Web::Http;

namespace winrt::RNSVG::implementation {
ImageProps::ImageProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void ImageProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

ImageView::ImageView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) : base_type(args) {}

void ImageView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto imageProps = props.try_as<ImageProps>();
  auto oldImageProps = oldProps.try_as<ImageProps>();
  if (imageProps) {
    m_props = imageProps;

    if (!oldImageProps || (oldImageProps->src.uri != imageProps->src.uri)) {
      LoadImageSourceAsync(true);
    }
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);
}

void ImageView::Draw(RNSVG::D2DDeviceContext const &context, Size const &size) {
  if (!m_wicbitmap) {
    return;
  }

  com_ptr<ID2D1DeviceContext> deviceContext{get_self<D2DDeviceContext>(context)->Get()};

  uint32_t imgWidth = 0;
  uint32_t imgHeight = 0;
  check_hresult(m_wicbitmap->GetSize(&imgWidth, &imgHeight));

  m_props->src.width = static_cast<float>(imgWidth);
  m_props->src.height = static_cast<float>(imgHeight);

  com_ptr<ID2D1Bitmap1> bitmap;
  check_hresult(deviceContext->CreateBitmapFromWicBitmap(m_wicbitmap.get(), nullptr, bitmap.put()));

  if (m_props->src.width == 0 || m_props->src.height == 0) {
    m_props->src.width = size.Width;
    m_props->src.height = size.Height;
  }

  float x{Utils::GetAbsoluteLength(m_props->x, size.Width)};
  float y{Utils::GetAbsoluteLength(m_props->y, size.Height)};
  float width{Utils::GetAbsoluteLength(m_props->width, size.Width)};
  float height{Utils::GetAbsoluteLength(m_props->height, size.Height)};

  if (width == 0) {
    width = m_props->src.width * m_props->src.scale;
  }

  if (height == 0) {
    height = m_props->src.height * m_props->src.scale;
  }

  com_ptr<ID2D1Geometry> clipPathGeometry;
  if (ClipPathGeometry(context)) {
    clipPathGeometry = get_self<D2DGeometry>(ClipPathGeometry(context))->Get();
  }

  D2DHelpers::PushOpacityLayer(deviceContext.get(), clipPathGeometry.get(), m_opacity);

  if (m_format == ImageSourceFormat::Bitmap) {
    D2D1_MATRIX_3X2_F transform{D2DHelpers::GetTransform(deviceContext.get())};

    if (m_propSetMap[RNSVG::BaseProp::Matrix]) {
      deviceContext->SetTransform(transform * D2DHelpers::AsD2DTransform(SvgTransform()));
    }

    if (m_align != "") {
      com_ptr<ID2D1Effect> bitmapEffects;
      check_hresult(deviceContext->CreateEffect(CLSID_D2D1BitmapSource, bitmapEffects.put()));
      check_hresult(bitmapEffects->SetValue(D2D1_BITMAPSOURCE_PROP_WIC_BITMAP_SOURCE, m_wicbitmap.get()));

      com_ptr<ID2D1Effect> transformEffect;
      Rect elRect{x, y, width, height};
      Rect vbRect{0, 0, m_props->src.width, m_props->src.height};
      deviceContext->CreateEffect(CLSID_D2D12DAffineTransform, transformEffect.put());
      transformEffect->SetValue(
          D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX,
          Utils::GetViewBoxTransform(vbRect, elRect, m_align, m_meetOrSlice));
      transformEffect->SetInputEffect(0, bitmapEffects.get());

      com_ptr<ID2D1Effect> cropEffect;
      deviceContext->CreateEffect(CLSID_D2D1Crop, cropEffect.put());
      cropEffect->SetValue(D2D1_CROP_PROP_RECT, D2D1::RectF(x, y, width, height));
      cropEffect->SetInputEffect(0, transformEffect.get());

      deviceContext->DrawImage(cropEffect.get());
    } else {
      deviceContext->DrawBitmap(bitmap.get());
    }

    deviceContext->SetTransform(transform);
  }

  deviceContext->PopLayer();
}

void ImageView::CreateResources() {
  LoadImageSourceAsync(true);
}

void ImageView::Unload() {
  if (m_wicbitmap) {
    m_wicbitmap = nullptr;
  }
}

void ImageView::RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGImage", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::ImageProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::ImageView>(args);
        });
      });
}

IAsyncAction ImageView::LoadImageSourceAsync(bool invalidate) {
  Uri uri{winrt::to_hstring(m_props->src.uri)};
  hstring scheme{uri ? uri.SchemeName() : L""};
  hstring ext{uri ? uri.Extension() : L""};

  if (ext == L".svg" || ext == L".svgz") {
    m_format = ImageSourceFormat::Svg;
    co_return;
  }

  if (scheme == L"http" || scheme == L"https") {
    m_type = ImageSourceType::Download;
  } else if (scheme == L"data") {
    m_type = ImageSourceType::InlineData;
    if (m_props->src.uri.find("image/svg+xml") != std::string::npos) {
      m_format = ImageSourceFormat::Svg;
      co_return;
    }
  }

  const bool fromStream{m_type == ImageSourceType::Download || m_type == ImageSourceType::InlineData};

  InMemoryRandomAccessStream stream{nullptr};

  // get weak reference before any co_await calls
  auto weak_this{get_weak()};

  try {
    stream = co_await GetImageMemoryStreamAsync(m_props->src);

    if (fromStream && !stream) {
      co_return;
    }
  } catch (winrt::hresult_error const &) {
    co_return;
  }

  if (stream) {
    generateBitmap(stream);
  }

  if (invalidate) {
    if (auto strong_this{weak_this.get()}) {
      strong_this->SvgRoot().Invalidate();
    }
  }
}

IAsyncOperation<InMemoryRandomAccessStream> ImageView::GetImageMemoryStreamAsync(
    ImageSource source) {
  switch (m_type) {
    case ImageSourceType::Download:
      co_return co_await GetImageStreamAsync(source);
    case ImageSourceType::InlineData:
      co_return co_await GetImageInlineDataAsync(source);
    default: // ImageSourceType::Uri
      co_return nullptr;
  }
}

IAsyncOperation<InMemoryRandomAccessStream> ImageView::GetImageStreamAsync(
    ImageSource source) {
  try {
    co_await resume_background();

    auto httpMethod{source.method.empty() ? HttpMethod::Get() : HttpMethod{winrt::to_hstring(source.method)}};

    Uri uri{winrt::to_hstring(source.uri)};
    HttpRequestMessage request{httpMethod, uri};

    //if (!source.headers.empty()) {
    //  for (auto const &pair : source.headers) {
    //    if (_stricmp(header.first.c_str(), "authorization") == 0) {
    //      request.Headers().TryAppendWithoutValidation(winrt::to_hstring(header.first), winrt::to_hstring(header.second));
    //    } else {
    //      request.Headers().Append(winrt::to_hstring(header.first), winrt::to_hstring(header.second));
    //    }
    //  }
    //}

    HttpClient httpClient;
    HttpResponseMessage response{co_await httpClient.SendRequestAsync(request)};

    if (response && response.StatusCode() == HttpStatusCode::Ok) {
      IInputStream inputStream{co_await response.Content().ReadAsInputStreamAsync()};
      InMemoryRandomAccessStream memoryStream;
      co_await RandomAccessStream::CopyAsync(inputStream, memoryStream);
      memoryStream.Seek(0);

      co_return memoryStream;
    }
  } catch (hresult_error const &) {
  }

  co_return nullptr;
}

IAsyncOperation<InMemoryRandomAccessStream> ImageView::GetImageInlineDataAsync(
    ImageSource source) {
  std::string uri{source.uri};

  size_t start = uri.find(',');
  if (start == std::string::npos || start + 1 > uri.length()) {
    co_return nullptr;
  }

  try {
    co_await winrt::resume_background();

    std::string_view base64String{uri.c_str() + start + 1, uri.length() - start - 1};
    auto const &buffer{CryptographicBuffer::DecodeFromBase64String(to_hstring(base64String))};

    InMemoryRandomAccessStream memoryStream;
    co_await memoryStream.WriteAsync(buffer);
    memoryStream.Seek(0);

    co_return memoryStream;
  } catch (hresult_error const &) {
    // Base64 decode failed
  }

  co_return nullptr;
}

com_ptr<IWICBitmapSource> ImageView::wicBitmapSourceFromStream(InMemoryRandomAccessStream const &results) {
  if (!results) {
    return nullptr;
  }

  com_ptr<IWICImagingFactory> imagingFactory;
  check_hresult(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(imagingFactory.put())));

  com_ptr<IStream> istream;
  check_hresult(CreateStreamOverRandomAccessStream(results.as<::IUnknown>().get(), __uuidof(IStream), istream.put_void()));

  com_ptr<IWICBitmapDecoder> bitmapDecoder;
  if (imagingFactory->CreateDecoderFromStream(
          istream.get(), nullptr, WICDecodeMetadataCacheOnDemand, bitmapDecoder.put()) < 0) {
    return nullptr;
  }

  com_ptr<IWICBitmapFrameDecode> decodedFrame;
  check_hresult(bitmapDecoder->GetFrame(0, decodedFrame.put()));
  return decodedFrame;
}

void ImageView::generateBitmap(InMemoryRandomAccessStream const &results) {
  com_ptr<IWICBitmapSource> decodedFrame = wicBitmapSourceFromStream(results);

  if (!decodedFrame) {
    return;
  }

  com_ptr<IWICImagingFactory> imagingFactory;
  check_hresult(
      CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(imagingFactory.put())));
  com_ptr<IWICFormatConverter> converter;
  check_hresult(imagingFactory->CreateFormatConverter(converter.put()));

  check_hresult(converter->Initialize(
      decodedFrame.get(),
      GUID_WICPixelFormat32bppPBGRA,
      WICBitmapDitherTypeNone,
      nullptr,
      0.0f,
      WICBitmapPaletteTypeMedianCut));

  check_hresult(
      imagingFactory->CreateBitmapFromSource(converter.get(), WICBitmapCacheOnLoad, m_wicbitmap.put()));
}
} // namespace winrt::RNSVG::implementation
