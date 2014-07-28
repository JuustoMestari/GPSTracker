GPSTracker
==========

## Arduino

Arduino Pro mini 3.3V 8Mhz ( copy )

### Libraries

#### SoftwareSerial

In order to run the code properly, you need to modify **SoftwareSerial.h** (Arduino\libraries\SoftwareSerial) before compiling.
At line **42** change _SS_MAX_RX_BUFF to 256 : `#define _SS_MAX_RX_BUFF 256 `

#### NeoPixel

Please use the library in this repo. It has been modified to use Analog Port as well (Leds are connected to Analog 5)
If you want to make the modification yourself, replace **PORTB** by **PORTC** between line **238** & **315** in Adafruit_NeoPixel.cpp (You won't be able to use the library with ports 8 to 13 tho...)

[See here for port manipulation](http://www.arduino.cc/en/Reference/PortManipulation)

## Hardware

### TP4056

* Green LED ON - Red LED Blink -> **No battery**
* RED LED ON -> **Loading**
* GREEN LED ON -> **Loaded**

The module in this project uses a mini USB connector but micro USB versions are available on eBay.

[Module test](http://lygte-info.dk/review/Review%20Charger%20TP4056%20UK.html)

## Datasheets

## Eagle
