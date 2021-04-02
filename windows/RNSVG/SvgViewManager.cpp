#include "pch.h"
#include "SvgViewManager.h"
#if __has_include("SvgViewManager.g.cpp")
#include "SvgViewManager.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.UI.Xaml.Shapes.h>

#include "RenderableView.h"
#include "SvgView.h"

namespace winrt {
using namespace Windows::Foundation::Collections;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;
using namespace Microsoft::ReactNative;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
} // namespace winrt

namespace winrt::RNSVG::implementation {
// IViewManager
hstring SvgViewManager::Name() {
  return L"RNSVGSvgView";
}

FrameworkElement SvgViewManager::CreateView() {
  return winrt::RNSVG::SvgView(m_reactContext);
}

// IViewManagerWithContext
IReactContext SvgViewManager::ReactContext() {
  return m_reactContext;
}

void SvgViewManager::ReactContext(IReactContext const &reactContext) {
  m_reactContext = reactContext;
}

// IViewManagerWithNativeProperties
IMapView<hstring, ViewManagerPropertyType> SvgViewManager::NativeProps() {
  auto const &nativeProps{winrt::single_threaded_map<hstring, ViewManagerPropertyType>()};

  nativeProps.Insert(L"height", ViewManagerPropertyType::Number);
  nativeProps.Insert(L"width", ViewManagerPropertyType::Number);
  nativeProps.Insert(L"opacity", ViewManagerPropertyType::Number);
  // viewBox
  nativeProps.Insert(L"minX", ViewManagerPropertyType::Number);
  nativeProps.Insert(L"minY", ViewManagerPropertyType::Number);
  nativeProps.Insert(L"vbWidth", ViewManagerPropertyType::Number);
  nativeProps.Insert(L"vbHeight", ViewManagerPropertyType::Number);
  // preserveAspectRatio
  nativeProps.Insert(L"align", ViewManagerPropertyType::String);
  nativeProps.Insert(L"meetOrSlice", ViewManagerPropertyType::Number);

  return nativeProps.GetView();
}

void SvgViewManager::UpdateProperties(FrameworkElement const &view, IJSValueReader const &propertyMapReader) {
  if (auto const &svgView{view.try_as<RNSVG::SvgView>()}) {
    svgView.UpdateProperties(propertyMapReader);
  }
}

// IViewManagerWithChildren
void SvgViewManager::AddView(FrameworkElement const &parent, UIElement const &child, int64_t /*index*/) {
  if (auto const &view{parent.try_as<RNSVG::SvgView>()}) {
    view.Views().Append(child);

    if (auto const &childView{child.try_as<RNSVG::RenderableView>()}) {
      childView.SvgParent(parent);
    }
  }
}

void SvgViewManager::RemoveAllChildren(FrameworkElement const &parent) {
  if (auto const &view{parent.try_as<RNSVG::SvgView>()}) {
    for (auto const &child : view.Views()) {
      if (auto const &childView{child.try_as<RNSVG::RenderableView>()}) {
        childView.SvgParent(nullptr);
      }
    }
    view.Views().Clear();
  }
}

void SvgViewManager::RemoveChildAt(FrameworkElement const &parent, int64_t index) {
  if (auto const &view{parent.try_as<RNSVG::SvgView>()}) {
    auto const &child{view.Views().GetAt(static_cast<uint32_t>(index))};
    if (auto const &childView{child.try_as<RNSVG::RenderableView>()}) {
      childView.SvgParent(nullptr);
    }
    view.Views().RemoveAt(static_cast<uint32_t>(index));
  }
}

void SvgViewManager::ReplaceChild(
    FrameworkElement const &parent,
    UIElement const &oldChild,
    UIElement const &newChild) {
  auto const &svgView{parent.try_as<RNSVG::SvgView>()};
  auto const &oldChildView{oldChild.try_as<RNSVG::RenderableView>()};
  auto const &newChildView{newChild.try_as<RNSVG::RenderableView>()};

  if (svgView && oldChildView && newChildView) {
    uint32_t index;

    if (svgView.Views().IndexOf(oldChild, index)) {
      svgView.Views().SetAt(index, newChild);
      oldChildView.SvgParent(nullptr);
      newChildView.SvgParent(parent);
    }
  }
}
} // namespace winrt::RNSVG::implementation
