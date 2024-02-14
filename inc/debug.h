#pragma once

#include <cstddef>

extern "C" {
#include <tonc_types.h>
}

namespace debug {

void print(const char *);
void printf(const char *format, ...);
void print(const char *, const size_t);
void println(s32);
void println(const char *);
void println(const char *, const size_t);

} // namespace debug
