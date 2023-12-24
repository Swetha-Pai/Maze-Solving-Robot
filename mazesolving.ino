#include <AFMotor.h>

#define MAX_SPEED 145
#define MIN_SPEED 0

#define TURN_SPEED 100

#define THRESHOLD 500

#define bg_right A5
#define ir_right A4
#define ir_left A2
#define bg_left A0

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(4, MOTOR34_1KHZ); 

boolean leftFlag = false;
boolean rightFlag = false; 
boolean bgLeftFlag = false;
boolean bgRightFlag = false; 

int leftSpeed = 0;
int rightSpeed = 0;

boolean isInBlackBackground = false;

boolean isRobotStarted = false;
boolean isStop = false;

void speedSet(int left, int right)
{
  motor1.setSpeed(left);
  motor2.setSpeed(right);
}

void halt()
{
  Serial.println("STOP");
  motor1.run(RELEASE);     
  motor2.run(RELEASE);  
}

void goSlow()
{
  Serial.println("go slow ");
  speedSet(TURN_SPEED,TURN_SPEED);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);    
}

void uturn()
{  
  Serial.println("turn right steep");
  speedSet(MAX_SPEED,MAX_SPEED);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  
  delay(500);
}

boolean allWhiteBG()
{
  return ((bgLeftFlag) && (leftFlag) && (rightFlag) && (bgRightFlag));
}

void goStright()
{
  Serial.println("go straight ");
  speedSet(MAX_SPEED,MAX_SPEED);
  motor1.run(FORWARD);     
  motor2.run(FORWARD); 
  delay(250);
}

boolean isStrightOnWhiteBG()
{
  return ((!bgLeftFlag) && (!bgRightFlag) && ( (leftFlag) && (rightFlag)));
}

void turnRightSlight()
{
  Serial.println("turn right slight");
  speedSet(TURN_SPEED,MIN_SPEED);
  motor1.run(FORWARD);  
}

boolean isSlightRightTurnOnWhiteBG()
{
  return ((!bgLeftFlag) && leftFlag && (!rightFlag) && (!bgRightFlag));
}

void turnLeftSlight()
{
  Serial.println("turn left slight");
  speedSet(MIN_SPEED,TURN_SPEED);
  motor2.run(FORWARD);    
}

boolean isSlightLeftTurnOnWhiteBG()
{
  return ((!bgLeftFlag) && (!leftFlag) && rightFlag && (!bgRightFlag));
}

void turnRight()
{
  Serial.println("turn right ");
  speedSet(TURN_SPEED,MIN_SPEED);
  motor1.run(FORWARD); 
  delay(250);
}

boolean isRightTurnOnWhiteBG()
{
  return ((!bgLeftFlag) && (rightFlag && leftFlag && bgRightFlag ));
}

void turnLeft()
{
  Serial.println("turn left ");
  speedSet(MIN_SPEED,TURN_SPEED);
  motor2.run(FORWARD);
  delay(250);
}

boolean isLeftTurnOnWhiteBG()
{
  return ((!bgRightFlag) && (rightFlag && leftFlag && bgLeftFlag));
}

void turnRightSteep()
{
  Serial.println("turn right steep");
  speedSet(MAX_SPEED,MIN_SPEED);
  motor1.run(FORWARD); 
  delay(500);
}

boolean isSteepRightTurnOnWhiteBG()
{
  boolean steepTurn = false;
  if((!bgLeftFlag) && (!leftFlag) && (rightFlag && bgRightFlag))
  {
    steepTurn = true;
  }
  else if((!bgLeftFlag) && (!leftFlag) && (!rightFlag) && bgRightFlag)
  {
    steepTurn = true;
  }
  return steepTurn;
}

void turnLeftSteep()
{
  Serial.println("turn left steep");
  speedSet(MIN_SPEED,MAX_SPEED);
  motor2.run(FORWARD);   
  delay(500);
}

boolean isSteepLeftTurnOnWhiteBG()
{
  boolean steepTurn = false;
  
  if((!bgRightFlag) && (!rightFlag) && (leftFlag && bgLeftFlag))
  {
    steepTurn = true;
  }
  else if((!bgRightFlag) && (!rightFlag) && (!leftFlag) && bgLeftFlag)
  {
    steepTurn = true;
  }
  return steepTurn;
}

void followLineOnWhiteBG()
{
  if(isSteepLeftTurnOnWhiteBG())
  {
    turnLeftSteep();
  }
  else if (isStrightOnWhiteBG())
  {
    goStright();
  }
  else if(isSteepRightTurnOnWhiteBG())
  {
    turnRightSteep();
  }
  else if (isLeftTurnOnWhiteBG())
  {
    turnLeft();
  }
  else if (isRightTurnOnWhiteBG())
  {
    turnRight();
  }
  else if (isSlightLeftTurnOnWhiteBG())
  {
    turnLeftSlight();
  }
  else if (isSlightRightTurnOnWhiteBG())
  {
    turnRightSlight();
  }
  else if (allWhiteBG())
  {
    uturn();
  }
}

boolean isStopDetected()
{
  if(!isInBlackBackground)
  {
    return false;
  }
  else
  {
    return false;
  } 
}

boolean isStartDetected()
{
    if(!bgLeftFlag && !bgRightFlag && !leftFlag && !rightFlag)
    {
      return true;
    }
    else
    {
      return false;
    } 
}

boolean isBlackBackground()
{
  if(!isInBlackBackground)
  {
      if(bgLeftFlag && bgRightFlag && ( (!leftFlag) && (!rightFlag)))
      {
        isInBlackBackground = true;
      }
  }
  
  return isInBlackBackground;
}

boolean isMiddleRightDetectorOnBlack()
{
  int sensorValue = analogRead(ir_right);
  
  if (sensorValue > THRESHOLD){
    return true;    //return true for black
  } else{
    return false;
  }
}

boolean isMiddleLeftDetectorOnBlack()
{
  int sensorValue = analogRead(ir_left);
  if (sensorValue > THRESHOLD){
    return true;    //return true for black
  } else{
    return false;
  }
}

boolean isBackgroundRightDetectorOnBlack()
{
  int sensorValue = analogRead(bg_right);

  if (sensorValue > THRESHOLD){
    return true;    //return true for black
  } else{
    return false;
  }
}

boolean isBackgroundLeftDetectorOnBlack()
{
  int sensorValue = analogRead(bg_left);
  
  if (sensorValue > THRESHOLD){
    return true;    //return true for black
  } else{
    return false;
  }
}

void checkStatus()
{
    bgLeftFlag = isBackgroundLeftDetectorOnBlack();
    bgRightFlag = isBackgroundRightDetectorOnBlack();
    leftFlag = isMiddleLeftDetectorOnBlack();
    rightFlag = isMiddleRightDetectorOnBlack();
    
    isInBlackBackground = isBlackBackground();
      
    if(!isRobotStarted)
    {
      isRobotStarted = isStartDetected();
    }
    
    if(!isStop)
    {
      isStop = isStopDetected();
    }   
}

void loop() 
{
  checkStatus();
  
  if(!isStop)
  {
    followLineOnWhiteBG();
  }
  else
  {
    halt();
  }

  delay(50);
}

void setup() 
{
  Serial.begin(9600);
  
  pinMode(ir_left, INPUT);
  pinMode(ir_right, INPUT);
  pinMode(bg_left, INPUT);
  pinMode(bg_right, INPUT);
  
  leftSpeed = TURN_SPEED;
  rightSpeed = TURN_SPEED; 
}
