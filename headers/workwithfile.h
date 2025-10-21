#ifndef WORKWITHFILE_H
#define WORKWITHFILE_H

#include "global.h"

size_t BuffSize(const char *filename);
void *ReadBuff(const char *filename, const char *mode, size_t buff_size, void *buff, size_t size);
void *ReadBuffResult(const char *filename, const char *mode, size_t *buff_size, size_t size);

#endif