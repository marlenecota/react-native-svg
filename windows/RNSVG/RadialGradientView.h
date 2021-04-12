#pragma once
#include "RadialGradientView.g.h"
#include "BrushView.h"

namespace winrt::RNSVG::implementation
{
    struct RadialGradientView : RadialGradientViewT<RadialGradientView, RNSVG::implementation::BrushView>
    {
        RadialGradientView() = default;

    };
}
namespace winrt::RNSVG::factory_implementation
{
    struct RadialGradientView : RadialGradientViewT<RadialGradientView, implementation::RadialGradientView>
    {
    };
}
