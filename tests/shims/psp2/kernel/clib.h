// NOLINTBEGIN(readability-identifier-naming)
#ifndef SHIM_PSP2_CLIB_H
#define SHIM_PSP2_CLIB_H
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
static inline void* sceClibMemcpy(void* d, const void* s, size_t n) { return memcpy(d, s, n); }
static inline void* sceClibMemset(void* d, int c, size_t n) { return memset(d, c, n); }
static inline int sceClibSnprintf(char* b, size_t n, const char* f, ...) {
    va_list a;
    va_start(a, f);
    int r = vsnprintf(b, n, f, a);
    va_end(a);
    return r;
}
static inline int sceClibPrintf(const char* f, ...) {
    (void)f;
    return 0;
}
#endif
// NOLINTEND(readability-identifier-naming)
