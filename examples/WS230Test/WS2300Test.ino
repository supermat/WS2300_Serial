#include <WS2300_Serial.h>
 
 WS2300_Serial WS2300 = WS2300_Serial(Serial1);
 
void setup()
{
    Serial.begin(115200);
  Serial.println("Hello Ethernet Enabled WS2355!");
}
 
void loop()
{
    int incomingByteSer;      // a variable to read incoming serial data into

  // see if there's incoming serial data:
  if (Serial.available() > 0)
  {
    // read the oldest byte in the serial buffer:
    incomingByteSer = Serial.read();
    //Serial.flush();
    
    if (incomingByteSer == 'o') 
    {
      Serial.print("o=Time ");
      String data=WS2300.getTime();
      Serial.println(data);
    }  
 
    if (incomingByteSer == 'd') 
    {
      Serial.print("d=Day ");
      String data=WS2300.getDay();
      Serial.println(data);
    }
    
    if (incomingByteSer == 't') 
    {
      Serial.print("t=Temperature internal (C) ");
      String data=WS2300.FahrenheitToCelsius(WS2300.getTemp(1));
      Serial.println(data);
    } 

    if (incomingByteSer == 'T') 
    {
      Serial.print("T=Temperature external (C) ");
      String data=WS2300.FahrenheitToCelsius(WS2300.getTemp(0));
      Serial.println(data);
    } 

    if (incomingByteSer == 'h') 
    {
      Serial.print("h=Humidity internal ");
      String data=WS2300.getHum(1);
      Serial.println(data);
    } 

    if (incomingByteSer == 'H') 
    {
      Serial.print("H=Humidity external ");
      String data=WS2300.getHum(0);
      Serial.println(data);
    } 

    if (incomingByteSer == 'p') 
    {
      Serial.print("p=Pressure (hPa) ");
      String data=WS2300.getPress(1);
      Serial.println(data);
    } 

    if (incomingByteSer == 'P') 
    {
      Serial.print("P=Pressure (Hg) ");
      String data=WS2300.getPress(0);
      Serial.println(data);
    } 

    if (incomingByteSer == 'w') 
    {
      Serial.print("w=Wind Speed (mph) ");
      String data=WS2300.getWind(0);
      Serial.println(data);     
    } 

    if (incomingByteSer == 'W') 
    {
      Serial.print("W=Wind Dir ");
      String data=WS2300.getWind(1);
      Serial.println(data);
    } 

    if (incomingByteSer == 'r') 
    {
      Serial.print("r=Rain (1hr) ");
      String data=WS2300.getRain(1);
      Serial.println(data);
    } 

    if (incomingByteSer == 'R') 
    {
      Serial.print("R=Rain (24hr) ");
      String data=WS2300.getRain(0);
      Serial.println(data);
    }  

    if (incomingByteSer == 'e') 
    {
      Serial.print("e=Dew point (C) ");
      String data=WS2300.FahrenheitToCelsius(WS2300.getDew());
      Serial.println(data);
    } 
  }
}