#pragma once

#include <cstddef>

namespace debug {

void print(const char *);
void printf(const char *format, ...);
void print(const char *, const size_t);
void println(const char *);
void println(const char *, const size_t);

} // namespace debug
