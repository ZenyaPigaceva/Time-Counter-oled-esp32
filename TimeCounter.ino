//#Main Code File
#include <GyverNTP.h>
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oleg;
uint32_t Ntc_Time;
uint32_t TimeCounting = 1757209080; //Сюда надо вписать Свое время 
uint32_t Time_passed; //это все переменные и их иницилизация 
uint8_t Years;
uint8_t Months;
uint8_t Days;
uint8_t Hours;
uint8_t Minutes;
uint8_t Seconds;
uint32_t Remaining; // вот тута они закончились

void setup() {
    
	Serial.begin(115200);                        // надо же дебажить 
	oleg.init(39,40);
  oleg.clear();                           // Это сюда подключена и2ц дисплея 39,40  sda scl 14,12 5,6
  oleg.setContrast(255);
	WiFi.begin("Wifi", "Pass");      // подключить к WiFi
  while (WiFi.status() != WL_CONNECTED) {   // Wait for the Wi-Fi to connect
    Serial.print('.');
    DisplayStart();
    delay(50);
  }
  delay(1000);
  oleg.clear();
	NTP.begin(3);              // запустить и указать часовой пояс
	NTP.setPeriod(30);        // период синхронизации в секундах
}

void loop() {
if (NTP.tick()) {

   Ntc_Time = NTP.getUnix();      // получение текушего времени и запись его в локальную переменную
   TimeMath();                   // Функция которая отвечает за конвертацию и сравнение прошедших секунд в дни,года,месяцы,итд..
   Debug();                     // Функция для отладки, по умолчанию закомментированна 
   DisplayPrint();             // Функция вывода готовых данных на экран, вызвать после  TimeMath();
 } // вызывать тикер в loop
}

void DisplayPrint() {
	oleg.setScale(1);
	oleg.setCursor(19,0 );
	oleg.print("Времени прошло");
  oleg.line(0, 10, 127, 10, 127);
	oleg.setCursor(1, 2);
	oleg.setScale(2);
	oleg.print(Months); 
  oleg.setScale(1);
  oleg.setCursor(15, 3);
  oleg.print("Месяца");
  oleg.setCursor(1, 4);
	oleg.setScale(2);
	oleg.print(Days); 
  oleg.setScale(1);
  oleg.setCursor(15, 5);
  oleg.print("Дней");

  



  }

void DisplayStart() {
  
   oleg.setScale(1);
   oleg.setCursor(10, 0);
   oleg.print("Подключение к wifi"); 
   oleg.setCursor(13, 1);
   oleg.print(WiFi.macAddress());
   oleg.setCursor(16,2);
   oleg.printf(ESP.getChipModel(),ESP.getChipRevision());
   
}


void TimeMath() {
	Time_passed = (Ntc_Time - TimeCounting); // Тут получаем кошличество времени которое прошло в секундах
	Years = Time_passed/ 31536000;    //Тут ещё небольшое количество вычислений да
	Remaining = Time_passed % 31536000 ;
	Months= Remaining / 2592000;
	Remaining %= 2592000;
	Days = Remaining / 86400;
	Remaining %= 86400;
	Hours = Remaining / 3600;
	Remaining %= 3600;
	Minutes = Remaining / 60;
	Seconds = Remaining % 60; }
void Debug() {
	Serial.print("Your Local IP is :");
  Serial.println(WiFi.localIP()); 
  Serial.println(String(Seconds));
	}
	
