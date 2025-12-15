# System Architecture

## Overview
High-level control on Raspberry Pi, real-time actuation on Arduino.

## Components
- PC (development)
- Raspberry Pi (C# control)
- Arduino (servo controller)
- Braccio arm

## Responsibilities
| Component | Responsibility |
|---------|---------------|
| Pi | State machine, sequencing |
| Arduino | PWM, timing, limits |

## Design Decisions
- Why USB serial
- Why Arduino for servos
- Why no real-time OS on Pi

## Project Goal and Scope

### Goal

The goal of this project is to build a fully functional robotic arm control system based on
a Raspberry Pi and an Arduino-controlled TinkerKit Braccio robotic arm.

The system shall be capable of reliably transferring parts between predefined positions.
All robot movements are executed through direct control of the hardware, without relying
on proprietary robot simulation or offline programming software.

The focus is on achieving a stable, understandable, and reproducible control setup rather
than maximum speed or payload.

---

### Scope

The scope of the project includes:

- Direct control of the Arduino TinkerKit Braccio robotic arm
- High-level motion sequencing implemented on a Raspberry Pi
- Deterministic real-time servo control handled by an Arduino
- Transfer of parts between fixed, predefined pick and place positions
- A clearly defined and documented communication protocol between Raspberry Pi and Arduino

Optional (out of scope for the MVP, but planned as an extension):

- Integration of a USB camera
- Vision-based object detection and position correction
- Vision-assisted pick-and-place operations

---

### Out of Scope

The following items are explicitly out of scope for this project:

- Use of RoboDK or other proprietary robot simulation software
- Closed-loop joint control using encoders
- Force or torque feedback
- Dynamic obstacle avoidance
- High-speed or high-precision industrial motion control

---

### Minimum Viable Product (MVP)

The Minimum Viable Product is defined as:

- The robotic arm can be powered on and safely initialized
- The Raspberry Pi can send motion commands to the Arduino via USB serial
- The Arduino executes interpolated joint movements within defined safety limits
- The robot can repeatedly pick up a part from a fixed pick position
- The robot can place the part at a fixed place position
- The system operates reliably without manual intervention during a transfer cycle

---

### Acceptance Criteria

The project goal is considered fulfilled when:

- The robotic arm successfully transfers parts between predefined positions
- Movements are repeatable and do not cause mechanical collisions
- Communication between Raspberry Pi and Arduino is stable and well-documented
- All design decisions and system boundaries are documented in this repository
