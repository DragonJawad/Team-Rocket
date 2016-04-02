#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define S1 5
#define S2 4
#define S3 3
#define S4 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
     Serial.begin(9600);
pinMode(S1, INPUT_PULLUP);
pinMode(S2, INPUT_PULLUP);
pinMode(S3, INPUT_PULLUP);
pinMode(S4, INPUT_PULLUP);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
int lastlastS1=0;
int lastlastS2=0;
int lastS1=0;
int lastS2=0;
int lastlastS3=0;
int lastlastS4=0;
int lastS3=0;
int lastS4=0;

//delay(5);
  while(1){
 //delay(1);  
 if(digitalRead(S3) && !digitalRead(S4)&& lastlastS3 && !lastlastS4 && lastS3 && !lastS4){
    Serial.println(digitalRead(S3));
    Serial.println(digitalRead(S4));
    startShow(0);
    
    break;
  }
 else if(digitalRead(S1) && !digitalRead(S2)&& lastlastS1 && !lastlastS2 && lastS1 && !lastS2){
    Serial.println(digitalRead(S1));
    Serial.println(digitalRead(S2));
    startShow(1);
    break;
 }
 else if(!digitalRead(S1) && digitalRead(S2)&& !lastlastS1 && lastlastS2 && !lastS1 && lastS2){
    Serial.println(digitalRead(S1));
    Serial.println(digitalRead(S2));
    startShow(2);
    break;
 }
 else if(!digitalRead(S3) && digitalRead(S4) && !lastlastS3 && lastlastS4 && !lastS3 && lastS4){
    Serial.println(digitalRead(S3));
    Serial.println(digitalRead(S4));
    startShow(3);
    break;
 }

lastlastS1=lastS1;
lastlastS2=lastS2;
lastS1=digitalRead(S1);
lastS2=digitalRead(S2);
lastlastS3=lastS3;
lastlastS4=lastS4;
lastS3=digitalRead(S3);
lastS4=digitalRead(S4);
  /* int sensorValue0 = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage0 = sensorValue0 * (5.0 / 1023.0);
  // print out the value you read:
  delay(30);
  Serial.println(sensorValue0);
   int sensorValue1 = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage1 = sensorValue1 * (3.3 / 1023.0);
  // print out the value you read:
   delay(30);
  //Serial.println(voltage1); */
}
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 1);    // Black/off
            break;
    case 1: colorWipe(strip.Color(0, 0, 255), 10);  // Blue
            theaterChase(strip.Color(  0,   0, 127), 30); // Blue
            colorWipe(strip.Color(0, 0, 0), 1); 
            break;
    case 2: colorWipe(strip.Color(255, 255, 0), 10);  // Y
            theaterChase(strip.Color(  127,   127, 0), 30); // Y
            colorWipe(strip.Color(0, 0, 0), 1); 
            break;
    case 3: colorWipe(strip.Color(255, 0, 0), 10); // Red
             colorWipe(strip.Color(255,255,0), 10); // Yellow
             theaterChase(strip.Color(0, 255, 0), 40); // Green
              colorWipe(strip.Color(0, 0, 0), 1); 
            break;
    case 4: rainbowCycle(20);
            colorWipe(strip.Color(0, 0, 0), 1); 
            break;
    case 5: theaterChase(strip.Color(127,   0,   0), 50); // Red
            break;
    case 6: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
            break;
    case 7: rainbow(20);
            break;
    case 8: rainbowCycle(20);
            break;
    case 9: theaterChaseRainbow(50);
            break;
  }
}

