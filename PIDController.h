#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PIDController
{
public:
	PIDController();
	PIDController(float Kp, float Ki, float Kd);

	void compute(double state, double goal, double &output);
	void setTunings(float Kp, float Ki, float Kd);
	void setTiming(int time);

private:
	float Kp;
	float Ki;
	float Kd;

	float errSum;
	float prevErr;

	long lastTime;
	int dt;

	bool isAngle;

};

#endif