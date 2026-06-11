#ifndef SHIM_LW_MUTEX_H
#define SHIM_LW_MUTEX_H
typedef struct SceKernelLwMutexWork {
    int _opaque[4];
} SceKernelLwMutexWork;
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
