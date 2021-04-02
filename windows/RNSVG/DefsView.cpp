#include "pch.h"
#include "DefsView.h"
#include "DefsView.g.cpp"

using namespace winrt;
using namespace Microsoft::Graphics::Canvas;

namespace winrt::RNSVG::implementation {
void DefsView::Render(UI::Xaml::CanvasControl const &/*canvas*/, CanvasDrawingSession const &/*session*/) {
  if (auto const &root{SvgRoot()}) {
    for (auto const &child : Children()) {
      if (auto const &defView{child.try_as<IDefinitionView>()}) {
        defView.SaveDefinition();
      } else if (child.Id() != L"") {
        root.Templates().Insert(child.Id(), child);
      }
    }
  }
}
} // namespace winrt::RNSVG::implementation
