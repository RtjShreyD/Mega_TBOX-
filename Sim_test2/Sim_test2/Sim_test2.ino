#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

int start=0;

int x;
byte rowPins[ROWS] = {50,51,52,53}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {49,48,47}; //connect to the column pinouts of the keypad

char odr_id = "1234";
char entrd = 0, cnt = 0;
char rec_str[10], rec_data[10];
char rec_start = 0, rec_stop=0;

char fed_id[10] = {'0','0','0'};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
   pinMode(22, OUTPUT);
   pinMode(23, OUTPUT);
   pinMode(24, OUTPUT);
   Serial.begin(9600);
   
}  


 void loop()
  {
    char key = keypad.getKey();
    
    //string format *....string...#//
    if (key)
    {
      
      if((key=='#') && (rec_start = 1))
      {
        strcpy(rec_data, rec_str);
        rec_start= 0;
        cnt = 0;
        memset(&rec_str[0], 0, sizeof(rec_str));
        //rec_str[0] = '\0';
        Serial.println();
        //Serial_str(rec_data);
        Serial.println(rec_data);
        //All play with rec_data here //
        if(strcmp(fed_id,rec_data)==0)  //fed_id is the id already fed by user, currently hard coded
        {
          Serial.println("Authenticated");  
          //Here open the box or add shelf selection functionality//  
          Serial.println("Enter the choice of shelf(1,2,3)");
           }
        else
        {
          Serial.println("Order id incorrect");
        } 
        memset(&rec_data[0], 0, sizeof(rec_data));
        }
        
      if((rec_start==1)&&(key!='#'))
      {
        rec_str[cnt] = key;
        Serial.print('X');
        //Serial.print(rec_str[cnt);
        cnt++;
        }

      if((key=='*') && (rec_start==0))
      {
       cnt=0;
       memset(&rec_str[0], 0, sizeof(rec_str));
       rec_start=1;
       Serial.println("Enter order id: ");
      }
       
    }
   }
