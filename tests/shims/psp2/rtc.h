// NOLINTBEGIN(readability-identifier-naming)
#ifndef SHIM_PSP2_RTC_H
#define SHIM_PSP2_RTC_H
#include <string.h>
typedef struct SceDateTime {
    unsigned short year, month, day, hour, minute, second;
    unsigned int microsecond;
} SceDateTime;
static inline int sceRtcGetCurrentClockLocalTime(SceDateTime* t) {
    if (t) memset(t, 0, sizeof(*t));
    return 0;
}
#endif
// NOLINTEND(readability-identifier-naming)
