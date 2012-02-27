/* Microscope controller for Arduino 
 * By Dustin Andrews, Frank Luecke, David Luecke and Allen Burnham, 2012
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 * This program is design to run on Arduino MEGA
 * Code available from https://github.com/dustinandrews/microscope
 */
/////////////
//Libraries//
/////////////

// The TimerThree library is required.
// Get it from http://www.arduino.cc/playground/Code/Timer1
// Unpack the files someplace then add TimerThree.cpp and TimerThree.h from the 
// Sketch->"Add File" menu.
#include "TimerThree.h"

#include "AsiMS2000.h"

/////////////////////////
//Serial Debug Messages//
/////////////////////////
#define DEBUG true

///////////////////
//Pin assignments//
///////////////////
//D0 and D1 are reserved for serial Communication with the PC

//Motors are driven by the EasyDriver board: http://www.sparkfun.com/products/10267
const int motorA_dir  = 2;
const int motorA_step = 3;
const int motorB_dir  = 4;
const int motorB_step = 5;
const int motorC_dir  = 6;
const int motorC_step = 7;

const int gnd_resetSteppers = 8;//ground to reset
const int disableSteppers = 9; //Enable on the A3967SLB is "Active Low", so the name is changed to make programming clearers.
const int gnd_sleepSteppers = 10;//ground to set boards to sleep mode.

const int motorA_in = A0;
const int motorB_in = A1;
const int motorC_in = A2;

/////////////////////////
//programming constants//
/////////////////////////
const int dead_zone = 0; //number of units out of 1024 that counts as centered;
const int pot_center = 512;
const int numInputPots = 3;

/////////////////////////////////////////
//Timing Constants and shared variables//
/////////////////////////////////////////
const int32_t intPerSec = 1500;//number of interupts proccessed per second.
const int input_delay = 500; //delay between reading inputs in microseconds.
long perSecRatio = 0;//set in setup routine based on interupts per sec.

//Variables to pass motor timing information into interupt routine.
volatile int ma_persec = 0;
volatile int mb_persec = 0;
volatile int mc_persec = 0;
volatile int interupts = 0;


void setup() 
{
  //First, disable the steppers during setup.
  pinMode(disableSteppers, OUTPUT);
  digitalWrite(disableSteppers, HIGH);
  
  //reset and sleep are active low. So set them high for normal operation.
  pinMode(gnd_resetSteppers, OUTPUT);
  digitalWrite(gnd_resetSteppers, HIGH);
  pinMode(gnd_sleepSteppers, OUTPUT);
  digitalWrite(gnd_sleepSteppers, HIGH);
  
  pinMode(motorA_step, OUTPUT);
  pinMode(motorB_step, OUTPUT);
  pinMode(motorC_step, OUTPUT);
  pinMode(motorA_step, OUTPUT);
  pinMode(motorA_dir, OUTPUT);
  pinMode(motorB_dir, OUTPUT);
  pinMode(motorC_dir, OUTPUT);
  
  digitalWrite(motorA_step, LOW);
  digitalWrite(motorA_step, LOW);
  digitalWrite(motorA_step, LOW);
  digitalWrite(motorA_dir, LOW);
  digitalWrite(motorB_dir, LOW);
  digitalWrite(motorC_dir, LOW);
  
  pinMode(motorA_in, INPUT);
  pinMode(motorB_in, INPUT);
  pinMode(motorC_in, INPUT);
 
  //enable output and reset the boards.
  digitalWrite(disableSteppers, LOW);
  digitalWrite(gnd_resetSteppers, LOW);
  delayMicroseconds(1);
  digitalWrite(gnd_resetSteppers, HIGH);
  
  //setup the interupt routine.
  perSecRatio = ((512L * 512L) / intPerSec)+1;//+1 to make up for not doing floating point calculations.
  int32_t timerFactor = 1000000 / intPerSec;  
  Timer3.initialize(timerFactor);
  Timer3.attachInterrupt(motorCallback);
  
  Serial.begin(115200);
  Serial.println("Startup Complete.");
}


void loop()
{
  static unsigned long lastInputTime = 0;
  static unsigned long lastOutputTime = 0;
  unsigned long time = 0;
  int inputArray[3];
  
  time = millis();
  if(time - lastInputTime > input_delay)
  {
    readInputs(inputArray);
    if(DEBUG)
    {
      char buffer [50];
      sprintf(buffer,"A:%d B:%d C:%d", inputArray[0],inputArray[1],inputArray[2]);
      Serial.println(buffer);
    }
    
    adjustInput(inputArray);
    setMotorDirection(inputArray);
    calculateMotorSpeeds(inputArray);
    lastInputTime = time; 
  }
  
  if(interupts >= intPerSec)
  {
    interupts = 0;
  }
  
}

void readInputs(int inputs[])
{
  inputs[0] = analogRead(motorA_in);
  inputs[1] = analogRead(motorB_in);
  inputs[2] = analogRead(motorC_in);
}

void adjustInput(int inputs[])
{
   inputs[0] = inputs[0] - pot_center;
   inputs[1] = inputs[1] - pot_center;
   inputs[2] = inputs[2] - pot_center;
}

void setMotorDirection(int inputs[])
{
   setDir(inputs[0], motorA_dir);
   setDir(inputs[1], motorB_dir);
   setDir(inputs[2], motorC_dir);
}

void calculateMotorSpeeds(int inputs[])
{
  //square the input to get a good input curve.
  ma_persec = abs(((long)inputs[0] * (long)inputs[0]) / perSecRatio);
  mb_persec = abs(((long)inputs[1] * (long)inputs[1]) / perSecRatio);
  mc_persec = abs(((long)inputs[2] * (long)inputs[2]) / perSecRatio);
}


void motorCallback()
{
    digitalWrite(motorA_step, LOW);
    digitalWrite(motorB_step, LOW);
    digitalWrite(motorC_step, LOW);
    
    interupts++;
    long ma_mod = intPerSec/ma_persec;
    long mb_mod = intPerSec/mb_persec;
    long mc_mod = intPerSec/mc_persec;
    
    if(ma_persec > 0 && interupts % ma_mod == 0)
    {
      digitalWrite(motorA_step, HIGH);    
    }
    
    if(mb_persec > 0 && interupts % mb_mod == 0)
    {
      digitalWrite(motorB_step, HIGH);
    }
    
    if(mb_persec > 0 && interupts % mc_mod == 0)
    {
      digitalWrite(motorC_step, HIGH);
    }    
}

void setDir(int pos, int pin)
{
   if(pos > 0)
   {
     digitalWrite(pin, HIGH);
   }
   else
   {
     digitalWrite(pin, LOW);
   }
}


