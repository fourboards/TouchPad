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
  
  TouchPad was successfully funded on Kickstarter on 12th March 2018.  A special thanks to the following backers who donated to the campaign as well as all those who showed their support by buying a TouchPad:
  Florence O’Neill, Alex, Thomas M, Melanie Thomson, Matthew, Julio Terra, Alejandro Esquivel, RandomColourzzz, JSYEHUA, Andrea Edgar, Apple Lo, Conor Nolan, Yves, senritsu, Charles Zyla, Acme Globes, Thomee Wright, Dag Henrik Bråtane

/*******************************************************************************

      .  - - - - - - - - - - - - - - - - - - - - - - -  .
    .                                                     .
    .   O                  TOP VIEW                   O   .
    .                                                     .
      .       Key Location Reference [column,row]        .
       . - - - - - - - - - - - - - - - - - - - - - - - .
       /       /       /       /       /       /       /
       /  0,5  /  1,5  /  2,5  /  3,5  /  4,5  /  5,5  /
       /       /       /       /       /       /       /
       - - - - - - - - - - - - - - - - - - - - - - - - -
       /       /       /       /       /       /       /
       /  0,4  /  1,4  /  2,4  /  3,4  /  4,4  /  5,4  /
       /       /       /       /       /       /       /
       - - - - - - - - - - - - - - - - - - - - - - - - -
       /       /       /       /       /       /       /
       /  0,3  /  1,3  /  2,3  /  3,3  /  4,3  /  5,3  /
       /       /       /       /       /       /       /
       - - - - - - - - - - - - - - - - - - - - - - - - -
       /       /       /       /       /       /       /
       /  0,2  /  1,2  /  2,2  /  3,2  /  4,2  /  5,2  /
       /       /       /       /       /       /       /
       - - - - - - - - - - - - - - - - - - - - - - - - -
       /       /       /       /       /       /       /
       /  0,1  /  1,1  /  2,1  /  3,1  /  4,1  /  5,1  /
       /       /       /       /       /       /       /
       - - - - - - - - - - - - - - - - - - - - - - - - -
       /       /       /       /       /       /       /
       /  0,0  /  1,0  /  2,0  /  3,0  /  4,0  /  5,0  /
       /       /       /       /       /       /       /
       - - - - - - - - - - - - - - - - - - - - - - - - -

   To assign code to a specific key, add it to the key's case statement,
   for example if you want key 1,3 in command set 3 to type "Hello World"
   find the corresponding case statement below and adjust it as follow:

  ""
   case 3://Use this case for row 3, column 1, commandSet 3
    Keyboard.print("Hello World");
    break;
  ""

   By default the JacoBurge UK Keyboard and Arduino Mouse libraries are included,
   reference for these can be found at:
   
    https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
      - Use the following commands to control the keyboard: Keyboard.press(),Keyboard.print(),
        Keyboard.println(),Keyboard.release(),Keyboard.releaseAll(),Keyboard.write()
      - Modifier keys are defined here: 
        https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
      - Keyboard.begin() is called in TouchPad.h so you do not need to call it again
    https://www.arduino.cc/reference/en/language/functions/usb/mouse/
      - Use the following commands to control the keyboard: Mouse.click(),Mouse.move(),
        Mouse.press(),Mouse.release(),Mouse.isPressed()
      - Mouse.begin() is called in TouchPad.h so you do not need to call it again
      
   If you want to add more libraries, for example MIDI, you can add it below.

   Please note:  The key presses defined here will only be executed if
   there are no key presses assigned via the 'On the go programmer'

 *******************************************************************************/

//Compiler directives - effects TouchPad behaviour in described way
#define ENABLE_MULTIPLE_COMMAND_SET //Comment this out (insert // before line) to fix command set to command set 1
#define ENABLE_OTG_PROGRAMMER //Comment this out (insert // before line) to prevent the built in programming mode
 
//Add any more libraries you need HERE (Serial is included in TouchPad.h at 9600 so this can be used without re-including):
#include "KeyboardUK.h" //Edited version of Arduino keyboard library to add backslash key for UK setup, include standard Arduino library for US setup (#include <Keyboard.h>)
#include <Mouse.h> //Arduino mouse library

#include "TouchPad.h" //File required for TouchPad functionality, DO NOT REMOVE!

