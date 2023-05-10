#include "pch.h"
#include "RenderableView.h"
#if __has_include("RenderableView.g.cpp")
#include "RenderableView.g.cpp"
#endif

#include "JSValueXaml.h"
#include "SvgView.h"
#include "Utils.h"

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
void RenderableView::UpdateProperties(IJSValueReader const &reader, bool forceUpdate, bool invalidate) {
  const JSValueObject &propertyMap{JSValue::ReadObjectFrom(reader)};
  auto const &parent{SvgParent().try_as<RNSVG::RenderableView>()};

  auto const &propList{propertyMap.find("propList")};
  if (propList != propertyMap.end()) {
    m_propList.clear();
    auto const &propValue{(*propList).second};
    for (auto const &item : propValue.AsArray()) {
      m_propList.push_back(Utils::JSValueAsString(item));
    }
  }

  bool fillSet{std::find(m_propList.begin(), m_propList.end(), "fill") != m_propList.end()};
  bool strokeSet{std::find(m_propList.begin(), m_propList.end(), "stroke") != m_propList.end()};

  for (auto const &pair : propertyMap) {
    auto const &propertyName{pair.first};
    auto const &propertyValue{pair.second};

    auto prop{RNSVG::BaseProp::Unknown};

    // name is not a prop we want to propagate to child elements
    // so we only set it when forceUpdate = true
    if (propertyName == "name" && forceUpdate) {
      if (parent) {
        SvgRoot().Templates().Remove(m_id);
      }
      m_id = to_hstring(Utils::JSValueAsString(propertyValue));
      if (parent) {
        SaveDefinition();
      }
    } else if (propertyName == "strokeWidth") {
      prop = RNSVG::BaseProp::StrokeWidth;
      if (forceUpdate || !m_propSetMap[prop]) {
        auto const &fallbackValue{parent ? parent.StrokeWidth() : RNSVG::SVGLength(1.0f, RNSVG::LengthType::Pixel)};
        m_strokeWidth = Utils::JSValueAsSVGLength(propertyValue, fallbackValue);
      }
    } else if (propertyName == "strokeOpacity") {
      prop = RNSVG::BaseProp::StrokeOpacity;
      if (forceUpdate || !m_propSetMap[prop]) {
        float fallbackValue{parent ? parent.StrokeOpacity() : 1.0f};
        m_strokeOpacity = Utils::JSValueAsFloat(propertyValue, fallbackValue);
      }
    } else if (propertyName == "fillOpacity") {
      prop = RNSVG::BaseProp::FillOpacity;
      if (forceUpdate || !m_propSetMap[prop]) {
        float fallbackValue{parent ? parent.FillOpacity() : 1.0f};
        m_fillOpacity = Utils::JSValueAsFloat(propertyValue, fallbackValue);
      }
    } else if (propertyName == "stroke") {
      prop = RNSVG::BaseProp::Stroke;
      if (forceUpdate || !m_propSetMap[prop]) {
        Windows::UI::Color fallbackColor{(parent && !strokeSet) ? parent.Stroke() : Windows::UI::Colors::Transparent()};

        if (!m_strokeBrushId.empty()) {
          m_strokeBrushId.clear();
        }

        SetColor(propertyValue.AsObject(), fallbackColor, propertyName);
      }
    } else if (propertyName == "fill") {
      prop = RNSVG::BaseProp::Fill;
      if (forceUpdate || !m_propSetMap[prop]) {
        Windows::UI::Color fallbackColor{Windows::UI::Colors::Black()};
        if (propertyValue.IsNull() && fillSet) {
          fallbackColor = Windows::UI::Colors::Transparent();
        } else if (parent) {
          fallbackColor = parent.Fill();
        }

        if (!m_fillBrushId.empty()) {
          m_fillBrushId.clear();
        }

        SetColor(propertyValue.AsObject(), fallbackColor, propertyName);
      }
    } else if (propertyName == "strokeLinecap") {
      prop = RNSVG::BaseProp::StrokeLineCap;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (propertyValue.IsNull()) {
          m_strokeLineCap = parent.StrokeLineCap();
        } else {
          m_strokeLineCap = propertyValue.AsInt32();
        }
      }
    } else if (propertyName == "strokeLinejoin") {
      prop = RNSVG::BaseProp::StrokeLineJoin;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (propertyValue.IsNull()) {
          m_strokeLineJoin = parent.StrokeLineJoin();
        } else {
          m_strokeLineJoin = propertyValue.AsInt32();
        }
      }
    } else if (propertyName == "fillRule") {
      prop = RNSVG::BaseProp::FillRule;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (propertyValue.IsNull()) {
          m_fillRule = parent.FillRule();
        } else {
          m_fillRule = propertyValue.AsInt32();
        }
      }
    } else if (propertyName == "strokeDashoffset") {
      prop = RNSVG::BaseProp::StrokeDashOffset;
      if (forceUpdate || !m_propSetMap[prop]) {
        float fallbackValue{parent ? parent.StrokeDashOffset() : 0.0f};
        m_strokeDashOffset = Utils::JSValueAsFloat(propertyValue, fallbackValue);
      }
    } else if (propertyName == "strokeMiterlimit") {
      prop = RNSVG::BaseProp::StrokeMiterLimit;
      if (forceUpdate || !m_propSetMap[prop]) {
        float fallbackValue{parent ? parent.StrokeMiterLimit() : 0.0f};
        m_strokeMiterLimit = Utils::JSValueAsFloat(propertyValue, fallbackValue);
      }
    } else if (propertyName == "strokeDasharray") {
      prop = RNSVG::BaseProp::StrokeDashArray;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (propertyValue.IsNull()) {
          m_strokeDashArray = parent.StrokeDashArray();
        } else {
          auto const &asArray = propertyValue.AsArray();

          if (!asArray.empty() && (asArray.size() % 2 == 0)) {
            m_strokeDashArray.Clear();

            for (auto const &item : asArray) {
              m_strokeDashArray.Append(SVGLength::From(item));
            }
          }
        }
      }
    } else if (propertyName == "matrix") {
      prop = RNSVG::BaseProp::Matrix;
      if (forceUpdate) {
        Numerics::float3x2 fallbackValue{parent ? parent.SvgTransform() : Numerics::make_float3x2_rotation(0)};
        m_transformMatrix = Utils::JSValueAsTransform(propertyValue, fallbackValue);
      }
    } else if (propertyName == "opacity" && forceUpdate) {
      m_opacity = Utils::JSValueAsFloat(propertyValue, 1.0f);
    } else if (propertyName == "clipPath") {
      m_clipPathId = to_hstring(Utils::JSValueAsString(propertyValue));
    }  else if (propertyName == "responsible") {
      m_isResponsible = propertyValue.AsBoolean();
    }

    // forceUpdate = true means the property is being set on an element
    // instead of being inherited from the parent.
    if (forceUpdate && (prop != RNSVG::BaseProp::Unknown)) {
      // If the propertyValue is null, that generally means the prop was deleted
      bool propSet{!propertyValue.IsNull()};

      // The exception being Fill and Stroke due to 'none' coming through as null
      if (prop == RNSVG::BaseProp::Fill) {
        propSet = fillSet;
      } else if (prop == RNSVG::BaseProp::Stroke) {
        propSet = strokeSet;
      }

      m_propSetMap[prop] = propSet;
    }
  }

  m_recreateResources = true;

  if (invalidate && SvgParent()) {
    SvgRoot().Invalidate();
  }
}

