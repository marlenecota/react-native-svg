#include "pch.h"
#include "RenderableGroupView.h"
#if __has_include("RenderableGroupView.g.cpp")
#include "RenderableGroupView.g.cpp"
#endif

#include "JSValueXaml.h"
#include "SVGLength.h"
#include "SvgView.h"
#include "Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace winrt;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
void RenderableGroupView::AddChild(IRenderable const &child) {
  m_children.Append(child);
  if (m_props) {
    child.UpdateProperties(m_props, false, false);
  }
}

void RenderableGroupView::UpdateProperties(IJSValueReader const &reader, bool forceUpdate, bool invalidate) {
  if (!m_props) {
    m_props = reader;
  }

  const JSValueObject &propertyMap = JSValue::ReadObjectFrom(reader);
  auto const &parent = SvgParent().as<IRenderable>();

  for (auto const &pair : propertyMap) {
    auto const &propertyName = pair.first;
    auto const &propertyValue = pair.second;

    bool propSet{false};
    auto prop{RNSVG::BaseProp::Fill};

    if (propertyName == "strokeWidth") {
      prop = RNSVG::BaseProp::StrokeWidth;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_strokeWidth = parent.StrokeWidth();
        } else {
          m_strokeWidth = SVGLength::From(propertyValue);
        }
        propSet = true;
      }
    } else if (propertyName == "strokeOpacity") {
      prop = RNSVG::BaseProp::StrokeOpacity;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_strokeOpacity = parent.StrokeOpacity();
        } else {
          m_strokeOpacity = propertyValue.AsSingle();
        }
        propSet = true;
      }
    } else if (propertyName == "fillOpacity") {
      prop = RNSVG::BaseProp::FillOpacity;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_fillOpacity = parent.FillOpacity();
        } else {
          m_fillOpacity = propertyValue.AsSingle();
        }
        propSet = true;
      }
    } else if (propertyName == "stroke") {
      prop = RNSVG::BaseProp::Stroke;
      if (forceUpdate || !m_propSetMap[prop]) {
        Windows::UI::Color newColor{Windows::UI::Colors::Transparent()};
        if (parent && Utils::JSValueIsNull(propertyValue)) {
          newColor = parent.Stroke();
        } else if (auto color = Utils::GetColorFromJSValue(propertyValue)) {
          newColor = color.value();
        }
        m_stroke = newColor;
        propSet = true;
      }
    } else if (propertyName == "fill") {
      prop = RNSVG::BaseProp::Fill;
      if (forceUpdate || !m_propSetMap[prop]) {
        Windows::UI::Color newColor{Windows::UI::Colors::Transparent()};
        if (parent && Utils::JSValueIsNull(propertyValue)) {
          newColor = parent.Fill();
        } else if (auto color = Utils::GetColorFromJSValue(propertyValue)) {
          newColor = color.value();
        }
        m_fill = newColor;
        propSet = true;
      }
    } else if (propertyName == "strokeLinecap") {
      prop = RNSVG::BaseProp::StrokeLineCap;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_strokeLineCap = parent.StrokeLineCap();
        } else {
          auto strokeLineCap{propertyValue.AsInt32()};
          switch (strokeLineCap) {
            case 2:
              m_strokeLineCap = Geometry::CanvasCapStyle::Square;
              break;
            case 1:
              m_strokeLineCap = Geometry::CanvasCapStyle::Round;
              break;
            case 0:
            default:
              m_strokeLineCap = Geometry::CanvasCapStyle::Flat;
              break;
          }
        }
        propSet = true;
      }
    } else if (propertyName == "strokeLinejoin") {
      prop = RNSVG::BaseProp::StrokeLineJoin;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_strokeLineCap = parent.StrokeLineCap();
        } else {
          auto strokeLineJoin{propertyValue.AsInt32()};
          switch (strokeLineJoin) {
            case 2:
              m_strokeLineJoin = Geometry::CanvasLineJoin::Bevel;
              break;
            case 1:
              m_strokeLineJoin = Geometry::CanvasLineJoin::Round;
              break;
            case 0:
            default:
              m_strokeLineJoin = Geometry::CanvasLineJoin::Miter;
              break;
          }
        }
        propSet = true;
      }
    } else if (propertyName == "fillRule") {
      prop = RNSVG::BaseProp::FillRule;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_fillRule = parent.FillRule();
        } else {
          auto fillRule{propertyValue.AsInt32()};
          switch (fillRule) {
            case 0:
              m_fillRule = Geometry::CanvasFilledRegionDetermination::Alternate;
              break;
            case 1:
            default:
              m_fillRule = Geometry::CanvasFilledRegionDetermination::Winding;
              break;
          }
        }
        propSet = true;
      }
    } else if (propertyName == "strokeDashoffset") {
      prop = RNSVG::BaseProp::StrokeDashOffset;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_strokeDashOffset = parent.StrokeDashOffset();
        } else {
          m_strokeDashOffset = propertyValue.AsSingle();
        }
        propSet = true;
      }
    } else if (propertyName == "strokeMiterlimit") {
      prop = RNSVG::BaseProp::StrokeMiterLimit;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_strokeMiterLimit = parent.StrokeMiterLimit();
        } else {
          m_strokeMiterLimit = propertyValue.AsSingle();
        }
        propSet = true;
      }
    } else if (propertyName == "strokeDasharray") {
      prop = RNSVG::BaseProp::StrokeDashArray;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_strokeDashArray = parent.StrokeDashArray();
        } else {
          auto const &asArray = propertyValue.AsArray();

          if (!asArray.empty() && (asArray.size() % 2 == 0)) {
            m_strokeDashArray.Clear();

            for (auto &item : asArray) {
              m_strokeDashArray.Append(SVGLength::From(item));
            }
          }
        }
        propSet = true;
      }
    } else if (propertyName == "scale") {
      prop = RNSVG::BaseProp::Scale;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_scale = parent.SvgScale();
        } else {
          m_scale = Numerics::make_float3x2_scale(propertyValue.AsSingle());
        }
        propSet = true;
      }
    } else if (propertyName == "rotation") {
      prop = RNSVG::BaseProp::Rotation;
      if (forceUpdate || !m_propSetMap[prop]) {
        if (Utils::JSValueIsNull(propertyValue)) {
          m_rotation = parent.SvgRotation();
        } else {
          m_rotation = Numerics::make_float3x2_rotation(propertyValue.AsSingle() * static_cast<float>(M_PI) / 180.0f);
        }
        propSet = true;
      }
    }

    // forceUpdate = true means the property is being set on an element
    // instead of being inherited from the parent.
    if (forceUpdate && propSet) {
      // If the propertyValue is null, that means we reset the property
      m_propSetMap[prop] = !Utils::JSValueIsNull(propertyValue);
    }
  }

  for (auto child : Children()) {
    child.UpdateProperties(reader, false, false);
  }

  m_recreateResources = true;

  if (invalidate) {
    InvalidateCanvas();
  }
}

void RenderableGroupView::CreateGeometry(ICanvasResourceCreator const &resourceCreator) {
  std::vector<Geometry::CanvasGeometry> geometries;
  for (auto child : Children()) {
    geometries.push_back(child.Geometry());
  }

  Geometry(Geometry::CanvasGeometry::CreateGroup(resourceCreator, geometries, FillRule()));
}

void RenderableGroupView::Render(
    Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const &canvas,
    Microsoft::Graphics::Canvas::CanvasDrawingSession const &session) {
  RenderGroup(canvas, session);
}

void RenderableGroupView::RenderGroup(UI::Xaml::CanvasControl const &canvas, CanvasDrawingSession const &session) {
  for (auto child : Children()) {
    child.Render(canvas, session);
  }
}

void RenderableGroupView::InvalidateCanvas() {
  if (SvgParent()) {
    if (auto svgView{SvgParent().try_as<RNSVG::SvgView>()}) {
      svgView.InvalidateCanvas();
    } else if (auto renderable{SvgParent().try_as<IRenderable>()}) {
      renderable.InvalidateCanvas();
    }
  }
}
} // namespace winrt::RNSVG::implementation
