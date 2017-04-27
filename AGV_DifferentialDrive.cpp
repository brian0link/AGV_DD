#include "AGV_DifferentialDrive.h"
#include <math.h>

#define PI 3.14159265
using int16 = int;	// Curtis 123X Controllers use signed 16 bit numbers

constexpr double MAX_SYSTEM_VELOCITY = 5;

// Returns the angle that the system is currently traveling
// For calculations, see https://github.com/brian0link/AGV_DD/blob/master/AGV_DD_Calcs.JPG
// TODO Allow for reverse control
double System_Angle(int16 RPM_L, int16 RPM_R)
{
	double angle = 0.0;
	double abs_L = (double) abs(RPM_L);
	double abs_R = (double) abs(RPM_R);

	//Convert to percent
	if (RPM_L > RPM_R) {
		abs_L = 100;
		abs_R = (abs_R / abs_L) * 100;
	} else {	// RPM_R >= RPM_L
		abs_L = (abs_L / abs_R) * 100;
		abs_R = 100;
	}

	//  \ 2 | 3 /
	//   \  |  /
	//    \ | /
	//	1  \|/  4
	//	----*----

	if (abs_R == 100) {		//Sections 1 or 2
		if (RPM_L < 0) {	//Section 1
			// L = -(100-(100*Y))
			// Angle = asin((100+L)/sqrt(R^2+(100+L)^2)
			angle = asin((100 - abs_L) / (sqrt(pow(abs_R, 2) + pow(100 - abs_L, 2))));
		} else {			//Section 2
			// L = (100+(100*Y))
			// Angle = asin((L-100)/sqrt(R^2+(L-100)^2)
			angle = asin((100 - abs_L) / (sqrt(pow(abs_R, 2) + pow(100 - abs_L, 2))));
		}
	} else {				//Sections 3 or 4
		if (RPM_R > 0) {	//Section 3
			// R = (100-(100*Y))
			// Angle = asin((100-R)/sqrt(L^2+(100-R)^2)
			angle = asin((100 - abs_R) / (sqrt(pow(abs_L, 2) + pow(100 - abs_R, 2))));
		} else {			//Section 4
			// R = -(100-(100*Y))
			// Angle = asin((R+100)/sqrt(L^2+(R+100)^2)
			angle = asin((abs_R + 100) / (sqrt(pow(abs_L, 2) + pow(abs_R + 100, 2))));
		}
	}

	return angle;
}

 
//void PDO_Tx(int16 accel, int16 decel, int16 throttle, int16 angle)
//{
//}

//Converts from scientific acceleration to Curtis acceleration
//v(final) / accel = accel_rate
//Must be in the same base units as MAX_SYSTEM_VELOCITY
int16 Convert_Accel(double accel)
{
	int16 accel_rate = 100;	//Range 100 - 30000

	accel_rate = (int16) (MAX_SYSTEM_VELOCITY / accel)*1000;
	if (accel_rate < 100) {
		accel_rate = 100;
	} else if (accel_rate > 30000) {
		accel_rate = 30000;
	}

	return accel_rate;
}

//Converts from scientific acceleration to Curtis acceleration
//Must be in the same base units as MAX_SYSTEM_VELOCITY
int16 Convert_Decel(double decel)
{
	int16 decel_rate = 100;	//Range 100 - 30000

	decel_rate = (int16)(MAX_SYSTEM_VELOCITY / decel) * 1000;
	if (decel_rate < 100) {
		decel_rate = 100;
	}
	else if (decel_rate > 30000) {
		decel_rate = 30000;
	}

	return decel_rate;
}

//Converts a speed command to a thottle command
int16 Convert_Speed(double velocity)
{
	int16 throttle_signal = 0;
	
	if (velocity > MAX_SYSTEM_VELOCITY) {
		throttle_signal = 32767;
	} else if (velocity < (-1 * MAX_SYSTEM_VELOCITY)) {
		throttle_signal = -32768;
	} else {
		throttle_signal = (int16)(velocity / MAX_SYSTEM_VELOCITY) * 32767;
	}

	return throttle_signal;
}

//Sets the speed of the left controller
//velocity in units of MAX_SYSTEM_VELOCITY
//steer_angle in degrees
int16 Set_SpeedL(int16 velocity, double steer_angle)
{
	int16 commanded_throttle_l = Convert_Speed(velocity);
	double cos_angle = cos((steer_angle)*PI / 180);
	if (steer_angle > 0) { cos_angle = 1; }
	commanded_throttle_l = (int16)(commanded_throttle_l * cos_angle);

	return commanded_throttle_l;
}

//Sets the speed of the right controller
//velocity in units of MAX_SYSTEM_VELOCITY
//steer_angle in degrees
int16 Set_SpeedR(int16 velocity, double steer_angle)
{
	int16 commanded_throttle_r = Convert_Speed(velocity);
	double cos_angle = cos((steer_angle)*PI / 180);
	if (steer_angle < 0) { cos_angle = 1; }
	commanded_throttle_r = (int16)(commanded_throttle_r * cos_angle);

	return commanded_throttle_r;
}

