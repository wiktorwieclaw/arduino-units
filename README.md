# units
C++ units template metaprogramming library for Arduino.

## Requirements
C++14

## Example
```c++
#include <units/units.hpp>

struct Distance {};
using Meter = units::unit<Distance, double>;
using Kilometer = units::unit<Distance, double, units::kilo>;

constexpr auto operator""_m(long double x) {
    return Meter{static_cast<double>(x)};
}

constexpr auto operator""_km(long double x) {
    return Kilometer{static_cast<double>(x)};
}

int main() {
    constexpr auto step = 1.5_m;
    auto dist = 2.5_km;
    dist -= Kilometer{step * 1000};
    // dist.value() == 1.0
}
```
