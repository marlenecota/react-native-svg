#include "pch.h"
#include "SymbolView.h"
#if __has_include("SymbolView.g.cpp")
#include "SymbolView.g.cpp"
#endif

#include "Utils.h"

using namespace winrt;

namespace winrt::RNSVG::implementation {
SymbolProps::SymbolProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void SymbolProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

SymbolView::SymbolView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) : base_type(args) {}

RNSVG::MeetOrSlice SymbolView::MeetOrSlice() {
  return Utils::GetMeetOrSlice(m_props->meetOrSlice);
}

void SymbolView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto symbolProps = props.try_as<SymbolProps>();
  if (symbolProps) {
    m_props = symbolProps;
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);
}

void SymbolView::RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGSymbol", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::SymbolProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::SymbolView>(args);
        });
      });
}
} // namespace winrt::RNSVG::implementation
