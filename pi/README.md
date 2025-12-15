# Raspberry Pi Control Application

This directory contains the high-level control software running on the Raspberry Pi.

The application is responsible for motion sequencing, state management, and communication
with the Arduino-based motion controller. All logic is implemented in C# and runs on Linux
using .NET.

## Responsibilities

- High-level motion control and sequencing
- State machine execution (idle, moving, error)
- Communication with the Arduino via USB serial
- Optional camera and vision processing
- Logging and diagnostics

## Notes

- This component does not perform real-time control.
- All timing-critical servo operations are delegated to the Arduino.
- The Raspberry Pi communicates with the Arduino using a text-based serial protocol
  defined in `docs/protocol.md`.
