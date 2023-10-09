#include "hash_rot13.h"

unsigned int hashRot13(const char * arr, const size_t nbytes)
{
  unsigned int hash = 0;

  for(size_t byte_cnt = 0; byte_cnt < nbytes; byte_cnt++)
    {
      hash += (unsigned char)arr[byte_cnt];
      hash -= (hash << 13) | (hash >> 19);
    }

  return hash;
}

