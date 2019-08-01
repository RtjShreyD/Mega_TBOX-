#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
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
//int f0=0, f1=0, f2=0; //flag to tell whhich id is already used 
int f=0;

//for gsm recving variables and buffers//
char rec = 0;
char rec_start = 0, rec_stop = 0, count = 0;
char rec_str[10], rec_data[10];
///.............///

volatile char shf1 = 0; //flags for each shelf declared to denote further whether open or close
volatile int b1 = 0; //pushbutton state

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
   pinMode(44, OUTPUT);
   pinMode(45, OUTPUT);
   pinMode(46, OUTPUT);  
   pinMode(2, INPUT);
   Serial.begin(9600);
   //gsm_port.begin(9600); 
   // Turn on the transmission, reception, and Receive interrupt 
   Serial1.begin(9600);    

   //attachInterrupt(0, pin_ISR, CHANGE); //0 here defines pin 2 of Mega2560
   
}  

/*void pin_ISR() //ISR for when box is manually closed a latch gets closed and high value is recvd on pin 2(only pins 2,3 are GPIO interupt pin of Mega2560)//
{
   //while(1)
   //{
    Serial1.println('i'); 
    b1 = digitalRead(2);
    if(b1==HIGH)
    {
        digitalWrite(44, LOW);
        digitalWrite(45, LOW);
        digitalWrite(46, LOW);
         //Closing the box for delivery above and reseting its flag//Could be used 
        Serial.println("Shelf has been closed");
        //memset(&fed_id[0],'v' , sizeof(fed_id)); //changing contents of fed_id completely so that once used cannot be used again to open box
        //Serial.println(fed_id);
        //Serial1.print("f here goes as");
        //Serial1.println(f);
        
        switch(f)
        {
          case 1: //used fed_id Can be sent to logger from here//
                  Serial1.print("Used fed_id displayed below will be removed");
                  Serial1.println(fed_buf0);
                  memset(&fed_buf0,'v' , sizeof(fed_buf0)); //using v as char is unable to be input from keypad
                  f=0;
                  //break;

          case 2: Serial1.print("Used fed_id displayed below will be removed");
                  Serial1.println(fed_buf1);
                  memset(&fed_buf1,'v' , sizeof(fed_buf1));
                  f=0;
                 // break;
                  
          case3 : Serial1.print("Used fed_id displayed below will be removed");
                  Serial1.println(fed_buf2);
                  memset(&fed_buf2,'v' , sizeof(fed_buf2));
                  f=0;
                  //break;
        }       
    } 
}*/
//}

