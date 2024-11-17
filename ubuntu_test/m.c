#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tiny_jpeg.h"


#define prt_var(x) printf(#x"=%d\n", x)
uint8_t   ycbcr_image[640*480*2];
uint8_t   yuvbuf[640*8*2];
int main(int argc, char **argv)
{
  jpeg_proc_t               processor;
  const jpeg_file_header_t  *header;

  int       width, height, offset;
  uint8_t   *input_image;
  uint8_t   *ycbcr_image_p=ycbcr_image;
  uint8_t   output_image[0xffff];
  int       file_size = 0;
  int       bytes;

  FILE* input_file = fopen(argv[1], "rb");
  width = 640;
  height = 480;
  fread(ycbcr_image, width * height * 2, 1, input_file);
  fclose(input_file);

  tjpeg_init(&processor, width, height);

  FILE* output_file = fopen(argv[2], "wb");

  header = tjpeg_get_header();
  prt_var(header->length);
  memcpy(output_image, header->data, header->length);
  output_image[header->image_width_offset] = (uint8_t) (width >> 8);
  output_image[header->image_width_offset + 1] = (uint8_t) (width & 0xff);
  output_image[header->image_height_offset] = (uint8_t) (height >> 8);
  output_image[header->image_height_offset + 1] = (uint8_t) (height & 0xff);
  file_size = header->length;

  fwrite(output_image, header->length, 1, output_file);

  tjpeg_init(&processor, width, height);

  //tjpeg_feed_data(&processor, width, height, yuvbuf);
  tjpeg_feed_data(&processor, width, height, ycbcr_image);

  for (int i = 0; i < 1; ++i) {

    do {
      //memcpy(yuvbuf, ycbcr_image_p, 640*2*8);
      //ycbcr_image_p+=640*2*8;
      bytes = tjpeg_write(&processor, output_image, 8192);
      fwrite(output_image, bytes, 1, output_file);
      fflush(output_file);
      file_size += bytes;
    } while (bytes > 0);
  }

  fclose(output_file);

  return 0;
}
