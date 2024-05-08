#include "pch.h"
#include "PatternView.h"
#if __has_include("PatternView.g.cpp")
#include "PatternView.g.cpp"
#endif

#include "Utils.h"
#include "D2DDevice.h"

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
PatternProps::PatternProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void PatternProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

PatternView::PatternView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args)
    : base_type(args) {}

void PatternView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto patternProps = props.try_as<PatternProps>();
  if (patternProps) {
    m_props = patternProps;

    m_patternUnits = Utils::JSValueAsBrushUnits(m_props->patternUnits);
    m_patternContentUnits = Utils::JSValueAsBrushUnits(m_props->patternContentUnits, "userSpaceOnUse");

    m_transform = Utils::JSValueAsD2DTransform(m_props->patternTransform);
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);

  SaveDefinition();

  if (auto const &root{SvgRoot()}) {
    root.Invalidate();
  }
}

void PatternView::UpdateBounds() {
  if (m_patternUnits == "objectBoundingBox") {
    com_ptr<ID2D1ImageBrush> brush{get_self<D2DBrush>(m_brush)->Get().as<ID2D1ImageBrush>()};

    D2D1_RECT_F rect{GetAdjustedRect(m_bounds)};
    CreateBrush(rect);
  }
}

void PatternView::CreateBrush() {
  auto const root{SvgRoot()};

  D2D1_RECT_F elRect{GetAdjustedRect({0, 0, root.ActualSize().Width, root.ActualSize().Height})};
  CreateBrush(elRect);
}

void PatternView::CreateBrush(D2D1_RECT_F rect) {
  auto const &root{SvgRoot()};

  com_ptr<ID2D1Device> device{get_self<D2DDevice>(root.Device())->Get()};

  if (auto const &cmdList{GetCommandList(device.get(), rect)}) {
    D2D1_IMAGE_BRUSH_PROPERTIES brushProperties{D2D1::ImageBrushProperties(rect)};
    brushProperties.extendModeX = D2D1_EXTEND_MODE_WRAP;
    brushProperties.extendModeY = D2D1_EXTEND_MODE_WRAP;

    com_ptr<ID2D1DeviceContext> deviceContext{get_self<D2DDeviceContext>(root.DeviceContext())->Get()};

    com_ptr<ID2D1ImageBrush> imageBrush;
    check_hresult(deviceContext->CreateImageBrush(cmdList.get(), brushProperties, imageBrush.put()));

    auto transform{D2D1::Matrix3x2F::Translation({rect.left, rect.top}) * m_transform};
    imageBrush->SetTransform(transform);

    m_brush = make<RNSVG::implementation::D2DBrush>(imageBrush.as<ID2D1Brush>());
  }
}

D2D1_RECT_F PatternView::GetAdjustedRect(D2D1_RECT_F bounds) {
  float width{D2DHelpers::WidthFromD2DRect(bounds)};
  float height{D2DHelpers::HeightFromD2DRect(bounds)};

  float x{Utils::GetAbsoluteLength(m_props->x, width) + bounds.left};
  float y{Utils::GetAbsoluteLength(m_props->y, height) + bounds.top};
  float adjWidth{Utils::GetAbsoluteLength(m_props->width, width)};
  float adjHeight{Utils::GetAbsoluteLength(m_props->height, height)};

  return {x, y, adjWidth + x, adjHeight + y};
}

com_ptr<ID2D1CommandList> PatternView::GetCommandList(ID2D1Device* device, D2D1_RECT_F rect) {
  com_ptr<ID2D1DeviceContext> deviceContext;
  check_hresult(device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, deviceContext.put()));

  com_ptr<ID2D1CommandList> cmdList;
  check_hresult(deviceContext->CreateCommandList(cmdList.put()));

  deviceContext->SetTarget(cmdList.get());

  deviceContext->BeginDraw();
  deviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));

  auto transform{D2D1::Matrix3x2F::Identity()};

  if (m_props->align != "") {
    Rect vbRect{
        m_props->minX,
        m_props->minY,
        (m_props->vbWidth + m_props->minX),
        (m_props->vbHeight + m_props->minY)};

    auto viewboxTransform{Utils::GetViewBoxTransform(
        vbRect,
        D2DHelpers::FromD2DRect(rect),
        m_props->align,
        Utils::GetMeetOrSlice(m_props->meetOrSlice))};

    transform = D2DHelpers::AsD2DTransform(viewboxTransform) * transform;
  }

  deviceContext->SetTransform(transform);

  auto context = make<D2DDeviceContext>(deviceContext);
  for (auto const &child : Children()) {
    child.as<IRenderable>().Draw(context, D2DHelpers::SizeFromD2DRect(rect));
  }

  cmdList->Close();

  deviceContext->EndDraw();

  return cmdList;
}

void PatternView::RegisterComponent(
    const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGPattern", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::PatternProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::PatternView>(args);
        });
      });
}

} // namespace winrt::RNSVG::implementation
