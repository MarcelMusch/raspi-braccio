# Serial Communication Protocol

## Purpose

This document defines the serial command protocol between the Raspberry Pi (high-level control,
C# application) and the Arduino (real-time motion controller) for the TinkerKit Braccio robotic arm.

The protocol is designed to be:
- human-readable and easy to debug
- deterministic on the Arduino side
- robust against invalid commands and timeouts
- simple to implement on both sides

---

## Transport

- Physical link: USB (CDC ACM)
- Device (typical on Linux): `/dev/ttyACM0`
- Baud rate: `115200`
- Data bits: `8`
- Parity: `None`
- Stop bits: `1`
- Line ending (recommended): `LF` (`\n`)
- Encoding: ASCII/UTF-8 (ASCII subset recommended)

---

## Message Framing

- One command per line
- Each line ends with `\n`
- Empty lines are ignored
- Whitespace between tokens is allowed (one or more spaces)

Example:
```
PING\n
```

---

## General Rules

- All commands are case-insensitive (`PING` == `ping`)
- Parameters are key/value pairs in the form `KEY=VALUE`
- Unknown commands must return an error response
- Out-of-range parameters must return an error response
- Arduino must never execute unsafe motion (limits enforced on Arduino side)

---

## Responses

### OK / Acknowledge
```
ACK[:<optional-info>]
```

Examples:
```
ACK
ACK:PING
ACK:IDLE
```

### Error
```
ERR:<code>[:<optional-info>]
```

Examples:
```
ERR:UNKNOWN_CMD
ERR:INVALID_PARAM:J3
ERR:OUT_OF_RANGE:J2
ERR:BUSY
ERR:TIMEOUT
```

### Status (optional but recommended)

Examples:
```
STATUS:READY
STATUS:IDLE
STATUS:MOVING
STATUS:ERROR:OUT_OF_RANGE
```

---

## Connection Handshake

After boot/reset the Arduino should print exactly one line:

```
BRACCIO_READY
```

The Raspberry Pi may respond with:
```
PING
```

Expected response:
```
ACK:PING
```

---

## Commands (MVP)

### 1) `PING`
Health check.

Command:
```
PING
```

Response:
```
ACK:PING
```

---

### 2) `GET_STATUS`
Requests current controller state.

Command:
```
GET_STATUS
```

Response Examples:
```
STATUS:IDLE
STATUS:MOVING
STATUS:ERROR:LIMIT
```

---

### 3) `STOP`
Immediate stop request.
Implementation may either stop motion immediately or move to a safe hold position.

Command:
```
STOP
```

Response:
```
ACK
```

If stopping is not possible in current state:
```
ERR:BUSY
```

---

### 4) `HOME`
Moves to a defined safe home position (joint angles defined in firmware).

Command:
```
HOME
```

Response:
```
ACK
```

If controller is busy:
```
ERR:BUSY
```

---

### 5) `MOVEJ` (Joint Move)
Moves the robot to target joint angles.
Angles are given in degrees.

Command format:
```
MOVEJ J1=<deg> J2=<deg> J3=<deg> J4=<deg> J5=<deg> J6=<deg> [T=<ms>]
```

Parameters:
- `J1..J6` (required): target joint angles in degrees
- `T` (optional): move duration in milliseconds (e.g. `T=1500`)
  - If omitted: Arduino uses a default duration

Examples:
```
MOVEJ J1=90 J2=45 J3=30 J4=0 J5=0 J6=20
MOVEJ J1=90 J2=45 J3=30 J4=0 J5=0 J6=20 T=1500
```

Responses:
- Success:
```
ACK
```

- Invalid parameter:
```
ERR:INVALID_PARAM:J3
```

- Out of range:
```
ERR:OUT_OF_RANGE:J2
```

- Busy:
```
ERR:BUSY
```

Notes:
- Arduino must validate that all joints are within configured limits before executing.
- Arduino should interpolate joint motion over time (no step jumps).

---

### 6) `GRIP`
Opens or closes the gripper. The gripper is mapped to `J6` on Braccio setups, but is
exposed as a dedicated command for readability.

Command format:
```
GRIP POS=<deg> [T=<ms>]
```

Example:
```
GRIP POS=10
GRIP POS=40 T=600
```

Response:
```
ACK
```

---

## Timing and Reliability

- The Raspberry Pi should treat a missing response within a timeout as an error.
- Recommended host timeout: 1000–2000 ms per command (tune later).
- Arduino should avoid long blocking reads and should parse line input incrementally.

---

## Safety and Limits

- Arduino firmware must enforce joint limits.
- If a command violates limits, Arduino must respond with `ERR:OUT_OF_RANGE:<joint>`.
- The controller should define a safe `HOME` pose.
- Servo power is external; the Arduino must never attempt to power servos from USB.

---

## Versioning (recommended)

To allow future protocol changes, a version command is recommended:

Command:
```
GET_PROTO
```

Response:
```
ACK:PROTO=1
```

Protocol version for this document:
- `PROTO=1`
