#pragma once

using int16 = int;

//From Controller
double System_Angle(int16, int16);	// Recieves PDO_Rx

//To Controller
int16 Convert_Accel(double);
int16 Convert_Decel(double);
int16 Convert_Speed(double);
int16 Set_SpeedL(int16, double);
int16 Set_SpeedR(int16, double);
char getHiByte(int16);
char getLoByte(int16);