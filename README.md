# OpenFlame Thermal Imager

The OpenFlame Thermal Imager was originally thought up when I had some ideas for some kits to make using the Adafruit Feather ecosystem that required minimal soldering and a quick upload of firmware to get working as intended. A whole year later, there is finally an alpha version 0 of the <150$ thermal camera.

Special shoutout to Alex Tam, the Systems Architect for the Lake Monsters Robotics Team (#2635), for designing and printing the case. While I wanted to make everything myself, I knew that my CAD abilities/patience are pretty bad. Plus it would be nice to have a thermal camera around the lab whenever we smoke a motor lol.

## Current Features

As it stands, it is pretty bare bones with nothing fancy happening.

- Thermal image displayed on screen (32x24)
- 16Hz refresh rate
- Small settings menu
	- Change capture mode (CHESS/INTERLACE)
	- Change refresh rate (16Hz/8Hz)
	- Temperature range
- FPS/Frametime (ms) counter
- Battery % readout (a bit buggy)

## Planned Features

**DISCLAIMER:** *This list is subject to change and should not be interpreted as a guaranteed promise.*

- [ ] Bilinear/bicubic interpolation (for better image quality)
- [ ] Automatic temperature ranging mode
- [ ] Range colour settings
- [ ] Hot and cold spot highlighting
- [ ] Image saving and downloading to computer
- [ ] Ambient temperature compensation? (requires populated BME680, also is inside case so probably won't be that useful)
- [ ] Power management settings
- [ ] OTA updates

## Required Hardware

- Adafruit Reverse TFT Feather ESP32-S2/3 (either works, untested with other feather boards)
- Qwiic/Stemma QT 50mm or 100mm connector
- 110 FOV or 55 FOV MLX90640 Breakout Module (110 for smaller targets, 55 for larger targets)
- 1200mAh Lithium Ion Battery (life expectancy not measured)
- M2.5 Metal Machine Screws (I used the one from Adafruit if you couldn't guess already)
- 3D printed case (print files and print settings found in `case-v0` folder)

## Required Software

- OS: Linux, Mac, or Windows
- Arduino IDE >v2.0
- git

### Arduino IDE Setup

- Adafruit ESP32 board definitions (found on Adafruit website)
- Adafruit MLX90640 library
- Adafruit MAX1704X library
- Adafruit ST7789 library

### Installation Instructions

*Installation instructions have not been created yet*

## Project Tracking

I eventually plan to create a GitHub Project for this repo (when I have a bit more time).

## Issues

This is effectively alpha firmware and as such, bugs are expected. If you do come across an issue attempting to set up, please write an issue with as much information as possible so I can help as best I can. A template is coming soon :TM:.

## Contributing

Although I do really appreciate it, contributions are currently not being accepted. Once this is in a state that I'm happy with (which won't happen for a while), I'll open it up for contributions.
