#include "Joystick.h"
#include <Keyboard.h>
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  20, 2,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  true, true, false,   //  Rx, Ry, no Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering
int button_pins[4] = {6, 7, 9, 14};//the pins on micro
int button_maps[4] = {11, 12, 9, 10};//the button to press
int no_head_button[3] = {8, 10, 15};//need input pullup
int no_head_maps[3] = {13, 14, 15};
bool stateofit = false;//only for tetri
int mode = 0;//0 is joystick, 1 is tetri

int sh_ld = 2;//shift register #1
int clock_enable = 3;//shift register #15
int clockpin = 4;//shift register #2
int data = 5;//shift register #7
//
byte get_data_from_74165() {
  byte input;
  // Write pulse to load pin
  digitalWrite(sh_ld, LOW);
  delayMicroseconds(5);
  digitalWrite(sh_ld, HIGH);
  delayMicroseconds(5);
  // Get data from 74HC165
  digitalWrite(clockpin, HIGH);
  digitalWrite(clock_enable, LOW);
  input = shiftIn(data, clockpin, LSBFIRST);
  digitalWrite(clock_enable, HIGH);
  return input;
}
bool bin[8] = {1, 1, 1, 1, 1, 1, 1, 1};
void turn_decimal_into_bin(int num) {
  for (int i = 7; i >= 0; i--) {
    bin[i] = 1;
    if (num % 2 == 0) {
      bin[i] = 0;
    }
    num /= 2;
  }
}

void setup() {
  Joystick.begin();
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(sh_ld, OUTPUT);
  pinMode(clock_enable, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(data, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  for (int i = 2; i < 4; i++) {
    pinMode(button_pins[i], INPUT);
  }
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
    
  pinMode(16, INPUT);
}

void loop() {
  while (mode == 0) {//joystick
    Joystick.begin(true);
    if (digitalRead(9)==HIGH && digitalRead(10)==LOW) {
      mode = 1;
      break;
    }
    Joystick.setXAxis(analogRead(A1));
    Joystick.setYAxis(1023-analogRead(A0));
    Joystick.setRxAxis(analogRead(A3));
    Joystick.setRyAxis(1023-analogRead(A2));

    if (digitalRead(9) == HIGH)
      Joystick.pressButton(9);
    else
      Joystick.releaseButton(9);

    if (digitalRead(14) == HIGH)
      Joystick.pressButton(10);
    else
      Joystick.releaseButton(10);
      
    if (digitalRead(6) == LOW)
      Joystick.pressButton(11);
    else
      Joystick.releaseButton(11);
      
    if (digitalRead(7) == LOW)
      Joystick.pressButton(12);
    else
      Joystick.releaseButton(12);

    if (digitalRead(8) == LOW)
      Joystick.pressButton(13);
    else
      Joystick.releaseButton(13);

    if (digitalRead(10) == LOW)
      Joystick.pressButton(14);
    else
      Joystick.releaseButton(14);

    if (digitalRead(15) == LOW)
      Joystick.pressButton(15);
    else
      Joystick.releaseButton(15);


    int input = get_data_from_74165();
    turn_decimal_into_bin(input);
    for (int i = 0; i < 8; i++) {
      if (bin[i] == false)
        Joystick.pressButton(i);
      else
        Joystick.releaseButton(i);
    }
  }





  
  while (mode == 1) {//tetri
    Joystick.releaseButton(14);
    Joystick.releaseButton(9);
    Joystick.end();

    if (digitalRead(14)==HIGH && digitalRead(15)==LOW) {
      mode = 0;
      break;
    }
    int input = get_data_from_74165();
    Serial.print("shift register:");
    Serial.println(input, BIN);
    turn_decimal_into_bin(input);
    for (int i = 0; i < 8; i++) {
      Serial.print(bin[i]);
    }
    Serial.println(' ');
  
  
    if (bin[0] == false)
      Keyboard.write(216);//left arrow

    else if (bin[1] == false) {
      Keyboard.write(217);//down arrow
      delay(25);
      continue;
    }

    else if (bin[2] == false)
      Keyboard.write(215);//right arrow

    else if (bin[7] == false || bin[3] == false) {
      Keyboard.write(218);// up arrow
      delay(180);
      continue;
    }

    else if (bin[4] == false)
      Keyboard.write(129);//left shift

    else if (bin[5] == false) {
      Keyboard.write(32);
      delay(200);
      continue;
    }
    delay(125);
  }
  while (stateofit == false) {
    if (digitalRead(14)==HIGH && digitalRead(15)==LOW) {
      mode = 0;
      break;
    }
    if (digitalRead(9)==HIGH && digitalRead(10)==LOW) {
      stateofit = true;
      break;
    }
  }
    
  
}

//pin 8 on micro could be broke!!!!
//code a situation to open and cancel joystick
