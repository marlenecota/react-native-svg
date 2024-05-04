#include "pch.h"

#include "RNSVGModule.h"

namespace winrt::RNSVG
{

// See https://microsoft.github.io/react-native-windows/docs/native-modules for details on writing native modules

void RNSVGModule::Initialize(React::ReactContext const &reactContext) noexcept {
  m_context = reactContext;
}

double RNSVGModule::multiply(double a, double b) noexcept {
  return a * b;
}

} // namespace winrt::RNSVG