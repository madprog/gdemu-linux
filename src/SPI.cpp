#include <stdio.h>
#include <time.h>

#include "SPI.h"

SPIClass SPI;

int random(int x, int y) {
  return random() % (y - x) + x;
}

void delay(unsigned long ms) {
  struct timespec ts = { ms / 1000, (ms % 1000) * 1000000 };
  printf("sleeping %ds %dns\n", ts.tv_sec, ts.tv_nsec);
  nanosleep(&ts, NULL);
}
