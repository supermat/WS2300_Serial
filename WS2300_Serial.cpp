#include "WS2300_Serial.h"
/*
mappa ws2300
 
Command Hex Digit       Command Hex Digit
82   0       A2  8
86   1       A6  9
8A   2       AA  A
8E   3       AE  B
92   4       B2  C
96   5       B6  D
9A   6       BA  E
9E   7       BE  F
 
*/


//Choix du port Serie en paramêtre
WS2300_Serial::WS2300_Serial(HardwareSerial& p_serial) : _HardSerial(p_serial)
{
// initialize serial communications to the WS2355:
  _HardSerial.begin(2400);
}

//Tc = (5/9)*(Tf-32)
String WS2300_Serial::FahrenheitToCelsius(String p_TempFahrenheit){
float v_tempF = 0;
float v_tempFC = 0;
char carray[p_TempFahrenheit.length() + 1]; //determine size of the array
    p_TempFahrenheit.toCharArray(carray, sizeof(carray)); //put readStringinto an array
    v_tempF = atof(carray);
	v_tempFC = (5.0/9.0)*(v_tempF-32.0);
	char v_TempC[5];
	dtostrf(v_tempFC,3,1,v_TempC);
	String v_TempCStr(v_TempC);
	return v_TempCStr;
}

String WS2300_Serial::RainWS2300Tomm(String p_RainWS2300){
float v_tempWS = 0;
float v_tempmm = 0;
char carray[p_RainWS2300.length() + 1]; //determine size of the array
    p_RainWS2300.toCharArray(carray, sizeof(carray)); //put readStringinto an array
    v_tempWS = atof(carray);
	v_tempmm = v_tempWS / 2.54;
	char v_TempC[7];
	dtostrf(v_tempmm,3,1,v_TempC);
	String v_TempCStr(v_TempC);
	return v_TempCStr;
}

String WS2300_Serial::RainWS2300ToWunderground(String p_RainWS2300){
float v_tempWS = 0;
float v_tempWU = 0;
char carray[p_RainWS2300.length() + 1]; //determine size of the array
    p_RainWS2300.toCharArray(carray, sizeof(carray)); //put readStringinto an array
    v_tempWS = atof(carray);
	v_tempWU = ((v_tempWS /2.54)/25.4);
	char v_TempC[10];
	dtostrf(v_tempWU,3,4,v_TempC);
	String v_TempCStr(v_TempC);
	return v_TempCStr;
}

String WS2300_Serial::getTime(){
      //'o'
      int my_array[] = {0x82,0x8A,0x82,0x82,0xCE};  //0200, Read 3 Bytes
      String dataStr;
      String inString;

      for (int ritenta=0; ritenta<10; ritenta++)
      {
          inString=Leggi(my_array);
          //Serial.print("getTime: ");
          //Serial.println(inString);
          
          if (inString != "")
          {
            dataStr = dataStr + inString.substring(6,8);  //estraggo i minuti
            dataStr = dataStr + ":";
            dataStr = dataStr + inString.substring(4,6);  //estraggo i secondi
            dataStr = dataStr + ":";
            dataStr = dataStr + inString.substring(2,4);  //estraggo l'ora
            break;
          }
          else
          {
            delay(500);
          } //if-else
      } //for  

      return(dataStr);
}

String WS2300_Serial::getDay(){
      //'d'
      String inString;
      String dataStr="";
      int my_array[] = {0x82,0x8A,0x8E,0xAE,0xEE}; 
      for (int ritenta=0; ritenta<10; ritenta++)
      {
          dataStr="";
          inString=Leggi(my_array);
          if (inString.length()>5)
          {
                dataStr.concat("20");
                dataStr.concat(inString.charAt(13));
                dataStr.concat(inString.charAt(10));
                dataStr.concat("-");
                dataStr.concat(inString.charAt(11));
                dataStr.concat(inString.charAt(8));
                dataStr.concat("-");
                dataStr.concat(inString.charAt(9));
                dataStr.concat(inString.charAt(6));
                if (((inString[13]<='9' && inString[13]>='0')&&(inString[10]<='9' && inString[10]>='0')) && ((inString[11]<='1' && inString[11]>='0')&&(inString[8]<='9' && inString[8]>='0')) && ((inString[9]<='3' && inString[9]>='0')&&(inString[6]<='9' && inString[6]>='0'))) 
                {
                    break;
                }
                else 
                {
                  dataStr="";
                }
           }
           else
           {
               delay(500);
           }
      }
      return(dataStr);
}

