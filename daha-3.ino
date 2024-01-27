/*
   -- дача-3 --
*/
// это программа для управление дачей
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// можете включить вывод отладочной информации в Serial на 115200
//#define REMOTEXY__DEBUGLOG

// определение режима соединения и подключение библиотеки RemoteXY
#define REMOTEXY_MODE__ESP8266WIFI_LIB_CLOUD
#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include <microDS18B20.h>
MicroDS18B20<D1> sensor1; //датчики температуры
MicroDS18B20<D2> sensor2;
// настройки соединения
#define REMOTEXY_WIFI_SSID "ABC"
#define REMOTEXY_WIFI_PASSWORD "123123123"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "29191aaebf15bbab85a32612958db1cd"


// конфигурация интерфейса
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 386 bytes
{ 255, 4, 0, 81, 0, 123, 1, 16, 93, 1, 130, 1, 2, 89, 59, 9, 132, 10, 49, 4,
  5, 11, 11, 178, 26, 31, 79, 78, 0, 31, 79, 70, 70, 0, 10, 49, 19, 5, 11, 11,
  4, 26, 31, 79, 78, 0, 31, 79, 70, 70, 0, 10, 49, 34, 5, 11, 11, 50, 26, 31,
  79, 78, 0, 31, 79, 70, 70, 0, 10, 49, 49, 5, 11, 11, 58, 26, 31, 79, 78, 0,
  31, 79, 70, 70, 0, 70, 19, 6, 20, 8, 3, 31, 37, 0, 70, 19, 20, 20, 8, 3,
  31, 37, 0, 70, 19, 35, 20, 8, 3, 31, 37, 0, 70, 19, 50, 20, 8, 3, 31, 37,
  0, 67, 4, 8, 49, 47, 6, 2, 26, 11, 67, 4, 8, 65, 47, 6, 2, 26, 11, 67,
  4, 8, 78, 47, 6, 2, 26, 11, 129, 0, 14, 43, 33, 4, 36, 208, 157, 208, 176, 208,
  191, 209, 128, 209, 143, 208, 182, 208, 181, 208, 189, 208, 184, 208, 181, 32, 209, 129, 208, 181,
  209, 130, 208, 184, 0, 129, 0, 12, 59, 37, 4, 36, 208, 162, 208, 181, 208, 188, 208, 191,
  208, 181, 209, 128, 208, 176, 209, 130, 209, 131, 209, 128, 208, 176, 32, 49, 32, 209, 141, 209,
  130, 208, 176, 208, 182, 0, 129, 0, 12, 73, 37, 4, 36, 208, 162, 208, 181, 208, 188, 208,
  191, 208, 181, 209, 128, 208, 176, 209, 130, 209, 131, 209, 128, 208, 176, 32, 50, 32, 209, 141,
  209, 130, 208, 176, 208, 182, 0, 129, 0, 6, 25, 7, 6, 8, 208, 154, 49, 0, 129, 0,
  20, 25, 7, 6, 8, 208, 154, 50, 0, 129, 0, 36, 25, 7, 6, 24, 208, 154, 51, 0,
  129, 0, 51, 25, 7, 6, 24, 208, 154, 52, 0, 67, 4, 5, 33, 9, 5, 2, 26, 11,
  67, 4, 20, 33, 9, 5, 2, 26, 11, 67, 4, 35, 33, 9, 5, 2, 26, 11, 67, 4,
  50, 33, 9, 5, 2, 26, 11, 129, 0, 6, 91, 52, 6, 31, 208, 163, 208, 191, 209, 128,
  208, 176, 208, 178, 208, 187, 208, 181, 208, 189, 208, 184, 208, 181, 32, 208, 180, 208, 176, 209,
  135, 208, 181, 208, 185, 0
};

// структура определяет все переменные и события вашего интерфейса управления
struct {

  // input variables
  uint8_t pushSwitch_1; // =1 если включено, иначе =0
  uint8_t pushSwitch_2; // =1 если включено, иначе =0
  uint8_t pushSwitch_3; // =1 если включено, иначе =0
  uint8_t pushSwitch_4; // =1 если включено, иначе =0

