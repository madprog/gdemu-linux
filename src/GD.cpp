#include <SDL.h>
#include <time.h>

#include "SPI.h"
#include "GD.h"

#define RAM_SIZE 0x8000
static byte RAM[RAM_SIZE];

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300
#define WINDOW_ZOOM 2

void GDClass::begin() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(WINDOW_WIDTH * WINDOW_ZOOM, WINDOW_HEIGHT * WINDOW_ZOOM, 15, SDL_HWSURFACE|SDL_DOUBLEBUF);
}

void GDClass::end() {
  SDL_Quit();
}

void GDClass::waitvblank() {
}

void GDClass::copy(unsigned int addr, prog_uchar *src, int count) {
  addr %= RAM_SIZE;
  if(addr + count > RAM_SIZE) count = RAM_SIZE - addr;
  memcpy(RAM + addr, src, count);
}

void GDClass::fill(int addr, byte v, unsigned int count) {
  addr %= RAM_SIZE;
  if(addr + count > RAM_SIZE) count = RAM_SIZE - addr;
  memset(RAM + addr, v, count);
}

void GDClass::wr(unsigned int addr, byte v) {
  addr %= RAM_SIZE;
  RAM[addr] = v;
}

void GDClass::wr16(unsigned int addr, unsigned int v) {
  addr %= RAM_SIZE;
  //((uint16_t *)RAM)[addr >> 1] = v;
  RAM[addr + 1] = (v & 0xff00) >> 8;
  RAM[addr + 0] = (v & 0x00ff) >> 0;
}

#include "font8x8.h"
static byte stretch[16] = {
  0x00, 0x03, 0x0c, 0x0f,
  0x30, 0x33, 0x3c, 0x3f,
  0xc0, 0xc3, 0xcc, 0xcf,
  0xf0, 0xf3, 0xfc, 0xff
};

void GDClass::ascii() {
  long i;
  for (i = 0; i < 768; i++) {
    byte b = font8x8[i];
    byte h = stretch[b >> 4];
    byte l = stretch[b & 0xf];
    GD.wr(0x1000 + (16 * ' ') + (2 * i), h);
    GD.wr(0x1000 + (16 * ' ') + (2 * i) + 1, l);
  }
  for (i = 0x20; i < 0x80; i++) {
    GD.setpal(4 * i + 0, TRANSPARENT);
    GD.setpal(4 * i + 3, RGB(255,255,255));
  }
  GD.fill(RAM_PIC, ' ', 4096);
}

void GDClass::putstr(int x, int y, const char *s) {
  int len = strlen(s);
  int start = y * 64 + x;

  if(start + len > 4096) len = 4096 - start;
  for(int i = 0; i < len; ++ i) {
    wr(RAM_PIC + start + i, s[i]);
  }
}

void GDClass::setpal(int pal, unsigned int rgb) {
  wr16(RAM_PAL + (pal << 1), rgb);
}

void redraw_background(SDL_Surface *surface) {
  int min_x = (RAM[SCROLL_X] | ((RAM[SCROLL_X + 1] & 0x01) << 8)) >> 3;
  int min_y = (RAM[SCROLL_Y] | ((RAM[SCROLL_Y + 1] & 0x01) << 8)) >> 3;
  uint16_t *pixels = (uint16_t *)surface->pixels;

  for(int y = min_y; y < min_y + 38; ++ y) {
    for(int x = min_x; x < min_x + 51; ++ x) {
      byte char_idx = RAM[RAM_PIC + (y & 0x3f) * 64 + (x & 0x3f)];
      byte *char_data = RAM + RAM_CHR + (char_idx << 4);
      uint16_t *color_data = (uint16_t *)(RAM + RAM_PAL) + (char_idx << 2);

      int first_pixel_x = (x << 3) - (RAM[SCROLL_X] | ((RAM[SCROLL_X + 1] & 0x01) << 8));
      int first_pixel_y = (y << 3) - (RAM[SCROLL_Y] | ((RAM[SCROLL_Y + 1] & 0x01) << 8));
      uint16_t *first_pixel = pixels + WINDOW_WIDTH * WINDOW_ZOOM * WINDOW_ZOOM * first_pixel_y + WINDOW_ZOOM * first_pixel_x;

      for(int py = 0; py < 8; ++ py) {
        for(int px = 0; px < 8; ++ px) {
          int pixel_address_in_char = (py << 3) | px;
          int bit_shift = (6 - ((pixel_address_in_char & 3) << 1));
          int color_index = (char_data[pixel_address_in_char >> 2] & (3 << bit_shift)) >> bit_shift;
          if(first_pixel_x + px >= 0 && first_pixel_x + px < WINDOW_WIDTH
              && first_pixel_y + py >= 0 && first_pixel_y + py < WINDOW_HEIGHT) {
            for(int i = 0; i < WINDOW_ZOOM; ++ i) {
              for(int j = 0; j < WINDOW_ZOOM; ++ j) {
                first_pixel[WINDOW_WIDTH * WINDOW_ZOOM * (WINDOW_ZOOM * py + j) + WINDOW_ZOOM * px + i] = color_data[color_index];
              }
            }
          }
        }
      }
      //exit(0);
    }
  }
}

int main() {
  const struct timespec sleep_time = { 0, 10000000 }; // 10 ms
  byte do_exit = 0;
  SDL_Event event;
  SDL_Surface *surface;

  memset(RAM, 0, RAM_SIZE);

  setup();

  while(!do_exit && clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep_time, NULL) == 0) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          do_exit = 1;
      }
    }

    surface = SDL_GetVideoSurface();

    SDL_LockSurface(surface);
    redraw_background(surface);
    SDL_UnlockSurface(surface);

    SDL_Flip(surface);

    loop();
  }

  GD.end();
}
