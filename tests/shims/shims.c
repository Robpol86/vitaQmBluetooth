// NOLINTBEGIN(readability-identifier-naming)
// No-op definitions for on-device symbols qm_state.c references but which have
// no effect on the pure state logic under test. Replace with recording stubs
// if you want to assert behavioral expectations later.
#include "vqmbt.h"

int QuickMenuRebornSetWidgetColor(const char* r, float a, float b, float c, float d) {
    (void)r;
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    return 0;
}
int QuickMenuRebornSetWidgetLabel(const char* r, const char* l) {
    (void)r;
    (void)l;
    return 0;
}

int sceKernelCreateLwMutex(void* work, const char* name, unsigned int attr, int init, void* opt) {
    (void)work;
    (void)name;
    (void)attr;
    (void)init;
    (void)opt;
    return 0;
}
int sceKernelDeleteLwMutex(void* work) {
    (void)work;
    return 0;
}

int kvqmbt_connect_device(unsigned int mac0, unsigned int mac1) {
    (void)mac0;
    (void)mac1;
    return 0;
}
int kvqmbt_disconnect_device(unsigned int mac0, unsigned int mac1) {
    (void)mac0;
    (void)mac1;
    return 0;
}

// logfile_write_line — declared in include/logfile.h, called by LOG_MESSAGE_TO_FILE.
void logfile_write_line(int y, int m, int d, const char* line, ...) {
    (void)y;
    (void)m;
    (void)d;
    (void)line;
}
void logfile_init(void) {}
// NOLINTEND(readability-identifier-naming)
