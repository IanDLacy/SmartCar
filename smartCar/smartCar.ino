/*
 Name:		Brother.ino
 Created:	12/4/2018 1:29:34 AM
 Author:	Chris Sayen
*/

#include <Servo.h>
#include <NewPing.h>
#define enA 8
#define enB 9
#define in1 4
#define in2 5
#define in3 6
#define in4 7
#define trig 3
#define echo 2 
int maximumRange = 200;
int pwm = 255;
int pos = 0;
long duration;
long dist;
long cm;
int i;
int servoDelay = 2;
int leftDegrees = 90;
int rightDegrees = 90;
int far = 100;
int near = 30;
Servo RangeFinderServo;

NewPing sonar(trig, echo, maximumRange);



void setup()
{
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	RangeFinderServo.attach(11);

}



void test()
{

}



bool noObstacle(int obsDis)
{
	dist = readRangefinder();
	if (dist > obsDis)
	{
		return true;
	}
	else
	{
		return false;
	}
}



void forward()
{
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	analogWrite(enA, pwm);
	analogWrite(enB, pwm);
	while (noObstacle(near));
	initLook();
}



void initLook()
{
	leftDegrees = 90;
	rightDegrees = 90;
	analogWrite(enA, 0);
	analogWrite(enB, 0);
	lookRight();
	lookLeft();
	decideDirection();
}



void lookRight()
{
	for (pos = 90; pos >= 0; pos -= 1)
	{
		RangeFinderServo.write(pos);
		if (noObstacle(far))
		{
			rightDegrees = (-1 * pos) + 90;
			RangeFinderServo.write(90);
			lookLeft();
		}
		delay(servoDelay);
	}
	RangeFinderServo.write(90);
	delay(10);
}



void lookLeft()
{
	for (pos = 90; pos <= 180; pos += 1)
	{
		RangeFinderServo.write(pos);
		if (noObstacle(far))
		{
			leftDegrees = pos - 90;
			RangeFinderServo.write(90);
			delay(10);
			decideDirection();
		}
		delay(servoDelay);
	}
	RangeFinderServo.write(90);
	delay(10);
}



void decideDirection()
{
	if (leftDegrees > rightDegrees)
	{
		turn("right");
	}
	else
	{
		turn("left");
	}
}



void turn(String direction)
{
	digitalWrite(in2, LOW);
	digitalWrite(in1, HIGH);
	digitalWrite(in4, LOW);
	digitalWrite(in3, HIGH);
	analogWrite(enA, pwm);
	analogWrite(enB, pwm);
	delay(100);

	if (direction == "right")
	{
		while (!noObstacle(far))
		{
			digitalWrite(in2, LOW);
			digitalWrite(in1, HIGH);
			digitalWrite(in3, LOW);
			digitalWrite(in4, HIGH);
			analogWrite(enA, pwm);
			analogWrite(enB, pwm);
			delay(10);
			analogWrite(enA, 0);
			analogWrite(enB, 0);
			delay(10);
		}
	}
	else
	{
		while (noObstacle(far))
		{
			digitalWrite(in1, LOW);
			digitalWrite(in2, HIGH);
			digitalWrite(in4, LOW);
			digitalWrite(in3, HIGH);
			analogWrite(enA, pwm);
			analogWrite(enB, pwm);
			delay(10);
			analogWrite(enA, 0);
			analogWrite(enB, 0);
			delay(10);
		}	
	}
	forward();
}



long readRangefinder()
{
	pinMode(trig, OUTPUT);
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	pinMode(echo, INPUT);
	duration = pulseIn(echo, HIGH);
	cm = microsecondsToCentimeters(duration);
	return cm;
}



long microsecondsToCentimeters(long microseconds)
{
	return microseconds / 29 / 2;
}



void loop()
{
	forward();
	/*test();*/
}