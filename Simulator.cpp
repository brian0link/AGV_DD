#include <iostream>
#include <string>
#include "AGV_DifferentialDrive.h"
using namespace std;

void transmissionTest() {
	int Motor_Swap = 1;
	int temp = 0;

	int inputAngle = 0;
	double sinAdj;
	double sinOpp;

	//sensorValueX = sinOpp * tVelocity / 10000;
	//sensorValueY = MulDiv(sinAdj, tVelocity, 10000)

	int sensorValueX = 0;
	int sensorValueY = 0;
	int absSensorX = 0;
	int absSensorY = 0;

	cout << "X input: ";
	cin >> sensorValueX;
	cout << "Y input: ";
	cin >> sensorValueY;

	int outputX = 0;
	int outputY = 0;
	int throttle_master_out = 0;
	int throttle_slave_out = 0;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	//				MOTOR SWAP
	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (Motor_Swap = 1) {
		sensorValueX = -1 * sensorValueX;
	}
	else if (Motor_Swap = 2) {
		; sensorValueY = -1 * sensorValueY;
	}
	else if (Motor_Swap = 3) {
		sensorValueX = -1 * sensorValueX;
		sensorValueY = -1 * sensorValueY;
	}
	else if (Motor_Swap = 4) {
		temp = sensorValueY;
		sensorValueY = sensorValueX;
		sensorValueX = temp;
	}
	else if (Motor_Swap = 5) {
		temp = sensorValueY;
		sensorValueY = sensorValueX;
		sensorValueX = -1 * temp;
	}
	else if (Motor_Swap = 6) {
		temp = sensorValueY;
		sensorValueY = -1 * sensorValueX;
		sensorValueX = temp;
	}
	else if (Motor_Swap = 7) {
		temp = sensorValueY;
		sensorValueY = -1 * sensorValueX;
		sensorValueX = -1 * temp;
	}
	else {
		sensorValueX = sensorValueX;
		sensorValueY = sensorValueY;
	}

	//-----------------------------------------------------
	// Maps the joystick into octants and then sends motor directions
	//-----------------------------------------------------

	//Should use get_ABS() but it was acting up
	if (sensorValueX < 0) { absSensorX = -1 * sensorValueX; }
	if (sensorValueY < 0) { absSensorY = -1 * sensorValueY; }

	outputX = sensorValueX;
	outputY = sensorValueY;

	if ((outputX == 0) && (outputY == 0)) {
		throttle_slave_out = 0;
		throttle_master_out = 0;
	}
	else if ((outputX >= outputY) && (outputY >= 0)) {
		throttle_slave_out = outputX;
		throttle_master_out = (outputY - outputX);
	}
	else if ((outputY >= outputX) && (outputX >= 0)) {
		throttle_slave_out = outputY;
		throttle_master_out = (outputY - outputX);
	}
	else if ((outputY >= absSensorX) && (outputX <= 0)) {
		throttle_slave_out = (outputY + outputX);
		throttle_master_out = outputY;
	}
	else if ((absSensorX >= outputY) && (outputY >= 0)) {
		throttle_slave_out = (outputY + outputX);
		throttle_master_out = -1 * outputX;
	}
	else if ((outputX <= outputY) && (outputY <= 0)) {
		throttle_slave_out = outputX;
		throttle_master_out = (outputY - outputX);
	}
	else if ((outputY <= outputX) && (outputX <= 0)) {
		throttle_slave_out = outputY;
		throttle_master_out = (outputY - outputX);
	}
	else if ((absSensorY >= outputX) && (outputY <= 0)) {
		throttle_slave_out = (outputY + outputX);
		throttle_master_out = outputY;
	}
	else if ((outputX >= absSensorY) && (outputY <= 0)) {
		throttle_slave_out = (outputY + outputX);
		throttle_master_out = -1 * outputX;
	}
	else {
		// Should never happen
		throttle_slave_out = 0;
		throttle_master_out = 0;
	}

	cout << "Throttle Manipulator: " << "L: " << throttle_master_out << "   R: " << throttle_slave_out << endl;

	int abs_Master = -1 * throttle_master_out;
	int abs_Slave = -1 * throttle_slave_out;
	int L_percent = 0;
	int R_percent = 0;

	if (throttle_master_out>abs_Slave) {
		L_percent = 100;
		R_percent = throttle_slave_out * 100 / throttle_master_out;
		if (R_percent < -100) { R_percent = -100; }
	}
	else if (abs_Master<throttle_slave_out) {
		L_percent = throttle_master_out * 100 / throttle_slave_out;
		R_percent = 100;
		if (L_percent < -100) { L_percent = -100; }
	}
	else {
		L_percent = 100;
		R_percent = 100;
	}

	cout << "Controller Output: " << "L: " << L_percent << "  R: " << R_percent << endl;

	double h = 0;
	double Angle = 0;

	if ((R_percent == 100) && (L_percent<0)) {
		h = 100 + L_percent;
		Angle = asin(h / sqrt((R_percent*R_percent) + (h*h)));
	}
	else if (R_percent == 100) {
		h = L_percent - 100;
		Angle = asin(h / sqrt((R_percent*R_percent) + (h*h)));
	}
	else if ((L_percent == 100) && (R_percent>0)) {
		h = 100 - R_percent;
		Angle = asin(h / sqrt((L_percent*L_percent) + (h*h)));
	}
	else if ((L_percent == 100)) {
		h = R_percent + 100;
		Angle = asin(h / sqrt((L_percent*L_percent) + (h*h)));
	}
	else {
		h = 0;
		Angle = 0;
	}

/*	double Angle = System_Angle((int16)throttle_master_out, (int16)throttle_slave_out);*/
	double degrees = Angle*180.0 / 3.14159265358979;

	cout << "Enterprise Output: " << "Angle: " << Angle << " Degrees: " << degrees << endl;
	cout << endl;
}

void main() {
	int in = 0;
	while (in == 0) {
		transmissionTest();
		cout << "Continue: ";
		cin >> in;
		cout << endl;
	}
}