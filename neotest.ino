// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).

#include <Adafruit_NeoPixel.h>

#define PIN      6
#define N_LEDS 150

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN - 1, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
 // strip2.begin();
}

void loop() {

  chase(strip.Color(255, 0, 0)); // Red
  chase(strip.Color(0, 255, 0)); // Green
  chase(strip.Color(0, 0, 255)); // Blue
    chase(strip.Color(235, 255, 100)); // Red
  chase(strip.Color(5, 75, 3)); // Green
  chase(strip.Color(140, 2, 255)); // Blue
    chase(strip.Color(203, 32, 23)); // Red
  chase(strip.Color(50, 54, 200)); // Green
  chase(strip.Color(0, 2, 25)); // Blue
   /*   chase(strip2.Color(203, 32, 23)); // Red
  chase(strip2.Color(50, 54, 3)); // Green
  chase(strip2.Color(0, 2, 25)); // Blue*/
    chase(strip.Color(200, 200, 200)); // Blue
    
      chase2(strip.Color(255, 0, 0)); // Red
  chase2(strip.Color(0, 255, 0)); // Green
  chase2(strip.Color(0, 0, 255)); // Blue
    chase2(strip.Color(235, 255, 100)); // Red
  chase2(strip.Color(5, 75, 3)); // Green
  chase2(strip.Color(140, 2, 255)); // Blue
    chase2(strip.Color(203, 32, 23)); // Red
  chase2(strip.Color(50, 54, 200)); // Green
  chase2(strip.Color(0, 2, 25)); // Blue
    /*  chase2(strip2.Color(203, 32, 23)); // Red
  chase2(strip2.Color(50, 54, 3)); // Green
  chase2(strip2.Color(0, 2, 25)); // Blue*/
    chase2(strip.Color(200, 200, 200)); // Blue
  chase(strip.Color(0, 0, 0)); // Blue

/*
  bitshift(strip.Color(0, 255, 0), strip.Color(255, 0, 0), strip.Color(200, 100, 100));
  bitshift(strip.Color(0, 0, 255), strip.Color(255, 255, 0), strip.Color(0, 255, 0));
  bitshift(strip.Color(255, 100, 0), strip.Color(0, 0, 255), strip.Color(255, 100, 255));

*/
}

static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      //strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
   //   strip2.setPixelColor(i - 4, c); // Draw new pixel
    //  strip2.show();
      strip.show();
      delay(10);
  }
}

static void chase2(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
    //  strip2.setPixelColor(i - 4, c); // Draw new pixel
    //  strip2.setPixelColor(i - 8, 0); // Draw new pixel

//      strip2.show();
      strip.show();
      delay(5);
  }
}

static void bitshift(uint32_t c, uint32_t c2, uint32_t c3) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
     for(uint16_t j=0; j<strip.numPixels(); j++){
       if (i % 2)
       {
         if (j % 2)
         {
              strip.setPixelColor(j, c); // Draw new pixel
  //            strip2.setPixelColor(j, c); // Draw new pixel  
         }
         else
         {
              strip.setPixelColor(j, c2); // Draw new pixel
    //          strip2.setPixelColor(j, c2); // Draw new pixel
         }
       }
       else
       {
          if (j % 2)
         {
              strip.setPixelColor(j, c2); // Draw new pixel
      //        strip2.setPixelColor(j, c2); // Draw new pixel  
         }
         else
         {
              strip.setPixelColor(j, c); // Draw new pixel
        //      strip2.setPixelColor(j, c); // Draw new pixel
         }
       }
       delay(50);
     }
      strip.setPixelColor(i, c3); // Draw new pixel
    //  strip2.setPixelColor(i, c3); // Draw new pixel
    //  strip2.show();
      strip.show();
  }
        delay(300);
}
