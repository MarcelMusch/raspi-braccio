#ifndef BRACCIO_H_
#define BRACCIO_H_

#include <Arduino.h>
#include <Servo.h>

// You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
#define SOFT_START_DISABLED -999

// The default value for the soft start
#define SOFT_START_DEFAULT 0

// The software PWM is connected to PIN 12. You cannot use the pin 12 if you are using a Braccio shield V4 or newer
#define SOFT_START_CONTROL_PIN 12

// Low and High Limit Timeout for the Software PWM
#define LOW_LIMIT_TIMEOUT 2000
#define HIGH_LIMIT_TIMEOUT 6000

// Define home position for each servo motor
#define HOME_POSITION_BASE 0
#define HOME_POSITION_SHOULDER 90
#define HOME_POSITION_ELBOW 90
#define HOME_POSITION_WRIST_VER 90
#define HOME_POSITION_WRIST_ROT 90
#define HOME_POSITION_GRIPPER 10

// Define servo motors limits
#define BASE_MIN_ANGLE 0
#define BASE_MAX_ANGLE 180
#define SHOULDER_MIN_ANGLE 15
#define SHOULDER_MAX_ANGLE 165
#define ELBOW_MIN_ANGLE 0
#define ELBOW_MAX_ANGLE 180
#define WRIST_VER_MIN_ANGLE 0
#define WRIST_VER_MAX_ANGLE 180
#define WRIST_ROT_MIN_ANGLE 0
#define WRIST_ROT_MAX_ANGLE 180
#define GRIPPER_MIN_ANGLE 10
#define GRIPPER_MAX_ANGLE 73

// Define limits for step delay
#define MAX_stepDelay 1000
#define MIN_stepDelay 10

class _Braccio
{

public:
  _Braccio();

  /**
   * Braccio initializations and set intial position
   * Modifing this function you can set up the initial position of all the
   * servo motors of Braccio
   *@param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
   * You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
   */
  unsigned int begin(int soft_start_level = SOFT_START_DEFAULT);

  /**
   * This function allow the user to control all the servo motors in the Braccio
   */
  int ServoMovement(int delay, int Vbase, int Vshoulder, int Velbow, int Vwrist_ver, int Vwrist_rot, int Vgripper, char jointComposition[]);

  /**
   * This function alows homing the robot
   */
  int Home();

private:
  /*
   * This function, used only with the Braccio Shield V4 and greater,
   * turn ON the Braccio softly and save Braccio from brokes.
   * The SOFT_START_CONTROL_PIN is used as a software PWM
   * @param soft_start_level: the minimum value is -70, , default value is 0 (SOFT_START_DEFAULT)
   */
  void _softStart(int soft_start_level);

  /*
   * Software implementation of the PWM for the SOFT_START_CONTROL_PIN,HIGH
   * @param high_time: the time in the logic level high
   * @param low_time: the time in the logic level low
   */
  void _softwarePWM(int high_time, int low_time);
};

extern _Braccio Braccio;

#endif // BRACCIO_H_
