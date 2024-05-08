#pragma once

#include "SymbolProps.g.h"
#include "SymbolView.g.h"
#include "GroupView.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(SymbolProps)
struct SymbolProps : SymbolPropsT<SymbolProps, SvgRenderableCommonProps> {
  SymbolProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;

  REACT_FIELD(minX)
  float minX{0.0f};
  REACT_FIELD(minY)
  float minY{0.0f};
  REACT_FIELD(vbWidth)
  float vbWidth{0.0f};
  REACT_FIELD(vbHeight)
  float vbHeight{0.0f};
  REACT_FIELD(align)
  std::string align{""};
  REACT_FIELD(meetOrSlice)
  uint32_t meetOrSlice{0}; // RNSVG::MeetOrSlice::Meet
};

struct SymbolView : SymbolViewT<SymbolView, RNSVG::implementation::GroupView> {
 public:
  SymbolView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  float MinX() { return m_props->minX; }
  float MinY() { return m_props->minY; }
  float VbWidth() { return m_props->vbWidth; }
  float VbHeight() { return m_props->vbHeight; }
  hstring Align() { return to_hstring(m_props->align); }
  RNSVG::MeetOrSlice MeetOrSlice();

  // RenderableView
  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void Draw(RNSVG::D2DDeviceContext const & /*deviceContext*/, Windows::Foundation::Size const & /*size*/){};

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  com_ptr<SymbolProps> m_props;
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct SymbolView : SymbolViewT<SymbolView, implementation::SymbolView> {};
} // namespace winrt::RNSVG::factory_implementation
