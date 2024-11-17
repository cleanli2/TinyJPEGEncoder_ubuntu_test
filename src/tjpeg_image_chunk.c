#include "tjpeg_image_chunk.h"

#include <stdlib.h>
#include <assert.h>

#define prt_line printf("%d:%s\n", __LINE__, __func__)
#define prt_var(x) printf(#x"=%d\n", x)

void mem_print(const char*buf, uint32_t ct_start, uint32_t len);
void tjpeg_image_chunk_init(tjpeg_image_chunk_t *ic, uint8_t *data, int width, int height)
{
  assert(ic != NULL);
  assert(width != 0);
  assert(height != 0);

  ic->data = data;
  ic->width = width;
  ic->height = height;
  ic->x_pos = -16;
  ic->y_pos = 0;
}


bool tjpeg_image_chunk_next_block(tjpeg_image_chunk_t *ic)
{
  ic->x_pos += 16;

  if (ic->x_pos >= ic->width) {
    ic->y_pos += 8;
    ic->x_pos = 0;
#ifdef LIMITED_INPUT_BUFF
    update_yuvbuf();
#endif
  }

  return (ic->y_pos < ic->height);
}


void tjpeg_image_chunk_copy_y1(tjpeg_image_chunk_t *ic, int16_t *destination)
{
#ifndef LIMITED_INPUT_BUFF
  uint8_t *base = ic->data + 2 * (ic->x_pos + ic->y_pos * ic->width);
#else
  uint8_t *base = ic->data + 2 * (ic->x_pos);
#endif
  uint8_t *pointer = NULL;

  assert(ic != NULL);

  prt_line;
  prt_var(ic->y_pos);
  prt_var(ic->x_pos);
  prt_var(ic->data);
  prt_var(ic->data+1);
  prt_var(base);
  prt_var((ic->x_pos + ic->y_pos * ic->width));
  prt_var(2 * (ic->x_pos + ic->y_pos * ic->width));
  prt_var(ic->data + 2 * (ic->x_pos + ic->y_pos * ic->width));
  if(ic->y_pos==8)
  mem_print(base, base, 1280*8);

  for (uint8_t y = 8; y; --y) {
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      *destination = *pointer - 128;
      ++destination;
      pointer += 2;
    }

    base += 2 * ic->width;
  }
}


void tjpeg_image_chunk_copy_y2(tjpeg_image_chunk_t *ic, int16_t *destination)
{
#ifndef LIMITED_INPUT_BUFF
  uint8_t *base = ic->data + 2 * (ic->x_pos + 8 + ic->y_pos * ic->width);
#else
  uint8_t *base = ic->data + 2 * (ic->x_pos + 8);
#endif
  uint8_t *pointer = NULL;

  assert(ic != NULL);

  prt_line;
  prt_var(ic->y_pos);
  for (uint8_t y = 8; y; --y) {
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      *destination = *pointer - 128;
      ++destination;
      pointer += 2;
    }

    base += 2 * ic->width;
  }
}


void tjpeg_image_chunk_copy_cr(tjpeg_image_chunk_t *ic, int16_t *destination)
{
#ifndef LIMITED_INPUT_BUFF
  uint8_t *base = ic->data + 2 * (ic->x_pos + ic->y_pos * ic->width) + 3;
#else
  uint8_t *base = ic->data + 2 * (ic->x_pos) + 3;
#endif
  uint8_t *pointer = NULL;

  assert(ic != NULL);

  prt_line;
  prt_var(ic->y_pos);
  for (uint8_t y = 8; y; --y) {
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      *destination = *pointer - 128;
      ++destination;
      pointer += 4;
    }

    base += 2 * ic->width;
  }
}


void tjpeg_image_chunk_copy_cb(tjpeg_image_chunk_t *ic, int16_t *destination)
{
#ifndef LIMITED_INPUT_BUFF
  uint8_t *base = ic->data + 2 * (ic->x_pos + ic->y_pos * ic->width) + 1;
#else
  uint8_t *base = ic->data + 2 * (ic->x_pos) + 1;
#endif
  uint8_t *pointer = NULL;

  assert(ic != NULL);

  prt_line;
  prt_var(ic->y_pos);
  for (uint8_t y = 8; y; --y) {
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      *destination = *pointer - 128;
      ++destination;
      pointer += 4;
    }

    base += 2 * ic->width;
  }
}
