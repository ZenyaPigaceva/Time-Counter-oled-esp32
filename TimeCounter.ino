#Main Code File
#include <GyverNTP.h>
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oleg;
uint32_t Ntc_Time;
uint32_t TimeZziuko = 1757209080;
uint32_t Time_passed;
uint32_t Years;
uint32_t Months;
uint32_t Days;
uint32_t Hours;
uint32_t Minutes;
uint32_t Seconds;
uint32_t Remaining;
void setup() {
    
	Serial.begin(115200); // надо же дебажить 
	oleg.init(39,40); // Это сюда подколючена и2ц дисплея
    oleg.clear();
    WiFi.begin("YourWIFISSID", "YOURFIFIPASS"); // подключить к WiFi
    NTP.begin(3); // запустить и указать часовой пояс
	NTP.setPeriod(30); // период синхронизации в секундах
}

void loop() {

if (NTP.tick()) {
  // вывод даты и времени строкой
 // Serial.print(NTP.toString());  // NTP.timeToString(), NTP.dateToString()
  //Serial.print(':');
 // Serial.println(NTP.ms());
 // Serial.println(NTP.getUnix()); 
  Ntc_Time = NTP.getUnix();
  TimeMath();
  Debug();
  Zziuko();
 


}	// вызывать тикер в loop
}
void Zziuko() {
  oleg.setScale(1);
  oleg.setCursor(30, 2);
  oleg.print(Ntc_Time);
  oleg.setCursor(0, 5);
  oleg.setScale(1);
  oleg.print(Months); oleg.setScale(1);
  //oleg.setCursor(0, 0);
  //oleg.print(Ntc_Time - TimeZziuko);
  //oleg.setCursor(0, 1);
  //oleg.print(NTP.toString());
  //Zziuko();
  
 


}

void TimeMath() {
	Time_passed = (Ntc_Time - TimeZziuko);
	//Serial.println(Time_passed);
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
	
	
