#include <string.h>

void* memmove(void* str1, void* str2, size_t n) {
	unsigned char* dst = (unsigned char *)str1;
	unsigned char* src = (unsigned char*)str2;
	for(size_t i = 0; i < n; i++) {
		dst[i] = src[i];
		src[i] = 0;
	}
	return dst;
}
