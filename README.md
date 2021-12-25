# Remote game controller on ESP8266

Implementions of DIY game controller based on ESP8266 (NodeMCU) and vJoy

## Backstory

This is a weekend project with the goal to build a game controller out of components that I had on hands. (Primarily for using it with X-Plane, but it should work with any other game)
I used NodeMCU microcontroller, which is not a good choice for such project because it doesn't provide USB HID. But fortunaylt it has built in WiFi module.
In this repo you will find:
* vJoy Feeder (Network client that listens for data from NodeMCU and translates it to commands for vJoy)
* ESP8266 firmware source code

### Demo:

[![Remote game controller on ESP8266](http://img.youtube.com/vi/HWvpHjzzCts/0.jpg)](http://www.youtube.com/watch?v=HWvpHjzzCts "Remote game controller on ESP8266")

Please look at follwing projects if you are looking for more mature Arduino projects:
* [Mobiflight](https://www.mobiflight.com/en/index.html)
* [RealSimControl](https://realsimcontrol.com/)
  
## Features

* Support for Buttons and Rotary Encoders
* Wifi
* Network auto-discovery of the controller using SSDP

## Schematics

Example how to connect a rotary encoder:

![Schematics](/docs/schematic.png "Schematics")

## Build and run

1. Download latest SDK for vJoy from https://sourceforge.net/projects/vjoystick/
2. Put `vJoyInterface.dll` to the project folder
3. Build and upload NodeMCU firmware from `esp8266` folder
4. Edit `controller_map` in `main.py`. For schematic above it would look like: `controller_map = { "buttons": [5], "encoders": [[0, 4]]}` where numbers are pin number where buttons/encoders connected to.
