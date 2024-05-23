// ESP8266 ESP-01 Internet real time clock
 
 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
#include <NTPClient.h>               // Include NTPClient library
#include <TimeLib.h>                 // Include Arduino time library

 
const char *ssid     = "MEO-699980";
const char *password = "bd2453658d";
 
WiFiUDP ntpUDP;
 
// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
NTPClient timeClient(ntpUDP, "time.nist.gov", 3600, 60000);
 
char Time[] = "TIME:00:00:00";
char Date[] = "DATE:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
int a=0;
 
void setup() {
 
  Serial.begin(9600);
 
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
 
  timeClient.begin();
}
 
void loop() {
  
  if (a == 0) {
    write_time();
    a = 1;
  }
  
 
  delay(1000);
 
}

void write_time(){
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server

  second_ = second(unix_epoch);
  minute_ = minute(unix_epoch);
  hour_   = hour(unix_epoch);
  day_    = day(unix_epoch);
  month_  = month(unix_epoch);
  year_   = year(unix_epoch);

  Time[12] = second_ % 10 + 48;
  Time[11] = second_ / 10 + 48;
  Time[9]  = minute_ % 10 + 48;
  Time[8]  = minute_ / 10 + 48;
  Time[6]  = hour_   % 10 + 48;
  Time[5]  = hour_   / 10 + 48;

  Date[5]  = day_   / 10 + 48;
  Date[6]  = day_   % 10 + 48;
  Date[8]  = month_  / 10 + 48;
  Date[9]  = month_  % 10 + 48;
  Date[13] = (year_   / 10) % 10 + 48;
  Date[14] = year_   % 10 % 10 + 48;

  // Send time and date to serial monitor
  //Serial.println(Time);
  //Serial.println(Date);

  Serial.write(Time);
  Serial.write(Date);
}