//********** НАСТРОЙКИ **********//


//Кнопки на часах (обрабатываются "CRMui")
#define BUT_SET 2     // Кнопка SET
#define BUT_UP 0      // Кнопка UP
#define BUT_DOWN 4    // Кнопка DOWN
#define BUT_RESET 14  // Кнопка Reset


//Зуммер
#define BUZZER 13  // Подключён к GPIO13

//Адресные светодиоды
#define LED_PIN 12           // Подключён к GPIO12
#define NUM_LEDS 58          // Количество светодиодов
#define MAX_BRIGHT 220       // Ограничиваем максимальную яркость для LED (максимум 254), будет конвертироваться в "PERC_MAX_BRIGHT"
#define MIN_BRIGHT 1         // Ограничиваем минимальную яркость для LED (минимум 0), будет конвертироваться в "PERC_MIN_BRIGHT"
#define PERC_MAX_BRIGHT 100  // Максимальная яркость в "%" для LED
#define PERC_MIN_BRIGHT 1    // Минимальная яркость в "%" для LED


//Датчик освещённости PR
#define ANALOG_PR A0  // Подключён к ADC


//Датчик температуры DS18B20
#define BUS_SENSOR 5  // Подключён к GPIO5
//#define SHOW_TEMP 7000  // Время показа температуры при нажатии кнопки "SET" (в миллисекундах)
#define SHOW_TEMP "10"  // Время (по умолчанию) в секундах в течение которого будет выводится температура при нажатии кнопки "SET" на часах
//Коэффициент корректировки температуры [температура в °C - CORRECT_TEMP]. Нужен для того, если к примеру датчик расположен рядом с AMS1117 или ESP8266
#define CORRECT_TEMP "3.48"  // Установка коэффициента по умолчанию. Чем больше число, тем меньше конечная температура, так же можно отключить коэффициента, если написать: "0.0". Настроить коэффициента можно на WEB интерфейсе



//Стартовые настройки NTP по умолчанию (в дальнейшем их можно изменить на WEB интерфейсе)
#define SET_GMT "6"                 // Установить часовой пояс "в GMT часах" (например Омск GMT+6, поэтому ставим число 6)
#define SET_PERIOD "360"            // Установить период обновления "в минутах" (период 1 минуты равен 60000 миллисекунд)
#define SET_HOST "ru.pool.ntp.org"  // Установить хост или пул хостов


//WebFramework "CRMui" (WEB интерфейс)
#define PROJECT "Online Часы"  // Название проекта на WEB интерфейсе

#define SSID_AP "Online-Clock"  // SSID сети (по умолчанию) в режиме AP (можно изменить позже на WEB интерфейсе)
#define PASSWD_AP "123456789"   // Пароль (по умолчанию) в AP режиме,(можно изменить позже на WEB интерфейсе)

#define SHOW_NOTIFIC 5  // Время (в секундах) показа всплывающего уведомления в правом нижнем углу WEB интерфейсе
#define CROSS_CLOSE 1   // Вкл./Выкл. крестика для закрытия всплывающего уведомления в правом нижнем углу WEB интерфейсе (1 - вкл. крестик, 0 - выкл. крестик)

#define CONTACTS 1                               // (1 - включить, 0 - выключить) свои контактные данные на WEB интерфейсе. Если свои данные выключены, то инфа НЕ выводится
#define CONTACTS_MAIL "den-life@mail.ru"         // Свой E-mail на WEB интерфейсе
#define CONTACTS_TELEGA "Otto_Lilo"              // Своё имя пользователя Телеграм на WEB интерфейсе
#define CONTACTS_WEBSITE "http://pc7.sytes.net"  // Свой адрес сайта на WEB интерфейсе

#define VERSION "ver. 1.7"  // Версия проекта (String)