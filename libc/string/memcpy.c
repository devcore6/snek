#include <string.h>

void* memcpy(void* str1, const void* str2, size_t n) {
	unsigned char* dst = (unsigned char *)str1;
	const unsigned char* src = (const unsigned char*)str2;
	for(size_t i = 0; i < n; i++) dst[i] = src[i];
	return dst;
}
