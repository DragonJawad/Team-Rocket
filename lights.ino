#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DATA_OUT 13
#define ACK 11
#define INTERRUPT 12



#define S1 2  //GPIO0
#define S2 3  //GPIO1
#define S3 4  //GPIO2
#define S4 5  //GPIO3
#define S5 6  //GPIO4
#define S6 7  //GPIO5
#define S7 8  //GPIO6
#define S8 9  //GPIO7


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(450, DATA_OUT, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {

  Serial.begin(9600);

  // Configure pins as outputs
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);
  pinMode(S5, INPUT_PULLUP);
  pinMode(S6, INPUT_PULLUP);
  pinMode(S7, INPUT_PULLUP);
  pinMode(S8, INPUT_PULLUP);
  pinMode(ACK, OUTPUT);
  pinMode(INTERRUPT, INPUT_PULLUP);

 //attachInterrupt(INTERRUPT, goal_scored, FALLING);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

/*void goal_scored(){
  //theaterChase2(strip.Color(0, 0, 0), 1);
  Serial.println("Interrupted");
  loop();
}*/
void loop() {
 Serial.println("LOOP");
  // Always keep ACK off when not in use
  digitalWrite(ACK, HIGH); // ACK off
  delay(50);


  // EGG 4 Doubles as the starting show when the game begins
  
    // EGG 4
  if(!digitalRead(S7)){
    digitalWrite(ACK, LOW); // ACK on 
    startShow(7);         
  } // EGG 4

  
  // BLUE
  else if(!digitalRead(S1)){
    digitalWrite(ACK, LOW); // ACK on 
    startShow(1);         
  } //BLUE

  // MAIZE
  else if(!digitalRead(S2)){
    digitalWrite(ACK, LOW); // ACK on 
    startShow(2);         
  } // MAIZE

  // START GAME
  else if(!digitalRead(S3)){
    digitalWrite(ACK, LOW); // ACK on 
    startShow(3);         
  } // START GAME


  
  // EASTER EGG SHOWS
  
  // EGG 1
  else if(!digitalRead(S5)){
    digitalWrite(ACK, LOW); // ACK on 
    startShow(4);         
  } // EGG 1

  // EGG 2
  else if(!digitalRead(S4)){
    digitalWrite(ACK, LOW); // ACK on 
    startShow(5);         
  } // TIE

  // EGG 3
  else if(!digitalRead(S6)){
    digitalWrite(ACK, LOW); // ACK on 
    startShow(6);         
  } // EGG 3
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
    //delay(wait);
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

void theaterChase2(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 1; q++) {
      for (int i=0; i < strip.numPixels(); i=i+1) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+1) {
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
    case 1: colorWipe(strip.Color(0, 0, 255), 5);  // Blue
            theaterChase(strip.Color(  0,   0, 127), 30); // Blue
             theaterChase2(strip.Color(0, 0, 0), 20);
            digitalWrite(ACK, HIGH);
            break;
    case 2: colorWipe(strip.Color(255, 255, 0), 5);  // Y
            theaterChase(strip.Color(  127,   127, 0), 30); // Y
             theaterChase2(strip.Color(0, 0, 0), 20);
            digitalWrite(ACK, HIGH);
            break;
    case 3: //colorWipe(strip.Color(255, 0, 0), 0.001); // Red
             //colorWipe(strip.Color(255,255,0), 0.001); // Yellow
             //strip.Color(255, 0, 0);
             //strip.Color(255,255,0);
             theaterChase2(strip.Color(255, 0, 0), 80);
             theaterChase2(strip.Color(255, 255, 0), 80);
             digitalWrite(ACK, HIGH);
             theaterChase(strip.Color(0, 255, 0), 40); // Green
             theaterChase2(strip.Color(0, 0, 0), 20);
             //colorWipe(strip.Color(0, 0, 0), 1); 
            break;
    case 4: theaterChase2(strip.Color(155,48,255),20);//PURPLE
            theaterChase2(strip.Color(255,0,255),20); //PINK
            theaterChase2(strip.Color(155,48,255),20);//PURPLE
            theaterChase2(strip.Color(255,0,255),20); //PINK
            theaterChase2(strip.Color(0, 0, 0), 20);
            digitalWrite(ACK, HIGH);
            break;
    case 5: theaterChase(strip.Color(  127,   127, 0), 30);
            theaterChase(strip.Color(  0,   0, 127), 30);
            theaterChase(strip.Color(  127,   127, 0), 30);
            theaterChase(strip.Color(  0,   0, 127), 30);
             theaterChase2(strip.Color(0, 0, 0), 20);
            digitalWrite(ACK, HIGH);
            break;
    case 6: rainbow(10);
            theaterChase2(strip.Color(0, 0, 0), 20);
            digitalWrite(ACK, HIGH);
            break;
    case 7: rainbow(5);
            theaterChase(strip.Color(255,0,255), 10); //PINK
            theaterChase2(strip.Color(0, 0, 0), 20);
            digitalWrite(ACK, HIGH);
            break;
  }
}

