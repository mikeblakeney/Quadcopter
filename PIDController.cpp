 #include "PIDController.h"

#include <arduino.h>
#include <math.h>

PIDController::PIDController()
{
	Kp = 0;
	Ki = 0;
	Kd = 0;

	errSum = 0;
	prevErr = 0;

	dt = 100;

	lastTime = millis();
}

PIDController::PIDController(float Kp, float Ki, float Kd)
{
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;

	errSum = 0;
	prevErr = 0;

	dt = 100;


	lastTime = millis();
}
void PIDController::compute(double state, double desired, double &output)
{
	
	long now = millis();
	float current_dt = (now - lastTime);
	


	if( current_dt >= dt)
	{
		double error = desired - state;
		
		current_dt = current_dt / 1000.0;

	

		double e_P = error;

		double e_I = errSum + error * current_dt;

		double e_D = (error - prevErr)/current_dt;

		output = Kp * e_P + Ki * e_I + Kd * e_D;
		
		errSum = e_I;
		prevErr = error;
		lastTime = now;
	}
}

void PIDController::setTunings(float Kp, float Ki, float Kd)
{
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
}

void PIDController::setTiming(int time)
{
	this->dt = time;
}