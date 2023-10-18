#include <EEPROM.h>

// Pinler
#define lm35Pin A0
#define redLed 8
#define greenLed 12
#define buzzer 9

// Değişkenler
const long onDuration = 1000;
const long offDuration = 750;
long rememberTime=0;
int ledState = LOW;
int okunan_deger = 0;
int kayit_adresi = 10;


void setup()
{
  // Pin Ayarları
   Serial.begin(9600);
   pinMode(redLed,OUTPUT);
   pinMode(greenLed,OUTPUT);
   digitalWrite(greenLed, ledState);
   pinMode(buzzer,OUTPUT);

}

// Değrlerin Celcius'a Dönüştürülmesi
double sicaklikHesapla(int deger)
{
  double sicaklik;
  sicaklik = log(((1024000 / deger) - 10000));
  // Calvin Cinsinden Sıcaklık
  sicaklik = 1/ (0.001129148 + (0.000234125 +(0.0000000876741 * sicaklik * sicaklik * sicaklik)) * sicaklik); 
  //Celcius Cinsinden Sıcaklık
  sicaklik = sicaklik -273.15;
  return sicaklik;
}

void loop()
{
  // Eğer Yeşil Led Yanıyorsa
  if(ledState == HIGH){
    if(( millis() - rememberTime) >= onDuration){
      ledState = LOW;
      rememberTime = millis();
    }

    // Sensörden Değer Oku
    okunan_deger = analogRead(lm35Pin);
    double sck = sicaklikHesapla(okunan_deger);

    // Kritik Değer Kontrolü
    if(sck >= 40){
    digitalWrite(redLed,HIGH);
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(redLed,LOW);
    digitalWrite(buzzer,LOW);
    delay(50);

    // Kritik Değerlerin EEPROM'a Kaydedilmesi
    EEPROM.write(kayit_adresi,sck);
    kayit_adresi += 5;
    Serial.print("Kritik Sicaklik Degeri:");
    Serial.println(sck);
    delay(50);
    }

    // Eğer Değer Kritik Değer Değilse
    else{
       digitalWrite(redLed,LOW);
       digitalWrite(buzzer,LOW);
       Serial.println(sck);
    }
  }

  // Eğer Yeşil Led Yanmıyorsa
  else{
    if(( millis() - rememberTime) >= offDuration){
      ledState = HIGH;
      rememberTime = millis();
    }
  }
  digitalWrite(greenLed, ledState);
}