void RenderableView::SaveDefinition() {
  if (m_id != L"") {
    SvgRoot().Templates().Insert(m_id, *this);
  }
}

void RenderableView::Draw(IInspectable const &context, Size size) {
  if (m_recreateResources) {
    CreateGeometry();
  }

  winrt::com_ptr<ID2D1Geometry> geometry;
  winrt::copy_to_abi(m_geometry, *geometry.put_void());

  if (!geometry) {
    return;
  }

  com_ptr<ID2D1DeviceContext1> deviceContext;
  copy_to_abi(context, *deviceContext.put_void());

  com_ptr<ID2D1Layer> transformLayer;
  check_hresult(deviceContext->CreateLayer(nullptr, transformLayer.put()));

  D2D1_MATRIX_3X2_F transform{D2DHelpers::GetTransform(deviceContext.get())};

  if (m_propSetMap[RNSVG::BaseProp::Matrix]) {
    transform = D2DHelpers::AsD2DTransform(SvgTransform());
  }

  com_ptr<ID2D1Geometry> clipPathGeometry;
  copy_to_abi(ClipPathGeometry(), *clipPathGeometry.put_void());

  com_ptr<ID2D1Factory> factory;
  deviceContext->GetFactory(factory.put());

  com_ptr<ID2D1GeometryGroup> geometryGroup;
  ID2D1Geometry *geometries[] = {geometry.get()};
  check_hresult(
      factory->CreateGeometryGroup(D2DHelpers::GetFillRule(FillRule()), geometries, 1, geometryGroup.put()));

  geometry = geometryGroup;

  D2DHelpers::PushOpacityLayer(deviceContext.get(), clipPathGeometry.get(), m_opacity, transform);

  if (FillOpacity()) {
    D2DHelpers::PushOpacityLayer(deviceContext.get(), clipPathGeometry.get(), FillOpacity());

    auto fill{Utils::GetCanvasBrush(FillBrushId(), Fill(), SvgRoot(), geometry)};
    deviceContext->FillGeometry(geometry.get(), fill.get());

    deviceContext->PopLayer();
  }

  if (StrokeOpacity()) {
    D2DHelpers::PushOpacityLayer(deviceContext.get(), clipPathGeometry.get(), StrokeOpacity());

    D2D1_CAP_STYLE capStyle{D2DHelpers::GetLineCap(m_strokeLineCap)};
    D2D1_LINE_JOIN lineJoin{D2DHelpers::GetLineJoin(m_strokeLineJoin)};

    D2D1_STROKE_STYLE_PROPERTIES strokeStyleProperties;
    strokeStyleProperties.startCap = capStyle;
    strokeStyleProperties.endCap = capStyle;
    strokeStyleProperties.dashCap = capStyle;
    strokeStyleProperties.lineJoin = lineJoin;
    strokeStyleProperties.dashOffset = StrokeDashOffset();
    strokeStyleProperties.miterLimit = StrokeMiterLimit();
    strokeStyleProperties.dashStyle = D2D1_DASH_STYLE_SOLID;

    float canvasDiagonal{Utils::GetCanvasDiagonal(size)};
    float strokeWidth{Utils::GetAbsoluteLength(StrokeWidth(), canvasDiagonal)};

    float *dashArray = nullptr;
    if (StrokeDashArray().Size() > 0) {
      strokeStyleProperties.dashStyle = D2D1_DASH_STYLE_CUSTOM;
      m_adjustedStrokeDashArray = Utils::GetAdjustedStrokeArray(StrokeDashArray(), strokeWidth, canvasDiagonal);
      dashArray = &m_adjustedStrokeDashArray[0];
    }

    com_ptr<ID2D1StrokeStyle> strokeStyle;
    check_hresult(
        factory->CreateStrokeStyle(strokeStyleProperties, dashArray, m_strokeDashArray.Size(), strokeStyle.put()));

    auto const stroke{Utils::GetCanvasBrush(StrokeBrushId(), Stroke(), SvgRoot(), geometry)};
    deviceContext->DrawGeometry(geometry.get(), stroke.get(), strokeWidth, strokeStyle.get());
    deviceContext->PopLayer();
  }

  deviceContext->PopLayer();
}

