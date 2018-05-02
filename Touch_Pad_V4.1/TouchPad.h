/*******************************************************************************

  MIT License

  Copyright (c) 2018, JacoBurge

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

 *******************************************************************************

  WARNING:  Changing the code in this file could prevent your TouchPad from working!

 *******************************************************************************/

//Libraries:
#include <EEPROM.h>
#include <Wire.h>
#include <avr/pgmspace.h>

//Compiler directives:
#define COMPATDELAY 1 //Defines increments in compatibility delay in mS
#define VIBRATE_LENGTH 38 //Defines number of interrupts motor will vibrate for, must be bigger than 8 for correct operation
#define SAFE_MODE //Disables OTG programming after first key press, comment out to disable
//#define RESET //Uncomment to reset EEPROM, upload, and then comment out again to prevent EEPROM being erased on every power cycle

//Function definitions
bool touchDetection();
void decodeArray(uint16_t dataIn, uint8_t *column, uint8_t *row);
void toggleKey(uint8_t key);
void GUI_keySelected(uint8_t column, uint8_t row);
void GUI_keyClear();
void GUI_prog();
void GUI_prog_leds();
void GUI_menu();
void GUI_clear_lines(uint8_t line);
void GUI_replace_lines();
void GUI_replace_functions(uint8_t line);
void GUI_clear();
void memoryKeyboard(uint16_t address, uint8_t stringLength, uint8_t printl);
void interruptSetup();
ISR(TIMER1_COMPA_vect);
void pulseLED();
uint8_t isTouchChangeDetected();
uint8_t readDataFromTS(uint8_t reg);
uint8_t writeDataToTS(uint8_t reg, uint8_t data);
uint8_t checkTSPres();
uint8_t capSetup();
void touchClearCurrentDetections();
uint16_t touchDetectionRoutine();
void keyPress(int x, int y, int commandSet);


const uint8_t SENr[6] = {1, 2, 3, 5, 6, 7};//Maps capacitive pads to pins
const uint8_t SENc[6] = {0, 4, 8, 9, 10, 11};

volatile uint8_t LEDs[6][6];//Stores current LED values
volatile uint8_t col = 0;//Keeps track of current multiplex column for LEDs

//Stores how to display each key in the 'On the go programmer' in flash
const uint8_t KEYS[100][5] PROGMEM = {"  0  ", "  1  ", "  2  ", "  3  ", "  4  ", "  5  ", "  6  ", "  7  ", "  8  ", "  9  ",
                                      "  a  ", "  b  ", "  c  ", "  d  ", "  e  ", "  f  ", "  g  ", "  h  ", "  i  ", "  j  ", "  k  ", "  l  ", "  m  ",
                                      "  n  ", "  o  ", "  p  ", "  q  ", "  r  ", "  s  ", "  t  ", "  u  ", "  v  ", "  w  ", "  x  ", "  y  ", "  z  ",
                                      " F 1 ", " F 2 ", " F 3 ", " F 4 ", " F 5 ", " F 6 ", " F 7 ", " F 8 ", " F 9 ", " F10 ", " F11 ", " F12 ",
                                      "L CTR", "L SHI", "L ALT", "L GUI", "PG UP", "R CTR", "R SHI", "R ALT", "R GUI", "PG DO", "LEFT ", " BSP ",
                                      " ESC ", " HOM ", " UP  ", "DOWN ", " TAB ", " INS ", " END ", "RIGHT", " RET ", " DEL ", "CAPS ", "SPACE",
                                      " 1mS ", "10mS ", "100mS", "  1S ", "  5S ", " 10S ", " x2  ", " x3  ", " x4  ", " x5  ", " x10 ", " x20 "
                                     };

//Stores ascii values for each keypress in flash
const uint8_t KEYS_PRESS[128] PROGMEM = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57,//9
                                         97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,//22
                                         110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,//35
                                         194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,//47
                                         128, 129, 130, 131, 211, 132, 133, 134, 135, 214, 216, 178,//59
                                         177, 210, 218, 217, 179, 209, 213, 215, 176, 212, 193, 32,//71(32)
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //83
                                         35, 38, 64, 92, 47, 36, 37, 42, 43, 45, 60, 61, 62, 63, 59, 46, 33, //100
                                         123, 125, 91, 93, 40, 41, 58, 44, 34, 39, 94, 95, 96, 124, 126, 30, 31 //117
                                        };

//Stores key locations for 'On the go programmer' menu in flash
const uint8_t KEYS_LOCATION[4][6][5] PROGMEM = {{{52, 51 , 50 , 49 , 48}, {57, 56 , 55 , 54 , 53}, {88, 87 , 86 , 85 , 84}, {61, 60 , 59 , 58 , 71}, {66, 65 , 64 , 63 , 62}, {70, 69 , 68 , 67 , 255}},
  {{34, 28 , 22 , 16 , 10}, {35, 29 , 23 , 17 , 11}, {97, 30 , 24 , 18 , 12}, {98, 31 , 25 , 19 , 13}, {99, 32 , 26 , 20 , 14}, {100, 33 , 27 , 21 , 15}},
  {{42, 36 , 91 , 6 , 0}, {43, 37 , 92 , 7 , 1}, {44, 38 , 93 , 8 , 2}, {45, 39 , 94 , 9 , 3}, {46, 40 , 95 , 89 , 4}, {47, 41 , 96 , 90 , 5}},
  {{113, 107 , 101 , 78 , 72}, {114, 108 , 102 , 79 , 73}, {115, 109 , 103 , 80 , 74}, {117 , 110 , 104 , 81 , 75}, {116, 111 , 105 , 82 , 76}, {255, 112 , 106 , 83 , 77}}
};
//83

