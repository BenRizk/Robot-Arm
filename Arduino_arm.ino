#include <Servo.h>
//servos
Servo baseservo;  // create servo object to control a servo
Servo reachservo;
Servo liftservo;
Servo clawservo;
//color sensor
#define S0 4
#define S1 7
#define S2 11
#define S3 12
#define sensorOut 8

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
   int distance; // int for knowing distance of objects
   int redValue;  // int for knowing the value for red 
   int blueValue; // int for knowing the value for blue 
   int greenValue; // int for knowing the value for green
   for (pos = 180; pos >= 140; pos--) { // goes from 180 degrees to 140 degrees
    // in steps of 1 degree
    baseservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    distance = distanceSensor(trigPin, echoPin); //find object distance from senor
      clawservo.write(180); // make sure the claw is closed
       delay(15);
    if(distance > 7 && distance < 20)
    {
      //reach out to grab object
      reachOut(trigPin,echoPin);
      delay(40);
      //make sure the arm is in original position
      reachservo.write(50);
      delay(30);
      liftservo.write(80);
      delay(30);
      // begin process of organizing by color
      //find the color values
      redValue = findRed();
      blueValue = findBlue();
      greenValue = findGreen();
      //move object to appropriate area
      moveBlock(redValue, blueValue, greenValue, pos);
    }
  }
  for (pos = 139; pos <= 180; pos++) { // goes from 180 degrees to 0 degrees
    baseservo.write(pos);              // tell servo to go to position in variable 'pos'//
    delay(15);                       // waits 15ms for the servo to reach the position
    distance = distanceSensor(trigPin, echoPin); //find object distance from senor
     clawservo.write(180); // make sure the claw is closed
     delay(15);
   if(distance > 7 && distance < 20)
    {
      //reach out to grab object
      reachOut(trigPin,echoPin);
      delay(40);
       //make sure the arm is in original position
      reachservo.write(50);
      delay(30);
      liftservo.write(80);
      delay(30);
      // begin process of organizing by color
      //find the color values
      redValue = findRed();
      blueValue = findBlue();
      greenValue = findGreen();
      //move object to appropriate area
      moveBlock(redValue, blueValue, greenValue, pos);
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
  //int a; // this integer is for knowing how close the head is to an object
  //int b; // this holds the angle the arm was last extended to
  clawservo.write(35); //open claw
  delay(300);

   //extend the arm
    for(int i = 0; i < 110; i++)
  {
    /* code for knowing how close sensor is to object
    a = distanceSensor(trigPin, echoPin);
    Serial.print("a:");
    Serial.print(a);
    */
      reachservo.write(i);
      if(i > 60)  //only use liftservo if reach servo doesn't reach object alone
      {
        liftservo.write(i+20);
        delay(30);
      }
      delay(30);
    /* code for knowing how close sensor is to object
    if(a <= 1)  //end for loop when the head is close enough
    {
      b = i; // hold the last angle the arm was at
      break; //for the loop to end
    }
    */
  }

   clawservo.write(180); //close claw
    delay(300);
  
  //retract arm
    for(int i = 110; i > 35; i--)
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
void moveBlue(int basePos)
{
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(100);
  liftservo.write(60); // raise the lift servo
  delay(100);
  for(int i = basePos; i > 70; i--) // make the base servo move slower
  { 
  baseservo.write(i); // move to blue zone
  delay(40);
  }
  reachservo.write(90); // lower to far distance
  delay(100);
  liftservo.write(100); //lower the lift servo
  delay(100);
  clawservo.write(35); //open claw
  delay(100);
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(100);
  liftservo.write(60); // raise the lift servo
  delay(100);
}
void moveRed(int basePos)
{
  reachservo.write(130); // extend reach servo
  delay(100);
  liftservo.write(60); // raise lift servo
  delay(100);
 for(int i = basePos; i > 40; i--) // make the base servo move slower
  { 
  baseservo.write(i); // move to red zone
  delay(40);
  }
  reachservo.write(90); // lower to far distance
  delay(100);
  liftservo.write(100); //lower the lift servo
  delay(100);
  clawservo.write(35); //open claw
  delay(100);
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(100);
  liftservo.write(60); // raise the lift servo
  delay(100);
}
void moveGreen(int basePos)
{
  reachservo.write(130); // raise reach servo
  delay(100);
  liftservo.write(60); // raise lift servo
  delay(100);
  for(int i = basePos; i > 115; i--) // make the base servo move slow 
  { 
  baseservo.write(i); // move to green zone
  delay(40);
  }
  reachservo.write(90); // lower to far distance
  delay(100);
  liftservo.write(100); //lower the lift servo
  delay(100);
  clawservo.write(35); //open claw
  delay(100);
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(100);
  liftservo.write(60); // raise the lift servo
  delay(100);
}

void moveBlack{
  reachservo.write(130); // raise reach servo
  delay(100);
  liftservo.write(60); // raise lift servo
  delay(100);
  for(int i = basePos; i < 170; i++) // make the base servo move slow 
  { 
  baseservo.write(i); // move to green zone
  delay(40);
  }
  reachservo.write(90); // lower to far distance
  delay(100);
  liftservo.write(100); //lower the lift servo
  delay(100);
  clawservo.write(35); //open claw
  delay(100);
  reachservo.write(130); //raise the arm high to avoid hitting objects
  delay(100);
  liftservo.write(60); // raise the lift servo
  delay(100);
}

int findRed()
{
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  int frequency = pulseIn(sensorOut, LOW);
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
  int frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing BLUE color frequency
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
  int frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing GREEN color frequency
  Serial.print("  ");
  delay(100);
  return frequency;
}

void moveBlock(int redValue,int blueValue,int greenValue,int basePos)
{
  //object is red when red value is least
      if(redValue < blueValue && redValue < greenValue)
      {
        moveRed(basePos);
      }
      //object is blue if blue value is least
      if(blueValue < redValue && blueValue < greenValue)
      {
        moveBlue(basePos);
      }
      // object is green if green value is least
      if(greenValue < blueValue && greenValue < redValue)
      {
        moveGreen(basePos);
      }
      //object is black if all values are greater than 100
      
      if(redValue > 100 && blueValue > 100 && greenValue > 100)
      {
        //not made yet
      }
    
}

