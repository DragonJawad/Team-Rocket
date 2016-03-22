//Code by Reichenstein7 (thejamerson.com)

//Edited by Benjamin Miron
//March 18, 2016

//Keyboard Controls:
//
// 1 -Motor 1 Left
// 2 -Motor 1 Stop
// 3 -Motor 1 Right
//
// 4 -Motor 2 Left
// 5 -Motor 2 Stop
// 6 -Motor 2 Right

// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Motor 1
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10; // Needs to be a PWM pin to be able to control motor speed

int on1 = 0;
int on2 = 0;

////////////////////////////////////////SETUP////////////////////////////////////////////////////
void setup() {  // Setup runs once per reset
  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);
  
  //Define L298N Dual H-Bridge Motor Controller Pins
  
  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  pinMode(dir1PinB,OUTPUT);
  pinMode(dir2PinB,OUTPUT);
  pinMode(speedPinB,OUTPUT);

}

void loop() {
  
  // Initialize the Serial interface:
  
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    int speed; // Local variable
    
    switch (inByte) {
      
      //______________Motor 1______________
      
      case '1': // Motor 1 Forward
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);
      Serial.print("Motor 1 Forward, Duty Cycle: "); // Prints out “Motor 1 Forward” on the serial monitor
      Serial.println(on1*10);
      Serial.println(); // Creates a blank line printed on the serial monitor
      break;
      
      case '2': // Motor 1 Stop (Freespin)
        digitalWrite(dir1PinA, LOW);
        digitalWrite(dir2PinA, LOW);
        Serial.println("Motor 1 Stop");
        Serial.println();
      break;
      
      
      case '3': // Motor 1 Reverse
        digitalWrite(dir2PinA, LOW);
        digitalWrite(dir1PinA, HIGH);
        Serial.print("Motor 1 Reverse, Duty Cycle: ");
        Serial.println(on1 * 10);
        Serial.println();
      break;

      case '+': //Increase the duty cycle
        if(on1 >= 10) on1 = 10;
        else on1 += 1;
        Serial.print("Duty Cycle Increased to: ");
        Serial.println(on1*10);
        Serial.println();
      break;

      case '-': //Decrease the duty cycle
        if(on1 <= 0) on1 = 0;
        else on1 -= 1;
        Serial.print("Duty Cycle Decreased to: ");
        Serial.println(on1*10);
        Serial.println();
      break;
      
      /*
      //______________Motor 2______________
      
      case '4': // Motor 2 Forward
      analogWrite(speedPinB, 255);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
      Serial.println("Motor 2 Forward");
      Serial.println("   ");
      break;
      
      case '5': // Motor 1 Stop (Freespin)
      analogWrite(speedPinB, 0);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
      Serial.println("Motor 2 Stop");
      Serial.println("   ");
      break;
      
      case '6': // Motor 2 Reverse
      analogWrite(speedPinB, 255);
      digitalWrite(dir2PinB, LOW);
      digitalWrite(dir1PinB, HIGH);
      Serial.println("Motor 2 Reverse");
      Serial.println("   ");
      break;
      */
      default:
      // turn all the connections off if an unmapped key is pressed:
      for (int thisPin = 2; thisPin < 11; thisPin++) {
        on1 = 0;
        digitalWrite(thisPin, LOW);
      }
    }
  }
  
  if(on1 >= 1){
    digitalWrite(speedPinA, HIGH);
    delayMicroseconds(on1*200);
    digitalWrite(speedPinA, LOW);
    delayMicroseconds(2000 - on1*200);
  }

  else{
    digitalWrite(speedPinA, LOW);
  }
}
