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
    Braccio.ServoMovement(20, 90, 90, 90, 90, 90, 10); // Initial position
    Serial.println("BRACCIO_READY");
}

void loop()
{
    if (!Serial.available())
        return;

    String line = Serial.readStringUntil('\n');
    line.trim();

    if (line.length() == 0)
        return;

    if (line == "PING")
    {
        Serial.println("ACK");
    }
    else
    {
        Serial.print("ERR:UNKNOWN_CMD ");
        Serial.println(line);
    }
}
