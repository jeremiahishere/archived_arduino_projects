/* Metronome, based on blink without delay tutorial
*/
 
//min and max bpm values
const int min_bpm = 60;
const int max_bpm = 1000;

//led 1, 2, and speed control
const int led1Pin =  2;
const int led2Pin = 3;
const int digitalSpeakerPin = 4;
const int sensorPin = A0;

//initial led states
int led1State = LOW;
int led2State = LOW;
//time since last state change, starts at 0 to force a state change
long previousMillis = 0;
//initialze speed controller value
int sensorValue = 0;

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(led1Pin, OUTPUT); 
  pinMode(led2Pin, OUTPUT);
  pinMode(digitalSpeakerPin, OUTPUT);
  
  //Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  interval = convert_analog_input_to_bpm(analogRead(sensorPin), min_bpm, max_bpm);
  
  if(interval == 0)  {
    turn_off_leds();
  } else if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    switch_led_state();
  }
}

//converts analog input to beats per minute, then beats per minute to milliseconds per beat
//if the input is 0, return 0 (treated as off)
//otherwise run the conversion
int convert_analog_input_to_bpm(int input, int min_bpm, int max_bpm)  {
  if(input == 0)  {
    return 0;
  }
  int analog_range = 1023;
  int bpm_range = max_bpm - min_bpm;
  double bpm = (((double)input * bpm_range) / analog_range) + min_bpm;
  double output = (60000 / bpm); //milliseconds in a minute
  return (int)output;
}

//turn off both leds
void turn_off_leds()  {
  //set both to low
  led1State = LOW;
  led2State = LOW;
    // set the LED with the ledState of the variable:
  digitalWrite(led1Pin, led1State);
  digitalWrite(led2Pin, led2State);
}

//switch which led is turned on
void switch_led_state() {
  //determine led states
  if (led1State == LOW)  {
    led1State = HIGH;
    led2State = LOW;
  } else {
    led1State = LOW;
    led2State = HIGH;
  }
  //update the leds
  digitalWrite(led1Pin, led1State);
  digitalWrite(led2Pin, led2State); 
  
  //this will create a click every time the pin changes state
  //digitalWrite(digitalSpeakerPin, led2State); 
  
  //this will create a tone for 50ms on every loop
  //apparently nonblocking
  //Set to A440 I think
  tone(digitalSpeakerPin, 440, 50);
}