/********************************************************************
 * Reads the temperature from the WS23xx and returns the string 
 * representation of it (Fahrenheit).
 *
 * Input:   int, that sets if to read internal or external temperature
 *   0 = External
 *   1 = Internal
 * 
 * Output:  
 * 
 * Returns: String containing temperature
 *
 ********************************************************************/
String WS2300_Serial::getTemp(int temp_source){
      //'t' (internal) & 'T' (external)
      String inString;
      String dataStr;
      char cdata [10];
      int lung=0;
      char buff[10];
      int my_array[6];

      if (temp_source==0)
      {
        //Current External Temperature Sensor
        // Address = 0x0373
        // Bytes = 2
        my_array[0] = (0x82); //0
        my_array[1] = (0x8E); //3
        my_array[2] = (0x9E); //7
        my_array[3] = (0x8E); //3
        my_array[4] = (0xCA); //2 Bytes to read
        //Test Time!
      }
      else if (temp_source==1)
      {
        //Current Internal Temperature Sensor
        // Address = 0x0346
        // Bytes = 2
        my_array[0] = (0x82); //0
        my_array[1] = (0x8E); //3
        my_array[2] = (0x92); //4
        my_array[3] = (0x9A); //6
        my_array[4] = (0xCA); //2 Bytes to read
      }
      else
      {
        return(""); //error!
      }
      
      for (int ritenta=0; ritenta<10; ritenta++){
          dataStr="";
          inString=Leggi(my_array);
          //Serial.print("getTemp: ");
          //Serial.println(inString);
          if (inString.length()>5)
          {
            (inString.substring(4,6)).toCharArray(buff,3);
            dataStr = dataStr + (atoi(buff)-30);
            dataStr = dataStr + ".";
            dataStr = dataStr + inString.substring(2,4);  //estraggo le decine e unità della temperatura
            if (((inString[4]<='9' && inString[4]>='0')&&(inString[5]<='9' && inString[5]>='0')) && ((inString[2]<='9' && inString[2]>='0')&&(inString[3]<='9' && inString[3]>='0'))) 
            {
                break;
            }
            else 
            {
               dataStr="";
            }
         }
         else
         {
            delay(500);
         }
      }
      if (dataStr.length()>2)
      {
        //convert to f
        dataStr.toCharArray(cdata, dataStr.length()+1);
        float fdata=(atof(cdata)*1.800+32.000);
        if (fdata<1000){lung=7;}
        if (fdata<100){lung=6;}
        if (fdata<10){lung=5;}
        dtostrf(fdata,lung,3,cdata);
        dataStr=cdata;
      }
      else
      {
        dataStr="";
      }
      
      return(dataStr);
}

String WS2300_Serial::getHum(int dato){
      //'h' (internal), 'H' (external)
      //data=0-->internal ,1-->external
      String dataStr="";
      String inString;
      int my_array[6];

      if (dato==1)
      {
        my_array[0] = (0x82);
        my_array[1] = (0x8E);
        my_array[2] = (0xBE);
        my_array[3] = (0xAE);
        my_array[4] = (0xDA);
      }
      else
      {
        my_array[0] = (0x82);
        my_array[1] = (0x92);
        my_array[2] = (0x86);
        my_array[3] = (0xA6);
        my_array[4] = (0xDA);
      }
      for (int ritenta=0; ritenta<10; ritenta++)
      {
            dataStr="";
            inString=Leggi(my_array);
            if (inString.length()>5)
            {
               dataStr = dataStr + inString.substring(2,4);  //estraggo le decine e unità della temperatura
               if ((inString[2]<='9' && inString[2]>='0')&&(inString[3]<='9' && inString[3]>='0')) 
               {
                   break;
               }
               else
               {
                  dataStr="";
               }
           }
           else
           {
               delay(500);
           }
      }
      return(dataStr);
}

