# SNES-To-PC
Using a real Super Nintendo Controller on the PC.

This project uses an Arduino R3 to act as a host to the Super Nintendo Controller.
The SNES' controller protocol can be found on Google.

Basically, the SNES will send a request for the state of the pad every so often, and then it will send 16 clock signals, and on each clock signal it receives a single bit of serial data representing each of 12 buttons on the pad with 4 permanently high bits.

**host.c**
The Arduino requests the state of the controller, receives the data from the controller, and sends it over the serial port to the PC.

**main.c**
On the PC, The SNESToKeyboard program listens on the serial port, and interprets the serial data, and generates virtual keypresses on Windows, which can be used in any emulator or game, anything really, since they are virtual keypresses, as far as the programs are concerned, they are receiving keypresses from the keyboard.

**Screenshots**
![1](http://i.imgur.com/9Wvokez.jpg)
![2](http://i.imgur.com/OCvQmQY.jpg)