//Stores 'On the go programmer' strings in flash
const uint8_t STRINGS[43][50] PROGMEM = {
  "/       /       / * * * /       /       /       /",
  "/ L CTR / R CTR /   #   / SPACE /  UP   /RELEASE/",
  "/ L SHI / R SHI /   &   / LEFT  / DOWN  / RIGHT /",
  "/ L ALT / R ALT /   @   /  BSP  /  TAB  /  RET  /",
  "/ L GUI / R GUI /   \\   /  ESC  /  INS  /  DEL  /",
  "/ PG UP / PG DO /   /   /  HOM  /  END  /  CAPS /",
  "/ <- -  /  DEL  / FUNCT /  abc  / F/123 / EXTRA /",
  "- - - - - - - - - - - - - - - - - - - - - - - - -",//7
  "/       /       /       /       /       /       /",//8
  "/       /       /       / * * * /       /       /",//9
  "/   a   /   b   /   c   /   d   /   e   /   f   /",
  "/   g   /   h   /   i   /   j   /   k   /   l   /",
  "/   m   /   n   /   o   /   p   /   q   /   r   /",
  "/   s   /   t   /   u   /   v   /   w   /   x   /",
  "/   y   /   z   /   ?   /   ;   /   .   /   !   /",
  "/       /       /       /       / * * * /       /",
  "/   0   /   1   /   2   /   3   /   4   /   5   /",
  "/   6   /   7   /   8   /   9   /   $   /   %   /",
  "/  F 1  /  F 2  /  F 3  /  F 4  /  F 5  /  F 6  /",
  "/  F 7  /  F 8  /  F 9  /  F10  /  F11  /  F12  /",//19
  "/       /       /       /       /       / * * * /",
  "!!! Warning !!!   When in the programming modes, ",//21
  "TouchPad takes control of your keyboard, this co ",
  "uld EDIT or DELETE text or files if you leave thi",
  "s window.   !!! Warning !!!                      ",//24
  "Now in macro program mode, for command set       ",//25
  "Use the following menus to assign keys on the key",//26
  "pad to macro keys for your program               ",//27
  "Now in LED program mode, for command set         ",//28
  "Touch LEDs to toggle them and swipe down when com",//29
  "plete                                            ",//30
  "Programming complete, you can now close the notep",
  "ad and safely resume work...                     ",
  "No key selected.                                 ",//33
  "Key                                              ",
  ",                                                ",
  " selected:                                       ",
  "/  1mS  / 10mS  / 100mS /   1S  /   5S  /  10S  /",//37
  "/  x2   /  x3   /  x4   /  x5   /  x10  /  x20  /",
  "/   *   /   +   /   -   /   <   /   =   /   >   /",//39
  "/   {   /   }   /   [   /   ]   /   (   /   )   /",
  "/   :   /   ,   /   \"   /   '   /   ^   /   _   /",
  "/   `   /   |   /   ~   /   "//42
};

volatile enum GUI {NORMAL, PROG, PROG1, PROG_LEDS, PROG_LEDS1, CHANGE_MODE, NORMAL1, FACTORY} state;//State machine variable for UI
volatile uint32_t blinkerC = 0; //Timing for blinking LED
volatile uint8_t blinker = 0, blinkerD = 0; //Counter and direction for blinking LED
volatile uint8_t vibrate = 0; //Trigger vibration in interrupt
uint8_t progR = 10, progC = 10;//10 when no key selected, row and column when key selected
uint8_t GUI_Menu = 0;//Stores current GUI menu, 0 when none displayed, 1-4 for menus, 5 for clear

int8_t currentMode = 0;//Stores current mode 0 - 4
uint16_t compat = 0;//Compatibility mode off;
uint8_t setupEnable = 1;//No entering prog mode after first key press
uint8_t factoryTest[12];//Used for factory testing of keys

//=========================================================================================================
//=========================================================================================================
// Setup
//=========================================================================================================
//=========================================================================================================

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Keyboard.begin();
  Mouse.begin();

  pinMode(7, OUTPUT);//Motor enable
  pinMode(6, OUTPUT);//Motor PWM

  pinMode(11, OUTPUT);//Power LED

  pinMode(A0, OUTPUT);//LEDs Columns
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  pinMode(12, OUTPUT);//LEDs Rows
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(0, INPUT);//Capacitive Interrupt

  if (EEPROM.read(1023) != 20) {//Setup EEPROM when first power up
    digitalWrite(11, 1);
    for (uint8_t j = 0; j < 5; j++) {//Setup EEPROM for default TouchPad
      for (uint8_t i = 0; i < 180; i++) {
        EEPROM.write((j * 200) + i, 255);
      }
    }
    for (uint8_t j = 0; j < 5; j++) {
      uint8_t i = 0;
      for ( i = 180; i < 181 + j; i++) {
        EEPROM.write((j * 200) + i, 1);
      }
      for ( i; i < 186; i++) {
        EEPROM.write((j * 200) + i, 0);
      }
    }
    EEPROM.write(1023, 20);//Setup complete
    EEPROM.write(1022, 0);//Current Mode
    EEPROM.write(1021, 20);//Factory Pass not complete
    EEPROM.write(1020, 0); //Compat setting

    state = FACTORY;
    for (int i = 0; i < 12; i++) {//All ones must be cleared to pass. set them here
      factoryTest[i] = 1;
    }
  }

#ifdef RESET
  EEPROM.write(1023, 0);//Perform eeprom reset, setup will be performed on next power cycle
#endif

  if (EEPROM.read(1021) == 20) {//If factory test not complete, put it in this mode
    state = FACTORY;
    for (int i = 0; i < 12; i++) {
      factoryTest[i] = 1;
    }
  }

#ifdef ENABLE_MULTIPLE_COMMAND_SET
  currentMode = EEPROM.read(1022);//Load current mode from EEPROM
#else
  currentMode = 0;
