#include <main.h>

#FUSES HS
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#use delay (clock=20000000) 

//!#define loa pin_C1
//!#define den pin_C2
void nhiet_do();
void khi_gas();
int16 temp, kt=0, nhietdo = 40;
int16 GiaTriADC;
float Tam;

void main()
{
   //khai bao bien cuc bo nay
//!   set_tris_c(0x00);
//!   set_tris_D(0x00);
   lcd_init();
   lcd_putc('\f');
   
   setup_adc(ADC_CLOCK_INTERNAL);  
   setup_adc_ports(AN0_AN1_AN3); 
   set_adc_channel(1);// doc tin hieu analog pin A1
   delay_us(10);

   while(TRUE)
   {
      
      GiaTriADC = read_adc();
      tam = (500f*(float)GiaTriADC)/1023f;
      temp = (int16)tam;
      
      set_adc_channel(1);
      
      if(input(pin_b0)==0)
      {
         while(input(pin_b0)==0);
         nhietdo += 1;
      }
      if(input(pin_b1)==0)
      {
         while(input(pin_b1)==0);
         nhietdo -=1;
      }
      nhiet_do();
      khi_gas();
      if(kt==0)
      {
         lcd_gotoxy(1,1);
         printf(lcd_putc,"NHIET DO = %02lu%C     ",temp,223);
         lcd_gotoxy(1,2);
         printf(lcd_putc,"CAI DAT = %02lu%C      ",nhietdo,223);
      }
           
   }
}

void khi_gas()
{
   if(input(pin_b2)==1)
   {      
      lcd_gotoxy(1,1);
      printf(lcd_putc,"PHAT HIEN     ");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"RO RI KHI GAS ");
      output_bit(pin_e0,0);//bat dèn
      output_bit(pin_e1,1);//loa
      delay_ms(200);
      output_bit(pin_e0,1);
      delay_ms(200);
      kt=1;
   }
   else kt=0;
}

void nhiet_do()
{
   if(temp>nhietdo)
   {
      kt=1;
      lcd_gotoxy(1,1); 
      printf(lcd_putc,"BAO DONG CO CHAY         ");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"  NHIET DO VUOT NG    ");
      Output_high(pin_e0);
      Output_high(pin_e1);
      delay_ms(200);
      output_low(pin_e0);
      delay_ms(200);
   }
   else
   {
      Output_low(pin_e1);
      kt=0;
   }
}


