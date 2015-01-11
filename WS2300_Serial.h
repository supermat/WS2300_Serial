#ifndef WS2300_SERIAL
#define WS2300_SERIAL      //Evite les inclusions multiples
 
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h> 
 
class WS2300_Serial
{
   private:
   bool isSoftSerialToUse; // Is Soft or Hard Serial to use
    HardwareSerial _HardSerial; // Global properties for serialv
    SoftwareSerial _SoftSerial; // Global properties for serialv
    String Leggi (int cmd[]); //Reads data from the weather station
 
   public:
    WS2300_Serial(HardwareSerial& p_serial);    //Constructeur de la class
    WS2300_Serial(SoftwareSerial & p_serial);    //Constructeur de la class
	String FahrenheitToCelsius(String p_TempFahrenheit);
	String RainWS2300Tomm(String p_RainWS2300);
	String RainWS2300ToWunderground(String p_RainWS2300);
    String getTime();	//Read Time from the WS
	String getDay();
	String getTemp(int temp_source);
	String getHum(int dato);
	String getPress(int dato);
	String getWind(int dato);
	String getRain(int dato);
	String getDew();
};
 
#endif
