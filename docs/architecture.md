# System Architecture

## Overview

This project implements a distributed control architecture for the Arduino TinkerKit Braccio
robotic arm. The system is designed around a clear separation between high-level control logic
and deterministic low-level actuation.

A Raspberry Pi is used as the main control unit, executing motion sequences, state machines,
and optional vision processing. An Arduino acts as a dedicated real-time controller responsible
for servo timing, motion interpolation, and safety constraints.

The architecture avoids proprietary robot simulation or offline programming software and
focuses on direct, transparent control of the hardware.

---

## Architecture Diagram

```
PC (Development)
|
| SSH / SCP
|
Raspberry Pi
|
| USB Serial
|
Arduino UNO
|
| PWM
|
Braccio Servos
```

---

## Components and Responsibilities

### PC (Development Environment)

The PC is used exclusively for development purposes and is not part of the runtime system.

Responsibilities:
- Source code development
- Version control
- Deployment to the Raspberry Pi
- Remote debugging and monitoring

---

### Raspberry Pi

The Raspberry Pi serves as the high-level controller of the robotic system.

Responsibilities:
- Execution of high-level motion sequences
- Implementation of state machines (e.g. idle, moving, error)
- Management of communication with the Arduino
- Optional processing of camera input for vision-based extensions
- Logging and diagnostics

The Raspberry Pi runs a Linux-based operating system and is not required to meet real-time
constraints.

---

### Arduino

The Arduino functions as a dedicated real-time motion controller.

Responsibilities:
- Deterministic generation of PWM signals for servo control
- Interpolation of joint movements over time
- Enforcement of joint limits and safety constraints
- Parsing and validation of commands received from the Raspberry Pi
- Reporting of execution status and errors

The Arduino operates without an operating system and provides predictable timing behavior.

---

### Braccio Robotic Arm

The Braccio robotic arm consists of six servo-driven joints and a gripper.

Characteristics:
- Open-loop position control using hobby servos
- Limited payload and positioning accuracy
- Mechanical backlash and compliance inherent to the design

These characteristics are explicitly accounted for in the system design.

---

## Communication Paths

### PC ↔ Raspberry Pi

- Medium: Ethernet (LAN or WLAN)
- Protocols: SSH, SCP
- Purpose: Development, deployment, and system monitoring

---

### Raspberry Pi ↔ Arduino

- Medium: USB serial (CDC ACM)
- Communication style: Command-response
- Protocol: Text-based, human-readable command format

This link represents the primary control interface of the system.

---

### Arduino ↔ Braccio Servos

- Medium: Direct PWM signals via the Braccio shield
- Timing: Deterministic, hardware-controlled

No feedback from the servos is assumed.

---

## Design Decisions

### Separation of Control Layers

High-level planning and sequencing are separated from low-level actuation to ensure
deterministic servo control while maintaining flexibility in the control logic.

---

### Use of USB Serial Communication

USB serial communication was chosen for its simplicity, robustness, and native support on
both the Raspberry Pi and the Arduino.

---

### Exclusion of Proprietary Simulation Software

No proprietary robot simulation or offline programming tools are used.
All motion control is performed directly on the target hardware.

---

## Acceptance Criteria

The architecture is considered complete when:

- A clear and documented system architecture is available
- Responsibilities of Raspberry Pi and Arduino are clearly defined
- Communication paths between all system components are specified
- The architecture supports the defined project goal and MVP
