# Arduino Serial Commands

This document outlines the serial commands to be implemented on the Arduino side for communication with the Raspberry Pi.

## Command List

### 1. `PING`
- **Description**: Health check command.
- **Command**: `PING`
- **Response**: `ACK:PING`
- **Notes**: Used to verify communication between Raspberry Pi and Arduino.

### 2. `MOVE`
- **Description**: Move the robotic arm to specified joint angles.
- **Command**: `MOVE J1=<deg> J2=<deg> J3=<deg> J4=<deg> J5=<deg> J6=<deg> [T=<ms>]`
  - `J1..J6` (required): Target joint angles in degrees.
  - `T` (optional): Move duration in milliseconds. Defaults to a predefined value if omitted.
- **Response**:
  - `OK`: Command executed successfully.
  - `ERROR:E02`: Joint limit exceeded.
  - `ERROR:E03`: Invalid parameters.

### 3. `HOME`
- **Description**: Move the robotic arm to its default home position.
- **Command**: `HOME`
- **Response**:
  - `OK`: Homing successful.
  - `ERROR:E01`: Command failed.

### 4. `STATUS`
- **Description**: Request the current joint angles.
- **Command**: `STATUS`
- **Response**: `STATUS J1=<deg> J2=<deg> J3=<deg> J4=<deg> J5=<deg> J6=<deg>`

### 5. `STOP`
- **Description**: Immediately stop all movements.
- **Command**: `STOP`
- **Response**:
  - `OK`: Stop command acknowledged.
  - `ERROR:E04`: Unable to stop.

### 6. `RESET`
- **Description**: Clear errors and return to the `Idle` state.
- **Command**: `RESET`
- **Response**:
  - `OK`: Errors cleared.
  - `ERROR:E05`: Reset failed.

## Error Codes
- `E01`: Command failed.
- `E02`: Joint limit exceeded.
- `E03`: Invalid parameters.
- `E04`: Unable to stop.
- `E05`: Reset failed.

## Implementation Notes
1. **Command Parsing**:
   - Use a line-based parser to read commands.
   - Validate the format and parameters of each command.

2. **Safety Checks**:
   - Enforce joint limits before executing movements.
   - Ensure smooth interpolation for all joint movements.

3. **State Management**:
   - Maintain a state machine to handle `Idle`, `Moving`, `Error`, `Homing`, and `Shutdown` states.

4. **Timing**:
   - Use non-blocking delays to ensure responsiveness.
   - Implement a timeout mechanism for unresponsive commands.

5. **Testing**:
   - Test each command individually.
   - Verify error handling and edge cases.

---

This document serves as a reference for implementing the Arduino firmware to handle serial communication with the Raspberry Pi.