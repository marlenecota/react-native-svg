#pragma once
#include "TSpanView.g.h"
#include "TextView.h"

namespace winrt::RNSVG::implementation {
struct TSpanView : TSpanViewT<TSpanView, RNSVG::implementation::TextView> {
public:
  TSpanView() = default;

  void UpdateProperties(Microsoft::ReactNative::IJSValueReader const &reader, bool forceUpdate, bool invalidate);
  virtual void Draw();

 private:
  std::string m_content;
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct TSpanView : TSpanViewT<TSpanView, implementation::TSpanView> {};
} // namespace winrt::RNSVG::factory_implementation
