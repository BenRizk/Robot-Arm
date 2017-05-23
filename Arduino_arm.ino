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
  // needed for color sensor
  // we need to assign s0, s1, s2, s3, and sensorout to digital port
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  Serial.begin(9600);
}

void loop() {
//beginning position
liftservo.write(85);  // 85 o.g
reachservo.write(40);  // 40 o.g

sweep(pos, trigPin, echoPin);

}

void sweep(int pos,int trigPin, int echoPin)
{
   int distance;
   int redValue;
   int blueValue;
   int greenValue;
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
      delay(40);
      reachservo.write(50);
      delay(30);
      liftservo.write(80);
      delay(30);
      // begin process of organizing by color
      redValue = findRed();
      blueValue = findBlue();
      greenValue = findGreen();
      //object is red when red value is least
      if(redValue < blueValue && redValue < greenValue)
      {
        moveRed();
      }
      //object is blue if blue value is least
      if(blueValue < redValue && blueValue < greenValue)
      {
        moveblue();
      }
      // object is green if green value is least
      if(greenValue < blueValue && greenValue < redValue)
      {
        movegreen();
      }
      //object is black if all values are greater than 100
      if(redValue > 100 && blueValue > 100 && greenValue > 100)
      {
        //not made yet
      }
    
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
      delay(40);
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
    if(a <= 3)  //end for loop when the head is close enough
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
   
}
void moveBlue()
{
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(40);
  liftservo.write(60); // raise the lift servo
  delay(30);
  baseservo.write(70); // move to blue zone
  delay(30);
  reachservo.write(90); // lower to far distance
  delay(30);
  liftservo.write(100); //lower the lift servo
  delay(30);
  clawservo.write(0); //open claw
  delay(30);
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(40);
  liftservo.write(60); // raise the lift servo
  delay(30);
}
void moveRed()
{
  reachservo.write(130); // extend reach servo
  delay(40);
  liftservo.write(60); // raise lift servo
  delay(30);
  baseservo.write(40); // move to red zone
  delay(30);
  reachservo.write(90); // lower to far distance
  delay(30);
  liftservo.write(100); //lower the lift servo
  delay(30);
  clawservo.write(0); //open claw
  delay(30);
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(40);
  liftservo.write(60); // raise the lift servo
  delay(30);
}
void moveGreen()
{
  reachservo.write(130); // raise reach servo
  delay(40);
  liftservo.write(60); // raise lift servo
  delay(30);
  baseservo.write(115); // move to green zone
  delay(30);
  reachservo.write(90); // lower to far distance
  delay(30);
  liftservo.write(100); //lower the lift servo
  delay(30);
  clawservo.write(0); //open claw
  delay(30);
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(40);
  liftservo.write(60); // raise the lift servo
  delay(30);
}

int findRed()
{
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  return frequency;
}
int findBlue()
{
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(100);
  return frequency;
}

int findGreen()
{
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  return frequency;
}

