#include <string.h>

char* strcpy(char* str1, const char* str2) {
	return memcpy((void*)str1, (const void*)str2, strlen(str2));
}
