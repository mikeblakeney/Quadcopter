#ifndef DEFINES_H
#define DEFINES_H

#define MOTOR_A 6
#define MOTOR_B 9
#define MOTOR_C 10
#define MOTOR_D 11

#define THROTTLE_PIN 3
#define ROLL_PIN	4
#define PITCH_PIN	7

#define THROTTLE_MIN 0
#define THROTTLE_MAX 2000

#define PITCH_MAX	30.0
#define PITCH_MIN	-30.0

#define ROLL_MAX	30.0
#define ROLL_MIN	-30.0

#define THROTTLE_SIG_MAX	1800
#define THROTTLE_SIG_MIN	1100

#define SIG_TOLERANCE		50

#define ARM_ESC_SIG			900

#define PITCH_SIG_MAX	1875
#define PITCH_SIG_MIN	1075

#define PITCH_BUFF_SIZE 5

#define ROLL_SIG_MAX	1890
#define ROLL_SIG_MIN	1030

#define MPU_INT_PIN 2

#define CYCLE_TIME	100

#define MPU_ARRAY_SIZE	7

#define ANALOG_TO_V_SENSOR	12.998
#define VOLTAGE_SENSOR_PIN	A0
#define LOW_LIPO_VOLTAGE	10.0
#define V_ALERT_PIN			8

#endif