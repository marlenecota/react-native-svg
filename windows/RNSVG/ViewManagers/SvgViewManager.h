#pragma once
#include "SvgViewManager.g.h"

// WARNING: This file is automatically generated by a tool. Do not directly
// add this file to your project, as any changes you make will be lost.
// This file is a stub you can use as a starting point for your implementation.
//
// To add a copy of this file to your project:
//   1. Copy this file from its original location to the location where you store 
//      your other source files (e.g. the project root). 
//   2. Add the copied file to your project. In Visual Studio, you can use 
//      Project -> Add Existing Item.
//   3. Delete this comment and the 'static_assert' (below) from the copied file.
//      Do not modify the original file.
//
// To update an existing file in your project:
//   1. Copy the relevant changes from this file and merge them into the copy 
//      you made previously.
//    
// This assertion helps prevent accidental modification of generated files.
static_assert(false, "This file is generated by a tool and will be overwritten. Open this error and view the comment for assistance.");

namespace winrt::RNSVG::implementation
{
    struct SvgViewManager : SvgViewManagerT<SvgViewManager>
    {
        SvgViewManager() = default;

        hstring Name();
        winrt::Windows::UI::Xaml::FrameworkElement CreateView();
        winrt::Microsoft::ReactNative::IReactContext ReactContext();
        void ReactContext(winrt::Microsoft::ReactNative::IReactContext const& value);
        winrt::Windows::Foundation::Collections::IMapView<hstring, winrt::Microsoft::ReactNative::ViewManagerPropertyType> NativeProps();
        void UpdateProperties(winrt::Windows::UI::Xaml::FrameworkElement const& view, winrt::Microsoft::ReactNative::IJSValueReader const& propertyMapReader);
        void AddView(winrt::Windows::UI::Xaml::FrameworkElement const& parent, winrt::Windows::UI::Xaml::UIElement const& child, int64_t index);
        void RemoveAllChildren(winrt::Windows::UI::Xaml::FrameworkElement const& parent);
        void RemoveChildAt(winrt::Windows::UI::Xaml::FrameworkElement const& parent, int64_t index);
        void ReplaceChild(winrt::Windows::UI::Xaml::FrameworkElement const& parent, winrt::Windows::UI::Xaml::UIElement const& oldChild, winrt::Windows::UI::Xaml::UIElement const& newChild);
        void OnPointerEvent(winrt::Windows::Foundation::IInspectable const& view, winrt::Microsoft::ReactNative::ReactPointerEventArgs const& args);
    };
}
namespace winrt::RNSVG::factory_implementation
{
    struct SvgViewManager : SvgViewManagerT<SvgViewManager, implementation::SvgViewManager>
    {
    };
}
