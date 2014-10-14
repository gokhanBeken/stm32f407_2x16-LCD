#include "lcd.h" // lcd.h dosyasi tanimlanip, degerler aliniyor
//#include "delays.h" // Gecikme fonksiyonu tanimlaniyor
void lcd_busy(void)
{
    //DelayMs(1);
    DelayMs(2); //3.3v icin
}

void SendLCD(unsigned char c, char islem) // Komut g�nderme fonksiyonu
{
    RW_LOW; // LCD'ye yazma yapilacak

    if(islem==KOMUT_GONDER) RS_LOW;  // LCD'ye komut g�nderilecek
    else if(islem==VERI_GONDER) RS_HIGH;


#if BIT_MODE==8
    E_HIGH; // D�sen kenar tetikleme oldugu i�in E �nce 1
    LCD_DATA_PINS = c; // butun bitler gonderiliyor
    E_LOW; // E, 0 yapiliyor
    lcd_busy(); //Belirli s�re bekleniyor
#elif BIT_MODE==4
    E_HIGH;
    LCD_DATA_PINS = ( c & 0xF0 ); // Y�ksek degerlikli bitler g�nderiliyor
    E_LOW;
    lcd_busy();
    E_HIGH;
    LCD_DATA_PINS = ( (c & 0x0F)<<4 ); // D�s�k degerlikli bitler g�nderiliyor
    E_LOW; // E, 0 yapiliyor
    lcd_busy(); // Belirli bir s�re bekleniyor
#endif

//LCD_DATA_PINS = ( c & LCD_DATA_AND_MASK ); // Y�ksek degerlikli bitler g�nderiliyor


}



void lcd_clear(void) // LCD siliniyor
{
    SendLCD(Sil,KOMUT_GONDER);
    //DelayMs(2); //5v icin
    DelayMs(4); //3.3v icin
}

void lcd_yaz(char * s) // LCD'ye string ifade g�nderiliyor
{
    lcd_busy();
    while(*s)
    {
        SendLCD(*s,VERI_GONDER);
        s++;
    }
}

void lcd_gotoxy(unsigned char x,unsigned char y) //LCD'nin belli //b�lgesine gidiliyor
{
    if(x==1)
        SendLCD(BirinciSatir+((y-1)%16),KOMUT_GONDER);
    else
        SendLCD(IkinciSatir+((y-1)%16),KOMUT_GONDER);

}

void lcd_init() // LCD ilk y�kleme ayarlari yapiliyor
{
    LCD_ALL_PINS_CLOCK_ENABLE;
    LCD_DATA_PINS_SET_OUTPUT;

    RS_SET_OUTPUT
    RW_SET_OUTPUT
    E_SET_OUTPUT


    RS_LOW;
    RW_LOW;
    E_LOW;




    //DelayMs(20); //5 volt icin
    DelayMs(50); // 4.5'ten daha dusuk bir voltaj icin


    E_HIGH;



    SendLCD(0x03,KOMUT_GONDER); //islemci resetlendiginde LCD'nin de yeniden baslamasini sagliyor


    SendLCD(BasaDon,KOMUT_GONDER);

    DelayMs(2);



#if BIT_MODE==8 && SATIR_MODE==1
    SendLCD(TekSatir8Bit,KOMUT_GONDER);
#elif BIT_MODE==8 && SATIR_MODE==2
    SendLCD(CiftSatir8Bit,KOMUT_GONDER);
#elif BIT_MODE==4 && SATIR_MODE==1
    SendLCD(TekSatir4Bit,KOMUT_GONDER);
#elif BIT_MODE==4 && SATIR_MODE==2
    SendLCD(CiftSatir4Bit,KOMUT_GONDER);

#elif BIT_MODE!=4 && BIT_MODE!=8
#error BIT MODUNU TANIMLAMAMISSINIZ
#elif SATIR_MODE!=1 && SATIR_MODE!=2
#error SATIR MODUNU TANIMLAMAMISSINIZ
#endif


    SendLCD(SagaYaz,KOMUT_GONDER);
    SendLCD(ImlecGizle,KOMUT_GONDER);
    lcd_clear();
    SendLCD(BirinciSatir,KOMUT_GONDER);
}
