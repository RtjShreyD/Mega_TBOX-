#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
const int rs = 22, rw = 23, en = 24, d4 = 26, d5 = 27, d6 = 28, d7 = 29;
LiquidCrystal lcd(rs, rw, en, d4, d5, d6, d7);

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {50,51,52,53}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {49,48,47}; //connect to the column pinouts of the keypad

char cnt = 0, auth_flag = 0; //counter and authentication flag defined
char key_str[10], key_data[10]; //key_str buffer array will recv each key and store it, key_data will finally copy the array and be used furthr
char key_start = 0, key_stop=0;
char c;
char fed_id[10] ; //Hard coded currently, will be updated by user Using Application. Can be handled through UART Interupt.
char fed_buf0[10]= {'0','0','0'};
char fed_buf1[10]= {'1','1','1'};
char fed_buf2[10]= {'2','2','2'};
//int f0=0, f1=0, f2=0; //flag to tell which id is already used 
int f=0;

//for serial recving variables and buffers// 
char rec = 0;
char rec_start = 0, rec_stop = 0, count = 0, count2 = 0, rec_nxt =0, stop_all = 0;
char rec_str[10], rec_data[10], rec_status[3];
///.............////

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
   pinMode(44, OUTPUT);
   pinMode(45, OUTPUT);
   pinMode(46, OUTPUT);  
   pinMode(2, INPUT);
   lcd.begin(20, 4);
   lcd.clear();
   lcd.setCursor(0, 0); 
   lcd.print("Welcome ParcelBox");
   Serial.begin(9600);
   Serial1.begin(9600);    

   attachInterrupt(digitalPinToInterrupt(2),pin_ISR, RISING);   
}  

void pin_ISR() //ISR for when box is manually closed a latch gets closed and high value is recvd on pin 2(only pins 2,3 are GPIO interupt pin of Mega2560)//
{
        digitalWrite(44, LOW);         //Closing the box for delivery above and reseting its flag//Could be used 
        lcd.clear();
        lcd.setCursor(0, 0) ;
        lcd.print("Locking Box");
        Serial.println("OK");
        delay(20000); //to be adjusted as per hardware caliberation
        lcd.clear();
        lcd.setCursor(0, 0); 
        lcd.print("Delivery success");
        delay(50000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome ParcelBox"); ///From here it has to get to the initial state of Welcome ParcelBox
        
        switch(f)
        {
          case 1: memset(&fed_buf0,'v' , sizeof(fed_buf0)); //using v as char is unable to be input from keypad
                  f=0;
                  break;

          case 2: memset(&fed_buf1,'v' , sizeof(fed_buf1));
                  f=0;
                  break;
                  
          case 3 :memset(&fed_buf2,'v' , sizeof(fed_buf2));
                  f=0;
                  break;
        }       
    
}

//
void  serialEvent1()  //Serial Rx ISR
{
    while (Serial1.available()) 
    {
      rec = Serial1.read();
      
        if((rec=='@') && (rec_start = 1))
        {
            memset(&fed_buf0[0], '\0', sizeof(fed_buf0));      //formatting a fed_id buffer here
            strcpy(fed_buf0, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1; 
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
        }

        if((rec=='#') && (rec_start = 1))
        {
            memset(&fed_buf1[0], '\0', sizeof(fed_buf1));      //formatting a fed_id buffer here
            strcpy(fed_buf1, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1; 
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
        }

        if((rec=='%') && (rec_start = 1))
        {
            memset(&fed_buf2[0], '\0', sizeof(fed_buf2));      //formatting a fed_id buffer here
            strcpy(fed_buf2, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1; 
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
        }

        if((rec_stop =1) && rec==',')
        {
           rec_nxt = 1; count2=0;         
        }

        if((rec_nxt==1)&&rec=='1')
        {
          rec_nxt==0; //all_stop = 1;  
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Box opened");
          Serial.println();
          digitalWrite(44, HIGH); 
        }

        if((rec_nxt==1)&&rec=='0')
        {
          
          rec_nxt==0; //all_stop = 1;
          Serial.println();
          digitalWrite(44, LOW);
        }
        
        if((rec_start==1)&&(rec!='@'))
        {
           rec_str[count] = rec;
           count++;
        }       
        
        if((rec=='$') && (rec_start==0))
        {
           count=0;
           memset(&rec_str[0], '\0', sizeof(rec_str));
           rec_start=1;         
        }
        
    }
  
}

void loop()
{
    char key = keypad.getKey();    
    //string format *....string...#//   
    
    if (key)
    {
        if((key=='#') && (key_start = 1))
        {
            strcpy(key_data, key_str);
            key_start= 0;
            key_stop = 1;
            cnt = 0;
            memset(&key_str[0], '\0', sizeof(key_str)); //Making all contents of key_str array as null
                       
            if(strcmp(fed_buf0,key_data)==0){f = 1;}  
            if(strcmp(fed_buf1,key_data)==0){f = 2;}
            if(strcmp(fed_buf2,key_data)==0){f = 3;}
             
            if((f==1|f==2|f==3) && (key_stop==1))
            {    
              lcd.clear(); //clear screen
              lcd.setCursor(0,0);
              lcd.print("Authenticated");             
              auth_flag = 1;  
            }
            else
            {
              lcd.clear(); //clear screen
              lcd.setCursor(0,0);
              lcd.print("Order Id Incorrect");
            } 
            memset(&key_data[0], '\0', sizeof(key_data));

            
         }
          
        if((key_start==1)&&(key!='#'))
        {
           key_str[cnt] = key;          
           lcd.setCursor(cnt,1);
           lcd.print(key_str[cnt]);
           cnt++;
        }
  
        if((key=='*') && (key_start==0))
        {
           cnt=0;
           memset(&key_str[0], '\0', sizeof(key_str));
           key_start=1;
           lcd.setCursor(0,0);
           lcd.print("Enter order id: ");
           auth_flag = 0;
                     
        }      
        
        if((auth_flag==1)) //To process after authentication//
        {
            lcd.setCursor(0,1);
            lcd.print("Press 1 to open box"); 
            if((key=='1'))//&&c==1)
            {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Box opened");
              digitalWrite(44, HIGH);        
            }
               
        }   
    }//closing for if(key)
   
}//closing for void loop()
