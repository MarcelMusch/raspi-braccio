# raspi-braccio

## Overview

raspi-braccio is a robotics control project for the Arduino TinkerKit Braccio robotic arm.
The system uses a Raspberry Pi as a high-level controller running C# on Linux, while an Arduino
acts as a dedicated real-time motion controller for the servo-driven joints.

The project focuses on a clean separation of responsibilities between high-level logic and
deterministic low-level actuation. No proprietary robot simulation software is required.
Instead, the emphasis is on transparent system architecture, reproducible motion behavior,
and hands-on understanding of robotic control principles.

---

## Architecture

The system follows a layered control architecture commonly used in industrial and embedded
robotic systems.

- **PC (Development Environment)**
  - Source code development
  - Deployment and debugging via SSH/SCP

- **Raspberry Pi**
  - High-level control logic written in C#
  - Motion sequencing and state machines
  - Optional computer vision processing
  - USB serial communication with the Arduino

- **Arduino (Braccio Controller)**
  - Real-time servo control
  - Motion interpolation and timing
  - Enforcement of joint limits and safety constraints

The Raspberry Pi and the servo power supply are electrically isolated.
The Arduino is connected to the Raspberry Pi via USB and acts as a deterministic actuator layer.

---

## Hardware Setup

The project is designed around commonly available and affordable hardware components.

Required hardware:
- Arduino TinkerKit Braccio robotic arm
- Raspberry Pi 4 (4 GB or 8 GB RAM recommended)
- Arduino UNO (included with the Braccio kit)
- External 5 V power supply for the servos

Optional hardware:
- USB webcam (UVC compatible) for vision-based extensions
- Additional base weight for mechanical stability

The Raspberry Pi is powered independently from the servo supply.
Servo power is never sourced from the Raspberry Pi or the Arduino.

---

## Software Stack

The following software components are used:

- Raspberry Pi OS (64-bit)
- .NET 8 (C#)
- Arduino framework
- USB serial communication

Optional components:
- OpenCV / OpenCVSharp for image processing
- Additional tooling for logging and visualization

---

## Repository Structure
raspi-braccio/
├─ arduino/ # Arduino firmware (servo control and protocol handling)
├─ pi/ # C# control application running on the Raspberry Pi
├─ docs/ # Architecture, protocol, and setup documentation
├─ tools/ # Helper scripts and utilities
└─ README.md

---

## Getting Started

1. Assemble the Braccio robotic arm according to the official Arduino documentation.
2. Install Raspberry Pi OS (64-bit) and enable SSH access.
3. Install .NET 8 on the Raspberry Pi.
4. Flash the Arduino firmware located in the `arduino/` directory.
5. Connect the Arduino to the Raspberry Pi via USB.
6. Start the C# control application from the `pi/` directory.

Detailed setup instructions and wiring information can be found in the `docs/` directory.

---

## Communication Protocol

The Raspberry Pi communicates with the Arduino using a simple, human-readable
text-based serial protocol.

Commands are sent from the Raspberry Pi and acknowledged by the Arduino.

Example command:


The complete protocol definition, including error handling and acknowledgements,
is documented in `docs/protocol.md`.

---

## Control Concept

- High-level motion planning and sequencing are performed on the Raspberry Pi.
- The Arduino executes time-controlled, interpolated joint movements.
- The system is designed as an open-loop controller, reflecting the limitations
  of standard hobby servos.
- Safety limits and sanity checks are enforced at the Arduino level.

This design mirrors common industrial control concepts, where a non-real-time system
handles planning and coordination while a dedicated controller performs deterministic actuation.

---

## Roadmap

Planned development milestones include:

- [ ] Hardware setup and basic communication
- [ ] Joint-level servo control
- [ ] Motion interpolation and timing
- [ ] Pick-and-place demonstration
- [ ] USB camera integration
- [ ] Vision-assisted grasp correction

---

## Status

This project is under active development.
The current focus is on establishing a robust hardware foundation and a stable
communication layer between the Raspberry Pi and the Arduino.
