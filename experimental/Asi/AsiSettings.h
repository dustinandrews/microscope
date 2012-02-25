/* Microscope controller for Arduino 
 * By Dustin Andrews, Frank Luecke, David Luecke and Allen Burnham, 2012
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 * This program is design to run on Arduino MEGA
 * This program emulates the MFC-2000 ASI protocol described at http://www.asiimaging.com/ftp2asi/Manuals/MS2000%20Programming.pdf
 * Code available from https://github.com/dustinandrews/microscope
 */

#ifndef AsiSettings_h
#define AsiSettings_h
#include "WProgram.h"

class AsiSettings
{
  public:
    AsiSettings();
    //3 element arrays are are X,Y,Z
    static float currentPos[3];
    static float maxSpeed[3];
    static int unitMultiplier[3];
    static int wait[3];
		
};


#endif
