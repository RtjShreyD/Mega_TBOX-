#include<avr/io.h>
#include<util/delay.h>

#define RS PA0
#define RW PA1
#define EN PA2
#define LCD PORTA

void lcd_command(unsigned char cmd)
{
  LCD = (cmd & 0xf0);
  LCD &= ~(1<<RS);
  LCD &= ~(1<<RW);
  LCD |= (1<<EN);
  _delay_ms(1);
  LCD &= ~(1<<EN);
  //_delay_ms(1);//

  LCD =((cmd<<4) & 0xf0);
  LCD &= ~(1<<RS);
  LCD &= ~(1<<RW);
  LCD |=(1<<EN);
  _delay_ms(1);
  LCD &= ~(1<<EN);
  //_delay_ms(1);//
}


void lcd_data(unsigned char data)
{
  LCD = (data & 0xf0);
  LCD |=(1<<RS);
  LCD &= ~(1<<RW);
  LCD |=(1<<EN);
  _delay_ms(1);
  LCD &= ~(1<<EN);
  //_delay_ms(1);//
  LCD = (data<<4) & 0xf0;
  LCD |=(1<<RS);
  LCD &=~(1<<RW);
  LCD |=(1<<EN);
  _delay_ms(1);
  LCD &= ~(1<<EN);
  //_delay_ms(1);//
}


void lcd_init()
{
  lcd_command(0x02);
  lcd_command(0x28);
  lcd_command(0x06);
  lcd_command(0x0c);
}

void lcd_string(unsigned char *str)
{
  char i=0;
  while(str[i]!='\0')
   {
     lcd_data(str[i]);
     i++;
   }
}

void lcd_number(unsigned int n)
{
  unsigned int d=0;
  lcd_command(0x04);
  if(n==0)
  {
    lcd_data(48);
  }
  while(n>0)
  {
    d=n%10;
    lcd_data(d+48);
    n=n/10;
  }
  lcd_command(0x06);
}

void setup()
{
  DDRA=0xFF;
  lcd_init();
}

void loop()
{
   lcd_command(0x8F);
   lcd_number(12); 
   _delay_ms(500);
   lcd_command(0xc0);
   lcd_string("Shreyansh");
   
}




