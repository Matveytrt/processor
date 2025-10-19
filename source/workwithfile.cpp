#include "workwithfile.h"

size_t BuffSize(const char *file_name)
{
    assert(file_name);

    struct stat file_info = {};
    
    assert(stat(file_name, &file_info) != -1);

    size_t Buff_size = (size_t) file_info.st_size;

    return Buff_size;
}

void *ReadBuff(const char *filename, const char *mode, size_t buff_size, void *buff, size_t size)
{
    assert(filename);
    assert(mode);
    assert(buff);
    
    FILE *file = fopen(filename, mode);
    assert(file);

    fread(buff, size, buff_size, file);

    fclose(file);
    return buff;
}

void *ReadBuffResult(const char *filename, const char *mode, size_t *buff_size, size_t size)
{
    assert(filename);
    assert(mode);
    assert(buff_size);

    *buff_size = BuffSize(filename);

    void *buff = calloc(*buff_size + 1, size); 
    assert(buff);
    
    ReadBuff(filename, mode, *buff_size, buff, size);
    assert(buff);   

    return buff;
}