String WS2300_Serial::getPress(int dato){
      //''h' (hPa), 'H' (hg)
      //dato=1-->hPa, 0-->hg
      String dataStr="";
      String inString;
      int my_array[6];
      if (dato==1)
      {
        my_array[0] = (0x82);
        my_array[1] = (0x96);
        my_array[2] = (0xBA);
        my_array[3] = (0x8A);
        my_array[4] = (0xD6);
      }
      else
      {
        my_array[0] = (0x82);      //press hg
        my_array[1] = (0x96);
        my_array[2] = (0xB6);
        my_array[3] = (0xB6);
        my_array[4] = (0xD6);
      } 

      for (int ritenta=0; ritenta<10; ritenta++)
      {
            dataStr="";
            inString=Leggi(my_array);
            if (inString.length()>5)
            {
                dataStr = dataStr + inString.charAt(7);
                dataStr = dataStr + inString.charAt(4);
                dataStr = dataStr + inString.charAt(5);
                if (dato==1)
                {
                  dataStr = dataStr + inString.charAt(2);
                  dataStr = dataStr + ".";
                  dataStr = dataStr + inString.charAt(3);
                }
                else
                {
                  dataStr = dataStr + ".";
                  dataStr = dataStr + inString.charAt(2);
                  dataStr = dataStr + inString.charAt(3);
                }

                if ((inString[7]<='9' && inString[7]>='0')&&(inString[4]<='9' && inString[4]>='0') && (inString[5]<='9' && inString[5]>='0')&&(inString[2]<='9' && inString[2]>='0') && (inString[2]<='9' && inString[2]>='0')) 
                {
                  break;
                }
                else 
                {
                  dataStr="";
                }
           }
           else
           {
              delay(500);
           }
      }
      return(dataStr);
}

String WS2300_Serial::getWind(int dato){
      //'w' (wind spd), 'W' (wind dir)
      //dato=0-->Speed, 1-->Direction
      String dataStr="";
      char cdata [10];
      int lung=0;
      String inString;
      char buff[10];
      int my_array[] = {0x82,0x96,0x8A,0x9E,0xF2};
      for (int ritenta=0; ritenta<10; ritenta++)
      {
          dataStr="";
          inString=Leggi(my_array);
          //Serial.print("getWind(): ");
          //Serial.println(inString);
          if (inString.length()>5)
          {
            if (dato==0)
            {
               dataStr = dataStr + inString.charAt(7);
               buff[0]=inString[4];
               buff[1]=inString[5];
               int tmp=(strtol(buff,NULL,16));
               if (tmp<=9)
               {
                 dataStr = dataStr + "0";
               }

               dataStr = dataStr + tmp; 
               dataStr = dataStr + dataStr.charAt(2);
               dataStr.setCharAt(2, '.');
               if ((inString[4]<='9' && inString[4]>='0')&&(inString[5]<='9' && inString[5]>='0')) 
               {
                  break;
               }
               else
               {
                dataStr="";
               }
            }
            else
            {
               buff[0]=inString[6];
               buff[1]=0;
               int tmp=((strtol(buff,NULL,16))*22.5);
               dataStr = dataStr + tmp;
               if ((tmp>=0)&&(tmp<=360))
               {
                 break;
               }
               else 
               {
                 dataStr="";
                 ritenta=0;
               }
             }
         }
         else
         {
             delay(500);
         }
      }

      if (dato==0)    //se richiedo la velocità convertoin miglia orarie, convert to mph    
      {
           if (dataStr.length()>2)
           {
                  dataStr.toCharArray(cdata, (dataStr.length()+1));
                  float fdata=(atof(cdata)*2.23);
                  //Serial.print("fdata: ");
                  //Serial.println(fdata);
                  if (fdata<1000){lung=6;}
                  if (fdata<100){lung=5;}
                  if (fdata<10){lung=4;}
                  dtostrf(fdata,lung,2,cdata);
                  dataStr=cdata;
          }
          else
          {
            dataStr="";
          }
      }

      return(dataStr);
}

