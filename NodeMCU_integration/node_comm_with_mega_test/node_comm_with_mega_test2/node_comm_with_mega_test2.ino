#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include<stdio.h>
#include<string.h>
#include<stdio.h>

//#define FIREBASE_HOST "restsvr.firebaseio.com" //firebase end point
//#define FIREBASE_AUTH "DAyn8zLBXmBecOYEZue81VirzEd3upyRukT23ghG" //secret key for authentication

#define FIREBASE_HOST "https://trondev-bf192.firebaseio.com" 
#define FIREBASE_AUTH "LptG0b3wMUa1Wr2mmP7ThssGIjJsG2Pius8GsHDl"  //tron-dev account

//#define WIFI_SSID "Tech_D3882400"  
//#define WIFI_PASSWORD "TAQAHYWG"

#define WIFI_SSID "Morphedo -2.4GHz"
#define WIFI_PASSWORD "82$morph"

//Global Variables block//
char sub_str[15], box_id[5], odr_id[5], charBuf[10], DataBuf[10];
char OldData[] = "XXXXXXXXX";
String nx = "";
int count = 0;
String Data = "";
int len=0;

//String path = "/boxes/Y3FRSKfkVcYAPygUD9cUP2762HY2";
  String path = "/user/jMR5hg8XH0a16mg1MtS34R0GH9y1/boxStatus" ;

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
    Serial.println();
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    Serial.println("------------------------------------");
    Serial.println("Get JSON test...");
 }

void loop() 
{ 
  if (Firebase.getString(firebaseData, path))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        //Serial.print("VALUE: ");

        //Data = Firebase.getString(firebaseData, path);
        Data = firebaseData.stringData();
//        Serial.print("Lets see Data");
//        Serial.println(Data);
        len=sizeof(Data);
        Data.toCharArray(DataBuf, len);
//        Serial.print("now see Old Data: ");
//        Serial.println(OldData);
//        Serial.print("now see DataBuf: ");
//        Serial.println(DataBuf);
////      Serial.println("then we have");
  
   
  
        if(!(strcmp(DataBuf, OldData)==0)) //not identical
          {    
            Serial.println("In if");
            Serial.println(DataBuf);
            memset(&OldData, '\0' , sizeof(OldData));
//            Serial.println("Old data after memset is " );
//            Serial.println(OldData);
            strcpy(OldData, DataBuf);
          }
       
       else
          {
            Serial.println("In else"); 
            memset(&Data, '\0' , sizeof(Data));
            memset(&DataBuf, '\0' , sizeof(DataBuf));
//            Serial.println("Data and DataBuf cleared");
//            Serial.println(Data);
//            Serial.println(DataBuf);
          }
     
     }
       
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
   
  delay(1000); 
  
}                   
