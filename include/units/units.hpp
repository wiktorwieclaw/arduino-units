#include <stdint.h>

namespace units {
namespace detail {
constexpr intmax_t sign(intmax_t x) {
    return x < 0 ? -1 : 1;
}

constexpr intmax_t abs(intmax_t x) {
    return x * sign(x);
}

constexpr intmax_t gcd(intmax_t x, intmax_t y) {
    if (x == 0) { return abs(y); }
    if (y == 0) { return abs(x); }
    return gcd(y, x % y);
}
} // namespace detail

template <intmax_t Num, intmax_t Den = 1>
struct ratio {
    static_assert(Den != 0);
    static_assert(Num >= -INTMAX_MAX && Den >= -INTMAX_MAX);
    static constexpr auto num = Num * detail::sign(Den) / detail::gcd(Num, Den);     
    static constexpr auto den = detail::abs(Den) / detail::gcd(Num, Den);
    using type = ratio<num, den>;
};

using kilo  = ratio<1000, 1>;
using hecto = ratio<100, 1>;
using deca  = ratio<10, 1>;
using deci  = ratio<1, 10>;
using centi = ratio<1, 100>;
using milli = ratio<1, 1000>;

template <typename Meas, typename Val, typename Ratio = ratio<1>>
class unit {
   public:
    using measurment_type = Meas;
    using value_type = Val;
    using ratio_type = Ratio;

    constexpr explicit unit(value_type v) : value_{v} {}

    template <typename R>
    constexpr unit(const unit<Meas, Val, R>& p)
        : unit{p.value() * R::num * Ratio::den / R::den / Ratio::num} {}

    constexpr value_type value() const { return value_; }

    constexpr unit& operator+=(const unit& other) {
        value_ += other.value_; 
        return *this;
    }
    
    constexpr unit& operator-=(const unit& other) {
        value_ -= other.value_; 
        return *this;
    }

    constexpr unit& operator*=(const value_type& rhs) {
        value_ *= rhs; 
        return *this;
    }

    constexpr unit& operator/=(const value_type& rhs) {
        value_ /= rhs; 
        return *this;
    }

   private:
    value_type value_;
};

template <typename M, typename V, typename R>
constexpr auto operator+(const unit<M, V, R>& lhs, const unit<M, V, R>& rhs) {
    return unit<M, V, R>{lhs.value() + rhs.value()};
}

template <typename M, typename V, typename R>
constexpr auto operator-(const unit<M, V, R>& lhs, const unit<M, V, R>& rhs) {
    return unit<M, V, R>{lhs.value() - rhs.value()};
}

template <typename M, typename V1, typename R, typename V2>
constexpr auto operator*(const unit<M, V1, R>& lhs, const V2& rhs) {
    return unit<M, V1, R>{lhs.value() * rhs};
}

template <typename M, typename V1, typename R, typename V2>
constexpr auto operator*(const V2& lhs, const unit<M, V1, R>& rhs) {
    return unit<M, V1, R>{lhs * rhs.value()};
}

template <typename M, typename V1, typename R, typename V2>
constexpr auto operator/(const unit<M, V1, R>& lhs, const V2& rhs) {
    return unit<M, V1, R>{lhs.value() / rhs};
}

template <typename M, typename V, typename R>
constexpr auto operator/(const unit<M, V, R>& lhs, const unit<M, V, R>& rhs) {
    return lhs.value() / rhs.value();
}
} // namespace units