#endif

  compat = EEPROM.read(1020);//Load compat setting from EEPROM

  if (currentMode > 4) {//Limit current mode to 0-5
    currentMode = 4;
  }
  else if (currentMode < 0) {
    currentMode = 0;
  }

  interruptSetup();//Setup LED matrix driver interrupt

  for (uint8_t i = 0; i < 6; i++) {//Clear all LEDs
    for (uint8_t j = 0; j < 6; j++) {
      LEDs[j][i] = 0;
    }
  }

  if (state != FACTORY) {//If not still in factory test
    delay(2000);
    digitalWrite(11, HIGH);//Light power LED briefly, signals calibrating
    delay(100);
    digitalWrite(11, LOW);
  }

  capSetup();//Setup capacitive touch IC
  writeDataToTS(0x06, 0x12);//Calibrate capacitive touch IC

  if (state != FACTORY) {//Set LEDs to match the current command set
    delay(1000);
    for (uint8_t i = 0; i < 6; i++) {
      for (uint8_t j = 0; j < 6; j++) {
        LEDs[j][i] = bitRead(EEPROM.read((currentMode * 200) + 180 + (j)), i);//Load values from EEPROM
      }
    }
  }
}

//=========================================================================================================
//=========================================================================================================
// Loop
//=========================================================================================================
//=========================================================================================================

void loop() {

  switch (state) {//Main state machine for operation
    case NORMAL:
      touchDetection();//Run touchDetection
      analogWrite(11, blinker);//Update LED
      if (blinker < 255) {
        if (millis() > blinkerC) {
          analogWrite(11, blinker);
          blinker++;
          blinkerC = millis() + 1;
        }
      }
      break;
    case PROG:
      touchDetection();//Run touchDetection
      if (millis() > blinkerC) {//Update LED
        analogWrite(11, blinker);
        pulseLED();
        blinkerC = millis() + 4;
      }
      break;
    case PROG_LEDS:
      touchDetection();//Run touchDetection
      if (millis() > blinkerC) {//Update LED
        analogWrite(11, blinker);
        pulseLED();
        blinkerC = millis() + 1;
      }
      break;
    case FACTORY://Only occurs when EEPROM reset.
      //Wait for all inputs to be tested
      if (factoryTest[0] + factoryTest[1] + factoryTest[2] + factoryTest[3] + factoryTest[4] + factoryTest[5] + factoryTest[6] + factoryTest[7] + factoryTest[8] + factoryTest[9] + factoryTest[10] + factoryTest[11] > 0) {
        touchDetection();//Run touchDetection
        if (millis() > blinkerC) {
          digitalWrite(11, !digitalRead(11));
          blinkerC = millis() + 300;
        }
      }
      else {//Test all LEDs
        for (uint8_t i = 0; i < 6; i++) {
          for (uint8_t j = 0; j < 6; j++) {
            LEDs[j][i] = 0;
          }
        }
        for (uint8_t i = 0; i < 6; i++) {
          for (uint8_t j = 0; j < 6; j++) {
            LEDs[j][i] = 1;
          }
          delay(200);
          for (uint8_t j = 0; j < 6; j++) {
            LEDs[j][i] = 0;
          }
        }
        for (uint8_t i = 0; i < 6; i++) {
          for (uint8_t j = 0; j < 6; j++) {
            LEDs[i][j] = 1;
          }
          delay(200);
          for (uint8_t j = 0; j < 6; j++) {
            LEDs[i][j] = 0;
          }
        }
        for (uint8_t i = 0; i < 6; i++) {//Load current LED pattern
          for (uint8_t j = 0; j < 6; j++) {
            LEDs[j][i] = bitRead(EEPROM.read((currentMode * 200) + 180 + (j)), i);
          }
        }
        state = NORMAL;//Enter normal operation
        EEPROM.write(1021, 0);//Set factory test pass
      }
      break;
  }
}

//=========================================================================================================
//=========================================================================================================
// Check capacitive array - Returns 1 for positive ID, 0 for negative ID
//=========================================================================================================
//=========================================================================================================

