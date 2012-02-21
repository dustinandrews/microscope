/* Microscope controller for Arduino 
 * By Dustin Andrews, Frank Luecke, David Luecke and Allen Burnham, 2012
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 * This program is design to run on Arduino MEGA
 * This program emulates the MFC-2000 ASI protocol described at http://www.asiimaging.com/ftp2asi/Manuals/MS2000%20Programming.pdf
 * Code available from https://github.com/dustinandrews/microscope
 */

#include "AsiMS2000.h"
//needs to be long enough to contain biggest command string.
#define BUFFERSIZE 128


AsiMS2000::AsiMS2000()
{
  _numCommands = NUMCOMMANDS;
//  _commands  =
//        {"ACCEL","AALIGN","AFCONT","AFLIM","AFOCUS","AFSET","AFMOVE",
//                  "AHOME","AIJ","ARRAY","AZERO","BACKLASH","BCUSTOM","BENABLE","BUILD",
//                  "CDATE","CNTS","CUSTOMA","CUSTOMB","DACK","DUMP","ENSYNC","EPOLARITY",
//                  "ERROR","HALT","HERE","HOME","INFO","JOYSTICK","JSSPD","KADC","KD","KI",
//                  "KP","LCD","LED","LLADDR","LOAD","LOCK","LOCKRG","LOCKSET","MAINTAIN",
//                  "MOTCTRL","MOVE","MOVREL","PCROS","PEDAL","RBMODE","RDADC","RDSBYTE",
//                  "RDSTAT","RELOCK","RESET","RT","RUNAWAY","SAVESET","SAVEPOS","SCAN",
//                  "SCANR","SCANV","SECURE","SETHOME","SETLOW","SETUP","SI","SPEED","SPIN",
//                  "STATUS","STOPBITS","TTL","UM","UNITS","UNLOCK","VB","VECTOR","VERSION",
//                  "WAIT","WHERE","WHO","WRDAC","ZERO","Z2B","ZS"
//                  };
//  _shortcuts  =
//        {"AC","AA","AFCONT","AFLIM","AFOCUS","AFSET","AFMOVE",
//                   "AH","IJ","AR","AZ","B","BCA","BE","BU",
//                   "CD","C","CCA","CCB","D","DU","ES","EP",
//                   "E","\\","H","!","I","J","JS","KA","KD","KI",
//                   "KP","LCD","LED","LL","LD","LK","LR","LS","MA",
//                   "MC","M","R","PC","PD","RM","RA","RB",
//                   "RS","RL","~","RT","RU","SS","SP","SN",
//                   "NR","NV","SECURE","HM","SL","SU","SI","S","@",
//                   "/","SB","TTL","UM","UN","UL","VB","VE","V",
//                   "WT","W","N","WRDAC","Z","Z2B","ZS"
//                   };
}

void AsiMS2000::checkSerial()
{
  int inByte = 0;
  static int bufferPos = 0;
  static char commandBuffer [BUFFERSIZE];
  if(Serial.available() > 0)
  {
    inByte = Serial.read();
    if(bufferPos > BUFFERSIZE)
    {
      bufferPos = 0;
      bufferOverunError(commandBuffer);
      return;
    }
    
    //check for <CR> or | since arduino env can't send CR.
    if(inByte == 27 || inByte == 124)
    {
      commandBuffer[bufferPos]  = '\0';
      interpretCommand(commandBuffer, bufferPos);
      bufferPos =0;
    }
    else
    {
      commandBuffer[bufferPos++] = inByte;
    }
  }
}


void AsiMS2000::interpretCommand(char commandBuffer[], int bufferPos)
{
    Serial.println("interpretting");
    Serial.println(commandBuffer);
    confirmSpaceChar(commandBuffer);
    clearCommandBuffer(commandBuffer);
}

int AsiMS2000::confirmSpaceChar(char commandBuffer[])
{
   String c = String(commandBuffer);
   int firstSpace = c.indexOf(' ');
   Serial.print("first space =");
   Serial.println(firstSpace);
   return firstSpace;
}

void AsiMS2000::bufferOverunError(char commandBuffer[])
{
  returnErrorToSerial(-6);
  clearCommandBuffer(commandBuffer);
}

