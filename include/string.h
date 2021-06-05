#ifndef __STRING_H
# define __STRING_H
# include <stdlib.h>

extern void* memcpy(void* str1, const void* str2, size_t n);
extern void* memmove(void* str1, void* str2, size_t n);
extern void* memset(void* str, int c, size_t n);

extern char* strcpy(char* str1, const char* str2);
extern char* strncpy(char* str1, const char* str2, size_t n);
extern size_t strlen(const char *str);

#endif
