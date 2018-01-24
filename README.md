The Red Pawn District
======================

Description coming soon ;)

Dependencies
====================
An Arduino IDE is recommended.

* ArduinoSound
  * 0.2.0
  * Handles the bulk of work of reading WAV files from SD cards and piping it over I2s.
  
Notes on the Circuit
=====================

Parts List
-----------

* Adafruit Feather M0 https://www.adafruit.com/product/2821
* Perma-Proto Half-sized Breadboard PCB https://www.adafruit.com/product/1609
* Adafruit I2S 3W Class D Amplifier https://www.adafruit.com/product/3006
* Slide Switch https://www.adafruit.com/product/805
* 3x AAA Battery Holder https://www.adafruit.com/product/727
* MicroSD card breakout https://www.adafruit.com/product/254
* buttons https://www.adafruit.com/product/1010

Pin Setup
----------

* RX0 to LRC on Amplifier
* TX1 to BCLK on Amplifier
* 9 to DIN on Amplifier

* SCK to CLK on SD card reader
* MISO to DO on SD card reader
* MOSI to DI on on SD card reader
* 10 to CS on SD card reader

* 13 for big button
* 12 for S mode slide switch
* 11 for D mode slide switch