/* Microscope controller for Arduino 
 * By Dustin Andrews, Frank Luecke, David Luecke and Allen Burnham, 2012
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 */
//Pin assignments
//D0 and D1 are reserved for serial Communication with the PC
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

//programming constants
int dead_zone = 30; //number of units out of 1024 that counts as centered;
int pot_center = 512;
int numInputPots = 3;

//timing variables
int step_delay = 1000; //delay between steps.
int input_delay = 1000; //delay between reading inputs.
//int base_speed = 130000;


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
  Serial.begin(115200);
  Serial.println("Startup Complete.");
}


void loop()
{
  static unsigned long lastInputTime = 0;
  static unsigned long lastOutputTime = 0;
  unsigned long time = 0;
  static unsigned int iterations = 0;
  iterations++;
  
  time = millis();
  int inputArray[3];
  if(time - lastInputTime > input_delay)
  {
    readInputs(inputArray);
    lastInputTime = time; 
    Serial.println(iterations);
    iterations = 0;
  }
  

  if(time - lastOutputTime > input_delay)
  {
    runMotors(inputArray);
    lastOutputTime = time;
  }
  

}

void readInputs(int inputs[])
{
  inputs[0] = analogRead(motorA_in);
  inputs[1] = analogRead(motorB_in);
  inputs[2] = analogRead(motorC_in);
}


void runMotors(int inputs[])
{
   int posA = inputs[0] - pot_center;
   int posB = inputs[1] - pot_center;
   int posC = inputs[2] - pot_center;

   setDir(posA, motorA_dir);
   setDir(posB, motorB_dir);
   setDir(posC, motorC_dir);
   
   posA = abs(posA);
   posB = abs(posB);
   posC = abs(posC);
   
   setPwmSpeed(posA, motorA_step);
   setPwmSpeed(posB, motorB_step);
   setPwmSpeed(posC, motorC_step);
   
   
   Serial.println("runmotor");
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

void setPwmSpeed(int pos, int pin)
{
   char buffer [50];
   if(pos < dead_zone)
   {
     analogWrite(pin, 0);
     sprintf(buffer, "%d %d", pin, pos );
     Serial.println(buffer);
   }
   else
   {
     pos = (pos - 3) / 2; //input can go to 512 but PWM is 0-255.
     analogWrite(pin, pos); //analog read goes to 1024. It's already divided in half.
     sprintf(buffer, "%d %d", pin, pos);
     Serial.println(buffer);     
   }
}
