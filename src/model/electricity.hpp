#pragma once

#include <cstddef>

#include <ftxui/component/component.hpp>

#include "../util/clamped.hpp"

namespace model {
using namespace ftxui;

namespace {
class electricity_impl {
public:
    std::size_t min() const { return value_.min(); }
    std::size_t max() const { return value_.max(); }
    std::size_t now() const { return value_.now(); }
private:
    clamped<std::size_t> value_{0, 100};
};
}

inline electricity_impl electricity;
}