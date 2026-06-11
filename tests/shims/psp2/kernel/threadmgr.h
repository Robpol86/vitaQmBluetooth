// NOLINTBEGIN(readability-identifier-naming)
#ifndef SHIM_PSP2_THREADMGR_H
#define SHIM_PSP2_THREADMGR_H
static inline int sceKernelGetThreadId(void) { return 0; }
static inline int sceKernelDelayThread(unsigned int us) {
    (void)us;
    return 0;
}
#endif
// NOLINTEND(readability-identifier-naming)
