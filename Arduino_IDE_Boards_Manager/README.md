# Arduino IDE Boards Manager Files

These files are required so users can install TouchPad in Arduino using the boards manager feature.

To install TouchPad:
1. Open the Arduino IDE and go to File/Preferences
2. Add the following web address to the field "Additional Boards Manager URLs:", "https://raw.githubusercontent.com/jacoburge/TouchPad/master/Arduino_IDE_Boards_Manager/package_jacoburge_index.json", ensure the list is comma seperated
3. Click OK
4. Go to Tools/Board/Boards Manager:
5. Search for TouchPad and select the JacoBurge Boards entry
5. Click install

All done!  Now you can select TouchPad from the boards menu, select the correct COM port and hit upload.