void keyPress(int x, int y, int commandSet) {
  switch (commandSet) {
    //============================================================================================== commandSet 1
    case 1:
      switch (x) {
        //================================================================================ column 0
        case 0:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 0, commandSet 1
              //Keyboard.println("Hello world"); //EXAMPLE Type Hello World
              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 0, commandSet 1
              //Keyboard.press(KEY_LEFT_CTRL); //EXAMPLE Press Ctrl key
              //Keyboard.press('c'); //EXAMPLE Also press c key
              //Keyboard.releaseAll(); //EXAMPLE Release both keys
              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 0, commandSet 1

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 0, commandSet 1

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 0, commandSet 1

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 0, commandSet 1

              break;
          }
          break;
        //================================================================================ column 1
        case 1:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 1, commandSet 1

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 1, commandSet 1

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 1, commandSet 1

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 1, commandSet 1

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 1, commandSet 1

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 1, commandSet 1

              break;
          }
          break;
        //================================================================================ column 2
        case 2:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 2, commandSet 1

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 2, commandSet 1

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 2, commandSet 1

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 2, commandSet 1

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 2, commandSet 1

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 2, commandSet 1

              break;
          }
          break;
        //================================================================================ column 3
        case 3:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 3, commandSet 1

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 3, commandSet 1

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 3, commandSet 1

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 3, commandSet 1

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 3, commandSet 1

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 3, commandSet 1

              break;
          }
          break;
        //================================================================================ column 4
        case 4:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 4, commandSet 1

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 4, commandSet 1

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 4, commandSet 1

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 4, commandSet 1

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 4, commandSet 1

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 4, commandSet 1

              break;
          }
          break;
        //================================================================================ column 5
        case 5:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 5, commandSet 1

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 5, commandSet 1

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 5, commandSet 1

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 5, commandSet 1

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 5, commandSet 1

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 5, commandSet 1

              break;
          }
          break;
      }
      break;
    //============================================================================================== commandSet 2
    case 2:
      switch (x) {
        //================================================================================ column 0
        case 0:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 0, commandSet 2

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 0, commandSet 2

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 0, commandSet 2

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 0, commandSet 2

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 0, commandSet 2

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 0, commandSet 2

              break;
          }
          break;
        //================================================================================ column 1
        case 1:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 1, commandSet 2

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 1, commandSet 2

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 1, commandSet 2

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 1, commandSet 2

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 1, commandSet 2

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 1, commandSet 2

              break;
          }
          break;
        //================================================================================ column 2
        case 2:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 2, commandSet 2

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 2, commandSet 2

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 2, commandSet 2

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 2, commandSet 2

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 2, commandSet 2

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 2, commandSet 2

              break;
          }
          break;
        //================================================================================ column 3
        case 3:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 3, commandSet 2

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 3, commandSet 2

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 3, commandSet 2

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 3, commandSet 2

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 3, commandSet 2

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 3, commandSet 2

              break;
          }
          break;
        //================================================================================ column 4
        case 4:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 4, commandSet 2

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 4, commandSet 2

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 4, commandSet 2

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 4, commandSet 2

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 4, commandSet 2

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 4, commandSet 2

              break;
          }
          break;
        //================================================================================ column 5
        case 5:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 5, commandSet 2

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 5, commandSet 2

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 5, commandSet 2

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 5, commandSet 2

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 5, commandSet 2

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 5, commandSet 2

              break;
          }
          break;
      }
      break;
    //============================================================================================== commandSet 3
    case 3:
      switch (x) {
        //================================================================================ column 0
        case 0:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 0, commandSet 3

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 0, commandSet 3

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 0, commandSet 3

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 0, commandSet 3

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 0, commandSet 3

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 0, commandSet 3

              break;
          }
          break;
        //================================================================================ column 1
        case 1:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 1, commandSet 3

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 1, commandSet 3

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 1, commandSet 3

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 1, commandSet 3

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 1, commandSet 3

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 1, commandSet 3

              break;
          }
          break;
        //================================================================================ column 2
        case 2:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 2, commandSet 3

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 2, commandSet 3

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 2, commandSet 3

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 2, commandSet 3

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 2, commandSet 3

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 2, commandSet 3

              break;
          }
          break;
        //================================================================================ column 3
        case 3:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 3, commandSet 3

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 3, commandSet 3

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 3, commandSet 3

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 3, commandSet 3

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 3, commandSet 3

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 3, commandSet 3

              break;
          }
          break;
        //================================================================================ column 4
        case 4:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 4, commandSet 3

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 4, commandSet 3

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 4, commandSet 3

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 4, commandSet 3

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 4, commandSet 3

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 4, commandSet 3

              break;
          }
          break;
        //================================================================================ column 5
        case 5:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 5, commandSet 3

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 5, commandSet 3

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 5, commandSet 3

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 5, commandSet 3

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 5, commandSet 3

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 5, commandSet 3

              break;
          }
          break;
      }
      break;
    //============================================================================================== commandSet 4
    case 4:
      switch (x) {
        //================================================================================ column 0
        case 0:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 0, commandSet 4

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 0, commandSet 4

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 0, commandSet 4

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 0, commandSet 4

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 0, commandSet 4

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 0, commandSet 4

              break;
          }
          break;
        //================================================================================ column 1
        case 1:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 1, commandSet 4

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 1, commandSet 4

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 1, commandSet 4

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 1, commandSet 4

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 1, commandSet 4

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 1, commandSet 4

              break;
          }
          break;
        //================================================================================ column 2
        case 2:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 2, commandSet 4

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 2, commandSet 4

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 2, commandSet 4

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 2, commandSet 4

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 2, commandSet 4

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 2, commandSet 4

              break;
          }
          break;
        //================================================================================ column 3
        case 3:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 3, commandSet 4

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 3, commandSet 4

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 3, commandSet 4

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 3, commandSet 4

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 3, commandSet 4

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 3, commandSet 4

              break;
          }
          break;
        //================================================================================ column 4
        case 4:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 4, commandSet 4

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 4, commandSet 4

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 4, commandSet 4

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 4, commandSet 4

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 4, commandSet 4

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 4, commandSet 4

              break;
          }
          break;
        //================================================================================ column 5
        case 5:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 5, commandSet 4

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 5, commandSet 4

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 5, commandSet 4

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 5, commandSet 4

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 5, commandSet 4

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 5, commandSet 4

              break;
          }
          break;
      }
      break;
    //============================================================================================== commandSet 5
    case 5:
      switch (x) {
        //================================================================================ column 0
        case 0:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 0, commandSet 5

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 0, commandSet 5

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 0, commandSet 5

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 0, commandSet 5

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 0, commandSet 5

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 0, commandSet 5

              break;
          }
          break;
        //================================================================================ column 1
        case 1:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 1, commandSet 5

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 1, commandSet 5

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 1, commandSet 5

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 1, commandSet 5

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 1, commandSet 5

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 1, commandSet 5

              break;
          }
          break;
        //================================================================================ column 2
        case 2:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 2, commandSet 5

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 2, commandSet 5

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 2, commandSet 5

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 2, commandSet 5

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 2, commandSet 5

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 2, commandSet 5

              break;
          }
          break;
        //================================================================================ column 3
        case 3:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 3, commandSet 5

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 3, commandSet 5

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 3, commandSet 5

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 3, commandSet 5

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 3, commandSet 5

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 3, commandSet 5

              break;
          }
          break;
        //================================================================================ column 4
        case 4:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 4, commandSet 5

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 4, commandSet 5

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 4, commandSet 5

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 4, commandSet 5

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 4, commandSet 5

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 4, commandSet 5

              break;
          }
          break;
        //================================================================================ column 5
        case 5:
          switch (y) {
            //===================================================================== row 0
            case 0://Use this case for row 0, column 5, commandSet 5

              break;
            //===================================================================== row 1
            case 1://Use this case for row 1, column 5, commandSet 5

              break;
            //===================================================================== row 2
            case 2://Use this case for row 2, column 5, commandSet 5

              break;
            //===================================================================== row 3
            case 3://Use this case for row 3, column 5, commandSet 5

              break;
            //===================================================================== row 4
            case 4://Use this case for row 4, column 5, commandSet 5

              break;
            //===================================================================== row 5
            case 5://Use this case for row 5, column 5, commandSet 5

              break;
          }
          break;
      }
      break;
  }
}