void  serialEvent1()  //Serial Rx ISR
{
    while (Serial1.available()) 
    {
      rec = Serial1.read();
//      Serial1.println(rec);
//      digitalWrite(44, HIGH); 

        if((rec=='@') && (rec_start = 1))
        {
            memset(&fed_buf0[0], '\0', sizeof(fed_buf0));      //formatting a fed_id buffer here
            strcpy(fed_buf0, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1;
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
            ///All play here with rec_data
            Serial1.println();
            //Serial1.println(rec_data);
            Serial1.println(fed_buf0);
            
        }

        if((rec=='#') && (rec_start = 1))
        {
            memset(&fed_buf1[0], '\0', sizeof(fed_buf1));      //formatting a fed_id buffer here
            strcpy(fed_buf1, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1;
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
            ///All play here with rec_data
            Serial1.println();
            //Serial1.println(rec_data);
            Serial1.println(fed_buf1);
            
        }

        if((rec=='%') && (rec_start = 1))
        {
            memset(&fed_buf2[0], '\0', sizeof(fed_buf2));      //formatting a fed_id buffer here
            strcpy(fed_buf2, rec_str); //copy the contents in buffer
            rec_start= 0;
            rec_stop = 1;
            count = 0;
            memset(&rec_str[0], '\0', sizeof(rec_str)); 
            ///All play here with rec_data
            Serial1.println();
            //Serial1.println(rec_data);
            Serial1.println(fed_buf2);
            
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

void func()
{
  //Serial1.println('i'); 
  
    //b1 = digitalRead(2);
    Serial.println('b1');
    if(digitalRead(2))
    {
        //while(b1==HIGH);
        //Serial.println(b1);
        digitalWrite(44, LOW);
        digitalWrite(45, LOW);
        digitalWrite(46, LOW);
         //Closing the box for delivery above and reseting its flag//Could be used 
        Serial.println("Shelf has been closed");
        //b1 = '\0';
        //memset(&fed_id[0],'v' , sizeof(fed_id)); //changing contents of fed_id completely so that once used cannot be used again to open box
        //Serial.println(fed_id);
        //Serial1.print("f here goes as");
        //Serial1.println(f);
        
        switch(f)
        {
          case 1: //used fed_id Can be sent to logger from here//
                  Serial1.print("Used fed_id displayed below will be removed");
                  Serial1.println(fed_buf0);
                  memset(&fed_buf0,'v' , sizeof(fed_buf0)); //using v as char is unable to be input from keypad
                  f=0;c = 1;
                  break;

          case 2: Serial1.print("Used fed_id displayed below will be removed");
                  Serial1.println(fed_buf1);
                  memset(&fed_buf1,'v' , sizeof(fed_buf1));
                  f=0;c = 1;
                  break;
                  
          case3 : Serial1.print("Used fed_id displayed below will be removed");
                  Serial1.println(fed_buf2);
                  memset(&fed_buf2,'v' , sizeof(fed_buf2));
                  f=0;c = 1;
                  break;
        
        }      
    } 
    /*else
    {
      Serial.println("Already closed");
      //c=1;
    } */ 
    //else{break;}
}



void loop()
{
    char key = keypad.getKey();    
    //string format *....string...#//
    
    //c=1;
    
    
    if (key)
    {
        if((key=='#') && (key_start = 1))
        {
            strcpy(key_data, key_str);
            key_start= 0;
            key_stop = 1;
            cnt = 0;
            memset(&key_str[0], '\0', sizeof(key_str)); //Making all contents of key_str array as null
            Serial.println();
            //Serial_str(key_data);
            Serial.println(key_data);
            //Flagging to Check whether Id is used or being input the first time     
            //All play with key_data here //
            
            if(strcmp(fed_buf0,key_data)==0){f = 1;}  
            if(strcmp(fed_buf1,key_data)==0){f = 2;}
            if(strcmp(fed_buf2,key_data)==0){f = 3;}
            
            //if(((strcmp(fed_buf0,key_data)==0)|(strcmp(fed_buf1,key_data)==0))&&(key_stop==1))  //fed_id is the id already fed by user, currently hard coded ...add more | as required 
            if((f==1|f==2|f==3) && (key_stop==1))
            {    
              Serial.println(f);
              Serial.println("Authenticated");   
              Serial.println("Select Shelf");
              //Serial.println("Enter choice 1 or 2 or 3");
              auth_flag = 1;  
            }
            else
            {
              Serial.println("Order id incorrect");
            } 
            memset(&key_data[0], '\0', sizeof(key_data));

            
         }
          
        if((key_start==1)&&(key!='#'))
        {
           key_str[cnt] = key;
           Serial.print('X');
            //Serial.print(rec_str[cnt);
           cnt++;
        }
  
        if((key=='*') && (key_start==0))
        {
           cnt=0;
           memset(&key_str[0], '\0', sizeof(key_str));
           key_start=1;
           Serial.println("Enter order id: ");
           auth_flag = 0;
                     
        }      
        
        if((auth_flag==1)) //To process after authentication//
        {
            Serial.println("Press 1 to open");
            if((key=='1'))//&&c==1)
            {
              digitalWrite(44, HIGH);
              //put fed_buf used in used
              Serial.println("Opening Box for delivery");
           
              //c=0;
              //while((c==0)&&(f!=0))
              //{
                func();
                //break;
                //}
            }
               
        }   
    }//closing for if(key)
   
}//closing for void loop()



 
