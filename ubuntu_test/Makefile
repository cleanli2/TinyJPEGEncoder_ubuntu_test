export CC = gcc           

TARGET = yuv640x480tojpeg
INC_FLAGS= -I../src -I.
C_SRC= \
m.c \
../src/dct.c \
../src/tiny_jpeg.c \
../src/tjpeg_buffer.c \
../src/tjpeg_image_chunk.c


C_OBJ=$(C_SRC:%.c=%.o)

CFLAGS =  -W -Wall $(INC_FLAGS) 

all:$(C_OBJ)
	$(CC) $(C_OBJ) -lm -o $(TARGET)

$(C_OBJ):%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm $(C_OBJ)