void RenderableView::MergeProperties(RNSVG::RenderableView const &other) {
  for (auto const &prop : m_propSetMap) {
    if (!prop.second) {
      switch (prop.first) {
        case RNSVG::BaseProp::Fill:
          m_fill = other.Fill();
          m_fillBrushId = other.FillBrushId();
          break;
        case RNSVG::BaseProp::FillOpacity:
          m_fillOpacity = other.FillOpacity();
          break;
        case RNSVG::BaseProp::FillRule:
          m_fillRule = other.FillRule();
          break;
        case RNSVG::BaseProp::Stroke:
          m_stroke = other.Stroke();
          m_strokeBrushId = other.StrokeBrushId();
          break;
        case RNSVG::BaseProp::StrokeOpacity:
          m_strokeOpacity = other.StrokeOpacity();
          break;
        case RNSVG::BaseProp::StrokeWidth:
          m_strokeWidth = other.StrokeWidth();
          break;
        case RNSVG::BaseProp::StrokeMiterLimit:
          m_strokeMiterLimit = other.StrokeMiterLimit();
          break;
        case RNSVG::BaseProp::StrokeDashOffset:
          m_strokeDashOffset = other.StrokeDashOffset();
          break;
        case RNSVG::BaseProp::StrokeDashArray:
          m_strokeDashArray = other.StrokeDashArray();
          break;
        case RNSVG::BaseProp::StrokeLineCap:
          m_strokeLineCap = other.StrokeLineCap();
          break;
        case RNSVG::BaseProp::StrokeLineJoin:
          m_strokeLineJoin = other.StrokeLineJoin();
          break;
        case RNSVG::BaseProp::Unknown:
        default:
          break;
      }
    }
  }
}

