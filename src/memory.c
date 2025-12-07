#include "types.h"
#include "memory.h"

void* memmove(void* dst, const void* src, size_t n) {
	char* d = (char*)dst;
	const char* s = (const char*)src;
	if (d < s) {
		for (size_t i = 0; i < n; ++i) {
			d[i] = s[i];
		}
	} else {
		for (size_t i = n; i > 0; --i) {
			d[i - 1] = s[i - 1];
		}
	}
	return dst;
}

void* memzero (void* dst, size_t n) {
	char* d = (char*)dst;
	for (size_t i = 0; i < n; ++i) {
		d[i] = 0;
	}
	return dst;
}