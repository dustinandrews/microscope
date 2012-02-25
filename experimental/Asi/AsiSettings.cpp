/* Microscope controller for Arduino 
 * By Dustin Andrews, Frank Luecke, David Luecke and Allen Burnham, 2012
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 * This program is design to run on Arduino MEGA
 * This program emulates the MFC-2000 ASI protocol described at http://www.asiimaging.com/ftp2asi/Manuals/MS2000%20Programming.pdf
 * Code available from https://github.com/dustinandrews/microscope
 */
#include "AsiSettings.h"


AsiSettings::AsiSettings()
{
  setSettings(&currentPos, 1.1, 2.02, 3.003); 
  setSettings(&maxSpeed, 7.1, 7.2, 7.3); 
  setSettings(&unitMultiplier, 1000, 1000, 1000);
  setSettings(&wait, 0,0,0);
}

void AsiSettings::setSettings(AxisSettings *s, int x, int y, int z)
{
  s->x = x;
  s->y = y;
  s->z = z;
}

void AsiSettings::setSettings(AxisSettingsF *s, float x, float y, float z)
{
  s->x = x;
  s->y = y;
  s->z = z;
}



////////////
//Defaults//
////////////
//float AsiSettings::currentPos[3] = {1.1, 2.02, 3.003};