void AsiMS2000::clearCommandBuffer(char commandBuffer[])
{
  for(int i = 0; i < BUFFERSIZE; i++)
  {
    commandBuffer[i] = '\0';
  }
}

void AsiMS2000::displayCommands()
{
  char buffer [50];
  for(int i = 0; i < _numCommands; i++)
  {
    sprintf(buffer, "%s => %s", _commands[i], _shortcuts[i]);
    Serial.println(buffer);
  }
}


void AsiMS2000::returnErrorToSerial(int errornum)
{
  Serial.print(":N");
  Serial.println(errornum);
}

void AsiMS2000::accel()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::aalign()
{
    returnErrorToSerial(-6);
    returnErrorToSerial(-6);


}
void AsiMS2000::afcont()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::aflim()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::afocus()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::afset()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::afmove()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::ahome()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::aij()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::array()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::azero()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::backlash()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::bcustom()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::benable()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::build()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::cdate()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::cnts()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::customa()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::customb()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::dack()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::dump()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::ensync()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::epolarity()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::error()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::halt()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::here()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::home()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::info()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::joystick()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::jsspd()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::kadc()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::kd()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::ki()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::kp()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::lcd()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::led()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::lladdr()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::load()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::lock()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::lockrg()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::lockset()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::maintain()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::motctrl()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::move()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::movrel()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::pcros()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::pedal()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::rbmode()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::rdadc()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::rdsbyte()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::rdstat()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::relock()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::reset()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::rt()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::runaway()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::saveset()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::savepos()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::scan()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::scanr()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::scanv()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::secure()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::sethome()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::setlow()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::setup()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::si()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::speed()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::spin()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::status()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::stopbits()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::ttl()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::um()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::units()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::unlock()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::vb()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::vector()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::version()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::wait()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::where()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::who()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::wrdac()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::zero()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::z2b()
{
    returnErrorToSerial(-6);


}


void AsiMS2000::zs()
{
    returnErrorToSerial(-6);


}


char*  AsiMS2000::_commands[] =
        {"ACCEL","AALIGN","AFCONT","AFLIM","AFOCUS","AFSET","AFMOVE",
                  "AHOME","AIJ","ARRAY","AZERO","BACKLASH","BCUSTOM","BENABLE","BUILD",
                  "CDATE","CNTS","CUSTOMA","CUSTOMB","DACK","DUMP","ENSYNC","EPOLARITY",
                  "ERROR","HALT","HERE","HOME","INFO","JOYSTICK","JSSPD","KADC","KD","KI",
                  "KP","LCD","LED","LLADDR","LOAD","LOCK","LOCKRG","LOCKSET","MAINTAIN",
                  "MOTCTRL","MOVE","MOVREL","PCROS","PEDAL","RBMODE","RDADC","RDSBYTE",
                  "RDSTAT","RELOCK","RESET","RT","RUNAWAY","SAVESET","SAVEPOS","SCAN",
                  "SCANR","SCANV","SECURE","SETHOME","SETLOW","SETUP","SI","SPEED","SPIN",
                  "STATUS","STOPBITS","TTL","UM","UNITS","UNLOCK","VB","VECTOR","VERSION",
                  "WAIT","WHERE","WHO","WRDAC","ZERO","Z2B","ZS"
                  };
                  
char* AsiMS2000::_shortcuts[] =
        {"AC","AA","AFCONT","AFLIM","AFOCUS","AFSET","AFMOVE",
                   "AH","IJ","AR","AZ","B","BCA","BE","BU",
                   "CD","C","CCA","CCB","D","DU","ES","EP",
                   "E","\\","H","!","I","J","JS","KA","KD","KI",
                   "KP","LCD","LED","LL","LD","LK","LR","LS","MA",
                   "MC","M","R","PC","PD","RM","RA","RB",
                   "RS","RL","~","RT","RU","SS","SP","SN",
                   "NR","NV","SECURE","HM","SL","SU","SI","S","@",
                   "/","SB","TTL","UM","UN","UL","VB","VE","V",
                   "WT","W","N","WRDAC","Z","Z2B","ZS"
                   };

