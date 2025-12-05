#Main Code File

#include <GyverNTP.h>
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oleg;
uint32_t Ntc_Time;
uint32_t TimeCounting = 1757209080; //Сюда надо вписать Свое время 
uint32_t Time_passed; //это все переменные и их иницилизация 
uint32_t Years;
uint32_t Months;
uint32_t Days;
uint32_t Hours;
uint32_t Minutes;
uint32_t Seconds;
uint32_t Remaining; // вот тута они закончились
void setup() {
    
	Serial.begin(115200); // надо же дебажить 
 oleg.init(39,40); // Это сюда подключена и2ц дисплея
 DisplayStart();
 WiFi.begin("Название вашей сети wifi", "пароль от вашего  wifi"); // подключить к WiFi
 oleg.clear();
 NTP.begin(3); // запустить и указать часовой пояс
	NTP.setPeriod(30); // период синхронизации в секундах
}

void loop() {

if (NTP.tick()) {

   Ntc_Time = NTP.getUnix(); // получение текушего времени и запись его в локальную переменную
   TimeMath(); // Функция которая отвечает за конвертацию и сравнение прошедших секунд в дни,года,месяцы,итд..
// Debug(); // Функция для отладки, по умолчанию закомментированна 
   DisplayPrint(); // Функция вывода готовых данных на экран, вызвать после  TimeMath();
 
}	// вызывать тикер в loop
}
void  DisplayPrint() {
  oleg.setScale(1);
  oleg.setCursor(30, 2);
  oleg.print(Ntc_Time);
  oleg.setCursor(0, 5);
  oleg.setScale(1);
  oleg.print(Months); oleg.setScale(1);
  //oleg.setCursor(0, 0);
  }

void DisplayStart() {
    oleg.clear();  // Очистка экрана перед анимацией

    // Эффект заполнения экрана линиями
    for (int i = 0; i < 64; i += 4) {
        oleg.drawLine(0, i, 127, i, OLED_STROKE_FILL);
        oleg.update();
        delay(50); }

    // Текст приветствия
    oleg.clear();
    oleg.setScale(2);
    oleg.setCursor(15, 20);
    oleg.print("Welcome!");
    oleg.update();
    delay(1000);

    // Подключение к Wi-Fi
    oleg.clear();
    oleg.setScale(1);
    oleg.setCursor(10, 20);
    oleg.print("Connecting to Wi-Fi...");
    oleg.update();

    // Эффект "мигающих точек"
    for (int i = 0; i < 3; i++) {
        oleg.print(".");
        oleg.update();
        delay(500); }
    oleg.clear();
    
    // Сообщение об успешном подключении
    oleg.setCursor(20, 20);
    oleg.print("Wi-Fi Connected!");
    oleg.update();
    delay(1000);
}

void TimeMath() {
	Time_passed = (Ntc_Time - TimeCounting); // Тут получаем кошличество времени которое прошло в секундах
	Years = Time_passed/ 31536000;
	Remaining = Time_passed % 31536000 ;
	Months= Remaining / 2592000;
	Remaining %= 2592000;
	Days = Remaining / 86400;
	Remaining %= 86400;
	Hours = Remaining / 3600;
	Remaining %= 3600;
	Minutes = Remaining / 60;
	Seconds = Remaining % 60;
}
void Debug() {
	//Serial.println(Days,Hours);
	
	
