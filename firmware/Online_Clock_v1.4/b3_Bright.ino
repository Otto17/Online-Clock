//********** ФУНКЦИИ ИЗМЕРЕНИЯ ОТ ДАТЧИКА ОСВЕЩЁННОСТИ **********//


// Авторегулировка яркости по датчику освещённости
void AutoBrightnessCheck() {
  static uint32_t bright_timer = 0;  // Объявляем статическую переменную для таймера (сохраняет свои значения между вызовами функции)

  //Получаем значение из конфига json через WebFramework "CRMui"
  // crm.var(["ID переменной"])
  int time_Update_PR = crm.var("PR_Update").toInt();  // Получаем из конфига время опроса фоторезистора (в мс), конвертированное из String в int
  int bright_Constant = crm.var("PR_Const").toInt();  // Получаем из конфига константу усиления от внешнего света (0 - 1023), конвертированную из String в int
  int max_Bright = crm.var("PR_MaxBright").toInt();   // Получаем из конфига максимальную яркость (0 - 254)
  int min_Bright = crm.var("PR_MinBright").toInt();   // Получаем из конфига минимальную яркость (0 - 254)
  String pool_C = crm.var("PR_Coef");                 // Получаем из конфига коэффициент фильтра (0.0 - 1.0), конвертированный из String в float

  //Блок "String to Float*" для корректного перевода строки в число с плавающей точкой. Необходим для установки коэфициента при чтении с WEB интерфейса
  char Buf_C[sizeof(pool_C)];                 // Создаём буфер и вычисляем его размер
  pool_C.toCharArray(Buf_C, sizeof(pool_C));  // myString.toCharArray(buf, len) – записывает строку в массив – буфер buf_C (типа char []) с начала и до длины len.
  float coef = atof(Buf_C);                   // Конвертируем "Buf_C", который содержит данные, идентичные тем, что хранятся в строке и пишем в "coef"


  if (millis() - bright_timer >= time_Update_PR) {                                // Сравниваем, если прошло больше указанного времени, то...
    bright_timer = millis();                                                      // Записываем новое время в переменную "bright_timer"
    int bright = analogRead(ANALOG_PR);                                           // Считываем сырые значения с датчика освещённости в переменную "bright" (от 0 до 1024 на ESP8266/32)
    int bright_r;                                                                 // Переменная для промежуточных расчётов
    bright_r = bright % 10 > 5 ? ((bright / 10) * 10) + 10 : (bright / 10) * 10;  // Расчёт от полученных значений освещённости с датчика
    new_bright = map(bright_r, 0, bright_Constant, min_Bright, max_Bright);       // Считаем обработанные показания с фоторезистора, переводим их в нужный нам диапазон
    new_bright = constrain(new_bright, min_Bright, max_Bright);                   // Защита от выхода за пределы заданных диапазонов
    new_bright_f = new_bright_f * coef + new_bright * (1 - coef);                 // Конечный расчёт для плавной регулировки
    new_bright_f += 1;                                                            // Каждый цикл по условию прибавляем +1 к расчитанной переменной

    LEDS.setBrightness(new_bright_f);  // Устанавливаем новую яркость
  }
}


//Ручная регулировка яркости
void ManualBrightnessSetting() {
  // Получить значение из конфига crm.var(["ID переменной"])
  byte Manual_Brigh = crm.var("manual_brigh").toInt();  // Получаем значение яркости из конфига (от 0 до 254 максимум)

  //Блок переводов диапазана для корректной регулировки яркости от 1% до 100% (что бы на WEB интерфейсе и часах было одно и то же заданное значение яркости)
  byte BrConvert = map(Manual_Brigh, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT, MIN_BRIGHT, MAX_BRIGHT);  // Конвертируем считанные значения с WEB интерфейса из (Мин./Макс. %) до (Мин./Макс. в ограниченой нами яркости), для идентичности, что бы считать Макс. оганиченную нами яркость за 100%
  BrConvert = constrain(BrConvert, PERC_MIN_BRIGHT, MAX_BRIGHT);                                 // Ограничиваем диапазон, что бы не выйти за его пределы

  byte val_Manual_Brigh = map(BrConvert, MIN_BRIGHT, MAX_BRIGHT, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT);  // Теперь переводим обратно из (Мин./Макс. в ограниченой нами яркости) до (Мин./Макс. %), тем самым выравнивая значения с WEB интерфейса и заданного нами диапазона яркости
  val_Manual_Brigh = constrain(val_Manual_Brigh, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT);                  // Ограничиваем диапазон, что бы не выйти за его пределы

  LEDS.setBrightness(val_Manual_Brigh);  // Устанавливаем новую яркость в %
}


