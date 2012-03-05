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

//AsiMS2000 encapsulates the interface to the MicroManager.
//AsiMS2000 defaults to PC communinication on Serial1 at 9600
//with debug output on Serial at 115200.
#include "AsiMS2000.h"
AsiMS2000 AsiMS2000;

/////////////////////////
//Serial Debug Messages//
/////////////////////////
#define DEBUG true

///////////////////
//Pin assignments//
///////////////////
//D0 and D1 are reserved for serial Communication with the PC

//Motors are driven by the EasyDriver board: http://www.sparkfun.com/products/10267
const int motorX_dir  = 2;
const int motorX_step = 3;
const int motorY_dir  = 4;
const int motorY_step = 5;
const int motorZ_dir  = 6;
const int motorZ_step = 7;

const int gnd_resetSteppers = 8;//ground to reset
const int disableSteppers = 9; //Enable on the A3967SLB is "Active Low", so the name is changed to make programming clearers.
const int gnd_sleepSteppers = 10;//ground to set boards to sleep mode.

//analog inputs that control the motors.
const int motorX_input = A0;
const int motorY_input = A1;
const int motorZ_input = A2;

/////////////////////////
//programming constants//
/////////////////////////
const int dead_zone = 10; //number of units out of 1024 that counts as centered;
const int pot_center = 512;


/////////////////////////////////////////
//Timing Constants and shared variables//
/////////////////////////////////////////
const int32_t intPerSec = 1500;//number of interupts proccessed per second. Also the max number of 1/8th motor steps per second.
const int input_delay = 500; //delay between reading inputs in microseconds.
long perSecRatio = 0;//set in setup routine based on interupts per sec.

//Variables to pass motor timing information into interupt routine.
const float moveTolerance = 0.000599;//define how close the position has to be in tenths of micrometers.
volatile int mx_persec = 0;
volatile int my_persec = 0;
volatile int mz_persec = 0;
volatile int interupts = 0;
volatile AxisSettings actualPosition;
volatile AxisSettings axisDirection;


//The actual position stored as INT divided by this factor will give
//The position in tenths of microns.
const int stepConversion = 1000;


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
  
  pinMode(motorX_step, OUTPUT);
  pinMode(motorY_step, OUTPUT);
  pinMode(motorZ_step, OUTPUT);
  pinMode(motorX_step, OUTPUT);
  pinMode(motorX_dir, OUTPUT);
  pinMode(motorY_dir, OUTPUT);
  pinMode(motorZ_dir, OUTPUT);
  
  digitalWrite(motorX_step, LOW);
  digitalWrite(motorX_step, LOW);
  digitalWrite(motorX_step, LOW);
  digitalWrite(motorX_dir, LOW);
  digitalWrite(motorY_dir, LOW);
  digitalWrite(motorZ_dir, LOW);
  
  pinMode(motorX_input, INPUT);
  pinMode(motorY_input, INPUT);
  pinMode(motorZ_input, INPUT);
 
  //enable output and reset the boards.
  digitalWrite(disableSteppers, LOW);
  digitalWrite(gnd_resetSteppers, LOW);
  delayMicroseconds(1);
  digitalWrite(gnd_resetSteppers, HIGH);
  
  //initialize the actual position.
  //TODO: create a homing routine to run to the negative limits.
  actualPosition.x = 0;
  actualPosition.y = 0;
  actualPosition.z = 0;
  
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

  handleAsiCommands();
   
  time = millis();
  //handle direct input according to the delay and only if not already busy.
  if(time - lastInputTime > input_delay)// && AsiMS2000.getBusyStatus() == false)
  {
    realTimeHandler(time);
    lastInputTime = time;     
  }
  
  //rollover the interupt timer for the timer callback routine.
  if(interupts >= intPerSec)
  {
    interupts = 0;
    char buffer[50];
    sprintf(buffer, "x %d    y %d    z %d", axisDirection.x, axisDirection.z, axisDirection.z);
    Serial.println(buffer);
    AxisSettings inputs;
    readInputs(&inputs);
    sprintf(buffer, "x %d    y %d    z %d", inputs.x, inputs.y, inputs.z);
    Serial.println(buffer);
    
    adjustInput(&inputs);
    sprintf(buffer, "x %d    y %d    z %d", inputs.x, inputs.y, inputs.z);
    Serial.println(buffer);   
    
    calculateMotorSpeeds(&inputs); 
    sprintf(buffer, "x %d    y %d    z %d", inputs.x, inputs.y, inputs.z);
    Serial.println(buffer);   

    sprintf(buffer, "x %d    y %d    z %d\n", mx_persec, my_persec, mz_persec);       
    Serial.println(buffer);
  }
  

   
}

void displayCurrentToDesired()
{
    char buffer[20];
    String reply = "";    
    AxisSettingsF a = actualPositionToF();
    AxisSettingsF d = AsiMS2000.getDesiredPos();
    dtostrf(a.x,1,4,buffer);
    reply.concat(String(buffer) + "->");
    dtostrf(d.x,1,4,buffer);
    reply.concat(String(buffer) + " ");
    reply.concat(mx_persec);
    reply.concat(" ");

    dtostrf(a.y,1,4,buffer);
    reply.concat(String(buffer) + "->");
    dtostrf(d.y,1,4,buffer);
    reply.concat(String(buffer) + " ");
    reply.concat(my_persec);
    reply.concat(" ");


    dtostrf(a.z,1,4,buffer);
    reply.concat(String(buffer) + "->");
    dtostrf(d.z,1,4,buffer);
    reply.concat(String(buffer) + " ");
    reply.concat(mz_persec);
    reply.concat(" ");
    
    Serial.println(reply);
}


