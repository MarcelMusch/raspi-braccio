#include <Arduino.h>
#include <Servo.h>
#include <Braccio.h>

Servo base;      // M1
Servo shoulder;  // M2
Servo elbow;     // M3
Servo wrist_ver; // M4
Servo wrist_rot; // M5
Servo gripper;   // M6

void setup()
{
    Serial.begin(115200);
    Braccio.begin();
    delay(2000);       
    Serial.println("BRACCIO_READY");
}

void HELLO() {
    base.write(90);          // Move base to neutral
    // Wave-like greeting pattern
    for (int i = 0; i < 3; i++) {
        shoulder.write(90); // Move shoulder to neutral
        elbow.write(45);    // Raise elbow
        delay(500);
        elbow.write(90);    // Lower elbow
        delay(500);
    }
}

void HOME() {
    base.write(90);      // Neutral base position
    shoulder.write(90);  // Neutral shoulder position
    elbow.write(90);     // Neutral elbow position
    wrist_ver.write(90); // Neutral wrist vertical position
    wrist_rot.write(90); // Neutral wrist rotation position
    gripper.write(90);   // Neutral gripper position
}

void MOVE(String command) {
    int J1, J2, J3, J4, J5, J6, T = 1000; // Default duration
    if (sscanf(command.c_str(), "MOVE J1=%d J2=%d J3=%d J4=%d J5=%d J6=%d T=%d", &J1, &J2, &J3, &J4, &J5, &J6, &T) >= 6) {
        if (J1 < 0 || J1 > 180 || J2 < 15 || J2 > 165 || J3 < 0 || J3 > 180 || J4 < 0 || J4 > 180 || J5 < 0 || J5 > 180 || J6 < 10 || J6 > 73) {
            Serial.println("ERROR:E02"); // Joint limit exceeded
            return;
        }
        Braccio.ServoMovement(T, J1, J2, J3, J4, J5, J6);
        Serial.println("OK");
    } else {
        Serial.println("ERROR:E03"); // Invalid parameters
    }
}

void STATUS() {
    Serial.print("STATUS J1=");
    Serial.print(base.read());
    Serial.print(" J2=");
    Serial.print(shoulder.read());
    Serial.print(" J3=");
    Serial.print(elbow.read());
    Serial.print(" J4=");
    Serial.print(wrist_ver.read());
    Serial.print(" J5=");
    Serial.print(wrist_rot.read());
    Serial.print(" J6=");
    Serial.println(gripper.read());
}

void STOP() {
    // Immediate stop logic (hold current position)
    Braccio.ServoMovement(20, base.read(), shoulder.read(), elbow.read(), wrist_ver.read(), wrist_rot.read(), gripper.read());
    Serial.println("OK");
}

void RESET() {
    // Clear errors and reset to idle state
    Serial.println("OK");
}

void loop()
{
    if (!Serial.available())
        return;

    String line = Serial.readStringUntil('\n');
    line.trim();

    if (line.length() == 0)
        return;

    if (line == "PING") {
        Serial.println("ACK:PING");
    } else if (line.startsWith("MOVE")) {
        MOVE(line);
    } else if (line == "HOME") {
        HOME();
        Serial.println("OK");
    } else if (line == "STATUS") {
        STATUS();
    } else if (line == "STOP") {
        STOP();
    } else if (line == "RESET") {
        RESET();
    } else {
        Serial.print("ERR:UNKNOWN_CMD ");
        Serial.println(line);
    }
}
