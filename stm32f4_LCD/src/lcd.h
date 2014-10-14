/*
* B4,B5,B6,B7 pinlerini kullanarak 4 bit iletisim kullanir.
* Cursor kapalidir.
* RW kullanilmadigi için direk topraga baglanabilir.
*
* 2x16 LCD Kullanim Klavuzu
* lcd_init(); ile LCD'nin ilk ayarlarini yap
* lcd_clear(); ile LCD'yi sil
* lcd_yaz("deneme"); seklinde yazi yazdir.
* veri_yolla('F'); seklinde tek ascii kodu yazdir.
* lcd_gotoxy(1,13); seklinde LCD'nin istenilen yerine git.
*/

#include "stm32f4xx.h"

/*PÝN TANIMLAMALARI*/
#define RS_HIGH                     GPIOB->ODR|= (1<<11)
#define RS_LOW                      GPIOB->ODR&=~ (1<<11)
#define RW_HIGH                     GPIOB->ODR|= (1<<13)
#define RW_LOW                      GPIOB->ODR&=~ (1<<13)
#define E_HIGH                      GPIOB->ODR|= (1<<15)
#define E_LOW                       GPIOB->ODR&=~ (1<<15)

#define RS_SET_OUTPUT               {GPIOB->MODER|= (1<< ((11*2)));  GPIOB->MODER&=~ (1<< ((11*2)+1));}
#define RW_SET_OUTPUT               {GPIOB->MODER|= (1<< ((13*2)));  GPIOB->MODER&=~ (1<< ((13*2)+1));}
#define E_SET_OUTPUT                {GPIOB->MODER|= (1<< ((15*2)));  GPIOB->MODER&=~ (1<< ((15*2)+1));}

#define LCD_DATA_PINS               GPIOC->ODR
//#define LCD_DATA_AND_MASK           0b000000000000000000000000000011111
//#define LCD_DATA_AND_MASK 0xFFFFFFF0

//#define LCD_DATA_PINS_SET_OUTPUT    {GPIOC->MODER|= (1<< ((0*2)));GPIOC->MODER&=~ (1<< ((0*2)+1));       GPIOC->MODER|= (1<< ((1*2)));GPIOC->MODER&=~ (1<< ((1*2)+1));       GPIOC->MODER|= (1<< ((2*2)));GPIOC->MODER&=~ (1<< ((2*2)+1));       GPIOC->MODER|= (1<< ((3*2)));GPIOC->MODER&=~ (1<< ((3*2)+1)); }
//#define LCD_DATA_PINS_SET_OUTPUT    {GPIOC->MODER|= (1<< ((4*2)));GPIOC->MODER&=~ (1<< ((4*2)+1));       GPIOC->MODER|= (1<< ((5*2)));GPIOC->MODER&=~ (1<< ((5*2)+1));       GPIOC->MODER|= (1<< ((6*2)));GPIOC->MODER&=~ (1<< ((6*2)+1));       GPIOC->MODER|= (1<< ((7*2)));GPIOC->MODER&=~ (1<< ((7*2)+1)); }
#define LCD_DATA_PINS_SET_OUTPUT    GPIOC->MODER=0b00000000000000000101010101010101


#define LCD_ALL_PINS_CLOCK_ENABLE   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE)

#define VERI_GONDER                 1
#define KOMUT_GONDER                0


//KULLANIM MODLARI:
#define SATIR_MODE                  2
//#define SATIR_MODE                1
//#define BIT_MODE                    8
#define BIT_MODE                  4










////////////////////////////////////////////

/* LCD'de kullanilan komutlarin tanimlamasi*/
#define Sil 1 // Ekrani temizler
#define BasaDon 2 // Imleci sol üst köseye getirir
#define SolaYaz 4 // Imlecin belirttigi adres azalarak gider
#define SagaYaz 6 // Imlecin belirttigi adres artarak gider
#define ImlecGizle 12 // Göstergeyi ac, kursor görünmesin
#define ImlecAltta 14 // Yanip sönen blok kursor
#define ImlecYanSon 15 // Yanip sönen blok kursor
#define ImlecGeri 16 // Kursoru bir karakter geri kaydir
#define KaydirSaga 24 // Göstergeyi bir karakter saga kaydir
#define KaydirSola 28 // Göstergeyi bir karakter sola kaydir
#define EkraniKapat 8 // Göstergeyi kapat (veriler silinmez)
#define BirinciSatir 128 // LCD'nin ilk satir baslangiç adresi
// (DDRAM adres)
#define IkinciSatir 192 // ikinci satirin baslangiç adresi
#define KarakUretAdres 64 // Karakter üreteci adresini belirle
// (CGRAM adres)
/* LCD'de Kullanilan Fonksiyon Seçimi */
#define CiftSatir8Bit 56 // 8 bit ara birim, 2 satir, 5*7 piksel
#define TekSatir8Bit 48 // 8 bit ara birim, 1 satir, 5*7 piksel
#define CiftSatir4Bit 40 // 4 bit ara birim, 2 satir, 5*7 piksel
#define TekSatir4Bit 32 // 4 bit ara birim, 1 satir, 5*7 piksel

void lcd_clear(void);
void lcd_yaz(char *s);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_init(void);
void SendLCD(unsigned char c, char islem);

extern void DelayMs(int ms);
