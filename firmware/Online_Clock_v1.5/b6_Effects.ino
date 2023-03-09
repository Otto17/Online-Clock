//********** ФУНКЦИИ С LED ЭФФЕКТАМИ **********//



//Функция обработки нажатия кнопки UP (переключает на рандомный цвет)
void UpEf() {
  buzBtn();                                // Подаём короткий звук нажатия кнопки
  Regim_Color = 2;                         // Устанавливаем Режим 2 (Рандомные цвета). Это нужно, если выбран другой режим отображения цветов в скетче
  crm.var(("select_Color"), Regim_Color);  // И записываем его в конфиг "crm.var("ID", "Значение")". Что бы переключить режим на WEB интерфейсе

  if (Flag_Each_Sigment == true) Random_Color_Segment();  // Выполняем функцию генерации и установки рандомного цвета для каждого сегмента
  if (Flag_Each_Sigment == false) Random_Color();         // Выполняем функцию генерации и установки рандомного цвета для всех сегментов

  crm.webUpdate("Each_Sigment", String(Flag_Each_Sigment));  // Обновляем значение флага разноцветных сегментов и точек на WEB интерфейсе
  crm.webUpdate("select_Color", String(Regim_Color));        // Обновляем значение режима отображения цветов на WEB интерфейсе
}



//Функция обработки нажатия кнопки DOWN (Вкл. / Выкл. разноцветные сегменты)
void DnEf() {
  buzBtn();                                // Подаём короткий звук нажатия кнопки
  Regim_Color = 2;                         // Устанавливаем Режим 2 (Рандомные цвета). Это нужно, если выбран другой режим отображения цветов в скетче
  crm.var(("select_Color"), Regim_Color);  // И записываем его в конфиг "crm.var("ID", "Значение")". Что бы переключить режим на WEB интерфейсе

  Flag_Each_Sigment = !Flag_Each_Sigment;        // Меняем состояние флага на противоположный
  crm.var(("Each_Sigment"), Flag_Each_Sigment);  // Записываем значение флага в конфиг "crm.var("ID", "Значение")" (То есть включает и выключаем разноцветные сегменты)

  //Так как не все элементы поддерживают автообновление при нажатии физической кнопки, делаем это вручную (вероятно это исправят в обновлении библиотеки)
  crm.webUpdate("Each_Sigment", String(Flag_Each_Sigment));  // Обновляем значение флага разноцветных сегментов и точек на WEB интерфейсе
  crm.webUpdate("select_Color", String(Regim_Color));        // Обновляем значение режима отображения цветов на WEB интерфейсе

  // Отправляем уведомление на WEB интерфейс о состоянии цветов на сегментах и точках
  // crm.webNotif("[Цвет]", "[Сообщение]", [время показа, с], [крестик закрыть (1, true)]);
  // Цвет: green, red, orange, blue, "" - без цвета
  crm.webNotif(Flag_Each_Sigment ? "green" : "Orange", Flag_Each_Sigment ? "Разноцветные сегменты Вкл. ✔" : "Разноцветные сегменты Выкл. X", SHOW_NOTIFIC, CROSS_CLOSE);
}



//Эффект бегающего символа "-" на всех сигментах слева направо сразу после включения (что бы DS18B20 успел вернуть считанную температуру)
void Ef_After_On() {
  const byte pixel_On[8] = { 7, 6, 21, 20, 37, 36, 50, 51 };  // Массив пикселей мигания
  FastLED.clear();                                            // Очищаем ленту перед началом анимации

  for (byte i = 1; i <= 3; i++) {      // Повторяем анимацию по кругу 3 раза (бегающий "-" на всех сигментах 3 раза)
    for (byte s = 0; s < 8; s++) {     // В массиве "pixel_On" 8 элементов, поэтому включаем по очереди все указанные в круглых скобках номера пикселей (от нуля включительно до 7 включительно, итого получается 8 элементов)
      leds[pixel_On[s]] = CRGB::Blue;  // Задаём цвет свечения всех пикселей по очереди
      FastLED.show();                  // Выводим на LED
      delay(80);                       // Ждём для эффекта (тут использование задержки не так важно)
    }
    FastLED.clear();  // Очищаем ленту
  }
}



