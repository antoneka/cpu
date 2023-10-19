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

int initAsmBuffer(int **buffer, size_t *buff_size, FILE *bin_file, size_t bin_file_size)
{
  assert(buffer != nullptr); 
  assert(buff_size != nullptr);
  assert(bin_file != nullptr);

  *buff_size = bin_file_size / sizeof(int);

  *buffer = (int*)calloc(*buff_size, sizeof(int));

  if (*buffer == nullptr)
    {
      return BUFFER_ALLOCATION_ERROR;
    }

  fread(*buffer, sizeof(int), *buff_size, bin_file);

  return EXECUTION_SUCCESS;
}

