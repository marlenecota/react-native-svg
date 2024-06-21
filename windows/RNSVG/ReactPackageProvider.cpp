#include "pch.h"
#include "ReactPackageProvider.h"
#if __has_include("ReactPackageProvider.g.cpp")
#include "ReactPackageProvider.g.cpp"
#endif

#include "RNSVGModule.h"

#ifdef USE_FABRIC
#include "SvgView.h"
#include "GroupView.h"
#include "RectView.h"
#include "CircleView.h"
#include "EllipseView.h"
#include "LineView.h"
#include "PathView.h"
#include "UseView.h"
#include "SymbolView.h"
#include "ImageView.h"
#include "DefsView.h"
#include "ClipPathView.h"
#include "MarkerView.h"
#include "LinearGradientView.h"
#include "RadialGradientView.h"
#include "PatternView.h"
#include "TextView.h"
#include "TSpanView.h"
#else
#include "ViewManagers/SvgViewManager.h"
#include "ViewManagers/GroupViewManager.h"
#include "ViewManagers/PathViewManager.h"
#include "ViewManagers/RectViewManager.h"
#include "ViewManagers/CircleViewManager.h"
#include "ViewManagers/EllipseViewManager.h"
#include "ViewManagers/LineViewManager.h"
#include "ViewManagers/UseViewManager.h"
#include "ViewManagers/ImageViewManager.h"
#include "ViewManagers/TextViewManager.h"
#include "ViewManagers/TSpanViewManager.h"
#include "ViewManagers/SymbolViewManager.h"
#include "ViewManagers/DefsViewManager.h"
#include "ViewManagers/LinearGradientViewManager.h"
#include "ViewManagers/RadialGradientViewManager.h"
#include "ViewManagers/PatternViewManager.h"
#include "ViewManagers/ClipPathViewManager.h"
#include "ViewManagers/MarkerViewManager.h"
#endif

using namespace winrt::Microsoft::ReactNative;

namespace winrt::RNSVG::implementation
{
  void ReactPackageProvider::CreatePackage(IReactPackageBuilder const &packageBuilder) noexcept
  {
    AddAttributedModules(packageBuilder);

#ifdef USE_FABRIC
    auto fabricPackageBuilder = packageBuilder.as<winrt::Microsoft::ReactNative::IReactPackageBuilderFabric>();

    SvgView::RegisterComponent(fabricPackageBuilder);
    RectView::RegisterComponent(fabricPackageBuilder);
    CircleView::RegisterComponent(fabricPackageBuilder);
    EllipseView::RegisterComponent(fabricPackageBuilder);
    LineView::RegisterComponent(fabricPackageBuilder);
    PathView::RegisterComponent(fabricPackageBuilder);
    UseView::RegisterComponent(fabricPackageBuilder);
    SymbolView::RegisterComponent(fabricPackageBuilder);
    ImageView::RegisterComponent(fabricPackageBuilder);
    GroupView::RegisterComponent(fabricPackageBuilder);
    DefsView::RegisterComponent(fabricPackageBuilder);
    ClipPathView::RegisterComponent(fabricPackageBuilder);
    MarkerView::RegisterComponent(fabricPackageBuilder);
    LinearGradientView::RegisterComponent(fabricPackageBuilder);
    RadialGradientView::RegisterComponent(fabricPackageBuilder);
    PatternView::RegisterComponent(fabricPackageBuilder);
    TextView::RegisterComponent(fabricPackageBuilder);
    TSpanView::RegisterComponent(fabricPackageBuilder);
#else
    packageBuilder.AddViewManager(L"SvgViewManager", []() { return winrt::make<SvgViewManager>(); });
    packageBuilder.AddViewManager(L"GroupViewManager", []() { return winrt::make<GroupViewManager>(); });
    packageBuilder.AddViewManager(L"PathViewManager", []() { return winrt::make<PathViewManager>(); });
    packageBuilder.AddViewManager(L"RectViewManager", []() { return winrt::make<RectViewManager>(); });
    packageBuilder.AddViewManager(L"CircleViewManager", []() { return winrt::make<CircleViewManager>(); });
    packageBuilder.AddViewManager(L"EllipseViewManager", []() { return winrt::make<EllipseViewManager>(); });
    packageBuilder.AddViewManager(L"LineViewManager", []() { return winrt::make<LineViewManager>(); });
    packageBuilder.AddViewManager(L"UseViewManager", []() { return winrt::make<UseViewManager>(); });
    packageBuilder.AddViewManager(L"ImageViewManager", []() { return winrt::make<ImageViewManager>(); });
    packageBuilder.AddViewManager(L"TextViewManager", []() { return winrt::make<TextViewManager>(); });
    packageBuilder.AddViewManager(L"TSpanViewManager", []() { return winrt::make<TSpanViewManager>(); });
    packageBuilder.AddViewManager(L"SymbolViewManager", []() { return winrt::make<SymbolViewManager>(); });
    packageBuilder.AddViewManager(L"DefsViewManager", []() { return winrt::make<DefsViewManager>(); });
    packageBuilder.AddViewManager(L"LinearGradientViewManager", []() { return winrt::make<LinearGradientViewManager>(); });
    packageBuilder.AddViewManager(L"RadialGradientViewManager", []() { return winrt::make<RadialGradientViewManager>(); });
    packageBuilder.AddViewManager(L"PatternViewManager", []() { return winrt::make<PatternViewManager>(); });
    packageBuilder.AddViewManager(L"ClipPathViewManager", []() { return winrt::make<ClipPathViewManager>(); });
    packageBuilder.AddViewManager(L"MarkerViewManager", []() { return winrt::make<MarkerViewManager>(); });
#endif

  }

} // namespace winrt::RNSVG::implementation
