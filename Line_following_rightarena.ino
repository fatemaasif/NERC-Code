//Servo setup
/*#include <Servo.h>
Servo hitter, collectorRight, collectorLeft, undocking;
int pos = 0; 
int posLeftCservo=180;*/

//define sensor connections & vars
#define IR_RM 52
#define IR_R 50 //S4
#define sensS3 48
#define IR_L 46 //S2 
#define IR_LM 44


int sensFrontRight, sensFrontLeft, sensRM, sensLM; //at black, sensor 0
int S3=1; //at black, sensor 0


//define motor connections
int pwmPin[]= {4,7};
int dir1[]= {22,30};
int dir2[]= {24,32};

//defining speeds
int maxSpeed = 75;
int turnSpeed = 85;

int treeNumber = 1;
/*stopMotor();
delay(2000);*/
void setup() {
  Serial.begin(9600);
  
  // Set pin states of motor 
  for (int i = 0; i<2;i++)
  {
    pinMode(pwmPin[i],OUTPUT);
    pinMode(dir1[i],OUTPUT);
    pinMode(dir2[i],OUTPUT);
  }
  
  //set pin states of sensors
  pinMode(IR_R,INPUT);
  pinMode(IR_L,INPUT);
  pinMode(sensS3,INPUT);
  pinMode(IR_RM,INPUT);
  pinMode(IR_LM,INPUT);
  
  /*//servo configuration
  hitter.attach(32);  
  collectorRight.attach(30);
  collectorRight.write(0);
  collectorLeft.attach(36);
  collectorLeft.write(180);  
  undocking.attach(34);
  undocking.write(110);*/
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensor();
  lineFollow();
}

void readSensor()
{
  sensFrontRight = digitalRead(IR_R);
  sensFrontLeft = digitalRead(IR_L);
  S3=digitalRead(sensS3);
  sensRM = digitalRead(IR_RM);
  sensLM = digitalRead(IR_LM);
}

void lineFollow()
{
  if (sensFrontRight == 0 && sensFrontLeft == 0)
  {treeRoutine();}
  else if (sensRM == 1 && sensFrontRight == 1 && sensFrontLeft == 1 && sensLM == 1)
  {forward(maxSpeed+30);}
  else if (sensFrontRight == 1 && sensFrontLeft == 0)
  {turnLeft();}
  else if (sensFrontRight == 0 && sensFrontLeft == 1)
  {turnRight();}
}

void treeRoutine()
{

  if (treeNumber==1)
  {
    treeNumber++;
    stopMotor();//stops on black line
    //delay(50);
    
    turnLeft();
    delay(1000);
    
    stopMotor(); //stops in front of the tree 
    delay(50); //for some seconds
    //servoAction();
    reverse(maxSpeed+30);
    delay(1300);
    turnRightonTree();
    delay(400);
  }

  else if(treeNumber==2)
  {
    treeNumber++;
    stopMotor();
    delay(50);
    
    turnLeft();
    delay(400);
    readSensor();
    while(sensFrontLeft==1)
    {
      turnLeft();
      readSensor();
    }
    
    stopMotor();
    delay(5); 
    forward(maxSpeed);
    delay(200);

    stopMotor(); //stops in front of the tree 
    delay(50); //for some seconds
    //servoAction();
    
    reverse(maxSpeed);
    delay(300);
    turnRightweird(); //to make a 90 turn towards the line
    delay(500);
    readSensor();
    while(S3==1)
    {
      turnRight();
      readSensor();
    }
  }
  else if (treeNumber==3)
  {
    treeNumber=1;
    forward(maxSpeed);
    delay(300);
    stopMotor();
    delay(50);
    
    turnLeftonTree();
    delay(200);
    readSensor();
    while(sensFrontLeft==1)
    {
      turnLeftonTree();
      readSensor();
    }
    stopMotor();
    delay(5); 
    
    forward(maxSpeed);
    delay(170);
    stopMotor();
    delay(50); //stops in front of tree
    //servoAction();
    
    turnRightonTree();//turn towards docking area
    delay(200);
    
    readSensor();
    while(S3==1)
    {
      turnRightweird();
      readSensor();
    }

    forward(maxSpeed);
    delay(500);
    stopMotor();
    delay(50); //stops in front of docking area
    //undockingAction();
    
    reverse(maxSpeed);
    delay(500);
    turnRightonTree();
    delay(200);
    readSensor();
    while(S3==1)
    {
      turnRightweird();
      readSensor();
    }
    turnRightonTree();
    delay(200);

    forward(maxSpeed+50);
    delay(800);
    stopMotor();
    while(1){} //stop code at this point
  }
  
}

