
char rec=0, rec_start=0;
char rec_str[10];
char fed_buf0[10];
signed int count = 0;
String number = "+918448935901";

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  //Begin serial communication with Arduino and SIM800L GSM module
  Serial1.begin(9600);
  configure();
}

void send_msg()
{
  //Serial.println ("Sending Message");
  Serial1.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial1.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "Hello, how are you?";
  Serial1.println(SMS);
  delay(100);
  Serial1.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void configure()
{
  Serial.println("Initializing..."); 
  delay(1000); 
  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
  response();
  Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
  response();
  Serial1.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  response();
}

void response()
{
  delay(500);
  while (Serial1.available()) 
  {
    Serial.write(Serial1.read());//Forward what GSM(Serial1) rec on Serial Monitor to debug
  }
}

void loop()
{
  while (Serial1.available()) 
    {
      rec = Serial1.read();
      
        if((rec=='@') && (rec_start = 1))
        {
            
            strcpy(fed_buf0, rec_str); //copy the contents in buffer
            rec_start= 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
            Serial.println();
            Serial.println(fed_buf0); // show what you recieve
            send_msg();
            
        }
               
        if((rec_start==1)&&(rec!='@'))
        {
           rec_str[count] = rec;
           //Serial.println(rec);
           count++;
        }       
        
        if((rec=='$') && (rec_start==0))
        {
           count=0;
           memset(&rec_str[0], '\0', sizeof(rec_str));
           rec_start=1;
           //Serial1.println("recieving with storing started");          
        }
        
    }
  
  
  }
