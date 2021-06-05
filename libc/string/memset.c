#include <string.h>

void* memset(void* str, int c, size_t n) {
	unsigned char* dst = (unsigned char *)str;
	for(size_t i = 0; i < n; i++) dst[i] = (unsigned char)c;
	return dst;
}
