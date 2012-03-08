/* Microscope controller for Arduino 
 * By Dustin Andrews, Frank Luecke, David Luecke and Allen Burnham, 2012
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 * This program is design to run on Arduino MEGA
 * This program emulates the MFC-2000 ASI protocol described at http://www.asiimaging.com/ftp2asi/Manuals/MS2000%20Programming.pdf
 * Code available from https://github.com/dustinandrews/microscope
 */

#ifndef AsiMS2000_h
#define AsiMS2000_h

#if ARDUINO>=100
#include <Arduino.h> // Arduino 1.0
#else
#include <Wprogram.h> // Arduino 0022
#endif

#include "AsiSettings.h"

#define NUMCOMMANDS 84
#define BUFFERLEN 128
class AsiMS2000
{    
  public:
        AsiMS2000();
        void checkSerial();
        void displayCommands();
        void clearBusyStatus();
        int  getBusyStatus();
        AxisSettingsF getCurrentPos();
        AxisSettingsF getDesiredPos();
        void setCurrentPos(AxisSettingsF pos);
        void displayCurrentToDesired(char message[]);
        
  private:
        volatile int _busyStatus;
        int _numCommands;
        int _isQuery;
        AxisSettings _isAxis;
        static char* _commands[NUMCOMMANDS];
        static char* _shortcuts[NUMCOMMANDS];
        String _args;       
        void serialPrint(char*);
        void serialPrint(String data);
        void serialPrintln(char *);
        void serialPrintln(String data);
        void interpretCommand(char commandBuffer[]);
        void bufferOverunError(char commandBuffer[]);
        void clearCommandBuffer(char commandBuffer[]);
        void returnErrorToSerial(int errornum);
        int getCommandNum(String c);
        void selectCommand(int commandNum);
        void debugPrintln(char* data);
        void debugPrintln(String data);
        void outputPrintln(char* data);
        void inputPrint(byte data);
        void inputPrintln(char * data);
        void parseXYZArgs(AxisSettings *);
        void parseXYZArgs(AxisSettingsF *);
        char* GetArgumentValue(char arg);
        int isQueryCommand(String command);
        void isAxisInCommand();
        void settingsQuery(AxisSettings settings);
        void settingsQuery(AxisSettings settings, String reply);
        void settingsQuery(AxisSettingsF settings);
        void settingsQuery(AxisSettingsF setting, String reply);
        void settingsSet(AxisSettings *settings);
        void settingsSet(AxisSettingsF *settings);
        void getSetCommand(AxisSettings *settings);
        void getSetCommand(AxisSettingsF *settings);
        void getSetCommand2(AxisSettings *setting);
        void getSetCommand2(AxisSettingsF *setting);
/////////////////////
//Protocol commands//
/////////////////////
	void accel();
	void aalign();
	void afcont();
	void aflim();
	void afocus();
	void afset();
	void afmove();
	void ahome();
	void aij();
	void array();
	void azero();
	void backlash();
	void bcustom();
	void benable();
	void build();
	void cdate();
	void cnts();
	void customa();
	void customb();
	void dack();
	void dump();
	void ensync();
	void epolarity();
	void error();
	void halt();
	void here();
	void home();
	void info();
	void joystick();
	void jsspd();
	void kadc();
	void kd();
	void ki();
	void kp();
	void lcd();
	void led();
	void lladdr();
	void load();
	void lock();
	void lockrg();
	void lockset();
	void maintain();
	void motctrl();
	void move();
	void movrel();
	void pcros();
	void pedal();
	void rbmode();
	void rdadc();
	void rdsbyte();
	void rdstat();
	void relock();
	void reset();
	void rt();
	void runaway();
	void saveset();
	void savepos();
	void scan();
	void scanr();
	void scanv();
	void secure();
	void sethome();
	void setlow();
	void setup();
	void si();
	void speed();
	void spin();
	void status();
	void stopbits();
	void ttl();
	void um();
	void units();
	void unlock();
	void vb();
	void vector();
	void version();
	void wait();
	void where();
	void who();
	void wrdac();
	void zero();
	void z2b();
	void zs();
        void overshoot();
};






#endif