bool touchDetection() {
  if (isTouchChangeDetected()) {
    uint16_t dataIn, row = 10, column = 10;
    dataIn = touchDetectionRoutine();
    if ((dataIn & 0b111100010001) > 0 && (dataIn & 0b000011101110) > 0) {
      uint8_t column, row;
      decodeArray(dataIn, &column, &row);

      if (column != 10 && row != 10) {
        vibrate = VIBRATE_LENGTH;//Trigger vibration

        LEDs[column][row] = 1;
        dataIn = touchDetectionRoutine();
        while ((dataIn & (0b1 << SENr[row])) > 0  || (dataIn & (0b1 << SENc[column])) > 0) {
          dataIn = touchDetectionRoutine();
#ifdef ENABLE_MULTIPLE_COMMAND_SET//disable multipe command sets
          //Horizontal swipe detect
          if ((dataIn & (0b1 << SENr[row])) > 0  && (dataIn & (0b1 << SENc[column])) == 0 && (dataIn & 0b111100010001) > 0 && state == NORMAL) {
            uint8_t columnNew, rowNew;
            decodeArray(dataIn, &columnNew, &rowNew);

            if (columnNew > column) {
              //===============================================================================================Swipe Right...
              for (uint8_t i = column; i < 6; i++) {
                LEDs[i][row] = 1;
                delay(50);
              }
              currentMode--;
              if (currentMode < 0) {
                currentMode = 4;
              }
              EEPROM.write(1022, currentMode);
              //Keyboard.println(currentMode);

              digitalWrite(7, HIGH);
              analogWrite(6, 180);
              for (int8_t i = 6; i >= 0; i--) {//Change
                if (i < 4) {
                  analogWrite(6, 127);
                  digitalWrite(7, LOW);
                }
                for (uint8_t j = 0; j < 6; j++) {
                  for (uint8_t k = 0; k < 6; k++) {
                    LEDs[j][k] = 0;
                    if (j - i >= 0) {
                      LEDs[j - i][k] = bitRead(EEPROM.read((currentMode * 200) + 180 + (j)), k);
                    }
                  }
                }
                delay(100);
              }

              state = CHANGE_MODE;

              //===============================================================================================...Swipe Right
            }
            else {
              //===============================================================================================Swipe Left...
              for (int8_t i = column; i >= 0; i--) {
                LEDs[i][row] = 1;
                delay(50);
              }

              currentMode++;
              if (currentMode > 4) {
                currentMode = 0;
              }
              EEPROM.write(1022, currentMode);
              //Keyboard.println(currentMode);

              digitalWrite(7, HIGH);
              analogWrite(6, 180);
              for (int8_t i = 6; i >= 0; i--) {//Change
                if (i < 4) {
                  analogWrite(6, 127);
                  digitalWrite(7, LOW);
                }
                for (uint8_t j = 0; j < 6; j++) {
                  for (uint8_t k = 0; k < 6; k++) {
                    LEDs[j][k] = 0;
                  }
                }
                for (uint8_t j = 0; j < 6; j++) {
                  for (uint8_t k = 0; k < 6; k++) {
                    if (j + i <= 5 ) {
                      LEDs[j + i][k] = bitRead(EEPROM.read((currentMode * 200) + 180 + (j)), k);
                    }
                  }
                }
                delay(100);
              }
              state = CHANGE_MODE;

              //===============================================================================================...Swipe Left
            }

            while ((dataIn & (0b1 << SENr[row])) > 0  || (dataIn & (0b1 << SENc[column])) > 0) {
              dataIn = touchDetectionRoutine();
            }
            break;
          }
#endif
#ifdef ENABLE_OTG_PROGRAMMER
          //Vertical swipe detect
          if ((dataIn & (0b1 << SENc[column])) > 0  && (dataIn & (0b1 << SENr[row])) == 0 && (dataIn & 0b000011101110) > 0) {
            uint8_t columnNew, rowNew;
            decodeArray(dataIn, &columnNew, &rowNew);
            if (rowNew > row) {
              //===============================================================================================Swipe Up...

              if (state == NORMAL) {
                if (setupEnable == 1) {//Only enter setup mode immediately after power up!
                  for (uint8_t i = row; i < 6; i++) {
                    LEDs[column][i] = 1;
                    delay(50);
                  }

                  digitalWrite(7, HIGH);
                  analogWrite(6, 180);
                  for (int8_t i = 6; i >= 0; i--) {
                    for (uint8_t j = 0; j < 6; j++) {
                      for (uint8_t k = 0; k < 6; k++) {
                        LEDs[j][k] = 0;
                        if (k - i >= 0) {
                          LEDs[j][k - i] = bitRead(EEPROM.read((currentMode * 200) + 180 + (j)), k);
                        }
                      }
                    }
                    delay(100);
                  }
                  analogWrite(6, 127);
                  digitalWrite(7, LOW);
                  state = PROG_LEDS1;
                }
              }
              else if (state == PROG) {
                digitalWrite(7, HIGH);
                analogWrite(6, 180);
                for (uint8_t i = row; i < 6; i++) {
                  LEDs[column][i] = 1;
                  delay(100);
                }
                analogWrite(6, 127);
                digitalWrite(7, LOW);
                state = NORMAL1;
                GUI_clear();
              }

              //===============================================================================================...Swipe Up
            }
            else {
              //===============================================================================================Swipe Down...
              if (state == NORMAL) {
                if (setupEnable == 1) {//Only enter setup mode immediately after power up!
                  digitalWrite(7, HIGH);
                  analogWrite(6, 180);
                  for (int8_t i = row; i >= 0; i--) {
                    LEDs[column][i] = 1;
                    delay(100);
                  }
                  analogWrite(6, 127);
                  digitalWrite(7, LOW);
                  state = PROG1;

                }
              }
              else if (state == PROG) {
                digitalWrite(7, HIGH);
                analogWrite(6, 180);
                for (int8_t i = row; i >= 0; i--) {
                  LEDs[column][i] = 1;
                  delay(100);
                }
                analogWrite(6, 127);
                digitalWrite(7, LOW);
                Keyboard.releaseAll();
                compat += COMPATDELAY;
                EEPROM.write(1020, compat);
                GUI_clear();
                Keyboard.press(129);//Shift
                for (uint8_t i = 0; i < 3; i++) {
                  toggleKey(218);//Up
                }
                Keyboard.release(129);//Shift
                toggleKey(178);//Backspace
                state = PROG1;
              }

              else if (state == PROG_LEDS) {

                for (int8_t i = row; i >= 0; i--) {
                  LEDs[column][i] = 1;
                  delay(50);
                }
                digitalWrite(7, HIGH);
                analogWrite(6, 180);
                for (uint8_t i = 0; i <= 6; i++) {



                  for (uint8_t j = 0; j < 6; j++) {
                    for (uint8_t k = 0; k < 6; k++) {
                      LEDs[j][k] = 0;
                      if (k - i >= 0) {
                        LEDs[j][k - i] = bitRead(EEPROM.read((currentMode * 200) + 180 + (j)), k);
                      }
                    }
                  }
                  delay(100);
                }
                analogWrite(6, 127);
                digitalWrite(7, LOW);
                state = NORMAL1;
                GUI_clear();
              }
              //===============================================================================================...Swipe Down

            }

            while ((dataIn & (0b1 << SENr[row])) > 0  || (dataIn & (0b1 << SENc[column])) > 0) {
              dataIn = touchDetectionRoutine();
            }
            break;
          }
#endif
          //===============================================================================================While Pressed...


          //===============================================================================================...While Pressed
        }
        touchClearCurrentDetections();

        //===============================================================================================When Released...
        uint8_t repeat[5] = {0, 0, 0, 0, 0};
        switch (state) {
          case CHANGE_MODE:
            state = NORMAL;
            break;
          case FACTORY:
            factoryTest[row] = 0;
            factoryTest[6 + column] = 0;
            break;
          case NORMAL:
            if (setupEnable == 1) {
#ifdef SAFE_MODE
              setupEnable = 0;
#endif
            }
            for (uint8_t i = 0; i < 6; i++) {
              for (uint8_t j = 0; j < 6; j++) {
                LEDs[i][j] = 0;
              }
            }

            for (int i = 0; i < 5; i++) {
#ifdef ENABLE_OTG_PROGRAMMER
              uint8_t temp = EEPROM.read((currentMode * 200) + (column * 30) + (row * 5) + i);
              if (temp != 255) {
                if (((temp & 0b01111111) < 72) || ((temp & 0b01111111) > 83)) {
                  uint8_t key = pgm_read_byte(&(KEYS_PRESS[temp & 0b01111111]));
                  Keyboard.press(key);
                  if (temp > 127) {
                    Keyboard.releaseAll();
                  }
                }
                else if (((temp & 0b01111111) >= 72 ) && ((temp & 0b01111111) <= 77)) {
                  switch (temp & 0b01111111) {
                    case 72: delay(1); break;
                    case 73: delay(10); break;
                    case 74: delay(100); break;
                    case 75: delay(1000); break;
                    case 76: delay(5000); break;
                    case 77: delay(10000); break;

                  }
                }
                else {
                  if (repeat[i] > 0) {
                    repeat[i]--;
                    if (repeat[i] > 0) {
                      i = -1;
                    }
                  }
                  else {
                    switch (temp & 0b01111111) {
                      case 78: repeat[i] = 1; break;
                      case 79: repeat[i] = 2; break;
                      case 80: repeat[i] = 3; break;
                      case 81: repeat[i] = 4; break;
                      case 82: repeat[i] = 9; break;
                      case 83: repeat[i] = 19; break;

                    }
                    i = -1;
                  }
                }
              }
              else {
#endif
                if (i == 0) {
                  keyPress(column, row, currentMode + 1); //If key not programmed, default to info stored in flash (KeyDefinitions.h)
                }
                break;
#ifdef ENABLE_OTG_PROGRAMMER
              }
#endif
            }
            Keyboard.releaseAll();

            break;
          case NORMAL1:
            for (uint8_t i = 0; i < 6; i++) {
              for (uint8_t j = 0; j < 6; j++) {
                LEDs[i][j] = 0;
              }
            }
            state = NORMAL;
            break;
          case PROG1:
            for (uint8_t i = 0; i < 6; i++) {
              for (uint8_t j = 0; j < 6; j++) {
                LEDs[i][j] = 0;
              }
            }
            if (compat > 0) {
              Keyboard.print("Compatibility level: ");
              Keyboard.print(compat / COMPATDELAY);
              Keyboard.releaseAll();
            }
            GUI_prog();
            state = PROG;
            progR = 10;
            progC = 10;
            GUI_Menu = 0;
            GUI_keySelected(progC, progR);
            GUI_menu();
            GUI_Menu = 6;
            break;
          case PROG:
            for (uint8_t i = 0; i < 6; i++) {
              for (uint8_t j = 0; j < 6; j++) {
                LEDs[i][j] = 0;
              }
            }

            if (progC != 10 || progR != 10) {
              LEDs[progC][progR] = 1;
            }

            if (progC == 10 || progR == 10) {
              progC = column;
              progR = row;
              LEDs[progC][progR] = 1;
              GUI_keyClear();
              GUI_menu();
              GUI_Menu = 1;
            }
            else if (column == 0 && row == 5) {
              LEDs[progC][progR] = 0;
              progC = 10;
              progR = 10;
              GUI_keyClear();
              GUI_Menu = 5;
              GUI_menu();
            }
            else if (column == 1 && row == 5 ) {
              for (uint8_t i = 0; i < 4; i++) {
                if (EEPROM.read((currentMode * 200) + (progC * 30) + (progR * 5) + (i + 1)) == 255) {
                  EEPROM.write((currentMode * 200) + (progC * 30) + (progR * 5) + (i), 255);
                }
              }
              if (EEPROM.read((currentMode * 200) + (progC * 30) + (progR * 5) + (4)) != 255) {
                EEPROM.write((currentMode * 200) + (progC * 30) + (progR * 5) + (4), 255);
              }
              GUI_keyClear();
            }
            else if (column == 2 && row == 5 && GUI_Menu != 1) {
              GUI_Menu = 1;
              GUI_menu();
            }
            else if (column == 3 && row == 5 && GUI_Menu != 2) {
              GUI_Menu = 2;
              GUI_menu();
            }
            else if (column == 4 && row == 5 && GUI_Menu != 3) {
              GUI_Menu = 3;
              GUI_menu();
            }
            else if (column == 5 && row == 5 && GUI_Menu != 4) {
              GUI_Menu = 4;
              GUI_menu();
            }
            else if (column == 5 && row == 0 && GUI_Menu == 4) {
              compat = 0;
              EEPROM.write(1020, compat);
            }
            else if (column == 5 && row == 4 && GUI_Menu == 1 ) {//Release
              for (uint8_t i = 1; i < 6; i++) {
                if (EEPROM.read((currentMode * 200) + (progC * 30) + (progR * 5) + (i)) == 255 || i == 5) {
                  uint8_t temp = EEPROM.read((currentMode * 200) + (progC * 30) + (progR * 5) + (i - 1));
                  if (temp != 255) {
                    if (((temp & 0b01111111) < 72) || ((temp & 0b01111111) > 83) ) {
                      if (temp > 127) {
                        EEPROM.write((currentMode * 200) + (progC * 30) + (progR * 5) + (i - 1), temp & 127);
                      }
                      else {
                        EEPROM.write((currentMode * 200) + (progC * 30) + (progR * 5) + (i - 1), temp | 128);
                      }
                    }
                    GUI_keyClear();
                  }
                  break;
                }
              }

            }
            else if (row < 5 && pgm_read_byte(&(KEYS_LOCATION[GUI_Menu - 1][column][row])) != 255 && GUI_Menu > 0 && GUI_Menu < 5) {
              for (uint8_t i = 0; i < 5; i++) {
                if (EEPROM.read((currentMode * 200) + (progC * 30) + (progR * 5) + (i)) == 255) {
                  EEPROM.write((currentMode * 200) + (progC * 30) + (progR * 5) + (i), pgm_read_byte(&(KEYS_LOCATION[GUI_Menu - 1][column][row])));
                  break;
                }
              }
              GUI_keyClear();
            }
            break;
          case PROG_LEDS1:
            for (uint8_t i = 0; i < 6; i++) {
              for (uint8_t j = 0; j < 6; j++) {
                LEDs[i][j] = bitRead(EEPROM.read((currentMode * 200) + 180 + (i)), j);
              }
            }
            GUI_prog_leds();
            state = PROG_LEDS;
            break;
          case PROG_LEDS:
            if (row > 0) {
              uint8_t temp = EEPROM.read((currentMode * 200) + 180 + (column));
              bitWrite(temp, row, !bitRead(temp, row));
              EEPROM.write((currentMode * 200) + 180 + (column), temp);
            }
            for (uint8_t i = 0; i < 6; i++) {
              for (uint8_t j = 0; j < 6; j++) {
                LEDs[i][j] = bitRead(EEPROM.read((currentMode * 200) + 180 + (i)), j);
              }
            }
            break;

        }
        //===============================================================================================...When Released
        return 1;
      }
    }
    else {
      touchClearCurrentDetections();
    }
  }
  return 0;
}

