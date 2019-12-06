#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include<stdio.h>
#include<string.h>
#include<stdio.h>

#define FIREBASE_HOST "https://trondev-ad771.firebaseio.com" 
#define FIREBASE_AUTH "U0zEMfLToxY7ZO36Wfk9i9bx3KiLHPLIuv60p2CD"  //tron-dev account

#define WIFI_SSID "Morphedo -2.4GHz"
#define WIFI_PASSWORD "82$morph"

//Global Variables block//
char sub_str[15], DataBuf[20], DataBuf2[10], new_odr[5] ;
char OldData[] = "XXXXXXXXXXXXXXXXXXX";
char OldData2[] ="XXXXXXXXXXXXXXXXXXX";
int count = 0;
String Data = "";
String Data2 = "";
int len=0;
int len2 = 0;

//String path = "/boxes/Y3FRSKfkVcYAPygUD9cUP2762HY2";
  String path = "/user/aXoBuRpONZdeIWvsrhZjNLNk8953/boxStatus" ;
  String path2 = "/user/aXoBuRpONZdeIWvsrhZjNLNk8953/NewOrderId" ;

//******** Global Variables block end ********//

//Custom functions block//

void str_slice(char string[], int pos, int len)  //to slice a string from in betwn//pos starts from 1 and len is no of chars from pos including pos itself
{
  int c = 0;
  while (c < len) 
  {
      sub_str[c] = string[pos+c-1]; 
      c++; 
    }
   sub_str[c] = '\0';
}
//*******Custom functions block end********//

FirebaseData firebaseData;

void setup() 
{
    Serial.begin(9600);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    //Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
//        Serial.print(".");
        delay(300);
    }
//    Serial.println();
//    Serial.print("Connected with IP: ");
//    Serial.println(WiFi.localIP());
//    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

//    Serial.println("------------------------------------");
//    Serial.println("Get JSON test...");
 }

void loop() 
{ 
  //Service Order
  if (Firebase.getString(firebaseData, path))
    {

        Data = firebaseData.stringData();
        len=sizeof(Data);
        Data.toCharArray(DataBuf, len); 
   
  
        if(!(strcmp(DataBuf, OldData)==0)) //not identical
          {    
            Serial.print('!');
            Serial.print(DataBuf);
            Serial.println('&');
            memset(&OldData, '\0' , sizeof(OldData));
            strcpy(OldData, DataBuf);
          }
       
       else
          {

            memset(&Data, '\0' , sizeof(Data));
            memset(&DataBuf, '\0' , sizeof(DataBuf));
          }
     
     }
    /// New order

    if (Firebase.getString(firebaseData, path2))
    {

        Data2 = firebaseData.stringData();
        len2=sizeof(Data2);
        Data2.toCharArray(DataBuf2, len2); 
   
  
        if(!(strcmp(DataBuf2, OldData2)==0)) //not identical
          {    
            Serial.print('$');
            Serial.println(DataBuf2);
            
            memset(&OldData2, '\0' , sizeof(OldData2));
            strcpy(OldData2, DataBuf2);
          }
       
       else
          {
            memset(&Data2, '\0' , sizeof(Data2));
            memset(&DataBuf2, '\0' , sizeof(DataBuf2));
            
          }
     
     }

    ///   
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
   
  delay(1000); 
  
}                   
