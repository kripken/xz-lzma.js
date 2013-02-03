#include "lzma.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


// don't inline, to be friendly to js engine osr
void __attribute__ ((noinline)) doit(char *buffer, int size, int i) {
  static char *buffer2 = NULL;
  static char *buffer3 = NULL;

  unsigned long maxCompressedSize = lzma_stream_buffer_bound(size);

  if (!buffer2) buffer2 = (char*)malloc(maxCompressedSize);
  if (!buffer3) buffer3 = (char*)malloc(size);

  size_t compressedSize = 0;
  int ret = lzma_easy_buffer_encode(LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64, NULL, (const uint8_t*)buffer, size, (uint8_t*)buffer2, &compressedSize, maxCompressedSize);
  assert(ret == LZMA_OK);
  if (i == 0) printf("sizes: %d,%d\n", size, compressedSize);

  lzma_stream strm = LZMA_STREAM_INIT;
  ret = lzma_stream_decoder(&strm, UINT64_MAX, 0);
  assert(ret == LZMA_OK);
  strm.next_in = (const uint8_t*)buffer2;
  strm.avail_in = compressedSize;
  strm.next_out = (uint8_t*)buffer3;
  strm.avail_out = size;
  ret = lzma_code (&strm, LZMA_FINISH);
  assert(ret == LZMA_OK || ret == LZMA_STREAM_END);
  size_t decompressedSize = size - strm.avail_out;
  assert(decompressedSize == size);
  if (i == 0) assert(strcmp(buffer, buffer3) == 0);
}

int main(int argc, char **argv) {
  int size = atoi(argv[1]);
  int iters = atoi(argv[2]);
  char *buffer = (char*)malloc(size);

  int i = 0;
  int run = 0;
  char runChar = 17;
  while (i < size) {
    if (run > 0) {
      run--;
    } else {
      if ((i & 7) == 0) {
        runChar = i & 7;
        run = i & 31;
      } else {
        runChar = (i*i) % 6714;
      }
    }
    buffer[i] = runChar;
    i++;
  }

  for (i = 0; i < iters; i++) {
    doit(buffer, size, i);
  }

  printf("ok.\n");

  return 0;
}

