//********** ВЫПОЛНЯЕТСЯ ОДИН РАЗ ПРИ ВКЛЮЧЕНИИ **********//


void setup() {
  //Настройки пинов
  pinMode(ANALOG_PR, INPUT);  // Датчик освещённости как ВХОД

  pinMode(BUZZER, OUTPUT);  // Зуммер как ВЫХОД

  //Физические кнопки на часах как ВХОД
  pinMode(BUT_SET, INPUT);
  pinMode(BUT_UP, INPUT);
  pinMode(BUT_DOWN, INPUT);
  pinMode(BUT_RESET, INPUT);

  // Serial.begin(115200);  // Serial для отладки

  LEDS.setBrightness(18);                                  // Устанавливем примерно среднюю яркость при старте
  FastLED.addLeds< WS2812, LED_PIN, GRB>(leds, NUM_LEDS);  // По порядку указывается тип ленты, пин, порядок цветов. В круглых скобках – имя нашего массива, а также его размер – длина ленты.


  SetupCRM();   // Функция с настройками для фреймворка "CRMui3" (запускается 1 раз при включении ESP8266)
  SetupNTP();   // Применяем установки параметров NTP из json конфига
  ntp.begin();  // Запускаем NTP
}