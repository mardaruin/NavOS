#ifndef STRING_H
#define STRING_H

#include "types.h"
                                                   
size_t strlen(const char* str);
char* itoa(int value, char* str, int base);
char* uitoa(size_t value, char* str, int base);
char* ptoa(const void* ptr, char* str);
void reverse(char* begin, size_t length);


#endif