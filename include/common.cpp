#include <assert.h>
#include "common.h"
#include <sys/stat.h>

/*
size_t getFileSize(const char *filename)
{
    assert(filename != nullptr);

    struct stat file_info = {};

    stat(filename, &file_info);

    if (file_info.st_size <= 0)
    {
        // ??
    }

    return (size_t)file_info.st_size;
}
*/

size_t getFileSize(FILE *file)
{
  size_t file_size = 0;

  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  return file_size;
}

