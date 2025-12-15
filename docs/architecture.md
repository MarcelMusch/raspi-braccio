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