//TO DO!!!  Seems to work OK for 1Hr, seems to have an issue in the conversion to imperial for 24Hr...
String WS2300_Serial::getRain(int dato){
      //'r' = 1Hr, 'R' = 24Hr
      //data=1-->
      String dataStr="";
      char cdata [10];
      int lung=0;
      String inString;
      int my_array[6];
      if (dato==1)      //pioggia 1 ora
      {
        my_array[0] = (0x82);	//04B4 24h
        my_array[1] = (0x92);
        my_array[2] = (0xAE);
        my_array[3] = (0x92);
        my_array[4] = (0xDA);
      }
      else if(dato==2)
      {
        my_array[0] = (0x82);      //04D2 1h
        my_array[1] = (0x92);
        my_array[2] = (0xB6);
        my_array[3] = (0x8A);
        my_array[4] = (0xD6);
      }
	  else
      {
        my_array[0] = (0x82);      //0497 PluieT
        my_array[1] = (0x92);
        my_array[2] = (0xA6);
        my_array[3] = (0x9E);
        my_array[4] = (0xD6);
      }
      for (int ritenta=0; ritenta<10; ritenta++)
      {
            dataStr="";
            inString=Leggi(my_array);
            if (inString.length()>5)
            {
                dataStr = dataStr + inString.substring(6,8);
                dataStr = dataStr + inString.substring(4,6);
                if ((inString[6]<='9' && inString[6]>='0')&&(inString[7]<='9' && inString[7]>='0') && (inString[4]<='9' && inString[4]>='0')&&(inString[5]<='9' && inString[5]>='0') && (inString[2]<='9' && inString[2]>='0')&& (inString[3]<='9' && inString[3]>='0')) 
                {
                  break;
                }
                else
                {
                  dataStr="";
                }
           }
           else
           {
               delay(500);
           }
     }

     if (dataStr.length()>2)
     {  
       dataStr.toCharArray(cdata, (dataStr.length()+1));
       //Serial.print("cdata ");
       //Serial.println(cdata);
       float fdata=(atof(cdata)*2.54);  //in pollici
       if (fdata<1000){lung=6;}
       if (fdata<100){lung=5;}
       if (fdata<10){lung=4;}
       dtostrf(fdata,lung,2,cdata);
       dataStr=cdata;
      }
      else
      {
        dataStr="";
      }
  
      return(dataStr);
}

String WS2300_Serial::getDew(){
      //'e'
      String inString;
      String dataStr="";
      char cdata [10];
      int lung=0;
      char buff[10];

      //Serial.println("leggo pioggia 1h ");
      int my_array[] = {0x82,0x8E,0xB2,0xBA,0xFA};
      for (int ritenta=0; ritenta<10; ritenta++)
      {
            dataStr="";
            inString=Leggi(my_array);
            if (inString.length()>5)
            {            
                (inString.substring(4,6)).toCharArray(buff,3);
                dataStr = dataStr + ((atoi(buff))-30);
                dataStr = dataStr + ".";
                dataStr = dataStr + inString.substring(2,4);  //estraggo le decine e unità della temperatura
                if (((inString[4]<='9' && inString[4]>='0')&&(inString[5]<='9' && inString[5]>='0')) && ((inString[2]<='9' && inString[2]>='0')&&(inString[3]<='9' && inString[3]>='0'))) 
                {
                   break;
                }
                else 
                {
                  dataStr="";
                }
           }
           else
           {
              delay(500);
           }
    }

    if (dataStr.length()>2)
    {
        dataStr.toCharArray(cdata, dataStr.length()+1);
        float fdata=(atof(cdata)*1.800+32.000);
        if (fdata<1000){lung=7;}
        if (fdata<100){lung=6;}
        if (fdata<10){lung=5;}
        dtostrf(fdata,lung,3,cdata);

        dataStr=cdata;
    }
    else
    {
      dataStr="";
    }

    return(dataStr);
}

/********************************************************************
 * Reads data from the weather station 
 *
 * Input: 
 *   cmd[0] = Encoded MSB address nibble (nibble 4)
 *   cmd[1] = Encoded address nibble (nibble 3)
 *   cmd[2] = Encoded address nibble (nibble 2)
 *   cmd[3] = Encoded address nibble (nibble 1)
 *   cmd[4] = Encoded bytes to read
 * 
 * Output:  
 * 
 * Returns: String containing bytes read from weather station
 *
 ********************************************************************/