//Process raw capacitive data, map pins to rows and columns
void decodeArray(uint16_t dataIn, uint8_t *column, uint8_t *row) {
  uint8_t i1 = 20, i2 = 20;
  for (uint8_t i = 0; i < 12; i++) {
    if ((dataIn & 0b1) == 1) {
      if (i1 == 20) {
        i1 = i;
      }
      else if (i2 == 20) {
        i2 = i;
      }
    }
    dataIn = dataIn >> 1;
  }

  for (uint8_t j = 0; j < 6; j++) {
    if (SENr[j] == i1 || SENr[j] == i2) {
      *row = j;
    }
    if (SENc[j] == i1 || SENc[j] == i2) {
      *column = j;
    }
  }
}

//=========================================================================================================
//=========================================================================================================
// GUI Functions
//=========================================================================================================
//=========================================================================================================

//Press key on keyboard, perform compat delay
void toggleKey(uint8_t key) {
  Keyboard.press(key);
  Keyboard.release(key);
  if (compat > 0) {
    delay(compat);
  }
}

//Update current key selection list
void GUI_keySelected(uint8_t column, uint8_t row) {
  if (column == 10 || row == 10) {
    memoryKeyboard(33, 49, 0);
  }
  else {
    memoryKeyboard(34, 4, 0);
    Keyboard.print(column);
    memoryKeyboard(35, 1, 0);
    Keyboard.print(row);
    memoryKeyboard(36, 11, 0);
    for (uint8_t i = 0; i < 5; i++) {
      uint8_t temp1 = EEPROM.read((currentMode * 200) + (column * 30) + (row * 5) + (i));
      if (temp1 != 255) {
        toggleKey(91);
        if ((temp1 & 0b01111111) < 84) {
          for (uint8_t j = 0; j < 5; j++) {
            toggleKey(pgm_read_byte(&(KEYS[temp1 & 0b01111111][j])));
          }
        }
        else {
          toggleKey(32);
          toggleKey(32);
          toggleKey(pgm_read_byte(&(KEYS_PRESS[temp1 & 0b01111111])));
          toggleKey(32);
          toggleKey(32);
        }
        toggleKey(93);
        if (temp1 > 127) {
          toggleKey(114);
        }
        else {
          toggleKey(32);
        }
      }
      else {
        for (uint8_t j = 0; j < (8 * (5 - i)); j++) {
          toggleKey(32);
        }
        break;
      }
    }
  }
  toggleKey(176); //Return
  toggleKey(176); //Return
  Keyboard.releaseAll();
}

