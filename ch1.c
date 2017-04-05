#pragma config(Sensor, S1,     touchSensorR,   sensorEV3_Touch)
#pragma config(Sensor, S2,     touchSensorL,   sensorEV3_Touch)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)

//set the robot to go straight.
void moveforward()
{
	setMotorSpeed(leftMotor, 50);		//Set the leftMotor to half power (50)
	setMotorSpeed(rightMotor, 50);  //Set the rightMotor to half power (50)
}
//reverse the robot.
void reverse(bool direction)
{
	moveforward();
	setMotorReversed(rightMotor,direction);
	setMotorReversed(leftMotor,direction);
}
//set the robot turing right.
void turnRight()
{
	setMotorSpeed(leftMotor,60);
	setMotorSpeed(rightMotor,30);
}
//set the robot turing left.
void turnLeft()
{
	setMotorSpeed(leftMotor,30);
	setMotorSpeed(rightMotor,60);
}
//stop moving for 2sec(2000ms)
void stopMoving()
{
	setMotorSpeed(leftMotor,0);
	setMotorSpeed(rightMotor,0);
	delay(2000);
}

//main
task main()
{
	//startTask(Moveforward);
	moveforward();

	while(true){
		bool right = false;
		bool left  = false;
		if (getTouchValue(S1) == 1){	//if the right side touches the wall first.
			right = true;
			delay(250); //wait for 1sec
			if (getTouchValue(S2) == 1){	//left side touches the wall as well.
				left = true;
			}
		}
		if (getTouchValue(S2) == 1 && right != true){		//if the left side touches the wall first
			left = true;
			delay(250); //wait for 1sec
			if (getTouchValue(S1) == 1){	//right side touches the wall as well.
				right = true;
			}
		}
		if (left == true || right == true){
0
			if (left == true && right == true){		//bump into wall

				playTone(784, 15); //beep
				//reverse and turn to somewhere
				reverse(true);
				turnRight();
				delay(1000);
				stopMoving();
				} else if (right == true) {		//only the right side hit the wall
				playTone(300, 15); //beep
				//reverse right, points to other direction
				reverse(true);
				turnRight();
				delay(1000);
				}else if (left == true) { 		//only the left side hit the wall
				playTone(1200, 15); //beep
				//reverse left
				reverse(true);
				turnLeft();
				delay(1000);
			}
			//start wandering
			reverse(false); //move forward.
			moveforward();
		}
	}
}
