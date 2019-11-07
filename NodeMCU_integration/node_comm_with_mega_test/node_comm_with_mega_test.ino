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
char sub_str[15], box_id[5], odr_id[5], charBuf[10];
String nx = "";

//******** Global Variables block end ********//

//Custom functions block//
//char *concat(const char *s1, const char *s2) //function to concatenate two strings 
//{
//    const size_t len1 = strlen(s1);
//    const size_t len2 = strlen(s2);
//    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
//    if(result==NULL)
//  {
//    //uart_TXstring("Fatal: Memory not allocated");
//    exit(1);
//  }
//    memcpy(result, s1, len1);
//    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
//    return result;
//}

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

    String path = "/boxes/Y3FRSKfkVcYAPygUD9cUP2762HY2";

    String Data = "";
    

    Serial.println("------------------------------------");
    Serial.println("Get JSON test...");

    if (Firebase.getString(firebaseData, path))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        //Serial.print("VALUE: ");
        if (firebaseData.dataType() == "int")
            Serial.println(firebaseData.intData());
        else if (firebaseData.dataType() == "float")
            Serial.println(firebaseData.floatData(), 5);
        else if (firebaseData.dataType() == "double")
            printf("%.9lf\n", firebaseData.doubleData());
        else if (firebaseData.dataType() == "boolean")
            Serial.println(firebaseData.boolData() == 1 ? "true" : "false");
        else if (firebaseData.dataType() == "string")
            Serial.println(firebaseData.stringData());
        else if (firebaseData.dataType() == "json"){
           Serial.println(firebaseData.jsonData());
        }

        //Data = firebaseData.stringData(); //store for next test
        //Serial.println(Data);
        
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
    
} 

void loop() 
{ 
    
}          