//Clear current key selection
void GUI_keyClear() {
  for (uint8_t i = 0; i < 26; i++) {
    toggleKey(218);//Up
  }
  toggleKey(217);//Down
  toggleKey(216);//Left

  Keyboard.press(129);//Shift
  Keyboard.press(218);//Up
  Keyboard.releaseAll();
  toggleKey(178);//Backspace
  toggleKey(176);//Return
  GUI_keySelected(progC, progR);
  toggleKey(178);//Backspace
  toggleKey(178);//Backspace
  for (uint8_t i = 0; i < 26; i++) {
    toggleKey(217);//Down
  }
  toggleKey(216);//Left
  toggleKey(217);//Down
  Keyboard.releaseAll();
}

//Type basic prog interface
void GUI_prog() {
  toggleKey(176); //Return
  toggleKey(176); //Return
  memoryKeyboard(21, 49, 0);
  memoryKeyboard(22, 48, 0);
  memoryKeyboard(23, 49, 0);
  memoryKeyboard(24, 27, 0);
  toggleKey(176); //Return
  toggleKey(176); //Return
  memoryKeyboard(25, 43, 0);
  Keyboard.print((currentMode + 1));
  toggleKey(176); //Return
  toggleKey(176); //Return
  memoryKeyboard(26, 49, 0);
  memoryKeyboard(27, 34, 0);
  toggleKey(176); //Return
  toggleKey(176); //Return
  Keyboard.releaseAll();
}

