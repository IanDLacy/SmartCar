/*
 Name:		Brother.ino
 Created:	12/4/2018 1:29:34 AM
 Author:	Chris Sayen
*/

#include <Servo.h>						// Include library to control servo under ultrasonic sensor.
#include <NewPing.h>					  // Include library to control ultrasonic sensor.
#define enA 8							 // Compiler will replace all instances of "enA" with "8".
#define enB 9							 // Compiler will replace all instances of "enB" with "9".
#define in1 4							 // Compiler will replace all instances of "in1" with "4".
#define in2 5							 // Compiler will replace all instances of "in2" with "5".
#define in3 6							 // Compiler will replace all instances of "in3" with "6".
#define in4 7							 // Compiler will replace all instances of "in4" with "7".
#define trig 3							// Compiler will replace all instances of "trig" with "3".
#define echo 2 						   // Compiler will replace all instances of "echo" with "2".
int maximumRange = 200;				   // Maximum range for ultrasonic sensor in centimeters.
int pwm = 255;							// Default value (0-255) for Pulse Width Modulation to control drive motor speed.
int pos = 0;							  // Variable representing the Sensor Servo position.												
long duration;							// Variable to contain the duration returned from the ultrasonic sensor.
long dist;							    // Variable to contain distance read from ultrasonic sensor.
long cm;							      // Variable to hold the distance value as converted from the duration read from the ultrasonic sensor.
int i;									// Generic counter for loops.
int servoDelay = 2;					   // Milliseconds to pause the program during function "lookRight" if "lookLeft" does not get called.
int leftDegrees = 90;					 // Number of degrees the sensor servo will turn left.
int rightDegrees = 90;					// Function "lookRight" resets this to the difference between 90 and a counter.
int far = 100;							// Threshold distance (cm) for detecting [or not detecting?] an obstacle in function "lookRight".
int near = 30;							// Threshold distance (cm) for detecting an obstacle.
Servo RangeFinderServo;				   // Create object "RangeFinderServo" as new instance of library "Servo".

NewPing sonar(trig, echo, maximumRange);  // Create object "sonar", declare trigger and echo pins, and set maximum range.



void setup()							  // Function "setup" is the standard Sketch function that runs once.
{
	pinMode(enA, OUTPUT);				 // Set pin "enA" as an "OUTPUT".
	pinMode(enB, OUTPUT);				 // Set pin "enB" as an "OUTPUT".
	pinMode(in1, OUTPUT);				 // Set pin "in1" as an "OUTPUT".
	pinMode(in2, OUTPUT);				 // Set pin "in2" as an "OUTPUT".
	pinMode(in3, OUTPUT);				 // Set pin "in3" as an "OUTPUT".
	pinMode(in4, OUTPUT);				 // Set pin "in4" as an "OUTPUT".
	RangeFinderServo.attach(11);		  // Attach object "RangeFinderSero" to pin "11".

}



void test()									// Custom function for testing components.
{											  // Move vehicle forward.
	digitalWrite(in1, LOW);						// Set half of H-Bridge A to open.
	digitalWrite(in2, HIGH);				   	// Set half of H-Bridge A to closed.
	digitalWrite(in3, LOW);						// Set half of H-Bridge B to open.
	digitalWrite(in4, HIGH);				   	// Set half of H-Bridge B to closed.
	analogWrite(enA, pwm);						 // Apply power to motor A (left) at full voltage (pwm is currently "255").
	analogWrite(enB, pwm);						 // Apply power to motor B (right) at full voltage (pwm is currently "255").
	/*for (pos = 90; pos >= 0; pos -= 1)	   // Sensor Servo position is "90". While greater than or equal to "0", do the following, then decrease by "1".
	{
		RangeFinderServo.write(pos);		   // Move the Sensor Servo position to the current value from above.
		delay(5);							  // Pause the program for 5 milliseconds. 
	}
	for (pos = 0; pos <= 180; pos += 1)		// Sensor Servo position is "0". While less than or equal to "180", do the following, then increase by "1". 
	{
		RangeFinderServo.write(pos);		   // Move the Sensor Servo position to the current value from above.
		delay(5);							  // Pause the program for 5 milliseconds.
	}
	analogWrite(enA, 0);					   // Kill power to motor A (left).
	analogWrite(enB, 0);*/					 // Kill power to motor B (right).
}



