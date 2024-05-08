#pragma once
#include "PatternProps.g.h"
#include "PatternView.g.h"
#include "BrushView.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(PatternProps)
struct PatternProps : PatternPropsT<PatternProps, SvgGroupCommonProps> {
  PatternProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;

  REACT_FIELD(x)
  RNSVG::SVGLength x{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(y)
  RNSVG::SVGLength y{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(width)
  RNSVG::SVGLength width{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(height)
  RNSVG::SVGLength height{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(patternUnits)
  std::optional<int32_t> patternUnits;
  REACT_FIELD(patternContentUnits)
  std::optional<int32_t> patternContentUnits;
  REACT_FIELD(patternTransform)
  std::optional<std::vector<float>> patternTransform;

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

struct PatternView : PatternViewT<PatternView, RNSVG::implementation::BrushView> {
 public:
  PatternView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  // RenderableView
  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void Draw(RNSVG::D2DDeviceContext const & /*deviceContext*/, Windows::Foundation::Size const & /*size*/){};

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  com_ptr<PatternProps> m_props;
  std::string m_patternUnits{"objectBoundingBox"};
  std::string m_patternContentUnits{"userSpaceOnUse"};

  // BrushView
  void CreateBrush();
  void UpdateBounds();

  // Helpers
  void CreateBrush(D2D1_RECT_F rect);
  D2D1_RECT_F GetAdjustedRect(D2D1_RECT_F bounds);
  winrt::com_ptr<ID2D1CommandList> GetCommandList(ID2D1Device* device, D2D1_RECT_F elRect);
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct PatternView : PatternViewT<PatternView, implementation::PatternView> {};
} // namespace winrt::RNSVG::factory_implementation