void handleAsiCommands()
{
  AsiMS2000.checkSerial();
}


//convert from int position to float postion
//actual position is maintained as INT for accuracy.
AxisSettingsF actualPositionToF()
{
  AxisSettingsF floatPosition;
  floatPosition.x = (float)actualPosition.x / (float)stepConversion;
  floatPosition.y = (float)actualPosition.y / (float)stepConversion;
  floatPosition.z = (float)actualPosition.z / (float)stepConversion;
  return floatPosition;
}

int isWithinTolerance(float one, float two, float tolerance)
{
  if(one > two + tolerance || one < two - tolerance)
  {
    return false;
  } 
  return true;
}


void realTimeHandler(unsigned long time)
{
    AxisSettings inputArray;
    readInputs(&inputArray);
    adjustInput(&inputArray);
    setMotorDirection(&inputArray);
    calculateMotorSpeeds(&inputArray);
    setMotorSpeeds(&inputArray);
}

void readInputs(AxisSettings *inputs)
{
  inputs->x = analogRead(motorX_input);
  inputs->y = analogRead(motorY_input);
  inputs->z = analogRead(motorZ_input);
}

void adjustInput(AxisSettings *inputs)
{
   inputs->x = inputs->x - pot_center;
   inputs->y = inputs->y - pot_center;
   inputs->z = inputs->z - pot_center;
}

void setMotorDirection(AxisSettings *inputs)
{
   axisDirection.x = setDir(inputs->x, motorX_dir);
   axisDirection.y = setDir(inputs->y, motorY_dir);
   axisDirection.z = setDir(inputs->z, motorZ_dir);
}

int setDir(float pos, int pin)
{
   if(pos > 0)
   {
     digitalWrite(pin, HIGH);
     return true;
   }
   else
   {
     digitalWrite(pin, LOW);
     return false;
   }
}


void calculateMotorSpeeds(AxisSettings *inputs)
{
  //square the input to get a good input curve
  if(abs(inputs->x) > dead_zone)
  {
   inputs->x = (inputs->x * inputs->x) / perSecRatio;
  }
  else
  {
    inputs->x = 0;
  }

  if(abs(inputs->y) > dead_zone)
  {
    inputs->y = (inputs->y * inputs->y) / perSecRatio;
  }
  else
  {
    inputs->y = 0;
  }

  if(abs(inputs->z) > dead_zone)
  {
    inputs->z = (inputs->z * inputs->z) / perSecRatio;
  }
  else
  {
    inputs->z = 0;
  }
  
}

void setMotorSpeeds(AxisSettings *inputs)
{
  mx_persec = inputs->x;
  my_persec = inputs->y;
  mz_persec = inputs->z;  
}


void motorCallback()
{
    //moveToDesired();
    digitalWrite(motorX_step, LOW);
    digitalWrite(motorY_step, LOW);
    digitalWrite(motorZ_step, LOW);
    
    interupts++;
    long ma_mod = intPerSec/mx_persec;
    long mb_mod = intPerSec/my_persec;
    long mc_mod = intPerSec/mz_persec;
   
   
    if(mx_persec > 0 && interupts % ma_mod == 0)
    {
      digitalWrite(motorX_step, HIGH);
      if(axisDirection.x){actualPosition.x++;}else{actualPosition.x--;}       
    }
    
    if(my_persec > 0 && interupts % mb_mod == 0)
    {
      digitalWrite(motorY_step, HIGH);
      if(axisDirection.y){actualPosition.y++;}else{actualPosition.y--;}
    }
    
    if(mz_persec > 0 && interupts % mc_mod == 0)
    {
      digitalWrite(motorZ_step, HIGH);
      if(axisDirection.z){actualPosition.z++;}else{actualPosition.z--;}
    }
    
    AsiMS2000.setCurrentPos(actualPositionToF());
}

void moveToDesired()
{
  if(!AsiMS2000.getBusyStatus())
  {
    return;
  }
  //Calculate which motors to move if busy (moving) status is true;
  
  AxisSettingsF desired = AsiMS2000.getDesiredPos();  
  AxisSettingsF actualF = actualPositionToF();
  int isAtDesired = true;
  
  if(!isWithinTolerance(actualF.x, desired.x, moveTolerance))
  {    
    axisDirection.x = setDir(desired.x - actualF.x, motorX_dir);
    mx_persec = intPerSec;
    isAtDesired = false;
  }
  else
  {
    mx_persec = 0;
  }
  
  if(!isWithinTolerance(actualF.y, desired.y, moveTolerance))
  {
    axisDirection.y = setDir(desired.y - actualF.y, motorY_dir);
    my_persec = intPerSec;
    isAtDesired = false;
  }
  else
  {
    my_persec = 0;
  }
  
  if(!isWithinTolerance(actualF.z, desired.z, moveTolerance))
  {
    axisDirection.z = setDir(desired.z - actualF.z, motorZ_dir);
    mz_persec = intPerSec;
    isAtDesired = false;
  }
  else
  {
    mz_persec = 0;
  }
  
  if(isAtDesired)
  {
    AsiMS2000.clearBusyStatus();
    displayCurrentToDesired();
  }
}




