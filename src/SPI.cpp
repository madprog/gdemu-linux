#include "SPI.h"

SPIClass SPI;

int random(int x, int y) {
  return random() % (y - x) + x;
}