  // output variables
  uint8_t led_1; // led state 0 .. 1
  uint8_t led_2; // led state 0 .. 1
  uint8_t led_3; // led state 0 .. 1
  uint8_t led_4; // led state 0 .. 1
  char text_1[11];  // =строка UTF8 оканчивающаяся нулем
  char text_2[11];  // =строка UTF8 оканчивающаяся нулем
  char text_3[11];  // =строка UTF8 оканчивающаяся нулем
  char text_4[11];  // =строка UTF8 оканчивающаяся нулем
  char text_5[11];  // =строка UTF8 оканчивающаяся нулем
  char text_6[11];  // =строка UTF8 оканчивающаяся нулем
  char text_7[11];  // =строка UTF8 оканчивающаяся нулем

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup()
{
  RemoteXY_Init ();

  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);

  Serial.begin(9600);
}

void loop(){
  RemoteXY_Handler ();

if (RemoteXY.pushSwitch_1!=0) {digitalWrite(D3, LOW);
  }
  else {digitalWrite(D3, HIGH);
  }
if (digitalRead(D3) == LOW) // если на пин 3 подан высокий уровень
    RemoteXY.led_1 = 1;   // тогда включаем красный цвет индикатора
  else                        // иначе
    RemoteXY.led_1 = 0;     // выключаем красный цвет

    

if (RemoteXY.pushSwitch_4!=0) {digitalWrite(D6, LOW);
  }
  else {digitalWrite(D6, HIGH);
  }
if (digitalRead(D6) == LOW) // если на пин 3 подан высокий уровень
    RemoteXY.led_4 = 1;   // тогда включаем красный цвет индикатора
  else                        // иначе
    RemoteXY.led_4 = 0;     // выключаем красный цвет

    

if (RemoteXY.pushSwitch_2!=0) {digitalWrite(D4, LOW);
  }
  else {digitalWrite(D4, HIGH);
  }
if (digitalRead(D4) == LOW) // если на пин 3 подан высокий уровень
    RemoteXY.led_2 = 1;   // тогда включаем красный цвет индикатора
  else                        // иначе
    RemoteXY.led_2 = 0;     // выключаем красный цвет

    

 if (RemoteXY.pushSwitch_3!=0) {digitalWrite(D5, LOW);
  }
  else {digitalWrite(D5, HIGH);
  }
if (digitalRead(D5) == LOW) // если на пин 3 подан высокий уровень
    RemoteXY.led_3 = 1;   // тогда включаем красный цвет индикатора
  else                        // иначе
    RemoteXY.led_3 = 0;     // выключаем красный цвет   

  

  if (sensor1.readTemp()) Serial.println(sensor1.getTemp());
  else Serial.println("ooooo");
  if (sensor2.readTemp()) Serial.println(sensor2.getTemp());
  else Serial.println("ooooo");

  sensor1.requestTemp();
  sensor2.requestTemp();
  delay(100);

  //double temper1 = sensor1.getTemp();
  //dtostrf (temper1, 0, 2, RemoteXY.text_2);

  //double temper2 = sensor2.getTemp();
  //dtostrf (temper2, 0, 2, RemoteXY.text_3);
  
  if (sensor1.readTemp()) dtostrf (sensor1.getTemp(), 0, 2, RemoteXY.text_2); //мерим температуру на 1 датчике
  else sprintf (RemoteXY.text_2, "ERROR");
  if (sensor2.readTemp()) dtostrf (sensor2.getTemp(), 0, 2, RemoteXY.text_3); //мерим температуру на 2 датчик
  else sprintf (RemoteXY.text_3, "ERROR");


  //char str[] = " 'C";
  //int val = sensor.getTemp();
  //sprintf (RemoteXY.text_3, "%d%s", val, str); //результат будет "28 'C"

double val = analogRead(A0) / 204.8; //для 220 вольт
  dtostrf(val, 0, 2, RemoteXY.text_1); 

}
