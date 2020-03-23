#include <stdio.h>
#include <stdint.h>

int main(void)
{

  volatile uint16_t *fb = (volatile uint16_t *)0xffe00000;
  int x, y, i;
  int width = 480;
  int height = 320;

  for (i = 0; i < width; i++) {
    x = i;
    y = 0;
    fb[y * width + x] = 0xf800;
    y = height - 1;
    fb[y * width + x] = 0xf800;
  }

  for (i = 1; i < height -1; i++) {
    x = 0;
    y = i;
    fb[y * width + x] = 0xf800;
    x = width - 1;
    fb[y * width + x] = 0xf800;
  }


  while (1) {
    for (y = 1; y < height - 1; y++) {
      for (i = 0; i < 200; i++) {
        x = (y & 0xff) + i;
	fb[y * width + x] = 0xffff;
      }
      for (i = 0; i < 10; i++) {
        x = (y & 0xff) + i + 200;
	fb[y * width + x] = 0xf800;
      }
      for (i = 0; i < 10; i++) {
        x = (y & 0xff) + i + 210;
	fb[y * width + x] = 0x07e0;
      }
      for (i = 0; i < 10; i++) {
        x = (y & 0xff) + i + 220;
	fb[y * width + x] = 0x001f;
      }
    }
  }
  return 0;
}
