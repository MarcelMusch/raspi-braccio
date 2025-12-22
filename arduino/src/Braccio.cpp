/*
 Braccio.cpp - board library Version 2.0
 Written by Andrea Martino and Angelo Ferrante

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Braccio.h"

extern Servo base;
extern Servo shoulder;
extern Servo elbow;
extern Servo wrist_rot;
extern Servo wrist_ver;
extern Servo gripper;

int step_base = HOME_POSITION_BASE;
int step_shoulder = HOME_POSITION_SHOULDER;
int step_elbow = HOME_POSITION_ELBOW;
int step_wrist_rot = HOME_POSITION_WRIST_ROT;
int step_wrist_ver = HOME_POSITION_WRIST_VER;
int step_gripper = HOME_POSITION_GRIPPER;

_Braccio Braccio;

// Initialize Braccio object
_Braccio::_Braccio()
{
}

/**
 * Braccio initialization and set intial position
 * Modifing this function you can set up the initial position of all the
 * servo motors of Braccio
 * @param soft_start_level: default value is 0 (SOFT_START_DEFAULT)
 * You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
 * SOFT_START_DISABLED disable the Braccio movements
 */
unsigned int _Braccio::begin(int soft_start_level)
{
	// Calling Braccio.begin(SOFT_START_DISABLED) the Softstart is disabled and you can use the pin 12
	if (soft_start_level != SOFT_START_DISABLED)
	{
		pinMode(SOFT_START_CONTROL_PIN, OUTPUT);
		digitalWrite(SOFT_START_CONTROL_PIN, LOW);
	}

	// initialization pin Servo motors
	base.attach(11);
	shoulder.attach(10);
	elbow.attach(9);
	wrist_rot.attach(6);
	wrist_ver.attach(5);
	gripper.attach(3);

	// For each step motor this set up the initial degree
	base.write(HOME_POSITION_BASE);
	shoulder.write(HOME_POSITION_SHOULDER);
	elbow.write(HOME_POSITION_ELBOW);
	wrist_ver.write(HOME_POSITION_WRIST_VER);
	wrist_rot.write(HOME_POSITION_WRIST_ROT);
	gripper.write(HOME_POSITION_GRIPPER);

	// Previous step motor position
	step_base = HOME_POSITION_BASE;
	step_shoulder = HOME_POSITION_SHOULDER;
	step_elbow = HOME_POSITION_ELBOW;
	step_wrist_ver = HOME_POSITION_WRIST_VER;
	step_wrist_rot = HOME_POSITION_WRIST_ROT;
	step_gripper = HOME_POSITION_GRIPPER;

	if (soft_start_level != SOFT_START_DISABLED)
		_softStart(soft_start_level);
	return 1;
}

/*
Software implementation of the PWM for the SOFT_START_CONTROL_PIN,HIGH
@param high_time: the time in the logic level high
@param low_time: the time in the logic level low
*/
void _Braccio::_softwarePWM(int high_time, int low_time)
{
	digitalWrite(SOFT_START_CONTROL_PIN, HIGH);
	delayMicroseconds(high_time);
	digitalWrite(SOFT_START_CONTROL_PIN, LOW);
	delayMicroseconds(low_time);
}

/*
 * This function, used only with the Braccio Shield V4 and greater,
 * turn ON the Braccio softly and save it from brokes.
 * The SOFT_START_CONTROL_PIN is used as a software PWM
 * @param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
 */
void _Braccio::_softStart(int soft_start_level)
{
	long int tmp = millis();
	while (millis() - tmp < LOW_LIMIT_TIMEOUT)
		_softwarePWM(80 + soft_start_level, 450 - soft_start_level); // the sum should be 530usec

	while (millis() - tmp < HIGH_LIMIT_TIMEOUT)
		_softwarePWM(75 + soft_start_level, 430 - soft_start_level); // the sum should be 505usec

	digitalWrite(SOFT_START_CONTROL_PIN, HIGH);
}

/**
 * This function allows homing the robot
 */
int _Braccio::Home()
{
	return ServoMovement(20, HOME_POSITION_BASE, HOME_POSITION_SHOULDER, HOME_POSITION_ELBOW, HOME_POSITION_WRIST_VER, HOME_POSITION_WRIST_ROT, HOME_POSITION_GRIPPER, "111111");
}

/**
 * This functions allow you to control all the servo motors
 *
 * @param stepDelay The delay between each servo movement
 * @param vBase next base servo motor degree
 * @param vShoulder next shoulder servo motor degree
 * @param vElbow next elbow servo motor degree
 * @param vWrist_ver next wrist rotation servo motor degree
 * @param vWrist_rot next wrist vertical servo motor degree
 * @param vgripper next gripper servo motor degree
 */
