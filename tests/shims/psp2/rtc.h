// NOLINTBEGIN(readability-identifier-naming)
#ifndef SHIM_PSP2_RTC_H
#define SHIM_PSP2_RTC_H
typedef struct SceDateTime {
    unsigned short year, month, day, hour, minute, second;
    unsigned int microsecond;
} SceDateTime;
static inline int sceRtcGetCurrentClockLocalTime(SceDateTime* t) {
    (void)t;
    return 0;
}
#endif
// NOLINTEND(readability-identifier-naming)
