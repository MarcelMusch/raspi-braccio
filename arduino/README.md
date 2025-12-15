# Arduino Firmware

This directory contains the firmware for the Arduino-based motion controller of the
TinkerKit Braccio robotic arm.

The Arduino is responsible for deterministic real-time control of the servo motors and
executes motion commands received from the Raspberry Pi.

## Responsibilities

- Real-time PWM generation for servo control
- Interpolation of joint movements
- Enforcement of joint limits and safety constraints
- Parsing and validation of serial commands
- Status and error reporting

## Notes

- The firmware operates without an operating system.
- All commands are received via USB serial from the Raspberry Pi.
- No closed-loop feedback from the servos is assumed.
