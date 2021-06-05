#include <string.h>

char* strncpy(char* str1, const char* str2, size_t n) {
	return memcpy((void*)str1, (const void*)str2, n);
}
