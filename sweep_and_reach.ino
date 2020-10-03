/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.
 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
Servo baseservo;     // Create servo object to control a servo
Servo reachservo;
Servo liftservo;
Servo clawservo;

int pos = 0;          // Variable to store the servo position

int trigPin = 10;     // Variables for ultrasonic sensor pin num
int echoPin = 13;
  
void setup() {
  baseservo.attach(9);    // Attaches the servo on pin 9 to the servo object
  reachservo.attach(3);   // Attach the servo for reaching to pin 3
  liftservo.attach(5);    // Attach the servo for lifting to pin 5
  clawservo.attach(6);    // Attach the servo for the claw to pin 6

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
  Serial.begin(9600);
}

void loop() {
// Beginning position
liftservo.write(80);
reachservo.write(50);

sweep(pos, trigPin, echoPin);
}

void sweep(int pos,int trigPin, int echoPin)
{
   int distance;
   for (pos = 0; pos <= 80; pos++) {   // Goes from 0 degrees to 180 degrees in steps of 1 degree
    baseservo.write(pos);              // Tell servo to go to position in variable 'pos'
    delay(15);                         // Waits 15ms for the servo to reach the position
    distance = distanceSensor(trigPin, echoPin);
      clawservo.write(180);
      delay(15);
    if(distance > 15 && distance < 20)
    {
      reachOut();
      reachservo.write(50);
      liftservo.write(80);
    }
  }
  for (pos = 79; pos >= 0; pos--) {    // Goes from 180 degrees to 0 degrees
    baseservo.write(pos);              // Tell servo to go to position in variable 'pos'
    delay(15);                         // Waits 15ms for the servo to reach the position
   distance = distanceSensor(trigPin, echoPin);
     clawservo.write(180);
     delay(15);
   if(distance > 15 && distance < 20)
    {
      reachOut();
      reachservo.write(50);
      liftservo.write(80);
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

void reachOut()
  {
  clawservo.write(35);  // Open claw
  delay(300);
  // Extend the arm
  for(int i = 0; i < 90; i++)
  {
    reachservo.write(i);
    delay(30);
  }
  clawservo.write(180); // Close claw
  delay(300);
  // Retract arm
  for(int i = 89; i > 35; i--)
  {
    reachservo.write(i);
    delay(30);
  }
 
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
