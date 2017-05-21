/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.
 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo baseservo;  // create servo object to control a servo
Servo reachservo;
Servo liftservo;
Servo clawservo;

int pos = 0;    // variable to store the servo position

int trigPin = 10;     // variables for ultrasonic sensor pin num
int echoPin = 13;
  
void setup() {
  baseservo.attach(9);  // attaches the servo on pin 9 to the servo object
  reachservo.attach(3); // attach the servo for reaching to pin 3
  liftservo.attach(5); // attach the servo for lifting to pin 5
  clawservo.attach(6); // attach the servo for the claw to pin 6

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  
}

void loop() {
//beginning position
liftservo.write(80);
reachservo.write(50);

sweep(pos, trigPin, echoPin);

}

void sweep(int pos,int trigPin, int echoPin)
{
   int distance;
   for (pos = 180; pos >= 140; pos--) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    baseservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    distance = distanceSensor(trigPin, echoPin);
      clawservo.write(180);
       delay(15);
    if(distance > 7 && distance < 20)
    {
      reachOut(trigPin,echoPin);
      reachservo.write(50);
      delay(30);
      liftservo.write(80);
      delay(30);
    }
  }
  for (pos = 139; pos <= 180; pos++) { // goes from 180 degrees to 0 degrees
    baseservo.write(pos);              // tell servo to go to position in variable 'pos'//
    delay(15);                       // waits 15ms for the servo to reach the position
   distance = distanceSensor(trigPin, echoPin);
     clawservo.write(180);
     delay(15);
   if(distance > 7 && distance < 20)
    {
      reachOut(trigPin,echoPin);
      reachservo.write(50);
      delay(30);
      liftservo.write(80);
      delay(30);
    }
  }
}

int distanceSensor(int trigPin, int echoPin)
{
  long duration;
  int distance;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}

void reachOut(int trigPin,int echoPin)
  {
  int a; // this integer is for knowing how close the head is to an object
  int b; // this holds the angle the arm was last extended to
  clawservo.write(35); //open claw
  delay(300);

   //extend the arm
  
    for(int i = 0; i < 110; i++)
  {
    a = distanceSensor(trigPin, echoPin);
    Serial.print("a:");
    Serial.print(a);
      reachservo.write(i);
      if(i > 60)  //only use liftservo if reach servo doesn't reach object alone
      {
        liftservo.write(i+20);
        delay(30);
      }
      delay(30);
    if(a <= 3)
    {
      b = i; // hold the last angle the arm was at
      i = 111; //for the loop to end
    }
  }

   clawservo.write(180); //close claw
    delay(300);
  
  //retract arm
    for(int i = b; i > 35; i--)
   {
    reachservo.write(i);
    delay(30);
    if(i > 60)
    {
      liftservo.write(i+20);
    }
    delay(30);
  }
   
//trying to make the arm stop before it reaches its object
/*
int a = 0;
int distance; 
   
  do{
    distance = distanceSensor(trigPin, echoPin);
    Serial.print("a:");
    Serial.print(a);
    a++;
    reachservo.write(a);
    delay(300);
  }while(distance < 4);
  clawservo.write(180); //close claw
    delay(300);
     for(int i = a; i > 35; i--)
   {
    reachservo.write(i);
    delay(30);
  }
*/

}
void moveBlue()
{
  liftservo.write(30);
  delay(30);
  baseservo.write(130);
  delay(30);
}
void moveRed()
{
  liftservo.write(30);
  delay(30);
  baseservo.write(175);
  delay(30);
}
void moveGreen()
{
  liftservo.write(30);
  delay(30);
  baseservo.write(115);
  delay(30);
}