//Эффект бегающего символа "-" по всем сигментам перед сбросом настроек
void Ef_Before_Reset() {
  const byte n_pixel[8] = { 7, 6, 21, 20, 37, 36, 50, 51 };  // Массив пикселей для вкл. слева направо
  const byte p_pixel[8] = { 51, 50, 36, 37, 20, 21, 6, 7 };  // Массив пикселей для вкл. справа налево
  byte fl_n = 0;                                             // Флаг для очистки LED через каждые 2 прохода по циклу for
  byte fl_p = 0;                                             // Флаг для очистки LED через каждые 2 прохода по циклу for (в обратную сторону)

  FastLED.clear();  // Очищаем ленту перед началом анимации

  for (byte i = 1; i <= 4; i++) {  // Повторяем анимацию по кругу 4 раза

    //Показываем - слева направо
    for (byte n = 0; n < 8; n++) {  // В массиве "pixel" 8 элементов, поэтому включаем по очереди все указанные в круглых скобках номера пикселей (от нуля включительно до 7 включительно, итого получается 8 элементов)
      if (fl_n > 1) {               // Счёт с нуля до 1 включительно (то есть 2 прохода). Если прошли 2 прохода, то...
        FastLED.clear();            // Очищаем ленту
        fl_n = 0;                   // Возвращаемся к нуля, с чего начали
      }
      fl_n++;                        // Прибавляем +1 к счётчику проходов
      leds[n_pixel[n]] = CRGB::Red;  // Задаём цвет свечения всех пикселей по очереди
      FastLED.show();                // Выводим на LED
      delay(50);                     // Ждём для эффекта (тут использование задержки не так важно)
    }

    //И в обратную сторону - справа налево
    for (byte p = 0; p < 8; p++) {  // В массиве "pixel" 8 элементов, поэтому включаем по очереди все указанные в круглых скобках номера пикселей (от нуля включительно до 7 включительно, итого получается 8 элементов)
      if (fl_p > 1) {               // Счёт с нуля до 1 включительно (то есть 2 прохода). Если прошли 2 прохода, то...
        FastLED.clear();            // Очищаем ленту
        fl_p = 0;                   // Возвращаемся к нуля, с чего начали
      }
      fl_p++;                        // Прибавляем +1 к счётчику проходов
      leds[p_pixel[p]] = CRGB::Red;  // Задаём цвет свечения всех пикселей по очереди
      FastLED.show();                // Выводим на LED
      delay(50);                     // Ждём для эффекта (тут использование задержки не так важно)
    }
  }
}



//Красивый эффект выключения LED (для функции cylon())
void fadeall() {
  for (byte i = 0; i < NUM_LEDS; i++) leds[i].nscale8(250);
}



//Эффект для часов (применяется при переключении минут)
void cylon() {
  static uint8_t hue = 0;

  //Cдвигаем светодиоды в начало
  for (int i = 0; i < NUM_LEDS; i++) {  // Сдвигаем светодиоды в одном направлении
    leds[i] = CHSV(hue++, 255, 255);    // Установливаем i-й светодиод на красный
    FastLED.show();                     // Отобразить буфер LED
    fadeall();                          // Применяем красивый эффект выключения
    delay(11);
  }

  //Теперь сдвигаем светодиоды в конец
  for (int i = (NUM_LEDS)-1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);  // Установливаем i-й светодиод на красный
    FastLED.show();                   // Отобразить буфер LED
    fadeall();                        // Применяем красивый эффект выключения
    delay(11);
  }
}


//Функция установки своего произвольного цвета с WEB интерфейса
void Your_Color() {
  if (Flag_RedNight == true) {
    ledColor = 0xff0000;

  } else {
    String pool_Hex = crm.var("YourColor");  // Получаем значение цвета в HEX из конфига
    pool_Hex.remove(0, 1);                   // Обрезаем символ "#" с нулевого по первый символ из строки "pool_Hex"

    //Блок "String to CharArray*" для корректного перевода строки в массив символов и конвертации HEX массива символов (цвета) в unsigned long.
    char Buf_Hex[sizeof(pool_Hex)];                   // Создаём буфер и вычисляем его размер
    pool_Hex.toCharArray(Buf_Hex, sizeof(pool_Hex));  // myString.toCharArray(buf, len) – записывает строку в массив – буфер Buf_Hex (типа char []) с начала и до длины len.
    ledColor = strtoul(pool_Hex.c_str(), NULL, 16);   // Конвертируем (без остатка от преобразования - NULL) HEX массив символов с указанием на шестнадцатеричную систему счисления (цифра 16 в конце) и пишем наше число с цветом в глобальную переменную
  }
}



//Функция рандомной генерации и установки цвета на часах
void Random_Color() {
  RndAllSegmHUE = random8();  // Генерируем рандомное число от 0 до 255 для всех сегментов
}


//Функция рандомной генерации и установки цвета на часах для каждого сегмента и точек
void Random_Color_Segment() {
  //Генерация цвета в глобальные переменные, для использования на вкладке "b0_clock"
  RndOneHUE = random8();    // Генерируем рандомное 8-ми битное число для первого сегмента
  RndTwoHUE = random8();    // Для второго сегмента
  RndThreeHUE = random8();  // Для третьего сегмента
  RndFourHUE = random8();   // Для четвёртого сегмента
  RndDotHUE = random8();    // Для точек
}