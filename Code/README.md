# TouchPad source code

This folder contains the Arduino source code for the JacoBurge TouchPad (http://jacoburge.co.uk/touch-pad).

Make sure you have installed the JacoBurge hardware definitions in the TouchPad/hardware folder before proceeding.

Open TouchPad_V1.4.ino in the Arduino IDE, follow the instructions at the top of the page to edit key functionality.

To upload your modified TouchPad firmware:
1.	Plug in TouchPad via the micro USB cable.
2.	In the Arduino IDE select Tools/Board/JacoBurge TouchPad.
3.	In the Arduino IDE select Tools/Port/[The COM port TouchPad is on].
4.	Press Upload (Right hand arrow).

Upload troubleshooting:
1.	Check you have selected the correct COM port (it should say TouchPad next to it in the port selection menu).
2.	Hold down the reset button on the back of TouchPad, hit upload in the Arduino IDE, when Arduino says “Uploading” in the bottom left hand corner release the reset button.

