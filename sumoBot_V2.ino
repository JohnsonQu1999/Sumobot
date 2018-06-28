///////////////////////////////////////////
//Program Name: Sumobot McMaster Edition //
//Programmer: Johnson Qu                 //
//Date: March 15 2018                    //
///////////////////////////////////////////

//Revision v3.0

//=== Notes BELOW ===//
//There are many delay()'s that are called throughout the program. The delay determines the distance or angle that the sumobot turns through.
//2018/3/15 --> ALPHA (v1.0): First iteration of code started and complete. Just making sure the basics are working.
//2018/3/18 --> UPDATE: Added activeSearch().
//2018/3/18 --> BETA (v2.0): Works better than expected.
//2018/3/19 --> UPDATE: Added checkLeftRight() to reduce the time the robot spends finding the enemy. Now the bot can check left and right instead of just turning in 1 direction.
//2018/3/23 --> UPDATE: Added confirmNoEnemy() method to stop the robot from calling checkLeftRight() even if there was something infront of it.
//2018/3/23 --> UPDATE: Added additional functionality. Push a button to start the 5 second countdown. See start().
//2018/3/25 --> RELEASED (v3.0): Complete. Competition over. Cleaned up code to improve readability. Added more detailed comments.
//=== Notes ABOVE ===//

//=== Credits BELOW ===//
//Contributers to code: Johnson
//Contributers to sumobot physical and electrical construction: Jason
//Contributers to sumobot testing: Jason, Johnson
//Team name: 2 Engineers and a B-Tech
//=== Credits ABOVE ===//





//=== Variable declaration BELOW ===//
//Pinouts here
const int mLeftFor = 2, mLeftBack = 3, mRightFor = 4, mRightBack = 5; //These are all outputs
const int trigPin = 6, echoPin = 7;                                   //trigPin is the trigger (output), the other is echo (input)
const int qrd0 = 8, qrd1 = 9, qrd2 = 10, qrd3 = 11;                   //These are all inputs ; numbering starting from front left, CW
const int startButton = 12;                                           //startButton
//------------------------------------------------------------------->//qrd0 --> LeftFor; qrd1 --> RightFor; qrd2 --> RightBack; qrd3 --> LeftBack

//Other Variables
const int criticalDist = 50;                                          //The enemy must be within this distance (cm) before the sumobot will start attacking it
//=== Variable declaration ABOVE ===//

//=== SETUP BELOW ===//

void setup() {
  //Initializing things
  //Methods used: start()
  
  .begin(9600);

  for (int a = 2; a < 7; a++) {   //Setting pins as outputs
    pinMode(a, OUTPUT);
  }
  for (int a = 7; a < 8; a++) {   //Setting pins as inputs
    pinMode(a, INPUT);
  }

  pinMode(12, INPUT);

  start();                        //Calls the start sequence. See void start() for more info
}

//=== SETUP ABOVE ===//

//=== MAIN LOOP BELOW ===// --------------------------------------------------------------------------------------------------------------------------

void loop() {
  //The 'main method' of the program
  //The robot will start out by looking for the robot. We can either use activeSearch() or randomSearch()
  //Both search methods will stop the robot once it sees something. The search methods terminate.
  //Next, the attack() method is initiated. The robot simply moves forwards, as long as there is something in front of it.
  //If the robot loses sight, it will check its left and right and attempt to attack once again
  //If the enemy can not be seen, it will exit the attack sequence and assume that the enemy is coming from the side, so it enters the evade sequence
  //The robot simply moves back and turns; afterwhich it exits the evade sequence and begins searching again.
  //activeSearch();   //keeps turning in circles until the opponent is detected
  //Methods used: activeSearch(), attack()

//   randomSearch();   //keeps searching until the opponent is detected
  activeSearch();
  attack();         //keeps following until the opponent cannot be detected
  evade();          //the robot undergoes evasive maneuvers if attack ends
}

//=== MAIN LOOP ABOVE ===// --------------------------------------------------------------------------------------------------------------------------

//=== Major methods BELOW ===//

void start() {
  //The starting sequence of the robot
  //Turn to the left, then go forwards, then stop
  //Methods used: left(), forward(), off()

  while (!digitalRead(startButton)) {
  }

  delay(5000);

  right();
  delay(150);
  forward();
  delay(1000);
  off();
}

void activeSearch() {
  //Looks for the other robot by turning in a circle until the enemy is detected
  //Methods used: right(), getDist(), off()
  left();

  while (getDist() > criticalDist) {
  }

  off();
}

void randomSearch() {
  //The robot keeps going forwards until it hits the edge of the ring, afterwhich it turns around a certain amount and continues to go forwards
  //This continues to iterate until the enemy is detected
  //Methods used: forward(), getDist(), checkQRD(), off()
  forward();

  while (getDist() > criticalDist) {
    checkQRD();
  }

  off();
}

void checkQRD() {
  //Defines appropriate reactions to 8 different combinations of Sumobot position
  //Robot position is defined by the combination of QRDs that are indicating they are on the ring
  //Methods used: qrd(), forward(), back(), left(), right()
  if (qrd(qrd0) || qrd(qrd1) || qrd(qrd2) || qrd(qrd3)) {
    if (qrd(qrd0) && qrd(qrd1)) {
      back();
      delay(100);       //set an appropriate delay for it to back up enough
      left();
      delay(1000);      //set an appropriate delay for it to turn 120°
      forward();
    }
    else if (qrd(qrd1) && qrd(qrd2)) {
      left();
      delay(300);       //set an appropriate delay for the robot to turn 90°
      forward();
    }
    else if (qrd(qrd2) && qrd(qrd3)) {
      forward();
    }
    else if (qrd(qrd3) && qrd(qrd0)) {
      right();
      delay(300);       //set an appropriate delay for the robot to turn 90°
      forward();
    }
    else if (qrd(qrd0)) { //120° will be adequate for the following I think
      right();
      delay(450);       //set an appropriate delay for the robot to turn 120°
      forward();
    }
    else if (qrd(qrd1)) {
      right();
      delay(450);       //set an appropriate delay for the robot to turn 120°
      forward();
    }
    else if (qrd(qrd2)) {
      forward();
    }
    else if (qrd(qrd3)) {
      forward();
    }
  }
}

