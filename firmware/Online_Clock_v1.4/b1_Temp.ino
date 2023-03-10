//********** ФУНКЦИИ ОПРОСА ДАТЧИКА ТЕМПЕРАТУРЫ **********//


//Функция получения и обработки температуры в "°C" с "DS18B20"
void TEMP() {
  sensor.requestTemp();  //Запрос новых значений температуры

  //Проверяем успешность чтения и записываем в переменную
  if (sensor.readTemp()) {                         // Если успешно считали температуру, то...
    String Correct_Temp = crm.var("CorrectTemp");  // Получаем коэффициент корректировки температуры

    //Блок "String to Float*" для корректного перевода строки в число с плавающей точкой. Необходим для установки коэфициента при чтении с WEB интерфейса
    char Buf_Correct[sizeof(Correct_Temp)];                       // Создаём буфер и вычисляем его размер
    Correct_Temp.toCharArray(Buf_Correct, sizeof(Correct_Temp));  // myString.toCharArray(buf, len) – записывает строку в массив – буфер Buf_Correct (типа char []) с начала и до длины len.
    float CoefTemp = atof(Buf_Correct);                           // Конвертируем "Buf_Correct", который содержит данные, идентичные тем, что хранятся в строке и пишем в "CoefTemp"

    float tempC = sensor.getTemp() - CoefTemp;  // Получаем температуру в градусах Цельсия и отнимаем коэффициент корректировки. Настраивается на вкладке "a0_Settings"
    I_tempC = tempC * 10;                       // Убираем запятую, получив целое цисло через умножение (получаем от 2 до 3-х знаков) для вывода на LED
    Conv_temp = String(tempC, 1);               // Конвертируем float в строку с одним символом после запятой
    str_tempC = Conv_temp + "°C";               // Добавляет строковую постоянную "°C" и присваиваем глобальной переменной

    // Обновление значений элементов веб интерфейса (просто обновляем значение без перезагрузки страницы)
    // crm.webUpdate("[ID элемента]", "[Значение]"); Интервал отправки 1 раз в сек.
    crm.webUpdate("TempC", str_tempC);        // Пишем в ID "CRMui" температуру со знаком "°C"
    crm.webUpdate("Graph_TempC", Conv_temp);  // Пишем в ID "CRMui" температуру БЕЗ знака "°C" (для Графика температуры на главной странице)

  } else {                                       // Иначе проверяем разрешён ли звуковой сигнал об ошибке или нет
    if (Flag_Err == true) {                      // Если флаг звука ошибки при чтении датчика температуры "DS18B20" поднят, то...
      Flag_Err = crm.var("Dallas_Sign") == "1";  // Считываем состояние ("1" - это true (условие проверки), так проще написать, так как WEB интерфейс возвращает String)
      buzErr();                                  // Подавать звук ошибки на бузер
    }
  }
}


//Функция вызывается при нажатии кнопки "SET" на часах (поднимает флаг для переключения с часов на температуру на указанное в "SHOW_TEMP" кол-во миллисекунд и обновляет время для таймера возврата обратно к часам)
void ShowTemp() {
  buzBtn();             // Подаём короткий звук нажатия кнопки
  FlagShowTemp = true;  // Поднимаем флаг
  tmrShTmp = millis();  // Обновляем текущее время таймера (обязательно обновлять тут, что бы не было расхождения с таймером)
}
