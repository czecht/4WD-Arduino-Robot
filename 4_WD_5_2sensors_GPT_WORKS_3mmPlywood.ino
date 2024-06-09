#include <MotorDriver.h>
#include <NewPing.h>

MotorDriver m;

#define TRIG_PIN_FRONT_RIGHT A5 // Trig pin for the front-right HC-SR04 sensor
#define ECHO_PIN_FRONT_RIGHT A4 // Echo pin for the front-right HC-SR04 sensor
#define TRIG_PIN_BACK A2 // Trig pin for the back HC-SR04 sensor
#define ECHO_PIN_BACK A3 // Echo pin for the back HC-SR04 sensor
#define DISTANCE_THRESHOLD 30 // Threshold for obstacle detection in centimeters
#define TURN_SPEED 180 // Speed for turning
#define FORWARD_SPEED 120 // Speed for forward movement
#define BACKWARD_SPEED 100 // Speed for backward movement
#define TURN_DELAY 500 // Delay in milliseconds for turning to achieve approximately 25 degrees
#define BACKWARD_DELAY 500 // Reduced delay in milliseconds for moving backward

NewPing sonar_front_right(TRIG_PIN_FRONT_RIGHT, ECHO_PIN_FRONT_RIGHT); // Front-right HC-SR04 sensor
NewPing sonar_back(TRIG_PIN_BACK, ECHO_PIN_BACK); // Back HC-SR04 sensor

void setup() {
  randomSeed(analogRead(0)); // Seed the random number generator
}

void loop() {
  unsigned int distance_front_right = getFrontSensorReading();
  unsigned int distance_back = getBackSensorReading();

  if (distance_front_right > 0 && distance_front_right < DISTANCE_THRESHOLD) {
    stopMotors();
    delay(500); // Short pause for stability
    moveBackward();
    delay(BACKWARD_DELAY); // Move backward for a specific duration
    stopMotors();
    delay(500); // Additional pause before turning
    turnLeftOrRight();
    delay(TURN_DELAY);
    stopMotors();
    delay(500); // Additional pause after turning
  } else if (distance_back > 0 && distance_back < DISTANCE_THRESHOLD) {
    stopMotors();
    delay(500); // Short pause for stability
    moveForward();
    delay(BACKWARD_DELAY); // Move forward for a specific duration to avoid the obstacle
    stopMotors();
    delay(500); // Additional pause before turning
    turnLeftOrRight();
    delay(TURN_DELAY);
    stopMotors();
    delay(500); // Additional pause after turning
  } else {
    moveForward();
  }

  delay(200); // Reduced delay between sensor readings for more frequent checks
}

unsigned int getFrontSensorReading() {
  unsigned int distance_front_right = 0;
  unsigned int valid_readings = 0;

  for (int i = 0; i < 5; i++) {
    unsigned int reading = sonar_front_right.ping_cm();
    if (reading > 0) {
      distance_front_right += reading;
      valid_readings++;
    }
    delay(20); // Reduced delay between individual sensor readings for quicker response
  }

  if (valid_readings > 0) {
    distance_front_right /= valid_readings; // Average the valid readings
  }

  return distance_front_right;
}

unsigned int getBackSensorReading() {
  unsigned int distance_back = 0;
  unsigned int valid_readings = 0;

  for (int i = 0; i < 5; i++) {
    unsigned int reading = sonar_back.ping_cm();
    if (reading > 0) {
      distance_back += reading;
      valid_readings++;
    }
    delay(20); // Reduced delay between individual sensor readings for quicker response
  }

  if (valid_readings > 0) {
    distance_back /= valid_readings; // Average the valid readings
  }

  return distance_back;
}

void moveForward() {
  m.motor(1, FORWARD, FORWARD_SPEED); // Front-left motor
  m.motor(2, FORWARD, FORWARD_SPEED); // Front-right motor
  m.motor(3, FORWARD, FORWARD_SPEED); // Rear-right motor
  m.motor(4, FORWARD, FORWARD_SPEED); // Rear-left motor
}

void moveBackward() {
  m.motor(1, BACKWARD, BACKWARD_SPEED); // Front-left motor
  m.motor(2, BACKWARD, BACKWARD_SPEED); // Front-right motor
  m.motor(3, BACKWARD, BACKWARD_SPEED); // Rear-right motor
  m.motor(4, BACKWARD, BACKWARD_SPEED); // Rear-left motor
}

void turnLeftOrRight() {
  if (random(2) == 0) {
    turnLeft();
  } else {
    turnRight();
  }
}

void turnLeft() {
  m.motor(1, BACKWARD, TURN_SPEED); // Front-left motor backward
  m.motor(2, FORWARD, TURN_SPEED); // Front-right motor forward
  m.motor(3, FORWARD, TURN_SPEED); // Rear-right motor forward
  m.motor(4, BACKWARD, TURN_SPEED); // Rear-left motor backward
}

void turnRight() {
  m.motor(1, FORWARD, TURN_SPEED); // Front-left motor forward
  m.motor(2, BACKWARD, TURN_SPEED); // Front-right motor backward
  m.motor(3, BACKWARD, TURN_SPEED); // Rear-right motor backward
  m.motor(4, FORWARD, TURN_SPEED); // Rear-left motor forward
}

void stopMotors() {
  for (int i = 1; i <= 4; i++) {
    m.motor(i, RELEASE, 1);
  }
  delay(500); // Ensure motors are completely stopped
}