String WS2300_Serial::Leggi (int cmd[]) 
{
    //String dataString;
    char dataString[35];
    char tempChar [3];
    int incommingByte;      // a variable to read incoming serial data into
    byte checkByte;
    byte cmdByte;
    byte bytesToRead = ((cmd[4] - 0xC2) / 4);
    byte rxBytes [17];      // maximum bytes that can be read is [check bytes to read] + 15 + [check byte]
    int time=0;

    //reinit comms!
    incommingByte = _HardSerial.read();
    while (incommingByte!=2)
    {
      _HardSerial.write(byte(0x06)); //0x06 is the 'reset' byte!
      delay(30);  

      incommingByte = _HardSerial.read();

      time++;  //retries counter
      if (time>20)  
      {
        //too many tries to re-init comms - give up!
//        Serial.println("Leggi() - Too Many Comms Reinit'd Trys!");
        return("");  //error!  return ""
      }
    }  //end reinit comms!
    //Serial.println("Leggi() - Comms Reinit'd");
    
    //Clear Rx buffer...
    delay(30);  
    incommingByte = _HardSerial.read();
    while (incommingByte != -1)
    {
      incommingByte = _HardSerial.read();
    } 


    //Read data from WS23xx
    for(int i=0; i<5; i++)  //for each address byte to transmitt (bytes 0,1,2,3,4 of mem[])
    {
        //TX Address Nibble / Bytes to read value
        _HardSerial.write(byte(cmd[i]));  //print byte
//        Serial.print("Tx=0x");
//        Serial.println(cmd[i], HEX);
        
        //Wait for response...      
        time=0;
        while ((_HardSerial.available() == 0) && (time<20))
        {
         delay(30);
         time++;
        }

        //Timeout!  Give up!
        if (time>18)
        {
          return("");  //error! return ""
        }        
        
        incommingByte = _HardSerial.read();  //read byte!
//        Serial.print("Rx=0x");
//        Serial.println(incommingByte, HEX);
         
        //check OK (checkByte)
        if(i<4)
        {
            //Address byte, The address is coded as hexdigit*4+0x82. 
            cmdByte = ((cmd[i] & B00111100)/4);
          
            //Address check byte, WS2300 returns 0S, 1S, 2S and 3S where S is a check digit calculated as (command-0x82)/16. I.e. S is the hex digits of the address.
            checkByte = incommingByte & B00001111;

            //Check upper nibble, should match nibble of sent address!
            if (i !=((incommingByte & B11110000)/16)) 
            {
              //error!
//              Serial.println("Leggi() - checkbyte error (addr1)");
              return("");  //error! return ""
            }

            //Check, check byte, should match address nibble sent!  THIS DOESN'T SEEN TO WORK AS THE API WEB PAGE SUGGESTS!!
            if (cmdByte != checkByte) 
            {
              //error!
//              Serial.println(cmdByte);
//              Serial.println(checkByte);
//              Serial.println("Leggi() - checkbyte error (addr2)");
              return("");  //error! return ""
            }
      }
      else //(i==4)
      {
          //Bytes to read check byte - WS2300 returns 3X where X is the number of data bytes to follow (excluding the checksum byte). The first byte 3 is some fixed header. 
          checkByte = incommingByte & B00001111;
          
          if (bytesToRead != checkByte) 
          {
            //error!
//            Serial.println("Leggi() - checkbyte error (bytes to read)");
            return("");  //error! return ""
          } 
          else
          {
            bytesToRead++;
            rxBytes[0] = incommingByte;
          }    
        } //end checking
 
  } //for        
        
  //rx bytes!
  for(int i=1; i<=bytesToRead; i++)
  {
    time = 0;
    while ((_HardSerial.available() == 0) && (time<20))
    {
     delay(30);
     time++;
    }

    //Timeout!  Give up!
    if (time>18)
    {
//      Serial.println("Leggi() - timeout reading databytes");
      return("");  //error! return ""
    }        
    rxBytes[i] = _HardSerial.read();  //read byte!          

//    Serial.print("Rx=0x");
//    Serial.println(rxBytes[i], HEX);
  }
  
  //check bytes read - checksum is the single byte (ignore overflow) sum of the databytes rx'd. 
  checkByte=0;
  for(int i=1; i<bytesToRead; i++)
  {
    checkByte = checkByte + rxBytes[i];
  }
  if(checkByte != rxBytes[bytesToRead])
  {
    //error!
//    Serial.println("Leggi() - data checkByte error");
    return("");
  }    
//  Serial.println("Post checkByte");


  //clear string!
  for(int i=0; i<sizeof(dataString); i++)
  {
    dataString[i] = 0;
  }

  //make string!
  for(int i=0; i<=(bytesToRead); i++)
  {
   checkByte = ((byte)(rxBytes[i] >> 4));
   dataString[i*2] = (char)(checkByte > 9 ? checkByte + 0x37 : checkByte + 0x30);
   checkByte = ((byte)(rxBytes[i] & 0xF));
   dataString[(i*2)+1] = (char)(checkByte > 9 ? checkByte + 0x37 : checkByte + 0x30);
  
   //sprintf(tempChar, "%02X", rxBytes[i]);
   //dataString = dataString + (String)tempChar;
  }
  
//  Serial.print("Leggi() = ");
//  Serial.println(dataString);
  return(dataString);
}
