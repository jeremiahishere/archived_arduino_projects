#include <Stepper.h>
#include "TimerOne.h"

//initialize the stepper
//the four output pins
const int stepper_pin_1 = 4;
const int stepper_pin_2 = 5;
const int stepper_pin_3 = 6;
const int stepper_pin_4 = 7;
//size of the stepper
const int steps_per_revolution = 100;
//speed of the stepper
const int revolutions_per_minute = 5;
//number of steps to move on each move
//note that this should take less than a second
//the step function is blocking
int steps_per_move = 10;
//4 pin stepper
Stepper stepper(steps_per_revolution, stepper_pin_1, stepper_pin_2, stepper_pin_3, stepper_pin_4);
//2 pin stepper
//Stepper stepper(steps_per_revolution, stepper_pin_1, stepper_pin_2);

//the two buttons
//advance only advances time
//devance only devances time
//both together turns the stepper off
const int advance_button_pin = 1;
const int devance_button_pin = 2;

//the timer
const int timer_pin = 9;
//how long between moves, in milliseconds
unsigned long timer_duration = 1000000;
//is greater than zero after the timer fires
int timer_fire_count = 0;
//number of timer fires to consume when moving the clock
//for example, if you need the motor to move once per minute,
//one option is to set the timer duration to 1 second, and fires per clock move to 60
int timer_fires_per_clock_move = 5;

//advances the stepper motor to keep time
const int MODE_CLOCK = 0;
//advance the stepper motor quickly
const int MODE_ADVANCE = 1;
//rewinds the stepper motor quickly
const int MODE_DEVANCE = 2;
//stops power to the stepper motor
const int MODE_FREE = 3;
//sets the current mode
//initializes to clock mode
int current_mode = MODE_CLOCK;

void setup()  {
  //stepper setup
  //the speed the motor moves
  stepper.setSpeed(revolutions_per_minute);

  //button setup
  pinMode(advance_button_pin, INPUT);
  pinMode(devance_button_pin, INPUT);

  //timer setup
  pinMode(timer_pin, OUTPUT);
  Timer1.initialize(timer_duration);
  //callback moves the clock each second
  Timer1.attachInterrupt(fire_timer);
}

//records that the timer has fired once if you are in clock mode
void fire_timer()  {
  if(current_mode == MODE_CLOCK)  {
    timer_fire_count++;
  }
}

void loop()  {
  current_mode = get_mode();
  
  if(current_mode == MODE_CLOCK)  {
    handle_mode_clock();
  } else if(current_mode == MODE_ADVANCE)  {
    handle_mode_advance();
  } else if(current_mode == MODE_DEVANCE)  {
    handle_mode_devance();
  } else if(current_mode == MODE_FREE)  {
    handle_mode_free();
  }
}

int get_mode()  {
  int advance_button_val = digitalRead(advance_button_pin);
  int devance_button_val = digitalRead(devance_button_pin);

  if(advance_button_val == LOW && devance_button_val == LOW)  {
    return MODE_FREE;
  } 
  else if(advance_button_val == LOW)  {
    return MODE_ADVANCE;
  } 
  else if(devance_button_val == LOW)  {
    return MODE_DEVANCE;
  } 
  else {
    return MODE_CLOCK;
  }
}

//increments the clock
//only if the timer has fired since this has last run
void handle_mode_clock()  {
  if(timer_fire_count >= timer_fires_per_clock_move)  {
    stepper.step(steps_per_move);
    timer_fire_count -= timer_fires_per_clock_move;
  }
}

//moves the clock forward quickly
void handle_mode_advance()  {
  stepper.step(steps_per_move);
}

//moves the clock backwards quickly
void handle_mode_devance()  {
  stepper.step(-steps_per_move);
}

//freees the stepper motor from input
//this is just a guess on how to do this
void handle_mode_free()  {
  digitalWrite(stepper_pin_1, LOW);
  digitalWrite(stepper_pin_2, LOW);
  digitalWrite(stepper_pin_3, LOW);
  digitalWrite(stepper_pin_4, LOW);
}