//Регулировка яркости по расписанию (Ночной режим)
void ScheduledBrightnessControl() {
  //Получаем сюда конвертированные в int чч:мм:сс из строк (начала и окончания ночного режима)
  int Start_hh, End_hh = 0;  // "Start_hh" - часы начала ночного режима; "End_hh" - часы окончания ночного режима.
  int Start_mm, End_mm = 0;  // "Start_mm" - минуты начала ночного режима; "End_mm" - минуты окончания ночного режима.
  int Start_ss, End_ss = 0;  // "Start_ss" - секунды начала ночного режима; "End_ss" - секунды окончания ночного режима.

  //Получить значение из конфига crm.var(["ID переменной"])
  String pool_StartNightMode = crm.var("Start_Night_Mode");  // Получаем время начала ночного режима
  String pool_EndNightMode = crm.var("End_Night_Mode");      // Получаем окончания начала ночного режима
  byte Init_NightBrigh = crm.var("night_brigh").toInt();     // Получаем значение заданной яркости для ночного режима из конфига (возможный диапазон от 0 до 254 максимум)
  byte Init_DayBrigh = crm.var("day_brigh").toInt();         // Получаем значение заданной яркости для дневного режима из конфига (возможный диапазон от 0 до 254 максимум)


  //Блок "String to const char*" для корректного перевода строки в массив символов. Необходим для функции "sscanf()"
  //Для переменной "pool_StartNightMode" - начало ночного режима
  char Buf_Start[sizeof(pool_StartNightMode)];                              // Создаём буфер и вычисляем его размер
  pool_StartNightMode.toCharArray(Buf_Start, sizeof(pool_StartNightMode));  // myString.toCharArray(buf, len) – записывает строку в массив – буфер Buf_Start (типа char []) с начала и до длины len.
  const char* StartNightMode = pool_StartNightMode.c_str();                 // Возвращаем указатель на массив символов "pool_StartNightMode" с завершающим нулем
  sscanf(StartNightMode, "%2d:%2d:%2d", &Start_hh, &Start_mm, &Start_ss);   // Конвертируем строку "чч:мм:сс" по маске в числа и пишем в разные переменные. Маска: "%2d:%2d:%2d" - первые два числа пишем в "Start_hh", двоеточия отсеиваем и от них ещё два числа пишем в "Start_mm" и так же с секундами

  //Блок "String to const char*" для корректного перевода строки в массив символов. Необходим для функции "sscanf()"
  //Для переменной "pool_EndNightMode" - окончание ночного режима
  char buf_End[sizeof(pool_EndNightMode)];                            // Создаём буфер и вычисляем его размер
  pool_EndNightMode.toCharArray(buf_End, sizeof(pool_EndNightMode));  // myString.toCharArray(buf, len) – записывает строку в массив – буфер buf_End (типа char []) с начала и до длины len.
  const char* EndNightMode = pool_EndNightMode.c_str();               // Возвращаем указатель на массив символов "pool_EndNightMode" с завершающим нулем
  sscanf(EndNightMode, "%2d:%2d:%2d", &End_hh, &End_mm, &End_ss);     // Конвертируем строку "чч:мм:сс" по маске в числа и пишем в разные переменные. Маска: "%2d:%2d:%2d" - первые два числа пишем в "End_hh", двоеточия отсеиваем и от них ещё два числа пишем в "End_mm" и так же с секундами


  //Блок переводов диапазана для корректной регулировки яркости от 1% до 100% (что бы на WEB интерфейсе и часах было одно и то же заданное значение яркости)
  //Для переменной "NightBrigh"
  byte BrConvertNight = map(Init_NightBrigh, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT, MIN_BRIGHT, MAX_BRIGHT);  // Конвертируем считанные значения с WEB интерфейса из (Мин./Макс. %) до (Мин./Макс. в ограниченой нами яркости), для идентичности, что бы считать Макс. оганиченную нами яркость за 100%
  BrConvertNight = constrain(BrConvertNight, PERC_MIN_BRIGHT, MAX_BRIGHT);                               // Ограничиваем диапазон, что бы не выйти за его пределы

  byte NightBrigh = map(BrConvertNight, MIN_BRIGHT, MAX_BRIGHT, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT);  // Теперь переводим обратно из (Мин./Макс. в ограниченой нами яркости) до (Мин./Макс. %), тем самым выравнивая значения с WEB интерфейса и заданного нами диапазона яркости
  NightBrigh = constrain(NightBrigh, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT);                             // Ограничиваем диапазон, что бы не выйти за его пределы

  //Для переменной "DayBrigh"
  byte BrConvertDay = map(Init_DayBrigh, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT, MIN_BRIGHT, MAX_BRIGHT);  // Конвертируем считанные значения с WEB интерфейса из (Мин./Макс. %) до (Мин./Макс. в ограниченой нами яркости), для идентичности, что бы считать Макс. оганиченную нами яркость за 100%
  BrConvertDay = constrain(BrConvertDay, PERC_MIN_BRIGHT, MAX_BRIGHT);                               // Ограничиваем диапазон, что бы не выйти за его пределы

  byte DayBrigh = map(BrConvertDay, MIN_BRIGHT, MAX_BRIGHT, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT);  // Теперь переводим обратно из (Мин./Макс. в ограниченой нами яркости) до (Мин./Макс. %), тем самым выравнивая значения с WEB интерфейса и заданного нами диапазона яркости
  DayBrigh = constrain(DayBrigh, PERC_MIN_BRIGHT, PERC_MAX_BRIGHT);                             // Ограничиваем диапазон, что бы не выйти за его пределы


  //Переводит чч:мм:сс в одно число (секунды) для удобного сравнения
  int NTP_str = (hour * 3600) + (minute * 60) + second;            // Перевод времени с NTP
  int Start_str = (Start_hh * 3600) + (Start_mm * 60) + Start_ss;  // Перевод времени начала периода
  int End_str = (End_hh * 3600) + (End_mm * 60) + End_ss;          // Перевод времени окончания периода


  //Проверка и сравнение установленого интервала времени
  if (Start_str < End_str) {                          // Если время начала МЕНЬШЕ времени окончания ночного режима, то выполняем следующие условия...
    if (NTP_str >= Start_str && NTP_str < End_str) {  // Если время с NTP больше или равно времени начала и меньше времени окончания...
      LEDS.setBrightness(NightBrigh);                 // Тогда включаем ночной режим

    } else if (NTP_str >= End_str) {  // Иначе, если время с NTP больше или равно времени окончания ночного режима...
      LEDS.setBrightness(DayBrigh);   // Тогда включаем дневной режим

    } else {                         // Иначе...
      LEDS.setBrightness(DayBrigh);  // Включаем дневной режим
    }
  }


  //Инвертированная проверка и сравнение установленого интервала времени
  if (Start_str > End_str) {                         // Если время начала БОЛЬШЕ времени окончания ночного режима, то выполняем следующие условия...
    if (NTP_str >= Start_str && NTP_str <= 86399) {  // Если время с NTP больше или равно времени начала ночного режима и меньше или равно 23:59:59, то...
      LEDS.setBrightness(NightBrigh);                // Включаем ночной режим

    } else if (NTP_str < End_str) {    // Иначе, если время с NTP меньше времени окончания ночного режима...
      LEDS.setBrightness(NightBrigh);  // Тогда включаем ночной режим

    } else if (NTP_str >= End_str && NTP_str < Start_str) {  // Иначе, если время с NTP больше или равно времени окончания и меньше времени начала ночного режима...
      LEDS.setBrightness(DayBrigh);                          // Тогда включаем дневной режим
    }
  }
}