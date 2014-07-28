GPSTracker
==========

## Arduino

Arduino Pro mini 3.3V 8Mhz ( copy )

### Libraries

#### SoftwareSerial

In order to run the code properly, you need to modify **SoftwareSerial.h** (Arduino\libraries\SoftwareSerial) before compiling.
At line **42** change _SS_MAX_RX_BUFF to 256 : `#define _SS_MAX_RX_BUFF 256 `

## Hardware

### TP4056

The module in this project use a mini USB connector but micro USB versions are available on eBay.

[Module test](http://lygte-info.dk/review/Review%20Charger%20TP4056%20UK.html)

## Datasheets

## Eagle