bool noObstacle(int obsDis)					// Declare boolean function "noObstacle" with integer parameter "obsDis" (obstacle distance).
{
	dist = readRangefinder();				  // Take a reading from the ultrasonic sensor and store it in this variable.
	if (dist > obsDis)						 // If the reading is greater than the value of the parameter passed through the function, do the following.
	{
		return true;						   // Function passes boolean value "true".
	}
	else									   // Otherwise do the following.
	{
		return false;						  // Function passes boolean value "false".
	}
}



void forward()								// Declare valueless function "forward" with no parameters.
{
	digitalWrite(in1, LOW);				   // See comments for function "test" above.
	digitalWrite(in2, HIGH);				  // ~~
	digitalWrite(in3, LOW);				   // ~~
	digitalWrite(in4, HIGH);				  // ~~
	analogWrite(enA, pwm);				    // ~~
	analogWrite(enB, pwm);				    // ~~
	while (noObstacle(near));				 // While the value of function "noObstacle", with the value of "near" as its parameter, returns true, do nothing, otherwise continue.
	initLook();							   // When an obstable is encountered (see above), call function "initLook".
}



void initLook()								// Declare function "initLook" to be called when an obstacle is detected.
{
	leftDegrees = 90;						  // Set "leftDegrees" to "90".
	rightDegrees = 90;						 // Set "rightDegrees" to "90".
	analogWrite(enA, 0);					   // Stop Motor A (left).
	analogWrite(enB, 0);					   // Stop Motor B (right).
	lookRight();							   // Call function "lookRight" with no parameters.
	lookLeft();								// Call function "lookLeft" with no parameters.
	decideDirection();						 // Call function "decideDirection" with no parameters.
}



void lookRight()								// Declare valuless function "lookRight" with no parameters.
{
	for (pos = 90; pos >= 0; pos -= 1)		  // Set position value to "90". While greater than or equal to "0", do the following, then decrease by "1".
	{
		RangeFinderServo.write(pos);			// Set Sensor Servo position to value from above.
		if (noObstacle(far))					// If the value of "noObstacle", passed with parameter of value equal to "far" (see above) returns true, do the following.
		{
			rightDegrees = (-1 * pos) + 90;	 // Set "rightDegrees" to the difference between "90" and the current Sensor Servo position.
			RangeFinderServo.write(90);		 // Move the Sensor Servo position to 90 degrees.
			lookLeft();						 // Call function "lookLeft" with no parameters.
		}
		delay(servoDelay);					  // Pause the program for milliseconds equal to the value of "servoDelay".
	}
	RangeFinderServo.write(90);				 // Move the Sensor Servo position to 90 degrees.
	delay(10);								  // Pause the program for 10 milliseconds.
}



void lookLeft()								// Declare valuless function "lookLeft" with no parameters.
{
	for (pos = 90; pos <= 180; pos += 1)	   // Set position value to "90". While less than or equal to "180", do the following, then increase by "1".
	{
		RangeFinderServo.write(pos);		   // Set Sensor Servo position to value from above.
		if (noObstacle(far))				   // If the value of "noObstacle", passed with parameter of value equal to "far" (see above) returns true, do the following.
		{
			leftDegrees = pos - 90;			// Set "leftDegres" to the difference between the current Sensor Servo position and "90".
			RangeFinderServo.write(90);		// Move the Sensor Servo Position to 90 degrees.
			delay(10);						 // Pause the program for 10 mlliseconds.
			decideDirection();				 // Call function "decideDirection" with no parameters.
		}
		delay(servoDelay);					 // Pause the program for milliseconds equal to the value of "servoDelay".
	}
	RangeFinderServo.write(90);				// Move the Sensor Servo position to 90 degrees.
	delay(10);								 // Pause the program for 10 milliseconds.
}



void decideDirection()						// Declare valuless function "decideDirection" with no parameters.
{
	if (leftDegrees > rightDegrees)		   // If the value of "leftDegrees" is greater than the value of "rightDegrees", do the following.
	{
		initTurn("right", rightDegrees);	  // Call function "initTurn" with parameters of string "right" and variable "rightDegrees".
	}
	else									  // Otherwise, do the following.
	{
		initTurn("left", leftDegrees);		// Call function "initTurn" with parameters of string "left" and variable "leftDegrees".
	}
}



