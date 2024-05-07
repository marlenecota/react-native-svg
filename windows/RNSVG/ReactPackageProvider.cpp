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

    AddAttributedModules(packageBuilder);
  }

} // namespace winrt::RNSVG::implementation
