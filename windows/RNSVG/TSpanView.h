#pragma once
#include "TSpanProps.g.h"
#include "TSpanView.g.h"
#include "TextView.h"

namespace winrt::RNSVG::implementation {
REACT_STRUCT(TSpanProps)
struct TSpanProps : TSpanPropsT<TSpanProps, SvgTextCommonProps> {
  TSpanProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;
  REACT_SVG_GROUP_COMMON_PROPS;
  REACT_SVG_TEXT_COMMON_PROPS;

  REACT_FIELD(content)
  std::string content{""};
};

struct TSpanView : TSpanViewT<TSpanView, RNSVG::implementation::TextView> {
public:
  TSpanView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  virtual void Draw(RNSVG::D2DDeviceContext const &deviceContext, Windows::Foundation::Size const &size);

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  com_ptr<TSpanProps> m_props;
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct TSpanView : TSpanViewT<TSpanView, implementation::TSpanView> {};
} // namespace winrt::RNSVG::factory_implementation