void setMotor(int dir, int pwmVal, int pwmPin, int dir1, int dir2)
{
  analogWrite(pwmPin,pwmVal);
  if (dir==1) //clockwise rotation
  {
    //digitalWrite(dir1,LOW);
    //digitalWrite(dir2,LOW);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,LOW);
  }
  else if (dir==-1) //a.clockwise rotation
  {
    //digitalWrite(dir1,LOW);
    //digitalWrite(dir2,LOW);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,HIGH);
  }
  else
  {
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,LOW);
  }
}

void forward(int pwm)
{
  setMotor(1, pwm, pwmPin[0], dir1[0], dir2[0]);
  setMotor(1, pwm, pwmPin[1], dir1[1], dir2[1]);
}

void reverse(int pwm)
{
  setMotor(-1, pwm, pwmPin[0], dir1[0], dir2[0]);
  setMotor(-1, pwm, pwmPin[1], dir1[1], dir2[1]);
}

void turnRight()
{
  setMotor(0, turnSpeed, pwmPin[0], dir1[0], dir2[0]);
  setMotor(1, turnSpeed, pwmPin[1], dir1[1], dir2[1]);
}

void turnRightonTree()
{
  setMotor(-1, turnSpeed, pwmPin[0], dir1[0], dir2[0]);
  setMotor(1, turnSpeed, pwmPin[1], dir1[1], dir2[1]);
}

void turnRightweird()
{
  setMotor(-1, turnSpeed, pwmPin[0], dir1[0], dir2[0]);
  setMotor(0, turnSpeed, pwmPin[1], dir1[1], dir2[1]);
}

void turnLeft()
{
  setMotor(1, turnSpeed, pwmPin[0], dir1[0], dir2[0]);
  setMotor(0, turnSpeed, pwmPin[1], dir1[1], dir2[1]);
}

void turnLeftonTree()
{
  setMotor(1, turnSpeed, pwmPin[0], dir1[0], dir2[0]);
  setMotor(-1, turnSpeed, pwmPin[1], dir1[1], dir2[1]);
}

void stopMotor()
{
  setMotor(0, 0, pwmPin[0], dir1[0], dir2[0]);
  setMotor(0, 0, pwmPin[1], dir1[1], dir2[1]);
} 

/*void servoAction()
{
  for (pos = 0; pos <= 180; pos += 1)
 { 
   collectorRight.write(pos);
   collectorLeft.write(posLeftCservo);
   posLeftCservo--;              
   delay(5);                       
 }
 delay(200);
  
 hitAction();
 delay(500);
 hitAction();
 
 for (pos = 180; pos >= 0; pos -= 1) 
 { 
   collectorRight.write(pos);
   collectorLeft.write(posLeftCservo);   
   posLeftCservo++;           
   delay(5);
 }
 delay(500);
}


void hitAction()
{
  for (pos = 80; pos <= 170; pos += 1) 
  { 
    hitter.write(pos);              
    delay(5);                       
  }
  delay(2000);
  for (pos = 170; pos >= 80; pos -= 1) 
  { 
    hitter.write(pos);              
    delay(5);
  }
}

void undockingAction()
{
  for (pos = 0; pos <= 180; pos += 1) //open collectors
  { 
   collectorRight.write(pos);
   collectorLeft.write(posLeftCservo);
   posLeftCservo--;              
   delay(5);                       
  }
  for (pos = 110; pos >= 0; pos -= 1) //open collector gate
  { 
    undocking.write(pos);            
    delay(5);                       
  }
  delay(100);
  
  //code to shake collectors at that position
  for (pos = 180; pos >= 140; pos -= 1) //close collectors
  {   
   collectorRight.write(pos);
   collectorLeft.write(posLeftCservo-40);   
   posLeftCservo++;           
   delay(5);
  }
  for (pos = 140; pos <= 180; pos += 1) //open collectors
  { 
   collectorRight.write(pos);
   collectorLeft.write(posLeftCservo);
   posLeftCservo--;              
   delay(5);                       
  }
   for (pos = 180; pos >= 140; pos -= 1) //close collectors
  {   
   collectorRight.write(pos);
   collectorLeft.write(posLeftCservo-40);   
   posLeftCservo++;           
   delay(5);
  }
  for (pos = 140; pos <= 180; pos += 1) //open collectors
  { 
   collectorRight.write(pos); //140 - 180
   collectorLeft.write(posLeftCservo); //  180 - 140
   posLeftCservo--;              
   delay(5);                       
  }
  delay(1000);

  posLeftCservo=180;
  for (pos = 180; pos >= 0; pos -= 1) //close collectors to reset
  {   
   collectorRight.write(pos);
   collectorLeft.write(posLeftCservo);   
   posLeftCservo++;           
   delay(5);
  }
  delay(500);
}*/
