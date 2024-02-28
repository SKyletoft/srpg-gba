#pragma once

#include <set>
#include <unordered_set>

// This exists as a way to quickly swap back and forth to test the performance
template <typename T>
using Set = std::set<T>;
