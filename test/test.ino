// https://lang-ship.com/reference/Arduino/libraries/Firebase_ESP8266_Client/ 
// https://firebase-arduino.readthedocs.io/en/latest/             Docs for this library

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

// Set these to run example.
#define FIREBASE_HOST "mybackendnodemcu.firebaseio.com"
#define FIREBASE_AUTH "7K7RcmMB1ad4nK1I8rUj6AfafL2NYP0yVFHb80gR"

#define WIFI_SSID "Morphedo -2.4GHz"
#define WIFI_PASSWORD "82$morph"

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

    String path = "/boxes/Json";

    String jsonData = "";
    

    Serial.println("------------------------------------");
    Serial.println("Get JSON test...");

    if (Firebase.getJSON(firebaseData, path))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        Serial.print("VALUE: ");
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
           
           jsonData = firebaseData.jsonData(); //store for next test
           Serial.println(firebaseData.jsonData());

        }
            
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

    FirebaseJsonObject jsonParseResult;


     Serial.println("------------------------------------");
     Serial.println("Iterate all get back json data...");

      json2.clear().setJsonData(jsonData);
      json2.parse();
      size_t count =json2.getJsonObjectIteratorCount();
      String key;
      String value;

      for (size_t i = 0; i < count; i++)
      {
        
        json2.jsonObjectiterator(i,key,value);
        jsonParseResult = json2.parseResult();

        Serial.print("KEY: ");
        Serial.print(key);
        Serial.print(", ");
        Serial.print("VALUE: ");
        Serial.print(value); 
        Serial.print(", ");
        Serial.print("TYPE: ");
        Serial.println(jsonParseResult.type);        

      }

     Serial.println("------------------------------------");
     Serial.println();


    Serial.println("------------------------------------");
    Serial.println("Parse for some child node...");

    
    json2.parse().get("anotherData").get("myVal");    

    jsonParseResult = json2.parseResult();

    if(jsonParseResult.success){
     
      Serial.println("1. Parse json data result for node /anotherData/myVal");
      Serial.print("TYPE: ");
      Serial.println(jsonParseResult.type);
      Serial.print("VALUE: ");
      Serial.println(jsonParseResult.stringValue);
      //Serial.println(jsonParseResult.doubleValue);

    }else{
      Serial.println("1. Parse json data for node /anotherData/myVal was failed!");
    } 

    //Set parse with false to get data through all children nodes, 
    //otherwise this would be failed to get child node data because of it's under data2 node.
    json2.parse(false).get("simpleData"); 


    jsonParseResult = json2.parseResult();

    Serial.println();

    if(jsonParseResult.success){
     
      Serial.println("2. Parse json data result for node /data2/simpleData with skipChild set to false");
      Serial.print("TYPE: ");
      Serial.println(jsonParseResult.type);
      Serial.print("VALUE: ");
      Serial.println(jsonParseResult.stringValue);
      //Serial.println(jsonParseResult.doubleValue);

    }else{
      Serial.println("2. Parse json data for node /data2/simpleData was failed!");
    }    



    //Any json object inside array also can be accessed by set parse(false),
    //The next test will ignore it and the result was failed as expected.
    json2.parse().get("test2"); 


    jsonParseResult = json2.parseResult();

    Serial.println();

    if(jsonParseResult.success){
     
      Serial.println("3. Parse json data result for node /myArray/test2 with skipChild set to true, or ignored");
      Serial.print("TYPE: ");
      Serial.println(jsonParseResult.type);
      Serial.print("VALUE: ");
      Serial.println(jsonParseResult.stringValue);
      //Serial.println(jsonParseResult.doubleValue);

    }else{
      Serial.println("3. Parse json data for node /myArray/test2 was failed as expected!");
    }     
        
     Serial.println("------------------------------------");
     Serial.println();


}

void loop()
{
}

  