void attack() {
  //This method is only called after either activeSearch() or randomSearch() have terminated. Those methods only terminate when the enemy bot is infront of it
  //The robot keeps going forward as long as the robot is in front of it
  //If the robot loses sight of the enemy, it checks left or right (see checkLeftRight() )
  //If checkLeftRight() returns true, that means it has relocated the enemy; if it returns false, it means it can't find it.
  //true causes the robot to keep attacking
  //false causes the robot to leave this method
  //Methods used: forward(), getDist(), checkLeftRight()
  boolean keepAttacking = true;

  while (keepAttacking) {
    forward();
    if (getDist() > criticalDist && confirmNoEnemy()) {
      keepAttacking = checkRightLeft();
    }
  }
}

boolean confirmNoEnemy() {
  int counter = 0;      //Counts how many times getDist() > criticalDist

  for (int a = 0; a < 10; a++) {
    if (getDist() > criticalDist) {
      counter++;
    }
  }

  if (counter >= 8) {
    return true;
  }
  return false;
}

boolean checkRightLeft() {
  //Turns to the left in 10° increments, checking for the enemy with each increment. After turning a full 40° it turns 80° to the right
  //If the enemy is found the robot stops moving and indicates that it has found it by returning true
  //If not it indicates it cannot find the enemy and returns false
  //Methods used: getDist(), left(), off(), right()
  for (int a = 0; a < 4; a++) {   //set an appropriate delay so it turns left 40° in total
    right();
    delay(100);                   //set an appropriate delay so it turns left 10° each time
    off();
    if (getDist() < criticalDist) {
      return true;
    }
  }

  for (int a = 0; a < 8; a++) {   //set an appropriate delay so it turns right 80° in total
    left();
    delay(100);                   //set an appropriate delay so it turns right 10° each time
    off();
    if (getDist() < criticalDist) {
      return true;
    }
  }
  return false;
}

void evade() {
  //This method is only called when the attack() method terminates, meaning it cannot not find the enemy.
  //This just assumes the enemy is coming from the side, so it backs up a little bit then turns to the side
  //Methods used: back(), qrd(), right(), off()

  float startTime = millis();
  float endTime = millis();

  while (endTime - startTime < 300) {    //checking to see how long the robot has been reversing for. Only stop this sequence once it reverses an appropriate time. Maybe 1000ms?
    back();
    checkQRD();
    endTime = millis();
  }
  right();
  delay(300);         //set an appropriate delay so it turns right enough
  off();
}

//=== MAJOR methods ABOVE ===//

//=== Motor methods BELOW ===//
//General comments for all motor methods follow below:
//If the robot does not exhibit the expected behaviour, simply change the wiring

void forward() {
  //Makes the robot go forwards
  digitalWrite(mLeftBack, HIGH);
  digitalWrite(mLeftFor, LOW);
  digitalWrite(mRightBack, HIGH);
  digitalWrite(mRightFor, LOW);
}

void back() {
  //Makes the robot go backwards
  digitalWrite(mLeftFor, HIGH);
  digitalWrite(mLeftBack, LOW);
  digitalWrite(mRightFor, HIGH);
  digitalWrite(mRightBack, LOW);
}

void left() {
  //Makes the robot pivot left
  //The right motor goes forwards and the left motor goes backwards
  digitalWrite(mLeftFor, HIGH);
  digitalWrite(mLeftBack, LOW);
  digitalWrite(mRightBack, HIGH);
  digitalWrite(mRightFor, LOW);
}

void right() {
  //Makes the robot pivot right
  //The left motor goes forwards and the right motor goes backwards
  digitalWrite(mLeftBack, HIGH);
  digitalWrite(mLeftFor, LOW);
  digitalWrite(mRightFor, HIGH);
  digitalWrite(mRightBack, LOW);
}

void off() {
  //Stops the robot
  //It could be HIGH or LOW. Both combinations BRAKE the motors
  digitalWrite(mLeftFor, LOW);
  digitalWrite(mLeftBack, LOW);
  digitalWrite(mRightFor, LOW);
  digitalWrite(mRightBack, LOW);
}

//=== Motor methods ABOVE ===//

//=== Ultra Sonic Sensor methods BELOW === //

float getDist() {
  //First turns off trigger to ensure a clean signal. Sends high to trigPin for 10e-6s. Turns off trigger
  //Records the length of the pulse in e-6s; the length of the pulse represents how long it took for the signal to reach the ultrasonic sensor again
  //pulseIn() --> times how long a pulse lasts (either HIGH or LOW, you specify) at a specified pin
  //Performs a calculation for distance in cm and returns the distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distanceCm = ((343 * duration) / 10000) / 2;

  return distanceCm;
}

//=== Ultra Sonic Sensor methods ABOVE === //

//=== QRD methods BELOW === //

boolean qrd(const int pinNum) {
  //When calling this method specify which pin you want to read
  //Black ~4V
  //White <1V
  //Returns true if it detects that the QRD is above the ring
  //Returns false if it detects that the QRD is within the ring
  float voltage = (analogRead(pinNum) * 5) / 1023;
  if (voltage < 1) {
    return true;
  }
  return false;
}

//=== QRD methods ABOVE === //
//=== Used with permission from Johnson Qu (March 24 2018) ===///