RNSVG::SvgView RenderableView::SvgRoot() {
  if (SvgParent()) {
    if (auto const &svgView{SvgParent().try_as<RNSVG::SvgView>()}) {
      if (svgView.SvgParent()) {
        if (auto const &parent{svgView.SvgParent().try_as<RNSVG::RenderableView>()}) {
          return parent.SvgRoot();
        }
      } else {
        return svgView;
      }
    } else if (auto const &renderable{SvgParent().try_as<RNSVG::RenderableView>()}) {
      return renderable.SvgRoot();
    }
  }

  return nullptr;
}

IInspectable RenderableView::ClipPathGeometry() {
  if (!m_clipPathId.empty()) {
    if (auto const &clipPath{SvgRoot().Templates().TryLookup(m_clipPathId)}) {
      if (!clipPath.Geometry()) {
        clipPath.CreateGeometry();
      }
      return clipPath.Geometry();
    }
  }
  return nullptr;
}

void RenderableView::Unload() {
  if (m_geometry) {
    //m_geometry.Close();
    m_geometry = nullptr;
  }

  m_parent = nullptr;
  m_reactContext = nullptr;
  m_propList.clear();
  m_propSetMap.clear();
  m_strokeDashArray.Clear();
}

RNSVG::IRenderable RenderableView::HitTest(Point const &point) {
  if (m_geometry) {
    BOOL strokeContainsPoint{false};
    D2D1_POINT_2F pointD2D{point.X, point.Y};

    com_ptr<ID2D1Geometry> geometry;
    copy_to_abi(m_geometry, *geometry.put_void());

    if (auto const &svgRoot{SvgRoot()}) {
      float canvasDiagonal{Utils::GetCanvasDiagonal(svgRoot.ActualSize())};
      float strokeWidth{Utils::GetAbsoluteLength(StrokeWidth(), canvasDiagonal)};
      
      check_hresult(geometry->StrokeContainsPoint(pointD2D, strokeWidth, nullptr, nullptr, &strokeContainsPoint));
    }

    BOOL fillContainsPoint{false};
    check_hresult(geometry->FillContainsPoint(pointD2D, nullptr, &fillContainsPoint));

    if (fillContainsPoint || strokeContainsPoint) {
      return *this;
    }
  }
  return nullptr;
}

void RenderableView::SetColor(const JSValueObject &propValue, ui::Color fallbackColor, std::string propName) {
  switch (propValue["type"].AsInt64()) {
    case 1: {
      auto const &brushId{to_hstring(Utils::JSValueAsString(propValue["brushRef"]))};
      propName == "fill" ? m_fillBrushId = brushId : m_strokeBrushId = brushId;
      break;
    }
    case 2: // currentColor
    case 3: // context-fill
    case 4: // context-stroke
      propName == "fill" ? m_fillBrushId = L"currentColor" : m_strokeBrushId = L"currentColor";
      break;
    default: {
      auto const &color {Utils::JSValueAsColor(propValue["payload"], fallbackColor)};
      propName == "fill" ? m_fill = color : m_stroke = color;
      break;
    }
  }
}
} // namespace winrt::RNSVG::implementation
