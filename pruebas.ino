#include <Servo.h>

Servo servo;

// Ultrasonic Module pins
const int trigPin = 13; // 10 microsecond high pulse causes chirp, wait 50 us
const int echoPin = 12; // Width of high pulse indicates distance

// Servo motor that aims ultrasonic sensor.
const int servoPin = 11; // PWM output for hobby servo

// Motor control pins: L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control

enum Motor { LEFT, RIGHT };

// Set motor speed: 255 full ahead, -255 full reverse, 0 stop
void go(enum Motor m, int speed)
{
  digitalWrite(m == LEFT ? in1Pin : in3Pin, speed > 0 ? HIGH : LOW);
  digitalWrite(m == LEFT ? in2Pin : in4Pin, speed <= 0 ? HIGH : LOW);
  analogWrite(m == LEFT ? enAPin : enBPin, speed < 0 ? -speed : speed);
}

// Initial motor test:
// left motor forward then back
// right motor forward then back
void testMotors()
{
  static int speed[8] = {128, 255, 128, 0,
                         -128, -255, -128, 0};
  go(RIGHT, 0);
  for (unsigned char i = 0; i < 8; i++)
    go(LEFT, speed[i]), delay(200);
  for (unsigned char i = 0; i < 8; i++)
    go(RIGHT, speed[i]), delay(200);
}

// Read distance from the ultrasonic sensor, return distance in mm
//
// Speed of sound in dry air, 20C is 343 m/s
// pulseIn returns time in microseconds (10^-6)
// 2d = p * 10^-6 s * 343 m/s = p * 0.00343 m = p * 0.343 mm/us
unsigned int readDistance()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long period = pulseIn(echoPin, HIGH);
  return period * 343 / 2000;
}

// Initial configuration
//
// Configure the input and output pins
// Center the servo
// Turn off the motors
// Test the motors
//
void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  servo.attach(servoPin);
  servo.write(90);
  go(LEFT, 0);
  go(RIGHT, 0);
  testMotors();
}

// Main loop:
//
// Move forward 20 cm, then scan left and right while detecting obstacles
// If an obstacle is detected within 20 cm, avoid it
//
void loop()
{
  const unsigned char NUM_ANGLES = 3; // Define the number of angles
  unsigned int distance[NUM_ANGLES];   // Array to store distances

  // Move forward 20 cm
  unsigned long startTime = millis();
  while (millis() - startTime < 2000) { // Assuming speed of ~10 cm/s
    go(LEFT, 255);
    go(RIGHT, 255);
  }

  // Scan left and right while detecting obstacles
  for (unsigned char i = 0; i < NUM_ANGLES; i++)
  {
    servo.write(i == 0 ? 45 : (i == 1 ? 90 : 135)); // Scan left, center, right
    delay(500); // Wait for the servo to reach the angle and stabilize
    distance[i] = readDistance();
  }

  // Check if an obstacle is within 20 cm
  bool obstacleDetected = false;
  for (unsigned char i = 0; i < NUM_ANGLES; i++)
  {
    if (distance[i] < 200) // 20 cm threshold
    {
      obstacleDetected = true;
      break;
    }
  }

  // Avoid obstacle if detected
  if (obstacleDetected)
  {
    go(LEFT, -180); // Turn left
    delay(500); // Assuming turning takes 0.5 seconds
  }
}



