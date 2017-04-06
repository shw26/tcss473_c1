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
	//int ratio = random(9);
	//int newRatio = (50%ratio) * ratio;
	setMotorSpeed(leftMotor,30);
	setMotorSpeed(rightMotor,60);
}
//stop moving for 2sec(2000ms)
void stopMoving()
{
	setMotorSpeed(leftMotor,0);
	setMotorSpeed(rightMotor,0);
	delay(1000);
}
// 0 move forward, 1 turn right, 2 turn left
task randomWalk()
{
	int prevDirection = 3;

	while(true){

		int direction = random(2);
		if (direction == 0){
			moveforward();
		} else if (direction == 1 && prevDirection != 1){
			turnRight();
			prevDirection = 1;
		} else if (direction == 2 && prevDirection != 2){
			turnLeft();
			prevDirection = 2;
		}
		int randomDelayTime = random(5);
		//delay(randomDelayTime* 400); //min: 400ms, max: 2000ms.
		delay(500);
	}
}

//main
task main()
{
	//startTask(Moveforward);
	moveforward();
	delay(100);
	startTask(randomWalk);


	while(true){
		bool right = false;
		bool left  = false;
		int direction = 3;
		if (getTouchValue(S1) == 1){	//if the right side touches the wall first.
			right = true;
			delay(100); //wait for 1sec
			if (getTouchValue(S2) == 1){	//left side touches the wall as well.
				left = true;
			}
		}
		if (getTouchValue(S2) == 1 && right != true){		//if the left side touches the wall first
			left = true;
			delay(100); //wait for 1sec
			if (getTouchValue(S1) == 1){	//right side touches the wall as well.
				right = true;
			}
		}
		if (left == true || right == true){
			stopTask(randomWalk);
			if (left == true && right == true){		//bump into wall
				playTone(784, 15); //beep
				direction = 0;
				//reverse and turn to somewhere
				reverse(true);
				turnRight(); //this will be changed to other position.
				delay(1000);
				stopMoving();


			} else if (right == true) {		//only the right side hit the wall
				//playTone(300, 15); //beep
				direction = 1;
				//reverse right, points to other direction
				reverse(true);
				turnRight();
				delay(1000);

			}else if (left == true) { 		//only the left side hit the wall
				//playTone(1200, 15); //beep
				direction = 2;
				//reverse left
				reverse(true);
				turnLeft();
				delay(1000);
			}
			//start wandering
			reverse(false); //move forward.
			delay(1000);
			//prevent form colliding on the same side more than once.
			bool success = false;
			while(!success){
				int randomDir = random(3);
				if (direction != 0 && randomDir == 0){
					moveforward();
					success = true;
				} else if (direction != 1 && randomDir == 1){
					turnRight();
					success = true;
				} else if (direction != 2 && randomDir == 2){
					turnLeft();
					success = true;
				}
			}
			delay(100);

			moveforward();
			startTask(randomWalk);
		}
	}
}
