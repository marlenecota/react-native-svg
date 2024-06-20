#pragma once

#include "SvgView.g.h"

#include "SvgViewProps.g.h"

#include <JSValueComposition.h>
#include "NativeModules.h"
#include "SVGLength.h"
#include "Utils.h"

namespace winrt::RNSVG::implementation {
REACT_STRUCT(SvgViewProps)
struct SvgViewProps : SvgViewPropsT<SvgViewProps> {
  SvgViewProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept;

  REACT_FIELD(bbWidth)
  winrt::RNSVG::SVGLength bbWidth{0.0, winrt::RNSVG::LengthType::Unknown};

  REACT_FIELD(bbHeight)
  winrt::RNSVG::SVGLength bbHeight{0.0, winrt::RNSVG::LengthType::Unknown};

  REACT_FIELD(minX)
  float minX;
  REACT_FIELD(minY)
  float minY;
  REACT_FIELD(vbWidth)
  float vbWidth;
  REACT_FIELD(vbHeight)
  float vbHeight;
  REACT_FIELD(align)
  std::string align;
  REACT_FIELD(meetOrSlice)
  uint32_t meetOrSlice;
  REACT_FIELD(tintColor)
  winrt::Microsoft::ReactNative::Color tintColor{nullptr};
  REACT_FIELD(color)
  winrt::Microsoft::ReactNative::Color color{nullptr};

 private:
  winrt::Microsoft::ReactNative::ViewProps m_props{nullptr};
};

struct SvgView : SvgViewT<SvgView> {
 public:
  SvgView(const winrt::Microsoft::ReactNative::Composition::CreateCompositionComponentViewArgs &args);

  RNSVG::GroupView Group() { return m_group; }
  void Group(RNSVG::GroupView const &value) { m_group = value; }

  winrt::Windows::Foundation::Size ActualSize() noexcept;

  RNSVG::D2DDevice Device() { return m_device; }

  RNSVG::D2DDeviceContext DeviceContext() { return m_deviceContext; }

  RNSVG::D2DGeometry Geometry() { return m_group ? m_group.Geometry() : nullptr; }
  void Geometry(RNSVG::D2DGeometry const & /*value*/) {}

  winrt::Microsoft::ReactNative::Color CurrentColor() { return m_currentColor; }

  bool IsResponsible() { return m_isResponsible; }
  void IsResponsible(bool isResponsible) { m_isResponsible = isResponsible; }

  Windows::Foundation::Collections::IMap<hstring, RNSVG::IRenderable> Templates() {
    return m_templates;
  }
  Windows::Foundation::Collections::IMap<hstring, RNSVG::IBrushView> Brushes() {
    return m_brushes;
  }

  // IRenderable
  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true);
  void MergeProperties(RNSVG::IRenderable const &other);
  void SaveDefinition();
  void Unload();
  void Draw(RNSVG::D2DDeviceContext const &deviceContext, Windows::Foundation::Size const &size);
  void CreateResources();
  void CreateGeometry(RNSVG::D2DDeviceContext const &deviceContext);
  RNSVG::IRenderable HitTest(Windows::Foundation::Point const & /*point*/) { return nullptr; }
  
  // Overrides
  // IInternalCreateVisual
  winrt::Microsoft::ReactNative::Composition::Experimental::IVisual CreateInternalVisual();

  // ComponentView
  void UpdateProps(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps) noexcept;
  void UpdateLayoutMetrics(
      const winrt::Microsoft::ReactNative::LayoutMetrics &metrics,
      const winrt::Microsoft::ReactNative::LayoutMetrics &oldMetrics);
  void MountChildComponentView(
      const winrt::Microsoft::ReactNative::ComponentView &childComponentView,
      uint32_t index) noexcept;
  void UnmountChildComponentView(
      const winrt::Microsoft::ReactNative::ComponentView &childComponentView,
      uint32_t index) noexcept;
  void OnThemeChanged() noexcept;

  void Invalidate();

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  bool m_hasRendered{false};
  bool m_isResponsible{false};
  Microsoft::ReactNative::IReactContext m_reactContext{nullptr};
  RNSVG::D2DDevice m_device;
  RNSVG::D2DDeviceContext m_deviceContext;
  RNSVG::GroupView m_group{nullptr};
  hstring m_id{L""};
  float m_minX{0.0f};
  float m_minY{0.0f};
  float m_vbWidth{0.0f};
  float m_vbHeight{0.0f};
  RNSVG::SVGLength m_bbWidth{0, RNSVG::LengthType::Unknown};
  RNSVG::SVGLength m_bbHeight{0, RNSVG::LengthType::Unknown};
  RNSVG::SVGLength m_width{0, RNSVG::LengthType::Unknown};
  RNSVG::SVGLength m_height{0, RNSVG::LengthType::Unknown};
  std::string m_align{""};
  winrt::Microsoft::ReactNative::Composition::Experimental::ISpriteVisual m_visual{nullptr};
  RNSVG::MeetOrSlice m_meetOrSlice{RNSVG::MeetOrSlice::Meet};
  winrt::Microsoft::ReactNative::Color m_currentColor{nullptr};
  winrt::Microsoft::ReactNative::LayoutMetrics m_layoutMetrics{{0, 0, 0, 0}, 1.0};
  winrt::Microsoft::ReactNative::Composition::Experimental::ICompositionContext m_compContext{nullptr};
  winrt::Microsoft::UI::Composition::Compositor m_compositor{nullptr};

  Windows::Foundation::Collections::IMap<hstring, RNSVG::IRenderable> m_templates{
      winrt::single_threaded_map<hstring, RNSVG::IRenderable>()};
  Windows::Foundation::Collections::IMap<hstring, RNSVG::IBrushView> m_brushes{
      winrt::single_threaded_map<hstring, RNSVG::IBrushView>()};
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct SvgView : SvgViewT<SvgView, implementation::SvgView> {};
} // namespace winrt::RNSVG::factory_implementation