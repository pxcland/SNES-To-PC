# SNES-To-PC
Using a real Super Nintendo Controller on the PC.

This project uses an Arduino R3 to act as a host to the Super Nintendo Controller.

**host.c**
The Arduino receives the data from the controller, and sends it over the serial port to the PC.

**main.c**
On the PC, The SnesToKeyboard program listens on the serial port, and interprets the serial data, and generates virtual keypresses on Windows, which can be used in any emulator or game, anything really, since they are virtual keypresses, as far as the programs are concerned, they are receiving keypresses from the keyboard.

**Screenshots**
![1](http://i.imgur.com/9Wvokez.jpg)
![2](http://i.imgur.com/OCvQmQY.jpg)

