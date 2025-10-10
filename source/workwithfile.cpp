#include "workwithfile.h"

size_t BuffSize(const char *file_name)
{
    assert(file_name);

    struct stat file_info = {};
    
    assert(stat(file_name, &file_info) != -1);
    const char *check_str = "";
    perror(check_str);

    size_t Buff_size = (size_t) file_info.st_size;
    //printf("count = %zd\n", Buff_size);

    return Buff_size;
}

void *ReadBuff(const char *filename, const char *mode, size_t buff_size, void *buff, size_t size)
{
    assert(buff);
    assert(filename);

    FILE *file = fopen(filename, mode);
    assert(file);
    //printf("buff0 = %d\n", ((int*)buff)[0]);

    //printf("size = %d\n", buff_size);
    fread(buff, size, buff_size, file);
    //printf("buff3 = %d\n", ((int*)buff)[3]);
    //printf("%s\n", buff);

    fclose(file);
    return buff;
}

void *ReadBuffResult(const char *filename, const char *mode, size_t *buff_size, size_t size)
{
    assert(filename);
    assert(buff_size);

    *buff_size = BuffSize(filename);
    //printf("size = %d\n", *buff_size);

    void *buff = calloc(*buff_size + 1, size); 
    assert(buff);
    
    ReadBuff(filename, mode, *buff_size, buff, size);
    //printf("buff0 = %d\n", ((int*)buff)[0]);
    assert(buff);
    //printf("buff_ptr = %s\n", buffer);   

    return buff;
}
