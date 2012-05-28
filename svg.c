#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#define PIE_SIZE 6

const char *colors[] = {
  "#C0F000",
  "#C0F0F0",
  "#C000F0",
  "#00FF00",
  "#00FFFF",
  "#0000FF",
  0
};

void
printPieGraph(const uint32_t radius, const uint8_t size[]) {
  int i;
  uint16_t total;
  long x, y, lastX, lastY;
  double tmp, seg;

  printf("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
  printf("\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  printf("<svg width=\"20cm\" height=\"20cm\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");

  lastX = radius;
  lastY = 0;
  seg = 0;
  total = 0;
  for (i = 0; i < PIE_SIZE; ++i) {
    total += size[i];
  }

  for (i = 0; i < PIE_SIZE; ++i) {
    if (size[i] != 0) {
      tmp = ((double)size[i])/((double)total) * 2 * M_PI;
      seg += tmp;
      x = (int) (cos(seg) * radius);
      y = (int) (sin(seg) * radius);
      if (tmp > M_PI) {
        printf("<path d=\"M 0,0 l %li,%li a %"PRIu32",%"PRIu32" 0 1,0 %li,%li z\" fill=\"%s\" stroke=\"none\" stroke-width=\"2\" stroke-linejoin=\"round\" />\n", \
                lastX, -lastY, radius, radius, x - lastX, lastY - y, colors[i]);
      } else {
        printf("<path d=\"M 0,0 l %li,%li a %"PRIu32",%"PRIu32" 0 0,0 %li,%li z\" fill=\"%s\" stroke=\"none\" stroke-width=\"2\" stroke-linejoin=\"round\" />\n", \
                lastX, -lastY, radius, radius, x - lastX, lastY - y, colors[i]);
      }
      lastX = x;
      lastY = y;
    }
  }
  printf("</svg>\n");
}

int main(void) {
  uint8_t s[] = { 4, 5, 6, 7, 8, 40 };
  uint32_t r = 100;
  printPieGraph(r, s);
}
