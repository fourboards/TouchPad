# TouchPad hardware files

This folder contains the Arduino hardware definitions for the JacoBurge TouchPad (http://jacoburge.co.uk/touch-pad).

To manually install the JacoBurge definitions in the Arduino IDE (alternative available here http://jacoburge.co.uk/touchpad-support#installing_arduino):

1. Close the Arduino IDE.  To ensure compatability keep your Arduino IDE up to date.
2. Extract the .zip to a location of your choice
3. Move the folder "jacoburge_hardware_master" to the correct folder for your operating system:

*** Windows ***
Libraries\Documents\Arduino\Hardware or My Documents\Arduino\Hardware

***   Mac   ***
Documents/Arduino/hardware

***  Linux  ***
Home/Arduino/hardware

(If the hardware folder does not exsist in this location, create it!)
4. Finally reopen the Arduino IDE, navigate to Tools/Board/JacoBurge Boards/TouchPad
