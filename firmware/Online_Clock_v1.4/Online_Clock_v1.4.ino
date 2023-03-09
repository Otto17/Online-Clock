/*

  Online Clock - часы работающие по NTP с настройкой через WEB интерфейс и с другими плюшками.


  Возможности:
    - Часы на адресной светодиодной ленте WS2812B с обновлением вермени по NTP.

    - Настройка NTP осуществяются через WEB интерфейс, в них можно указать (параметры применятся только после перезагрузки ESP8266):
        - Свой NTP сервер;
        - Часовой пояс в "GMT" формате (цифры или цифры с "-") [К примеру: У Омска GMT+6, указываем просто цифру 6 или у Гренландии GMT-3, указываем -3];
        - Период обновления (в минутах). Через какое время будет синхранизация с NTP сервером;
        - Можно сразу увидеть дату и время (в случае изменений настроек, требуется перезагрузка);
        - Вывод на WEB интерфейсе даты и времени последней синхронизации с NTP;
        - Кнопка принудительного обновления времени и даты с NTP;
        - Кнопка Применить и Перезагрузить (для применения и перезагрузки ESP8266). 

    - Три вида регулировки яркости через WEB интерфейс (настройка в реальном времени):
        - Авторегурировка яркости по аналоговому датчику освещённости (с тонкой настройкой плавности и резкости);
        - Ручная регулировка яркости (всегда работает на заданном % яркости);
        - По заданному интервалу времени (со скольки и до скольки включать ночной режим и установка % яркости для дневного и ночного режимов);

    - Измерение температуры с помощью цифрового датчика DS18B20 (погрешность примерно +/-0.5°C):
        - Встроено поле коэффициента корректировки температуры на WEB интернейсе. Нужен для того, если к примеру датчик расположен рядом с AMS1117 или ESP8266;
        - Вывод температуры на LED (от 0.0°C до +99.9°C) на 5 секунд по нажатию кнопки;
        - Вывод температуры на WEB интерфейс (от -55.0°C до +125.0°C).

    - Зуммер (звукоизлучатель):
        - Издаёт серию коротких сигналов при нажатии кнопки сброса настроек на часах;
        - Издаёт короткий звуковой сигнал при нажатии физических кнопок на часах;
        - Возможность Вкл./Выкл. (через WEB интерфейс) подачу короткого звукового сигнала каждый час;
        - Возможность Вкл./Выкл. (через WEB интерфейс) подачу звукового сигнала ошибки, при неудачных считываниях температуры.
        
    - Физические кнопки на часах (используются только короткие нажатия):
        - SET:   Выводит температуру на LED часы в "°C";
        - UP:    Переключает на рандомный цвет;
        - DOWN:  Вкл./Выкл. разноцветные сегменты;
        - Reset: Сброс настроек по умолчанию (форматирование FS), перезагрузка и переход в режим AP (IP: 192.168.4.1).
    
    - Изменение цвета на LED часах:
        - Через WEB интерфейс можно задать любой произвольный цвет (RGB);
        - Через WEB интерфейс или кнопкой UP на часах можно генерировать рандомные цвета (HUE);
        - Через WEB интерфейс или кнопкой DOWN на часах можно Вкл./Выкл. разноцветные сегменты и точки.

    - WEB интерфейс с возможностью Вкл./Выкл. Авторизации по Логину/Паролю (по умолчанию выключена).
    - Работа в режимах точки доступа и клиента (по умолчанию Точка доступа с IP адресом: 192.168.4.1).
    - Краткая статистика работы ESP8266 через WEB интерфейс (левый нижний угол).
    - Возможность на WEB интерфейсе изменить цвет: заголовков, оконтовок кнопок, переключателей, разделителей и текста;
    - Обновление прошивки по OTA и Импорт/Экспорт/Стирание конфига в формате Json через WEB интерфейс.



  Краткое описание действий:
  Программирование:
  1) На часах имеется колодка (6 PIN) для USB TTL. Программировать можно с автосбросом (используя все 6 проводков) как указано на схеме.
  2) Или зажав кнопку "UP" перед подачей питания на часы - тогда ESP8266 принудительно перейдёт в режим программирования.

  После прошивки:
  1) Находим на часах точку доступа (по умолчанию "Online-Clock"), подключаемся с паролем (по умолчанию "123456789");
  2) Переходим на WEB интерфейс либо нажав на всплывающее PUSH уведомление, либо по адресу "192.168.4.1";
  3) Во вкладке "Wi-Fi" меняем режим работы на "Клиент" и подключаемся к нужной WiFi сети, сохраняем с перезагрузкой;
  4) Узнаём назначенный IP адрес часам (через роутер или любым порт сканом) и подключаемся к ним по IP (порт 80);
  5) Настраиваем NTP и остальные пункты по мене надобности и сохраняем с перезагрузкой;



  Параметры платы в Arduino IDE:
  !!! ВАЖНО !!! - Использовать плату ESP8266 Community версии "3.0.2",
  иначе на версиях начиная с "3.1.X" и выше компиляция будет выдавать кучу ошибок!
  
  1) Плата "Generic ESP8266 Module"
  2) Flash Mode: QIO(fast)
  3) Upload Speed: 921600
  4) Flash Size: 4MB (FS:2MB OTA:~1019KB)
  5) lwIP Variant: v2 Higher Bandwidth
  6) Builtin Led: 2
  7) Espressif FW: nonos-sdk 2.2.1+100 (190703)
  8) Erase Flash: All Flash Contents

  Остальное настройки не трогал, стоят по умолчанию.



  Дата создания: 29.12.2022г.
  Версия: 1.4
  Автор: Otto.
*/