//        if((rec_stop =1) && rec==',')
//        {
//           rec_nxt = 1; count2=0;         
//        }
//
//        if((rec_nxt==1)&&rec=='1')
//        {
//          rec_nxt==0; //all_stop = 1;  
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Box opened");
//          Serial.println();
//          digitalWrite(44, HIGH); 
//        }
//
//        if((rec_nxt==1)&&rec=='0')
//        {
//          
//          rec_nxt==0; //all_stop = 1;
//          Serial.println();
//          digitalWrite(44, LOW);
//        }



//Service Added order block below//
          
        if((rec=='&') && (ser_start == 1))
        {
          //clean the global buffer variable  
          //copy contents of rec_str to global buffer variable
          //now we have global buf containing string of fmt !<order>,<status>&  
          //we need to slice the global buf now and compare the order with all 3 fed_bufs and empty the fed_buf accordingly and with status we need to open and close the box//
          Serial.println("Servicing Block reached");
          ser_start = 0;
          ser_stop = 1;
          count2 = 0;
          memset(&ser_str[0], '\0', sizeof(ser_str));
        }
        
        if((ser_start==1)&&(rec!=='&'))
        {
          ser_str[count2] = rec;
          count2++;
        }
        
        if((rec=='!') && (ser_start==0))
        {
          count2 = 0;
          memset(&ser_str[0], '\0', sizeof(ser_str));
          ser_start=1;  
        }
        
    }
