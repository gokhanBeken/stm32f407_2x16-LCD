/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: Gokhan Beken
   Last changed date:  $Date:  12.10.2014
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "lcd.h"

#define LED_HIGH            {GPIOD->ODR|= (1<<15);}
#define LED_LOW             {GPIOD->ODR&=~ (1<<15);}
#define LED_SET_OUTPUT      {GPIOD->MODER|= (1<< ((15*2)));  GPIOD->MODER&=~ (1<< ((15*2)+1));}

volatile short MyDelay;
void DelayMs(int ms);
void SysTick_Handler(void);
int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000); //1milisaniyelik system clock elde edelim



    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // GPIOD clock enable

    LED_SET_OUTPUT
    LED_HIGH
    DelayMs(100);
    LED_LOW
    DelayMs(100);
    LED_HIGH


    char s1[16]="M. Gokhan BEKEN\0";
    char s2[16]="GokhanBeken.com\0";

    lcd_init(); // LCD ilk ayarlar yapýlýyor


    while(1)
    {

        lcd_clear(); //LCD yi temizle
        lcd_gotoxy(1,1); //birinci satýrýn birinci sutununa geç
        lcd_yaz(s1);
        lcd_gotoxy(2,1); // ikinci satýrýn birinci sutununa geç
        lcd_yaz(s2);

        LED_LOW
        DelayMs(1000);
        LED_HIGH
        DelayMs(1000);
    }
}

void SysTick_Handler(void)
{
    if (MyDelay)
    {
        MyDelay--;
    }
}

void DelayMs(int ms)
{
    MyDelay=ms;
    while(MyDelay);
}
/*
Baglantilar:
C portunun 0-7 arasi pinlerini, LCD'nin D0-D7 arasi pinlerine bagla
Eger, 4 bitlik kullanacaksan, PC0-PC3 pinlerini baglamana gerek yok, PC4-PC7 pinleri D4-D7 pinlerine bagli olsun yeter

Sadece yazma yaptigimiz icin, RW pinini islemciye baglamana gerek yok, ama GND'ye cekmek gerekiyor.

v0 pinini GND'ye çekersen ekran ledi en parlak sekilde yanar.


*/


















