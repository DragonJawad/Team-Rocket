#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define S1 5  //GPIO2
#define S2 4  //GPIO1
#define S3 3  //GPIO3
#define S4 2  //GPIO0
#define S5 1  //GPIO5
#define S6 0  //GPIO6
#define S7 7  //GPIO7
#define S8 8  //GPIO8
#define S9 9  //GPIO4

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
pinMode(S5, INPUT_PULLUP);
pinMode(S6, INPUT_PULLUP);
pinMode(S7, INPUT_PULLUP);
pinMode(S8, INPUT_PULLUP);
pinMode(S9, OUTPUT);

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
int lastlastS5=0;
int lastlastS6=0;
int lastS5=0;
int lastS6=0;
int lastlastS7=0;
int lastlastS8=0;
int lastS7=0;
int lastS8=0;

//delay(5);
  while(1){
 digitalWrite(S9, HIGH);
 //delay(1);  
 if(digitalRead(S3) && !digitalRead(S4)&& lastlastS3 && !lastlastS4 && lastS3 && !lastS4){
    Serial.println(digitalRead(S3));
    Serial.println(digitalRead(S4));
   // digitalWrite(S9, LOW); don't do this for blackout
    startShow(0);
    
    break;
  } // BLACKOUT
 else if(digitalRead(S1) && !digitalRead(S2)&& lastlastS1 && !lastlastS2 && lastS1 && !lastS2){
    Serial.println(digitalRead(S1));
    Serial.println(digitalRead(S2));
    digitalWrite(S9, LOW);
    startShow(1);
    break;
 } //B
 else if(!digitalRead(S1) && digitalRead(S2)&& !lastlastS1 && lastlastS2 && !lastS1 && lastS2){
    Serial.println(digitalRead(S1));
    Serial.println(digitalRead(S2));
    digitalWrite(S9, LOW);
    startShow(2);
    break;
 } //Y
 else if(!digitalRead(S3) && digitalRead(S4) && !lastlastS3 && lastlastS4 && !lastS3 && lastS4){
    Serial.println(digitalRead(S3));
    Serial.println(digitalRead(S4));
    digitalWrite(S9, LOW);
    startShow(3);
    break;
 } // show start
  // end of reg shows
 else if(!digitalRead(S5) && digitalRead(S6)&& !lastlastS5 && lastlastS6 && !lastS5 && lastS6){
    Serial.println(digitalRead(S5));
    Serial.println(digitalRead(S6));
    digitalWrite(S9, LOW);
    startShow(4);
    break;
 }// egg1
 else if(digitalRead(S5) && !digitalRead(S6) && lastlastS5 && !lastlastS6 && lastS5 && !lastS6){
    Serial.println(digitalRead(S3));
    Serial.println(digitalRead(S4));
    digitalWrite(S9, LOW);
    startShow(5);
    break;
 }// egg2
 else if(!digitalRead(S7) && digitalRead(S8)&& !lastlastS7 && lastlastS8 && !lastS7 && lastS8){
    Serial.println(digitalRead(S7));
    Serial.println(digitalRead(S8));
    digitalWrite(S9, LOW);
    startShow(6);
    break;
 }// egg3
 else if(digitalRead(S7) && !digitalRead(S8) && lastlastS7 && !lastlastS8 && lastS7 && !lastS8){
    Serial.println(digitalRead(S7));
    Serial.println(digitalRead(S8));
    digitalWrite(S9, LOW);
    startShow(7);
    break;
 }// egg4
lastlastS1=lastS1;
lastlastS2=lastS2;
lastS1=digitalRead(S1);
lastS2=digitalRead(S2);
lastlastS3=lastS3;
lastlastS4=lastS4;
lastS3=digitalRead(S3);
lastS4=digitalRead(S4);
lastlastS5=lastS5;
lastlastS6=lastS6;
lastS5=digitalRead(S5);
lastS6=digitalRead(S6);
lastlastS7=lastS7;
lastlastS8=lastS8;
lastS7=digitalRead(S7);
lastS8=digitalRead(S8);
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
            digitalWrite(S9, HIGH);
            break;
    case 2: colorWipe(strip.Color(255, 255, 0), 10);  // Y
            theaterChase(strip.Color(  127,   127, 0), 30); // Y
            colorWipe(strip.Color(0, 0, 0), 1); 
            digitalWrite(S9, HIGH);
            break;
    case 3: colorWipe(strip.Color(255, 0, 0), 10); // Red
             colorWipe(strip.Color(255,255,0), 10); // Yellow
             theaterChase(strip.Color(0, 255, 0), 40); // Green
             colorWipe(strip.Color(0, 0, 0), 1); 
             digitalWrite(S9, HIGH);
            break;
    case 4: theaterChaseRainbow(20);
            colorWipe(strip.Color(0, 0, 0), 1); 
            digitalWrite(S9, HIGH);
            break;
    case 5: colorWipe(strip.Color(255, 0, 0), 5); // Red
            colorWipe(strip.Color(255, 127, 0), 5); // Orange
            colorWipe(strip.Color(255,255,0), 5); // Yellow
            colorWipe(strip.Color(0, 255, 0), 5); // Green
            colorWipe(strip.Color(0, 0, 255), 5); // Blue
            colorWipe(strip.Color(127,0,255), 5); // Violet
            colorWipe(strip.Color(0, 0, 0), 1); 
            digitalWrite(S9, HIGH);
            break;
    case 6: rainbowCycle(10);
            colorWipe(strip.Color(0, 0, 0), 1); 
            digitalWrite(S9, HIGH);
            break;
    case 7: rainbow(5);
            theaterChase(strip.Color(255,0,255), 10); //PINK
            colorWipe(strip.Color(0, 0, 0), 1); 
            digitalWrite(S9, HIGH);
            break;
  }
}

