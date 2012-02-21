/* Microscope controller for Arduino 
 * By Dustin Andrews, Frank Luecke, David Luecke and Allen Burnham, 2012
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 * This program is design to run on Arduino MEGA
 * This program emulates the MFC-2000 ASI protocol described at http://www.asiimaging.com/ftp2asi/Manuals/MS2000%20Programming.pdf
 * Code available from https://github.com/dustinandrews/microscope
 */

#include "AsiMS2000.h"

AsiMS2000 AsiMS2000;
                 
void setup()
{
  Serial.begin(9600);
  Serial.println("init complete");
  AsiMS2000.displayCommands();
}

void loop()
{
  AsiMS2000.checkSerial();
}

