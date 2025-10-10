#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#ifndef WORKWITHBUFF_H
#define WORKWITHBUFF_H

size_t BuffSize(const char *filename);
void *ReadBuff(const char *filename, const char *mode, size_t buff_size, void *buff, size_t size);
void *ReadBuffResult(const char *filename, const char *mode, size_t *buff_size, size_t size);

#endif