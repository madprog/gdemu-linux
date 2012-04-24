#include <SPI.h>
#include <GD.h>

void setup()
{
  GD.begin();

  GD.ascii();
  for(int i = 0; i < 26; ++ i) GD.wr(RAM_PIC + i, 'A' + i);
  for(int i = 0; i < 26; ++ i) GD.wr(RAM_PIC + 64 + i, 'a' + i);
  for(int i = 0; i < 10; ++ i) GD.wr(RAM_PIC + 128 + i, '0' + i);
  GD.putstr(0, 3, "Hello, world!");
}

void loop()
{
}
