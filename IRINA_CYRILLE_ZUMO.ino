// Import libraries
#include <Wire.h>
#include <ZumoShield.h>
#include <ZumoMotors.h>
#include <Pushbutton.h> 


// We don't touch that:
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1500 // microseconds

// Define pin to check if the void setup is working
#define LED_PIN 13

// Assign echo pin and trig pin for our ultrasonic sensor
const int TRIG_PIN = 4;
const int ECHO_PIN = 1;

// Define motor speed for each direction and mouvement
//these might need to be tuned for different motor types
#define REVERSE_SPEED     400 // 0 is stopped, 400 is full speed  
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms
#define FORWARD_DURATION  200 // ms


// Define number of line sensors
#define nbr_line_detectors 6
unsigned int sensor_values[nbr_line_detectors];




#define MELODY_LENGTH 95


// These arrays take up a total of 285 bytes of RAM (out of a limit of 1k (ATmega168), 2k (ATmega328), or 2.5k(ATmega32U4))
unsigned char note[MELODY_LENGTH] =
{
  NOTE_E(5), SILENT_NOTE, NOTE_E(5), SILENT_NOTE, NOTE_E(5), SILENT_NOTE, NOTE_C(5), NOTE_E(5),
  NOTE_G(5), SILENT_NOTE, NOTE_G(4), SILENT_NOTE,

  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),

  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_G_SHARP(4), NOTE_A(4), NOTE_C(5), SILENT_NOTE, NOTE_A(4), NOTE_C(5), NOTE_D(5),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_C(6), SILENT_NOTE, NOTE_C(6), SILENT_NOTE, NOTE_C(6),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_G_SHARP(4), NOTE_A(4), NOTE_C(5), SILENT_NOTE, NOTE_A(4), NOTE_C(5), NOTE_D(5),

  SILENT_NOTE, NOTE_E_FLAT(5), SILENT_NOTE, NOTE_D(5), NOTE_C(5)
};

unsigned int duration[MELODY_LENGTH] =
{
  100, 25, 125, 125, 125, 125, 125, 250, 250, 250, 250, 250,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 125, 125, 125, 250, 125, 125, 200, 50, 100, 25, 500,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 250, 125, 375, 500
};








//Define buzzer, motors, activation button, and sensors
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
unsigned char currentIdx;



void setup()
{
  // Turn on LED 13 to check that the code is working
  pinMode(LED_PIN, HIGH);

  // Index for sound
    currentIdx = 0;

  
  // Initialize serial monitor for ultrasonic sensor
  Serial.begin(9600);

  // Will begin working once button 12 is pressed
  button.waitForButton();
}


void loop()
{

  //-----------------------
  //SONG
  // if we haven't finished playing the song and
  // the buzzer is ready for the next note, play the next note
  if (currentIdx < MELODY_LENGTH && !buzzer.isPlaying())
  {
    // play note at max volume
    buzzer.playNote(note[currentIdx], duration[currentIdx], 15);
    currentIdx++;
  }

  // Insert some other useful code here...
  // the melody will play normally while the rest of your code executes
  // as long as it executes quickly enough to keep from inserting delays
  // between the notes.

  // For example, let the user pushbutton function as a stop/reset melody button
  if (button.isPressed())
  {
    buzzer.stopPlaying(); // silence the buzzer
    if (currentIdx < MELODY_LENGTH)
      currentIdx = MELODY_LENGTH;        // terminate the melody
    else
      currentIdx = 0;                    // restart the melody
    button.waitForRelease();  // wait here for the button to be released
  }


  //----------------------------
  // ULTRASONIC SENSOR
  
  long duration;
  long cm;  
  
  digitalWrite(LED_PIN, HIGH);
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
//  This part is for ultrasonic sensors calibration so we didn't touch anything
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the  (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);

  // convert the  into a distance
  cm = duration/ 29 / 2;
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  



  //------------------------
  // LINE DETECTOR
    sensors.read(sensor_values);

  
  if (sensor_values[0] < QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right

    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
  }

   else if (sensor_values[1] < QTR_THRESHOLD)
  {
    // if 2nd leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
  }

   else if (sensor_values[4] < QTR_THRESHOLD)
  {
    // if 2nd rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
  }
  
  else if (sensor_values[5] < QTR_THRESHOLD)
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
  }
  else
  {
    if(cm < 8){      // accelerate      
    digitalWrite(LED_PIN, LOW);   

    // double speed
    motors.setSpeeds(FORWARD_SPEED*2, FORWARD_SPEED*2);
    delay(FORWARD_DURATION);
 
  }

    else
    {
      digitalWrite(LED_PIN, LOW); 
      
      // turn to randomize direction
      motors.setSpeeds(FORWARD_SPEED*0.5, FORWARD_SPEED);

      delay(FORWARD_DURATION);
      }
  }

  
  
  
}
