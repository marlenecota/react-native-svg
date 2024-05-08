#include "pch.h"
#include "ReactPackageProvider.h"
#if __has_include("ReactPackageProvider.g.cpp")
#include "ReactPackageProvider.g.cpp"
#endif

#include "RNSVGModule.h"
#include "SvgView.h"
#include "GroupView.h"
#include "RectView.h"
#include "CircleView.h"
#include "EllipseView.h"
#include "LineView.h"
#include "PathView.h"
#include "UseView.h"
#include "SymbolView.h"
#include "DefsView.h"
#include "ClipPathView.h"
#include "MarkerView.h"
#include "LinearGradientView.h"
#include "RadialGradientView.h"
#include "PatternView.h"
#include "TextView.h"
#include "TSpanView.h"

using namespace winrt::Microsoft::ReactNative;

namespace winrt::RNSVG::implementation
{
  void ReactPackageProvider::CreatePackage(IReactPackageBuilder const &packageBuilder) noexcept
  {
    auto fabricPackageBuilder = packageBuilder.as<winrt::Microsoft::ReactNative::IReactPackageBuilderFabric>();

    SvgView::RegisterComponent(fabricPackageBuilder);
    GroupView::RegisterComponent(fabricPackageBuilder);
    RectView::RegisterComponent(fabricPackageBuilder);
    CircleView::RegisterComponent(fabricPackageBuilder);
    EllipseView::RegisterComponent(fabricPackageBuilder);
    LineView::RegisterComponent(fabricPackageBuilder);
    PathView::RegisterComponent(fabricPackageBuilder);
    UseView::RegisterComponent(fabricPackageBuilder);
    SymbolView::RegisterComponent(fabricPackageBuilder);
    DefsView::RegisterComponent(fabricPackageBuilder);
    ClipPathView::RegisterComponent(fabricPackageBuilder);
    MarkerView::RegisterComponent(fabricPackageBuilder);
    LinearGradientView::RegisterComponent(fabricPackageBuilder);
    RadialGradientView::RegisterComponent(fabricPackageBuilder);
    PatternView::RegisterComponent(fabricPackageBuilder);
    TextView::RegisterComponent(fabricPackageBuilder);
    TSpanView::RegisterComponent(fabricPackageBuilder);

    AddAttributedModules(packageBuilder);
  }

} // namespace winrt::RNSVG::implementation
