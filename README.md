# OpenFlame Thermal Imager

The OpenFlame Thermal Imager was originally thought up when I had some ideas for some kits to make using the Adafruit Feather ecosystem that required minimal soldering and a quick upload of firmware to get working as intended. A whole year later, there is finally an alpha version 0 of the <150$ thermal camera.

Special shoutout to Alex Tam, the Systems Architect for the Lake Monsters Robotics Team (#2635), for designing and printing the case. While I wanted to make everything myself, I knew that my CAD abilities/patience are pretty bad. Plus it would be nice to have a thermal camera around the lab whenever we smoke a motor lol.

## Current Features

As it stands, it is pretty bare bones with nothing fancy happening.

- Thermal image displayed on screen (32x24 camera resolution)
- Small settings menu
  - Change capture mode (CHESS/INTERLACE)
  - Change refresh rate (16Hz/8Hz)
  - Temperature range settings (including an autorange toggle)
- Battery % readout (a bit buggy)
- Software on/off (not fully implemented)

## Project Tracking

All currently planned features are listed in the GitHub Projects tab of this repository. This should be the most up to date feature list for this project.

**DISCLAIMER:** *This list is subject to change and should not be interpreted as a guaranteed implementation.*

## Required Hardware

*Links and exact quantities have not been defined yet*

- 1x Feather board *(choose one, either works)*
  - [Adafruit Reverse TFT Feather ESP32-S2](https://www.adafruit.com/product/5345) (24.95$)
  - [Adafruit Reverse TFT Feather ESP32-S3](https://www.adafruit.com/product/5691) (24.95$)
- 1x Thermal Camera Module *(choose one, either works)*
  - [Adafruit MLX90640 Thermal Camera Module - 55 Degree](https://www.adafruit.com/product/4407) (74.95$)
  - [Adafruit MLX90640 Thermal Camera Module - 110 Degree](https://www.adafruit.com/product/4469) (74.95$)
- 1x [3.7v 1200mAh Lithium Ion Battery](https://www.adafruit.com/product/258) (9.95$)
- 1x [Qwiic/Stemma QT 100mm connector](https://www.adafruit.com/product/4210) (0.95$)
- 1x M2.5 Machine Screw Set *(choose one, either works)*
  - [Black Machine Screw Set](https://www.adafruit.com/product/3299) (16.95$)
  - [White Machine Screw Set](https://www.adafruit.com/product/3658) (14.95$)
- 3D printed case (print files and print settings found in `case-v0` folder)

**TOTAL COST (one unit): $127.75\***

\* Excludes price of 3D printed case

### Hardware Assembly Instructions

*Assembly instructions have not been created yet*

## Required Software

- OS: Linux, Mac, or Windows
- Arduino IDE >v2.0
- git

### Firmware Installation Instructions

*Installation instructions have not been created yet*

## Issues

This is effectively alpha firmware and as such, bugs are expected. If you do come across an issue attempting to set up, please write an issue with as much information as possible so I can help as best I can. A template is coming soon :TM:.

## Contributing

Although I do really appreciate it, contributions are currently not being accepted. Once this is in a state that I'm happy with (which won't happen for a while), I'll open it up for contributions.