//Type basic LED prog interface
void GUI_prog_leds() {
  toggleKey(176); //Return
  toggleKey(176); //Return
  memoryKeyboard(21, 49, 0);
  memoryKeyboard(22, 48, 0);
  memoryKeyboard(23, 49, 0);
  memoryKeyboard(24, 27, 0);
  toggleKey(176); //Return
  toggleKey(176); //Return
  memoryKeyboard(28, 41, 0);
  Keyboard.print((currentMode + 1 ));
  toggleKey(176); //Return
  toggleKey(176); //Return
  memoryKeyboard(29, 49, 0);
  memoryKeyboard(30, 5, 0);
  Keyboard.releaseAll();
}

//Manage typing and clearing GUI menus for prog interface
void GUI_menu() {
  toggleKey(215);
  switch (GUI_Menu) {
    case 0:
      for (uint8_t i = 0; i < 6; i++) {
        memoryKeyboard(7, 49, 1);
        memoryKeyboard(8, 49, 1);
        memoryKeyboard(8, 49, 1);
        memoryKeyboard(8, 49, 1);
      }
      memoryKeyboard(7, 49, 0);
      GUI_Menu = 1;
      break;
    case 1:
      for (uint8_t i = 0; i < 21; i++) {
        toggleKey(218);//Up
      }
      GUI_replace_lines();
      memoryKeyboard(0, 49, 0);
      GUI_replace_functions(3);
      memoryKeyboard(1, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(2, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(3, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(4, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(5, 49, 0);
      toggleKey(217);//Down
      toggleKey(217);//Down
      break;
    case 2:
      for (uint8_t i = 0; i < 21; i++) {
        toggleKey(218);//Up
      }
      GUI_replace_lines();
      memoryKeyboard(9, 49, 0);
      GUI_replace_functions(3);
      memoryKeyboard(10, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(11, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(12, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(13, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(14, 49, 0);
      toggleKey(217);//Down
      toggleKey(217);//Down
      break;
    case 3:
      for (uint8_t i = 0; i < 21; i++) {
        toggleKey(218);//Up
      }
      GUI_replace_lines();
      memoryKeyboard(15, 49, 0);
      GUI_replace_functions(3);
      memoryKeyboard(16, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(17, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(39, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(18, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(19, 49, 0);
      toggleKey(217);//Down
      toggleKey(217);//Down
      break;
    case 4:
      for (uint8_t i = 0; i < 21; i++) {
        toggleKey(218);//Up
      }
      GUI_replace_lines();
      memoryKeyboard(20, 49, 0);
      GUI_replace_functions(3);
      memoryKeyboard(37, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(38, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(40, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(41, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(42, 28, 0);
      toggleKey(31);
      Keyboard.print("   /   ");
      toggleKey(30);
      Keyboard.print("   /  R C  /");
      toggleKey(217);//Down
      toggleKey(217);//Down
      break;
    case 5:

      for (uint8_t i = 0; i < 22; i++) {
        toggleKey(218);//Up
      }
      GUI_replace_lines();
      memoryKeyboard(8, 49, 0);
      GUI_replace_functions(1);
      memoryKeyboard(8, 49, 0);
      GUI_replace_functions(3);
      memoryKeyboard(8, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(8, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(8, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(8, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(8, 49, 0);
      toggleKey(217);//Down
      toggleKey(217);//Down
      GUI_Menu = 6;
      break;
    case 6:
      for (uint8_t i = 0; i < 22; i++) {
        toggleKey(218);//Up
      }
      GUI_replace_lines();
      memoryKeyboard(6, 49, 0);
      GUI_replace_functions(1);
      memoryKeyboard(0, 49, 0);
      GUI_replace_functions(3);
      memoryKeyboard(1, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(2, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(3, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(4, 49, 0);
      GUI_replace_functions(4);
      memoryKeyboard(5, 49, 0);
      toggleKey(217);//Down
      toggleKey(217);//Down
      break;
  }
  Keyboard.releaseAll();
}

//Clear x number lines above cursor position
void GUI_clear_lines(uint8_t line) {
  Keyboard.press(129);//Shift
  for (uint8_t i = 0; i < line; i++) {
    toggleKey(218);//Up
  }
  Keyboard.release(129);//Shift
  toggleKey(178);//Backspace
}

//Delete single line left of cursor
void GUI_replace_lines() {
  Keyboard.press(129);//Shift
  toggleKey(218);//Up
  Keyboard.release(129);//Shift
  toggleKey(176); //Return
}

//Scroll down and call replace line
void GUI_replace_functions(uint8_t line) {
  for (uint8_t i = 0; i < line; i++) {
    toggleKey(217);//Down
  }
  GUI_replace_lines();
}

//Delete all and type prog done message
void GUI_clear() {
  Keyboard.press(129);//Shift
  for (uint8_t i = 0; i < 35; i++) {
    toggleKey(218);//Up
  }
  Keyboard.release(129);//Shift
  toggleKey(178);//Backspace
  toggleKey(176); //Return
  Keyboard.releaseAll();
  memoryKeyboard(31, 49, 0);
  memoryKeyboard(32, 28, 0);
  //Keyboard.println("");
  Keyboard.releaseAll();
}

//Type data from program memory
void memoryKeyboard(uint16_t address, uint8_t stringLength, uint8_t printl) {

  for (uint8_t j = 0; j < stringLength; j++) {
    toggleKey(pgm_read_byte(&(STRINGS[address][j])));
  }
  if (printl == 1) {
    toggleKey(176); //Return
  }
}

//=========================================================================================================
//=========================================================================================================
// LED Display Functions
//=========================================================================================================
//=========================================================================================================

//Setup interrupt to handle LEDs
void interruptSetup() {
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  // set compare match register to desired timer count
  OCR1A = 10;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts
  TCNT1 = 0;
  sei();
}

//LED driving interrupt
ISR(TIMER1_COMPA_vect) {

  for (uint8_t c = 0; c < 6; c ++) {
    switch (c) {
      case 0: digitalWrite(12, 0); break;
      case 1: digitalWrite(8, 0); break;
      case 2: digitalWrite(9, 0); break;
      case 3: digitalWrite(10, 0); break;
      case 4: digitalWrite(5, 0); break;
      case 5: digitalWrite(13, 0); break;
    }
  }

  switch (col) {
    case 0: digitalWrite(A0, 1); break;
    case 1: digitalWrite(A2, 1); break;
    case 2: digitalWrite(A3, 1); break;
    case 3: digitalWrite(A4, 1); break;
    case 4: digitalWrite(A5, 1); break;
    case 5: digitalWrite(A1, 1); break;
  }

  switch (col) {
    case 0: digitalWrite(A2, 0); break;
    case 1: digitalWrite(A3, 0); break;
    case 2: digitalWrite(A4, 0); break;
    case 3: digitalWrite(A5, 0); break;
    case 4: digitalWrite(A1, 0); break;
    case 5: digitalWrite(A0, 0); break;
  }

  for (uint8_t c = 0; c < 6; c ++) {
    switch (c) {
      case 0: digitalWrite(12, LEDs[col][c]); break;
      case 1: digitalWrite(8, LEDs[col][c]); break;
      case 2: digitalWrite(9, LEDs[col][c]); break;
      case 3: digitalWrite(10, LEDs[col][c]); break;
      case 4: digitalWrite(5, LEDs[col][c]); break;
      case 5: digitalWrite(13, LEDs[col][c]); break;
    }
  }

  col++;
  if (col > 5) {
    col = 0;
  }

  if (vibrate == VIBRATE_LENGTH) {
    digitalWrite(7, HIGH);
    analogWrite(6, 255);
    vibrate--;
  }
  else if (vibrate == 8) {
    analogWrite(6, 0);
    vibrate--;
  }
  else if (vibrate == 1) {
    analogWrite(6, 127);
    digitalWrite(7, LOW);
    vibrate--;
  }
  else if (vibrate > 0) {
    vibrate--;
  }
}

//Handle power LED fades
void pulseLED() {
  if (blinkerD == 0) {
    if (blinker < 255) {
      blinker++;
    }
    else {
      blinkerD = 1;
    }
  }
  if (blinkerD == 1) {
    if (blinker > 0) {
      blinker--;
    }
    else {
      blinkerD = 0;
    }
  }
}


//=========================================================================================================
//=========================================================================================================
// AT42QT2120 Functions
//=========================================================================================================
//=========================================================================================================

//Check interrupt pin
uint8_t isTouchChangeDetected()
{
  return !digitalRead(0);
}

//Read data from the cap touch IC
uint8_t readDataFromTS(uint8_t reg)
{
  Wire.beginTransmission(0x1C);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)0x1C, (uint8_t)1);
  if (Wire.available() == 1)
  {
    return Wire.read();
  }
  else {
    return 0;
  }
}

//Write data to cap touch IC
uint8_t writeDataToTS(uint8_t reg, uint8_t data)
{
  Wire.beginTransmission(0x1C);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
  return 1;
}


uint8_t checkTSPres()
{
  uint8_t temp_byte;
  temp_byte = readDataFromTS(0x00);
  if (temp_byte != 0x3E)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

//Setup cap touch IC
uint8_t capSetup()
{

  uint8_t temp_return = checkTSPres();

  if (temp_return == 1)
  {
    // Perform measurements every 16ms
    writeDataToTS(0x08, 1);

    // Increase detection integrator value
    writeDataToTS(0x0B, 1);

    // Oversample to gain two bits for columns
    writeDataToTS(0x28, 0x42);
    writeDataToTS(0x29, 0x00);
    writeDataToTS(0x2A, 0x00);
    writeDataToTS(0x2B, 0x00);
    writeDataToTS(0x2C, 0x42);
    writeDataToTS(0x2D, 0x00);
    writeDataToTS(0x2E, 0x00);
    writeDataToTS(0x2F, 0x00);
    writeDataToTS(0x30, 0x42);
    writeDataToTS(0x31, 0x42);
    writeDataToTS(0x32, 0x42);
    writeDataToTS(0x33, 0x42);

    // Recalibration if touch detected for more than 8 seconds n*0.16s
    writeDataToTS(0x0C, 50);

    // Enable keys and set key groups
    writeDataToTS(0x1C, 0x00 | 0x04);
    writeDataToTS(0x1D, 0x00 | 0x08);
    writeDataToTS(0x1E, 0x00 | 0x08);
    writeDataToTS(0x1F, 0x00 | 0x08);
    writeDataToTS(0x20, 0x00 | 0x04);
    writeDataToTS(0x21, 0x00 | 0x08);
    writeDataToTS(0x22, 0x00 | 0x08);
    writeDataToTS(0x23, 0x00 | 0x08);
    writeDataToTS(0x24, 0x00 | 0x04);
    writeDataToTS(0x25, 0x00 | 0x04);
    writeDataToTS(0x26, 0x00 | 0x04);
    writeDataToTS(0x27, 0x00 | 0x04);

  }
  return temp_return;
}

void touchClearCurrentDetections()
{
  uint8_t temp_uint;
  temp_uint = readDataFromTS(0x05);
  temp_uint = readDataFromTS(0x02);
  temp_uint = readDataFromTS(0x03);
  temp_uint = readDataFromTS(0x04);
}

uint16_t touchDetectionRoutine()
{
  uint16_t data;
  uint8_t temp1, temp2;

  temp1 = readDataFromTS(0x04);
  temp2 = readDataFromTS(0x03);
  data = temp1;
  data = (data << 8) | temp2;
  return data;

}

