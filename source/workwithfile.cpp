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

void StrArrayCtor(const char *filename, const char *mode, File_Info_t *info)
{
    assert(info);

    info->buff = (char *) ReadBuffResult(filename, mode, &info->buff_size, sizeof(char));

    CountLines(info);
    printf("lines: %d\n", info->nlines);

    info->str_data = (Str_info_t *) calloc(info->nlines, sizeof(Str_info_t));
    assert(info->str_data);

    FillArray(info);
}

void StrArrayDestroy(File_Info_t *info)
{
    assert(info);

    free(info->buff);
    free(info->str_data);
}

void CountLines(File_Info_t *info)
{
    assert(info);

    info->nlines = 0;
    int nextline = '\n';
    char *start_ptr = info->buff;
    char *str_ptr = NULL;

    while (*info->buff != '\0')
    {
        str_ptr = strchr(info->buff, nextline);

        if (str_ptr == NULL)
        {
            break;
        }

        info->buff = str_ptr;
        *info->buff = '\0';

        info->buff++;
        info->nlines++;        
    }
    info->nlines++;
    info->buff = start_ptr;
    //printf("nlines = %d\n", info->nlines);
}

void FillArray(File_Info_t *info)
{
    assert(info);

    char *start_ptr = info->buff;

    for (size_t i = 0; i < info->nlines; i++)
    {
        info->str_data->str_ptr = info->buff;
        assert(info->str_data->str_ptr != NULL);
        
        info->str_data->str_len = strlen(info->buff);
        info->str_data->orig_num = i;

        info->str_data++;
        info->buff += strlen(info->buff) + 1;
    }

    info->buff = start_ptr;
    info->str_data -= info->nlines;
}
