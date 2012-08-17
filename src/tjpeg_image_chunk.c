#include "tjpeg_image_chunk.h"


void tjpeg_image_chunk_init(tjpeg_image_chunk_t *ic, uint8_t *data, int width, int height)
{
  assert(chunk != NULL)
  ic->data = data;
  ic->width = width;
  ic->height = hight;
  ic->x_pos = 0;
  ic->y_pos = 0;
}


bool tjpeg_image_chunk_next_block(tjpeg_image_chunk_t *ic)
{
  if (ic->y_pos > ic->height)
    return false;

  ic->x_pos += 16;

  if (ic->x_pos >= ic->width)
    ic->y_pos += 8;
}


void tjpeg_image_chunk_copy_y1(tjpeg_image_chunk_t *ic, int16_t *destination)
{
  uint8_t *base = ic->data + 2 * (ic->x_pos + ic->y_pos * ic->width);
  uint8_t *pointer = NULL;

  for (uint8_t y = 8; y; --y)
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      destination = *pointer - 128;
      ++destination;
      pointer += 2;
    }

    base += 2 * ic->width;
  }
}


void tjpeg_image_chunk_copy_y2(tjpeg_image_chunk_t *ic, int16_t *destination)
{
  uint8_t *base = ic->data + 2 * (ic->x_pos + 8 + ic->y_pos * ic->width);
  uint8_t *pointer = NULL;

  for (uint8_t y = 8; y; --y)
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      destination = *pointer - 128;
      ++destination;
      pointer += 2;
    }

    base += 2 * ic->width;
  }
}


void tjpeg_image_chunk_copy_cr(tjpeg_image_chunk_t *ic, int16_t *destination)
{
  uint8_t *base = ic->data + (ic->x_pos + ic->y_pos * ic->width) / 2 * 2 + 1;
  uint8_t *pointer = NULL;

  for (uint8_t y = 8; y; --y)
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      destination = *pointer - 128;
      ++destination;
      pointer += 4;
    }

    base += 2 * ic->width;
  }
}


void tjpeg_image_chunk_copy_cb(tjpeg_image_chunk_t *ic, int16_t *destination)
{
  uint8_t *base = ic->data + (ic->x_pos + ic->y_pos * ic->width) / 2 * 2 + 3;
  uint8_t *pointer = NULL;

  for (uint8_t y = 8; y; --y)
    pointer = base;

    for (uint8_t x = 8; x; --x) {
      destination = *pointer - 128;
      ++destination;
      pointer += 4;
    }

    base += 2 * ic->width;
  }
}
