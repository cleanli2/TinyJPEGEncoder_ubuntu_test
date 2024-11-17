#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tiny_jpeg.h"
#include "tjpeg_image_chunk.h"

void mem_print(const char*buf, uint32_t ct_start, uint32_t len)
{
    const char*line_stt = buf;
    uint32_t left=len, line_len;

    printf("\nMemShow Start:");
    while(left){
        int j, li;
        line_len = left>16?16:left;
        li=line_len;
        printf("\n%08x: ", ct_start);
        j=0;
        while(li--){
            printf("%02x",line_stt[j]&0xff);
            printf(j == 7 ? "-":" ");
            j++;
        }
        li=line_len;
        j=0;
        printf(" ");
        while(li--){
            if(line_stt[j]>=0x20 && line_stt[j]<0x7f){
                printf("%c", line_stt[j]);
            }
            else{
                printf("_");
            }
            j++;
        }
        left-=line_len;
        line_stt+=line_len;
        ct_start+=line_len;
    }
    printf("\nMemShow End:\n");
}


#define prt_var(x) printf(#x"=%d\n", x)
uint8_t   ycbcr_image[640*480*2];
uint8_t   yuvbuf[640*8*2];
uint8_t   output_image[8196];
uint8_t   *ycbcr_image_p=ycbcr_image;
int main(int argc, char **argv)
{
  jpeg_proc_t               processor;
  const jpeg_file_header_t  *header;

  int       width, height, offset;
  uint8_t   *input_image;
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
  ycbcr_image[640*8*2]=0x55;
  ycbcr_image[640*8*2+1]=0x56;
  ycbcr_image[640*8*2+2]=0x57;
  ycbcr_image[640*8*2+3]=0x58;
  ycbcr_image[640*8*4-1]=0xaa;
  ycbcr_image[640*8*4-2]=0xa9;
  ycbcr_image[640*8*4-3]=0xa8;
  ycbcr_image[640*8*4-4]=0xa7;
  file_size = header->length;

  fwrite(output_image, header->length, 1, output_file);

  tjpeg_init(&processor, width, height);

#ifndef LIMITED_INPUT_BUFF
  tjpeg_feed_data(&processor, width, height, ycbcr_image);
#else
  tjpeg_feed_data(&processor, width, height, yuvbuf);
#endif

  update_yuvbuf();
  for (int i = 0; i < 1; ++i) {

    do {
      bytes = tjpeg_write(&processor, output_image, 8192);
      fwrite(output_image, bytes, 1, output_file);
      fflush(output_file);
      file_size += bytes;
    } while (bytes > 0);
  }

  fclose(output_file);

  return 0;
}


void update_yuvbuf()
{
      memcpy(yuvbuf, ycbcr_image_p, 640*2*8);
      ycbcr_image_p+=640*2*8;
      printf("===%02x %02x %02x %02x\n", yuvbuf[0], yuvbuf[1],yuvbuf[2], yuvbuf[3]);
      printf("===%02x %02x %02x %02x\n", yuvbuf[640*8*2-1], yuvbuf[640*8*2-2],yuvbuf[640*8*2-3], yuvbuf[640*8*2-4]);
}
