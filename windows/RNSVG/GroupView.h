#pragma once
#include "GroupView.g.h"
#include "RenderableView.h"
#include "SvgGroupCommonProps.g.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(FontObject)
struct FontObject {
  REACT_FIELD(fontStyle)
  std::optional<std::string> fontStyle; // 'normal' | 'italic' | 'oblique'
  REACT_FIELD(fontVariant)
  std::optional<std::string> fontVariant; // 'normal' | 'small-caps'
  REACT_FIELD(fontWeight)
  std::optional<std::string> fontWeight;
  REACT_FIELD(fontStretch)
  std::optional<std::string> fontStretch;
  REACT_FIELD(fontSize)
  std::optional<float> fontSize;
  REACT_FIELD(fontFamily)
  std::optional<std::string> fontFamily;
  REACT_FIELD(textAnchor)
  std::optional<std::string> textAnchor; // TODO: type->TextAnchor
  REACT_FIELD(textDecoration)
  std::optional<std::string> textDecoration; // TODO: type-> TextDecoration
  REACT_FIELD(letterSpacing)
  std::optional<float> letterSpacing;
  REACT_FIELD(wordSpacing)
  std::optional<float> wordSpacing;
  REACT_FIELD(kerning)
  std::optional<float> kerning;
  REACT_FIELD(fontFeatureSettings)
  std::optional<std::string> fontFeatureSettings;
  REACT_FIELD(fontVariantLigatures)
  std::optional<std::string> fontVariantLigatures; // TODO: Type -> FontVariantLigatures
  REACT_FIELD(fontVariationSettings)
  std::optional<std::string> fontVariationSettings;

  bool operator==(const FontObject &rhs) const {
    return fontStyle == rhs.fontStyle && fontVariant == rhs.fontVariant &&
        fontWeight == rhs.fontWeight && fontStretch == rhs.fontStretch &&
        fontSize == rhs.fontSize && fontFamily == rhs.fontFamily &&
        textAnchor == rhs.textAnchor && textDecoration == rhs.textDecoration &&
        letterSpacing == rhs.letterSpacing && wordSpacing == rhs.wordSpacing &&
        kerning == rhs.kerning &&
        fontFeatureSettings == rhs.fontFeatureSettings &&
        fontVariantLigatures == rhs.fontVariantLigatures &&
        fontVariationSettings == rhs.fontVariationSettings;
  }

  bool operator!=(const FontObject &rhs) const {
    return !(*this == rhs);
  }
};

#define REACT_SVG_GROUP_COMMON_PROPS  \
  REACT_FIELD(fontSize)               \
  REACT_FIELD(fontWeight)             \
  REACT_FIELD(font)

REACT_STRUCT(SvgGroupCommonProps)
struct SvgGroupCommonProps
    : SvgGroupCommonPropsT<SvgGroupCommonProps, SvgRenderableCommonProps> {
  SvgGroupCommonProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(
      uint32_t hash,
      winrt::hstring propName,
      winrt::Microsoft::ReactNative::IJSValueReader value) noexcept;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;
  REACT_SVG_GROUP_COMMON_PROPS;

  std::string fontSize;
  std::string fontWeight;
  FontObject font;
};

struct GroupView
    : GroupViewT<GroupView, RNSVG::implementation::RenderableView> {
 public:
  GroupView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  hstring FontFamily() { return m_fontFamily; }
  void FontFamily(hstring const &value) { m_fontFamily = value; }

  float FontSize() { return m_fontSize; }
  void FontSize(float value) { m_fontSize = value; }

  hstring FontWeight() { return m_fontWeight; }
  void FontWeight(hstring const &value) { m_fontWeight = value; }

  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;

  virtual void CreateGeometry(RNSVG::D2DDeviceContext const &context);

  virtual void SaveDefinition();

  virtual void MergeProperties(RNSVG::RenderableView const &other);

  void Draw(RNSVG::D2DDeviceContext const &deviceContext, Windows::Foundation::Size const &size);

  virtual void DrawGroup(RNSVG::D2DDeviceContext const &deviceContext, Windows::Foundation::Size const &size);

  virtual void CreateResources();

  virtual void Unload();

  virtual RNSVG::IRenderable HitTest(Windows::Foundation::Point const &point);

  static void RegisterComponent(
      const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric
          &builder) noexcept;

 private:
  Microsoft::ReactNative::IReactContext m_reactContext{nullptr};

  float m_fontSize{12.0f};
  hstring m_fontFamily{L"Segoe UI"};
  hstring m_fontWeight{L"auto"};

  std::map<RNSVG::FontProp, bool> m_fontPropMap{
    {RNSVG::FontProp::FontSize, false},
    {RNSVG::FontProp::FontWeight, false},
    {RNSVG::FontProp::FontFamily, false},
  };
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct GroupView : GroupViewT<GroupView, implementation::GroupView> {};
} // namespace winrt::RNSVG::factory_implementation