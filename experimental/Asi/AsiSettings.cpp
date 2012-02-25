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
		
}

////////////
//Defaults//
////////////
float AsiSettings::currentPos[3] = {1.1, 2.02, 3.003};
int AsiSettings::unitMultiplier[3] = {1000, 1000, 1000};
int AsiSettings::wait[3] = {0, 0, 0};
