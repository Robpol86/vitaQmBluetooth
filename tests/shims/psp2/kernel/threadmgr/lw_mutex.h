// NOLINTBEGIN(readability-identifier-naming)
#ifndef SHIM_LW_MUTEX_H
#define SHIM_LW_MUTEX_H
typedef struct SceKernelLwMutexWork {
    int _opaque[4];
} SceKernelLwMutexWork;

static inline int sceKernelCreateLwMutex(SceKernelLwMutexWork* work, const char* name, unsigned int attr, int init_count,
                                         void* opt) {
    (void)work;
    (void)name;
    (void)attr;
    (void)init_count;
    (void)opt;
    return 0;
}
static inline int sceKernelDeleteLwMutex(SceKernelLwMutexWork* work) {
    (void)work;
    return 0;
}
static inline int sceKernelLockLwMutex(SceKernelLwMutexWork* m, int n, unsigned int* t) {
    (void)m;
    (void)n;
    (void)t;
    return 0;
}
static inline int sceKernelUnlockLwMutex(SceKernelLwMutexWork* m, int n) {
    (void)m;
    (void)n;
    return 0;
}
#endif
// NOLINTEND(readability-identifier-naming)
