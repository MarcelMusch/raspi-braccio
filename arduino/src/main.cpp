#include <Arduino.h>
#include <Servo.h>
#include <Braccio.h>
#include <stdio.h>

Servo base;      // M1
Servo shoulder;  // M2
Servo elbow;     // M3
Servo wrist_ver; // M4
Servo wrist_rot; // M5
Servo gripper;   // M6

const int J1_MIN = 0;
const int J1_MAX = 180;
const int J2_MIN = 15;
const int J2_MAX = 165;
const int J3_MIN = 0;
const int J3_MAX = 180;
const int J4_MIN = 0;
const int J4_MAX = 180;
const int J5_MIN = 0;
const int J5_MAX = 180;
const int J6_MIN = 10;
const int J6_MAX = 73;
const int DEFAULT_STEP_DELAY = 20; // in milliseconds

void setup()
{
    Serial.begin(115200);
    Braccio.begin();
    delay(2000);
    Serial.println("BRACCIO_READY");
}

void HOME()
{
    Braccio.Home();
    Serial.println("OK");
}

void SET_JOINT_POS(String command)
{
    int J1 = -1, J2 = -1, J3 = -1, J4 = -1, J5 = -1, J6 = -1;
    char jointComposition[7] = "111111"; // Default: move all joints
    int stepDelay = DEFAULT_STEP_DELAY;
    int numArguments = sscanf(command.c_str(), "SET_JOINTS JC=%6s T=%d J1=%d J2=%d J3=%d J4=%d J5=%d J6=%d", jointComposition, &stepDelay, &J1, &J2, &J3, &J4, &J5, &J6);

    /*
    Serial.print("  Joint Composition: "); Serial.println(jointComposition);
    Serial.print("  Step Delay: "); Serial.println(stepDelay);
    Serial.print("  J1: "); Serial.println(J1);
    Serial.print("  J2: "); Serial.println(J2);
    Serial.print("  J3: "); Serial.println(J3);
    Serial.print("  J4: "); Serial.println(J4);
    Serial.print("  J5: "); Serial.println(J5);
    Serial.print("  J6: "); Serial.println(J6);
    */

    // Check arguments count
    if (numArguments < 3 || numArguments > 8)
    {
        Serial.println("ERROR:INVALID_PARAMETERS");
        return;
    }

    // Check joint limits
    if (
        (J1 < J1_MIN || J1 > J1_MAX) ||
        (J2 < J2_MIN || J2 > J2_MAX) ||
        (J3 < J3_MIN || J3 > J3_MAX) ||
        (J4 < J4_MIN || J4 > J4_MAX) ||
        (J5 < J5_MIN || J5 > J5_MAX) ||
        (J6 < J6_MIN || J6 > J6_MAX))
    {
        Serial.println("ERROR:JOINT_LIMIT_EXCEEDED");
        return;
    }

    // Now we perform the movement
    Braccio.ServoMovement(stepDelay, J1, J2, J3, J4, J5, J6, jointComposition);
    Serial.println("OK");
}

void GET_CURRENT_POSITION()
{
    Serial.print("CP ");
    Serial.print("J1=");
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
    Serial.println(gripper.read()); // println ends the frame
}

void STOP()
{
    // Immediate stop logic (hold current position)
    Braccio.ServoMovement(DEFAULT_STEP_DELAY, base.read(), shoulder.read(), elbow.read(), wrist_ver.read(), wrist_rot.read(), gripper.read(), "111111");
    Serial.println("OK");
}

void RESET()
{
    Serial.flush();
    Braccio.Home();
    // Clear errors and reset to idle state
    Serial.println("OK");
}

void handleCommand(const String &line)
{
    if (line == "PING")
    {
        Serial.println("ACK:PING");
    }
    else if (line.startsWith("SET_JOINTS"))
    {
        SET_JOINT_POS(line);
    }
    else if (line == "HOME")
    {
        HOME();
    }
    else if (line == "GET_CURRENT_POSITION")
    {
        GET_CURRENT_POSITION();
    }
    else if (line == "STOP")
    {
        STOP();
    }
    else if (line == "RESET")
    {
        RESET();
    }
    else
    {
        Serial.print("ERR:UNKNOWN_CMD ");
        Serial.println(line);
    }
}

void loop()
{
    static String line;

    while (Serial.available())
    {
        char c = (char)Serial.read();

        // treat both \n and \r as end-of-line
        if (c == '\n' || c == '\r')
        {
            line.trim();
            if (line.length() > 0)
            {
                handleCommand(line);
            }
            line = "";
        }
        else
        {
            line += c;
        }
    }
}
