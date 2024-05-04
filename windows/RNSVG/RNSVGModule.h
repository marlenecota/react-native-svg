#pragma once

#include "pch.h"
#include "resource.h"

#include "JSValue.h"
#include "NativeModules.h"

namespace winrt::RNSVG
{

  REACT_MODULE(RNSVGModule, L"RNSVG")
  struct RNSVGModule
  {
    REACT_INIT(Initialize)
    void Initialize(React::ReactContext const &reactContext) noexcept;

    REACT_SYNC_METHOD(multiply)
    double multiply(double a, double b) noexcept;

  private:
    React::ReactContext m_context;
  };

} // namespace winrt::RNSVG