void initTurn(String direction, int amount)	// Declare valuless function "initTurn" with parameters string "direction" and integer "amount".
{
	amount = (ceil((amount / 10) + 1));		// Set amount to the sum of "1" and rounded up, the value of passed "amount" divided by 10.
	if (direction = "left")					// If the value of "drection" is equal to "left", do the following.
	{
		turnRight(amount);					 // Call function "turnRight", passing "amount" as a parameter.
	}
	else									   // Otherwise, do the following.
	{
		turnLeft(amount);					  // Call function "turnLeft", passing "amount" as a parameter.
	}
}



void turnRight(int turnCount)				// Declare valuless function "turnRight" with parameter integer "turnCount".
{											// Move vehicle in reverse.
	digitalWrite(in2, LOW);				  	// Set half of H-Bridge A to open.
	digitalWrite(in1, HIGH);				 	// Set half of H-Bridge A to closed.
	digitalWrite(in4, LOW);				  	// Set half of H-Bridge B to open.
	digitalWrite(in3, HIGH);				 	// Set half of H-Bridge B to closed.
	analogWrite(enA, pwm);				   	// Apply power to motor A (left) at full voltage (pwm is still "255").
	analogWrite(enB, pwm);				   	// Apply power to motor B (right) at full voltage (pwm is still "255").
	delay(100);							  // Pause the program for 100 milliseconds.
	for (i = 0; i < turnCount; i++)		  // Set "i" to "0". While less than "turnCount", do the following, then increase by "1".
	{										// Spin the vehicle clockwise.
		digitalWrite(in2, LOW);			  	// I don't need to comment this out again, I think we get the picture.
		digitalWrite(in1, HIGH);				 //
		digitalWrite(in3, LOW);				  //
		digitalWrite(in4, HIGH);				 //
		analogWrite(enA, pwm);				   //
		analogWrite(enB, pwm);				   //
		delay(10);						  // Pause the program for 10 milliseconds.
		analogWrite(enA, 0);				// Stop the left motor.
		analogWrite(enB, 0);				// Stop the right motor.
		delay(10);						  // Pause the program for 10 milliseconds.
	}
	forward();							  // Call function "forward" with no parameters.
}



void turnLeft(int turn)					// Function "turnLeft" is exactly the same as "turnRight" except for the following.
{										  //
	digitalWrite(in2, LOW);				//
	digitalWrite(in1, HIGH);			   //
	digitalWrite(in4, LOW);				//
	digitalWrite(in3, HIGH);			   //
	analogWrite(enA, pwm);				 //
	analogWrite(enB, pwm);				 //
	delay(100);							//
	for (i = 0; i < turn; i++)			 // Differently named local variable as the parameter.
	{									  // Opposite spin direction.
		digitalWrite(in1, LOW);			//
		digitalWrite(in2, HIGH);		   //
		digitalWrite(in4, LOW);			//
		digitalWrite(in3, HIGH);		   //
		analogWrite(enA, pwm);			 //
		analogWrite(enB, pwm);			 //
		delay(10);						 //
		analogWrite(enA, 0);			   //
		analogWrite(enB, 0);			   //
		delay(10);						 //
	}									  //
	forward();							 //
}										  //



long readRangefinder()								// Declare long function "readRangefinder" with no parameters.
{
	pinMode(trig, OUTPUT);							// Set pin "trig" as an "OUTPUT".
	digitalWrite(trig, LOW);						  // "LOW" to pin "trig".
	delayMicroseconds(2);							 // Pause the program for 2 microseconds.
	digitalWrite(trig, HIGH);						 // "HIGH" to pin "trig".
	delayMicroseconds(10);							// Pause the program for 10 microseconds.
	digitalWrite(trig, LOW);						  // "LOW" to pin "trig".
	pinMode(echo, INPUT);							 // Set pin "echo" as an "INPUT".
	duration = pulseIn(echo, HIGH);				   // Set variable "duration" to the value of function "pulseIn" with parameters "echo" and "HIGH".
	cm = microsecondsToCentimeters(duration);		 // Set variable "cm" to the returned value of function "microsecondsToCentieters" as was passsed variable "duration".
	return cm;										// Function passes the value of variable "cm".
}

long microsecondsToCentimeters(long microseconds)			// Declare long function "microsecondsToCentimeters" with long parameter "microseconds".
{
	return microseconds / 29 / 2;							// Function passes the result of a formula of some sort.
}



void loop()								// Function "loop" is the standard Sketch function that loops forever after "setup" runs.
{
	forward();							 // Call function "forward" with no parameters.
	/*test();*/							// Call function "test" with no parameters.
}