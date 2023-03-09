//********** ПОДКЛЮЧЕНИЕ БИБЛИОТЕК, ОБЪЕКТОВ, КОНСТАНТ И ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ**********//

#include "FastLED.h"           // Библиотека для работы с адресными светодиодами "https://github.com/FastLED/FastLED"
#include <microDS18B20.h>      // Лёгкая библиотека для работы сдатчиком температуры DS18B20 "https://github.com/GyverLibs/microDS18B20"
#include <NoDelayNTPClient.h>  // Библиотека получения даты и времени по NTP (мой форк) "https://github.com/Otto17/NoDelayNTPClient"
#include <CRMui3.h>            // Библиотека позволяет быстро и просто создать веб интерфейс для настройки и управления устройством на базе ESP8266/ESP32 "https://github.com/WonderCRM/CRMui3"
#include <Ticker.h>            // Библиотека тикер (входит в состав ядра), для вызова функций с определённым периодом времени
#include <WiFiUdp.h>           // Библиотека для работы с UDP пакетами(входит в состав ядра), нужен для получения времени с NTP




/* ГЛОБАЛЬНАЯ ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ */

//Адресные светодиоды
CRGB leds[NUM_LEDS];  // Создаём массив цветов "CRGB" и сколько светодиодов "NUM_LEDS" в ленте

WiFiUDP ntpUDP;         // Инициализация объекта UDP
NTPClient ntp(ntpUDP);  // Инициализация класс ntp

CRMui3 crm;     // WebFramework "CRMui" (WEB интерфейс)
Ticker myLoop;  // Функция - планировщик (цикличное выполнение через указанный интервал, настраивается в "SetupCRM()")


//Датчик температуры
MicroDS18B20<BUS_SENSOR> sensor;  // Указываем пин в "<BUS_SENSOR>" (один датчик лучше читать без адресации, это экономит память)



/* ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ (тут ничего менять не нужно!)*/

//Цвета и эффекты
bool FlagEfReqTmp = true;        // Флаг одноразового выполнения анимации с предварительным запросом температуры
bool FlagEfRand_Min = false;     // Флаг запуска функции с рандомным выбором цвета 1 раз в минуту (при переключении минут)
uint32_t ColorTimer = 0;         // Таймер, для проверки установленного цвета 1 раз в секунду
bool Flag_EfSw = false;          // Флаг для Вкл./Выкл. эффекта смены минут (срабатывает каждую минуту)
bool Flag_Each_Sigment = false;  // Флаг для активации разноцветных сегментов и точек (работает только при включеных режимах: Фиксированный набор цветов и Рандомные цвета)
byte RndOneHUE = 0;              // Тут меняем и храним каждую минуту новое 8-ми битное число для первого сегмента (для установки через ".setHue()")
byte RndTwoHUE = 0;              // Для второго сегмента
byte RndThreeHUE = 0;            // Для третьего сегмент
byte RndFourHUE = 0;             // Для четвёртого сегмента
byte RndDotHUE = 0;              // Для точек
byte RndAllSegmHUE = 0;          // Тут меняем и храним рандомное число от 0 до 255 для всех сегментов (для установки через ".setHue()")


// Авторизация через WebFramework "CRMui"
bool WebAuthorization = false;  // (true - включить, false - выключить) авторизацию по Логину/Паролю на WEB интерфейсе
//bool WebMqtt = false;           // (true - включить, false - выключить) MQTT через WEB интерфейс


//Датчик освещённости
int valPR = 0;  // Получаем сюда raw значения от датчика освещённости (от 0 до 1024), для отображения на WEB интерфейсе


//Зуммер
bool Flag_Sig = false;      // Флаг разрешения подачи короткого сигнала 1 раз каждый час
bool Flag_One_Sig = false;  // Данный флаг отрабатывает при поднятом флаге "Flag_Sig", через него производится одноразовая подача короткого звука (без него звук будет повторяться в течении всей секунды, а это не нужно)
bool Flag_Err = false;      // Флаг Вкл./Выкл. звука ошибки при чтении датчика температуры "DS18B20"


//Температура
//float tempC = 0.0;  // ТЕСТ
int I_tempC = 0;            // Получаем целое число температуры без запятой (от 2-х до 3-х знаков) для вывода на LED
String str_tempC = "";      // Получаем строку температуры с символами "°C"
String Conv_temp = "";      // Получаем строку температуры без символов "°C"
bool FlagShowTemp = false;  // Состояние отображения температуры на часах (true - включено, false - выключено)
uint32_t tmrShTmp = 0;      // Переменная для таймера для вывода температуры на часы при нажатии кнопки SET


//NTP
String val_Time_NTP = "";    // Получаем значение времени "чч:мм:сс" через NTP в формате строки
String val_Date_NTP = "";    // Получаем значение даты "дд:мм:гггг" через NTP в формате строки
String val_Update_NTP = "";  // Сохраняем сюда дату и время с последней синхронизации по NTP

int hour = 0;    // Получаем сюда часы с NTP
int minute = 0;  // Получаем сюда минуты с NTP
int second = 0;  // Получаем сюда секунды с NTP


//Яркость Часов
byte Regim_bright;                     // Тут храним цифру (1-3), какой режим яркости в данный момент включён и от этого переключаемся на нужную функцию
byte Regim_Color;                      // Тут храним цифру (1-2), какой из режимов отображения цветов включён в данный момент включён и от этого переключаемся на нужную функцию
int new_bright = 0, new_bright_f = 0;  // Переменные для рассчёта регулировки автояркости по датчику освещённости
uint32_t ledColor = 0xff0000;          // Используемый цвет при включении (по умолчанию) в hex формате. "0xff0000" - Красный цвет


//Часы
bool Dot = false;      // Состояние разделительного двоеточия для часов : минут
bool TimeDot = false;  // Время переключения точек на часах (true - раз 1 секунду, false - раз в пол секунды)
//byte FixedHexColor;    // Сохраняем сюда порядковые номера для переключения эффектов по кругу через WEB интерейс и кнопкой DOWN
int last_digit = 0;  // Сюда сохраняются предыдущее время одним числом, полученные от переменной "digit" (для эффектов часов)


//2D массив цифр для вывода на LED
//Первый индекс это количество символов сверну вниз (от 0 до 9), а второй это количество пикселей (LED) в одном сегменте
byte digits[10][14] = {
  //1  2  3  4  5  6  7  8  9  10 11 12 13 14
  { 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1 },  // Цифра 0
  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1 },  // Цифра 1
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },  // Цифра 2
  { 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1 },  // Цифра 3
  { 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1 },  // Цифра 4
  { 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1 },  // Цифра 5
  { 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },  // Цифра 6
  { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1 },  // Цифра 7
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },  // Цифра 8
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1 }   // Цифра 9
};


//Одномерный массив символов для вывода на LED
byte symbols[14] = { 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 };  // Знак маленького "C" (Цельсия)
