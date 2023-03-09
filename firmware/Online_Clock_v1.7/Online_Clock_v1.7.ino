/*

  Online Clock - часы работающие по NTP с настройкой через WEB интерфейс и с другими плюшками.
  Первая работающая версия: 1.4 (29.12.2022г.):


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

    - Два вида регулировки яркости через WEB интерфейс (настройка в реальном времени):
        - Ручная регулировка яркости (всегда работает на заданном % яркости);
        - По заданному интервалу времени (со скольки и до скольки включать ночной режим и установка % яркости для дневного и ночного режимов),
          у данного режима есть ещё настройка порога регулировки яркости в ночном и дневном режимах, для более точной настройки;

    - Измерение температуры с помощью цифрового датчика DS18B20 (погрешность примерно +/-0.5°C):
        - Поле коэффициента корректировки температуры на WEB интернейсе. Нужен для того, если к примеру датчик расположен рядом с AMS1117 или ESP8266;
        - Поле ввода времени (в секундах) на WEB интерфейсе, для отображения температуры при нажатии кнопки "SET" на часах;
        - Вывод температуры на LED (от 0.0°C до +99.9°C) на 5 секунд по нажатию кнопки;
        - Вывод температуры на WEB интерфейс (от -55.0°C до +125.0°C).

    - Зуммер (звукоизлучатель):
        - Издаёт серию коротких сигналов при нажатии кнопки сброса настроек на часах;
        - Издаёт короткий звуковой сигнал при нажатии физических кнопок на часах;
        - Возможность Вкл./Выкл. (через WEB интерфейс) подачу короткого звукового сигнала каждый час [автоматически отключается в ночном режиме];
        
    - Физические кнопки на часах (используются только короткие нажатия):
        - SET:   Выводит температуру на LED часы в "°C";
        - UP:    Переключает на рандомный цвет;
        - DOWN:  Вкл./Выкл. разноцветные сегменты;
        - Reset: Сброс настроек по умолчанию (форматирование FS), перезагрузка и переход в режим AP (IP: 192.168.4.1).
    
    - Изменение цвета на LED часах:
        - Через WEB интерфейс можно задать любой произвольный цвет (RGB). [Работает только при режиме "Ручная регулировка яркости"];
        - Через WEB интерфейс или кнопкой UP на часах можно генерировать рандомные цвета (HUE);
        - Через WEB интерфейс или кнопкой DOWN на часах можно Вкл./Выкл. разноцветные сегменты и точки [автоматически отключается в ночном режиме].


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
  4) Узнаём назначенный IP адрес часам (через роутер или любым порт сканом) и подключаемся к нему по IP (порт 80);
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

  Остальное настройки не трогал, стоят по умолчанию.

  Все нужные библиотеки находятся в каталоге "libraries".
  P.S. Скетч может НЕ компилироваться, если обновить некоторые библиотеки, в основном это касается библиотеки "ArduinoJson",
  так как там иногда изменяют код и он становится несовместим с предыдущей версией. Последняя рабочая версия для данного скетча "ArduinoJson 6.19.4"



  ИЗМЕНЕНИЯ В ПОСЛЕДУЮЩИХ ВЕРСИЯХ:

    Версия: 1.5 (03.01.2023г.):
      - Добавлен флаг отключения подачи короткого ужечасного звукового сигнала в ночное время (что бы не мешал спать). Звук автоматически престаёт издаваться, если автивирован "Режим по расписанию" и настало время ночного режима,
        в остальных случаях звук будет издаваться (при условии, что включена подача ежечасового сигнала на WEB интерфейсе).
      - Добавлен флаг включения красного цвета на часах, если Вкл. Режим по расписанию и настало ночное время. Красный цвет лучше всего подходит для ночи, т.к. не бьёт по глазам и хорошо читаем.
      - Закомментировал строки с авторизацией на WEB интерфейсе и MQTT, так как авторизация работает НЕ корректно, очень часто не загружается WEB содержимое (проблема со стороны CRM), а MQTT не реализовывал, пока нет надобности.


    Версия: 1.6 (09.02.2023г.):
      - Убран режим "Авторегулировка яркости по датчику освещённости", так как показала практика в нём нет нужды. Удобен оказался режим "Регулировка яркости по расписанию".
        Вместо этого совместил эти 2 режима следующим образом - "Регулировка яркости по расписанию" имеет настройки ДНЕВНОГО и НОЧНОГО режимов. В обоих режимах устанавливается своё значение освещённости и по два ползунка с установкой яркости.
        Происходит сравнение датчика освещённости и установленного значения, если реальная освещённость выше заданного значения, то устанавливается одна яркость, если же ниже, то другая яркость.
        Данная настройка присутствует в дневном и ночном режиме, позволяет к примеру ночью держать подсветку на минимуме, а при включении света увеличивать яркость и на оборот. Так же и днём, когда солнечно и пасмурно яркость будет меняться на установленную.
      - Убрана (тестовая) функция "Звук при ошибки DS18B20".
      - Палитра цветов будет работать только если выбран режим "Ручная настройка" яркости.
      - Добавил на WEB интерфейс возможность установки времени показа температуры (в секундах) при нажатии кнопки "SET" на часах.
      - Подправил подачу короткого звукового сигнала, что бы он подавался сразу, как настанет новый час (до этого сигнал подавался, а потом примерно через секунду менялось время на новый час).


    Версия: 1.7:
      - Уменьшил время опроса планировщика "myLoopRun()" до 1 секунды.
      - Изменил некоторые значения "по умолчанию".




  Дата создания: 10.02.2023г.
  Версия: 1.7
  Автор: Otto.

*/