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

char fed_buf0[10]= {'0','0','0'};
char fed_buf1[10]= {'1','1','1'};
char fed_buf2[10]= {'2','2','2'}; 
int f=0, pointer=0;  //f lis flag bit for adding new, pointer is flag bit for checking which fed_buf the order belongs while servicing 

//for serial recving variables and buffers// 
char rec = 0;
char rec_start = 0, rec_stop = 0, count = 0;
char ser_start = 0, ser_stop = 0, count2 = 0;
char rec_str[10], rec_data[10];
char ser_str[15], ser_data[15], sub_str[30]; //ser_data will have data between '!' and '&' 
char odr_cmp[5], stats = '0';   //odr_cmp will be order id sliced from ser_data, stats will be status of the box sliced from ser_data
///.............////

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Custom functions block//

void str_slice(char string[], int pos, int len)  //to slice a string(actually a Char Array) from one position to certain length
                                                 //pos starts from 1 and len is no of chars from pos including pos itself
{
  int c = 0;
  while (c < len) 
  {
      sub_str[c] = string[pos+c-1];
      c++;
    }
   sub_str[c] = '\0';
}

void nullify_arr(char arr[])
{
  memset(&arr[0], '\0', sizeof(arr));   
}

//Custom functions block end//

void setup()
{

   pinMode(44, OUTPUT); //lock pin
   pinMode(45, OUTPUT);
   pinMode(46, OUTPUT);  
   pinMode(2, INPUT);
   pinMode(3, OUTPUT);
   lcd.begin(20, 4);
   lcd.clear();
   lcd.setCursor(0, 0); 
   lcd.print("Welcome ParcelBox");
   Serial.begin(9600);
   Serial1.begin(9600);    
   Serial.println("Welcome Parcebox");

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
       

        if((f==1)|(pointer==1))
        {
          nullify_arr(fed_buf0);
          f=0; pointer=0;  
        }

        if((f==2)|(pointer==2))
        {
          nullify_arr(fed_buf1);
          f=0; pointer=0;  
        }

        if((f==2)|(pointer==2))
        {
          nullify_arr(fed_buf2);
          f=0; pointer=0;  
        }
           
        lcd.print("Welcome ParcelBox"); ///From here it has to get to the initial state of Welcome ParcelBox
}

//Adding new order serial event //

void  serialEvent1()  //Serial Rx ISR
{
    while (Serial1.available()) 
    {
      rec = Serial1.read();
      Serial.print(rec);
        //Add new order block below//
        
        if((rec == '@') && (rec_start == 1))
        {
            memset(&fed_buf0[0], '\0', sizeof(fed_buf0));      //formatting a fed_id buffer here
            strcpy(fed_buf0, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1; 
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
        }

        if((rec == '#') && (rec_start == 1))
        {
            memset(&fed_buf1[0], '\0', sizeof(fed_buf1));      //formatting a fed_id buffer here
            strcpy(fed_buf1, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1; 
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
        }

        if((rec == '%') && (rec_start == 1))
        {
            memset(&fed_buf2[0], '\0', sizeof(fed_buf2));      //formatting a fed_id buffer here
            strcpy(fed_buf2, rec_str); //copy the contents in buffer
            rec_start = 0;
            rec_stop = 1; 
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
        }

        if((rec_start == 1)&&(rec != '@'))
        {
           rec_str[count] = rec;
           count++;
        }       
        
        if((rec == '$') && (rec_start == 0))
        {
           count=0;
           memset(&rec_str[0], '\0', sizeof(rec_str));
           rec_start=1;         
        }

        //Add new order block ends //

        //Service Added order block below//
          
        if((rec == '&') && (ser_start == 1))
        {
          strcpy(ser_data, ser_str);
          str_slice(ser_data, 1, 4);
          strcpy(odr_cmp, sub_str); //odr_cmp now contains my order id which needs to be compared with fed_bufs
          nullify_arr(sub_str);

          str_slice(ser_data, 6, 6);

          if(strcmp(fed_buf0,odr_cmp)==0){pointer = 1;}  
          if(strcmp(fed_buf1,odr_cmp)==0){pointer = 2;}
          if(strcmp(fed_buf2,odr_cmp)==0){pointer = 3;}
          
          if((sub_str[0]=='1')&&(strlen(sub_str) == 1)&&((pointer==1)|(pointer==2)|(pointer==3)))
          { 
            stats = 'o';
            nullify_arr(sub_str);  
          }
          if((sub_str[0]=='0')&&(strlen(sub_str) == 1)&&((pointer==1)|(pointer==2)|(pointer==3)))
          {
            stats = 'c'; 
            nullify_arr(sub_str); 
          }
                                              
          ser_start = 0;
          ser_stop = 1;
          count2 = 0;
          memset(&ser_str[0], '\0', sizeof(ser_str));
        }
        
        if((ser_start == 1)&&(rec != '&'))
        {
          ser_str[count2] = rec;
          count2++;
        }
        
        if((rec == '!') && (ser_start == 0))
        {
          count2 = 0;
          memset(&ser_str[0], '\0', sizeof(ser_str));
          ser_start=1;  
        }

        //Service Added order block ends//
        
  }
}

//Adding new order serial event end//
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
              f = 0;            
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
           stats = '\0';
                     
        }      
        
        if((auth_flag==1)) //To process after authentication//
        {
            lcd.setCursor(0,1);
            lcd.print("Press 1 to open box"); 
            if((key=='1'))
            {
                stats = 'o'; 
                auth_flag = 0;      
            }
               
        }   
    }//closing for if(key)

    if (stats=='o') 
    {      
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Box opened");
          digitalWrite(44, HIGH); 
          stats = '\0'; 
    }

//    if (stats=='c') 
//    { 
////        digitalWrite(44, LOW);         //Closing the box for delivery above and reseting its flag//Could be used 
////        lcd.clear();
////        lcd.setCursor(0, 0) ;
////        lcd.print("Locking Box");
////        Serial.println("OK");
////        delay(20000); //to be adjusted as per hardware caliberation
////        lcd.clear();
////        lcd.setCursor(0, 0); 
////        lcd.print("Delivery success");
////        delay(50000);
////        lcd.clear();
////        lcd.setCursor(0, 0);
//        stats = '\0'; 
//        digitalWrite(3, HIGH);
//    }
    
   
}//closing for void loop()



///////////So the leftover Glitch is after stats is set 0 from APP, the box gets locked forever untill it is reset///////////
