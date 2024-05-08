#include "pch.h"
#include "TextView.h"
#if __has_include("TextView.g.cpp")
#include "TextView.g.cpp"
#endif

#include "D2DHelpers.h"

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
SvgTextCommonProps::SvgTextCommonProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void SvgTextCommonProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

TextView::TextView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) : base_type(args) {}

void TextView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto textProps = props.try_as<SvgTextCommonProps>();
  if (textProps) {
    m_props = textProps;

    if (textProps->x != std::nullopt) {
      m_x.Clear();

      for (auto const &item : *textProps->x) {
        m_x.Append(item);
      }
    }

    if (textProps->y != std::nullopt) {
      m_y.Clear();

      for (auto const &item : *textProps->y) {
        m_y.Append(item);
      }
    }

    if (textProps->dx != std::nullopt) {
      m_dx.Clear();

      for (auto const &item : *textProps->dx) {
        m_dx.Append(item);
      }
    }

    if (textProps->dy != std::nullopt) {
      m_dy.Clear();

      for (auto const &item : *textProps->dy) {
        m_dy.Append(item);
      }
    }

    if (textProps->rotate != std::nullopt) {
      m_rotate.Clear();

      for (auto const &item : *textProps->rotate) {
        m_rotate.Append(item);
      }
    }
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);

  SaveDefinition();
}

void TextView::DrawGroup(RNSVG::D2DDeviceContext const &context, Size const &size) {
  com_ptr<ID2D1DeviceContext> deviceContext{get_self<D2DDeviceContext>(context)->Get()};

  D2D1_MATRIX_3X2_F transform{D2DHelpers::GetTransform(deviceContext.get())};

  bool translateXY{X().Size() > 0 || Y().Size() > 0};
  if (translateXY) {
    float x{X().Size() > 0 ? X().GetAt(0).Value : 0};
    float y{Y().Size() > 0 ? Y().GetAt(0).Value : 0};
    deviceContext->SetTransform(D2D1::Matrix3x2F::Translation({x,y}) * transform);
  }
  __super::DrawGroup(context, size);
  if (translateXY) {
    deviceContext->SetTransform(transform);
  }
}

void TextView::RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGText", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::SvgTextCommonProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::TextView>(args);
        });
      });
}
} // namespace winrt::RNSVG::implementation