int _Braccio::ServoMovement(int stepDelay, int vBase, int vShoulder, int vElbow, int vWrist_ver, int vWrist_rot, int vgripper, char jointComposition[])
{
	// Check values, to avoid dangerous positions for the Braccio
	if (stepDelay > MAX_stepDelay)
		stepDelay = MAX_stepDelay;
	if (stepDelay < MIN_stepDelay)
		stepDelay = MIN_stepDelay;
	if (vBase < BASE_MIN_ANGLE)
		vBase = BASE_MIN_ANGLE;
	if (vBase > BASE_MAX_ANGLE)
		vBase = BASE_MAX_ANGLE;
	if (vShoulder < SHOULDER_MIN_ANGLE)
		vShoulder = SHOULDER_MIN_ANGLE;
	if (vShoulder > SHOULDER_MAX_ANGLE)
		vShoulder = SHOULDER_MAX_ANGLE;
	if (vElbow < ELBOW_MIN_ANGLE)
		vElbow = ELBOW_MIN_ANGLE;
	if (vElbow > ELBOW_MAX_ANGLE)
		vElbow = ELBOW_MAX_ANGLE;
	if (vWrist_ver < WRIST_VER_MIN_ANGLE)
		vWrist_ver = WRIST_VER_MIN_ANGLE;
	if (vWrist_ver > WRIST_VER_MAX_ANGLE)
		vWrist_ver = WRIST_VER_MAX_ANGLE;
	if (vWrist_rot > WRIST_ROT_MAX_ANGLE)
		vWrist_rot = WRIST_ROT_MAX_ANGLE;
	if (vWrist_rot < WRIST_ROT_MIN_ANGLE)
		vWrist_rot = WRIST_ROT_MIN_ANGLE;
	if (vgripper < GRIPPER_MIN_ANGLE)
		vgripper = GRIPPER_MIN_ANGLE;
	if (vgripper > GRIPPER_MAX_ANGLE)
		vgripper = GRIPPER_MAX_ANGLE;

	//Serial.println("JOINT COMPOSITION: " + String(jointComposition));

	// Now we check what motors have to move by comparing to the joint composition
	// Joint Composition is a binary string where each bit represent a motor
	// from the LSB to the MSB: J1 J2 J3 J4 J5 J6
	// Example: "101011" means move J1, J3, J5 and J6
	if (jointComposition != NULL)
	{
		// J1..J6 => [0]..[5]
		if (jointComposition[0] == '0')
			vBase = step_base;
		if (jointComposition[1] == '0')
			vShoulder = step_shoulder;
		if (jointComposition[2] == '0')
			vElbow = step_elbow;
		if (jointComposition[3] == '0')
			vWrist_ver = step_wrist_ver;
		if (jointComposition[4] == '0')
			vWrist_rot = step_wrist_rot;
		if (jointComposition[5] == '0')
			vgripper = step_gripper;
	}

	int exit = 1;

	// Until the all motors are in the desired position
	while (exit)
	{
		// Check if there is a command from Serial to stop the movement
		if (Serial.available())
		{
			String s = Serial.readStringUntil('\n');
			s.trim();
			if (s == "STOP")
			{
				return 0; // aborted
			}
		}

		// For each servo motor if next degree is not the same of the previuos than do the movement
		if (vBase != step_base)
		{
			base.write(step_base);
			// One step ahead
			if (vBase > step_base)
			{
				step_base++;
			}
			// One step beyond
			if (vBase < step_base)
			{
				step_base--;
			}
		}

		if (vShoulder != step_shoulder)
		{
			shoulder.write(step_shoulder);
			// One step ahead
			if (vShoulder > step_shoulder)
			{
				step_shoulder++;
			}
			// One step beyond
			if (vShoulder < step_shoulder)
			{
				step_shoulder--;
			}
		}

		if (vElbow != step_elbow)
		{
			elbow.write(step_elbow);
			// One step ahead
			if (vElbow > step_elbow)
			{
				step_elbow++;
			}
			// One step beyond
			if (vElbow < step_elbow)
			{
				step_elbow--;
			}
		}

		if (vWrist_rot != step_wrist_rot)
		{
			wrist_rot.write(step_wrist_rot);
			// One step ahead
			if (vWrist_rot > step_wrist_rot)
			{
				step_wrist_rot++;
			}
			// One step beyond
			if (vWrist_rot < step_wrist_rot)
			{
				step_wrist_rot--;
			}
		}

		if (vWrist_ver != step_wrist_ver)
		{
			wrist_ver.write(step_wrist_ver);
			// One step ahead
			if (vWrist_ver > step_wrist_ver)
			{
				step_wrist_ver++;
			}
			// One step beyond
			if (vWrist_ver < step_wrist_ver)
			{
				step_wrist_ver--;
			}
		}

		if (vgripper != step_gripper)
		{
			gripper.write(step_gripper);
			if (vgripper > step_gripper)
			{
				step_gripper++;
			}
			// One step beyond
			if (vgripper < step_gripper)
			{
				step_gripper--;
			}
		}

		// delay between each movement
		delay(stepDelay);

		// It checks if all the servo motors are in the desired position
		if ((vBase == step_base) && (vShoulder == step_shoulder) && (vElbow == step_elbow) && (vWrist_ver == step_wrist_ver) && (vWrist_rot == step_wrist_rot) && (vgripper == step_gripper))
		{
			step_base = vBase;
			step_shoulder = vShoulder;
			step_elbow = vElbow;
			step_wrist_rot = vWrist_rot;
			step_wrist_ver = vWrist_ver;
			step_gripper = vgripper;
			exit = 0;
		}
		else
		{
			exit = 1;
		}
	}
	return 1;
}
