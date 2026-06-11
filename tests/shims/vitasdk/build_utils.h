// NOLINTBEGIN(readability-identifier-naming)
#ifndef SHIM_VITASDK_BUILD_UTILS_H
#define SHIM_VITASDK_BUILD_UTILS_H
// Convert the SDK's compile-time size assertion into a real static_assert.
// Keeps the production check meaningful on the host.
#define VITASDK_BUILD_ASSERT_EQ(size, type) _Static_assert(sizeof(type) == (size), #type " size mismatch")
#endif
// NOLINTEND(readability-identifier-naming)
