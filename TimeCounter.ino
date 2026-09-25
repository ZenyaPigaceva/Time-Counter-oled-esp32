//#Main Code File
#include <WiFi.h>          
#include <GyverNTP.h>
#include <GyverOLED.h>
#include <time.h>          
#include "driver/temperature_sensor.h"

// НАСТРОЙКИ WI-FI СЕТИ
const char* ssid = "Ryo'Apls";
const char* pass = "zziukobest";

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oleg;

uint32_t Ntc_Time;
uint32_t TimeCounting = 175720908; 
uint32_t Time_passed; 

uint8_t Years;
uint8_t Months;
uint8_t Days;
uint8_t Hours;
uint8_t Minutes;
uint8_t Seconds;

float getChipTemperature() {
  float tsens_out = 0;
  temperature_sensor_config_t tsens_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);
  temperature_sensor_handle_t tsens = NULL;
  
  if (temperature_sensor_install(&tsens_config, &tsens) == ESP_OK) {
    if (temperature_sensor_enable(tsens) == ESP_OK) {
      temperature_sensor_get_celsius(tsens, &tsens_out);
      temperature_sensor_disable(tsens);
    }
    temperature_sensor_uninstall(tsens);
  }
  return tsens_out;
}

void drawWiFiIcon(int16_t rssi) {
  oleg.rect(114, 6, 115, 7, OLED_FILL);
  
  if (rssi > -80) oleg.rect(118, 4, 119, 7, OLED_FILL);
  else            oleg.rect(118, 4, 119, 7, OLED_CLEAR); 
  
  if (rssi > -70) oleg.rect(122, 2, 123, 7, OLED_FILL);
  else            oleg.rect(122, 3, 123, 7, OLED_CLEAR);
  
  if (rssi > -60) oleg.rect(126, 0, 127, 7, OLED_FILL);
  else            oleg.rect(126, 0, 127, 7, OLED_CLEAR);
}

void setup() {
  Serial.begin(115200);                        
  delay(500); 
  Serial.println("\n[SYSTEM] Booting...");
  Serial.print("[SYSTEM] SDK Version: ");
  Serial.println(ESP.getSdkVersion()); 
  
  oleg.init(39, 40); 
  oleg.clear();                           
  oleg.setContrast(255);
  
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("esp32-time");
  WiFi.begin(ssid, pass); // Используем переменные     
  
  // Передаем правильное имя сети в функцию отрисовки экрана
  DisplayStart(ssid); 
  
  while (WiFi.status() != WL_CONNECTED) {   
    delay(200); 
  }
  
  Serial.println("[SYSTEM] WiFi Connected!");
  oleg.clear();
  
  NTP.begin(3);              
  NTP.setPeriod(30);        
}

void loop() {
  if (NTP.tick()) {
    Ntc_Time = NTP.getUnix();      
    TimeMath();                   
    Debug();                     
    DisplayPrint();             
  }
}

void DisplayPrint() {
  oleg.setScale(1);
  oleg.setCursor(0, 0);
  oleg.print("Времени прошло");
  
  oleg.line(0, 10, 127, 10, 127);
  
  drawWiFiIcon(WiFi.RSSI());

  oleg.setCursor(1, 2);   oleg.print("Лет -     ");   oleg.setCursor(65, 2);  oleg.print(String(Years) + "   ");
  oleg.setCursor(1, 3);   oleg.print("Месяцев - ");   oleg.setCursor(65, 3);  oleg.print(String(Months) + "   ");
  oleg.setCursor(1, 4);   oleg.print("Дней -    ");   oleg.setCursor(65, 4);  oleg.print(String(Days) + "   ");
  
  oleg.setCursor(1, 5);   oleg.print("Часов -   ");   oleg.setCursor(65, 5);  
  if (Hours < 10) oleg.print("0"); oleg.print(String(Hours) + "   ");
  
  oleg.setCursor(1, 6);   oleg.print("Минут -   ");   oleg.setCursor(65, 6);  
  if (Minutes < 10) oleg.print("0"); oleg.print(String(Minutes) + "   ");
  
  oleg.setCursor(1, 7);   oleg.print("Секунд -  ");   oleg.setCursor(65, 7);  
  if (Seconds < 10) oleg.print("0"); oleg.print(String(Seconds) + "   ");
}

void DisplayStart(const char* wifi_name) {
  oleg.clear();
  oleg.setScale(1);
  
  // Строка 0: Статус
  oleg.setCursor(0, 0);
  oleg.print("Подключение к WiFi..."); 
  
  // Строка 1: Имя сети берется напрямую из настроек
  oleg.setCursor(0, 1);
  oleg.print("SSID: ");
  oleg.print(wifi_name);
  
  // Строка 3-4: Компактный вывод MAC
  oleg.setCursor(0, 3);
  oleg.print("MAC-адрес устройства:");
  oleg.setCursor(0, 4);
  oleg.print(WiFi.macAddress()); 
  
  // Строка 6: Модель процессора
  oleg.setCursor(0, 6);
  oleg.print("Chip: ");
  oleg.print(ESP.getChipModel()); 
  
  // Строка 7: Температура
  oleg.setCursor(0, 7);
  oleg.print("Temp: ");
  oleg.print(getChipTemperature(), 1); 
  oleg.print(" C");
}

void TimeMath() {
  if (Ntc_Time < TimeCounting) return; 

  Time_passed = (Ntc_Time - TimeCounting); 
  time_t passed_t = (time_t)Time_passed;
  struct tm *t = gmtime(&passed_t);

  Years   = t->tm_year - 70; 
  Months  = t->tm_mon;       
  Days    = t->tm_mday - 1;  
  Hours   = t->tm_hour;
  Minutes = t->tm_min;
  Seconds = t->tm_sec;
}

void Debug() {
  Serial.printf("[%02d:%02d:%02d] SSID: %s | RSSI: %d dBm | Temp: %.1fC | Free Heap: %d KB\n", 
                Hours, Minutes, Seconds, ssid, WiFi.RSSI(), getChipTemperature(), ESP.getFreeHeap() / 1024);
}
