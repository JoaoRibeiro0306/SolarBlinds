#include <TimeLib.h>

#include <SolarCalculator.h>

//Global Variables
double latitude = 38.736860;     // Latitude do técnico
double longitude = -9.137392;  // Longitude do técnico
double current_angle=0; //Current prependicular angle of the panel
int ctr_a =9;
int ctr_b =8;
int ctr_c =11;
int ctr_d =10;
int t=1500;

void setup() {
  Serial.begin(9600);
  pinMode(ctr_a,OUTPUT);
  pinMode(ctr_b,OUTPUT);
  pinMode(ctr_c,OUTPUT);
  pinMode(ctr_d,OUTPUT); 

  String Time = Serial.readString();
  String Date = Serial.readString();

  String hr_s, min_s, sec_s, dy_s, mnth_s, yr_s;

  hr_s = String(Time[5] + Time[6]);
  min_s = String(Time[8] + Time[9]);
  sec_s = String(Time[11] + Time[12]);
  dy_s = String(Date[5] + Date[6]);
  mnth_s = String(Date[8] + Date[9]);
  yr_s = String(Date[11] + Date[12] + Date[13] + Date[14]);

  // Set time manually (hr, min, sec, day, mo, yr)
  setTime(hr_s.toInt(), min_s.toInt(), sec_s.toInt(), dy_s.toInt(), mnth_s.toInt(), yr_s.toInt());

  delay(1);

}

void loop() {
  time_t utc = now(); //Get the time and date of the measurement
  double azimuth, elevation, gamma=0; //Gamma is the degrees that the panel has to turn to maximize its power production
  int rotating_notches = 0; //Amount of notches that need to be rotated to turn gamma degrees

  calcHorizontalCoordinates(utc, latitude, longitude, azimuth, elevation); //Calculate the azimuth and elevation of the sun

  Serial.print(elevation);

  gamma = current_angle - elevation;

  if(gamma > 0){ //The angle of the panel is getting smaller by minus gamma
    rotating_notches = round((gamma*64)/360); //Calculating what are the number of notches that need to be turned for the panel to rotate gamma degrees

    for(int i=rotating_notches;i>=1;i--){ //Rotate the panel
       digitalWrite(ctr_a,LOW);//A
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,HIGH);
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,LOW);
       digitalWrite(ctr_b,LOW);//AB
       digitalWrite(ctr_c,HIGH);
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,LOW);//B
       digitalWrite(ctr_c,HIGH);
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,LOW);
       digitalWrite(ctr_c,LOW);//BC
        digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,LOW);//C
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,LOW);//CD
       digitalWrite(ctr_d,LOW);
       delayMicroseconds(t);
        digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,HIGH);//D
       digitalWrite(ctr_d,LOW);
       delayMicroseconds(t);
       digitalWrite(ctr_a,LOW);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,HIGH);//DA
       digitalWrite(ctr_d,LOW);
       delayMicroseconds(t);      
    }
  }

  if(gamma < 0){ //The angle of the panel is getting bigger by gamma
    gamma = abs(gamma); //Getting the absolute value of gamma

    rotating_notches = round((gamma*64)/360); //Calculating what are the number of notches that need to be turned for the panel to rotate gamma degrees

    for(int i=rotating_notches;i>=1;i--){ //Rotate the panel in the other direction (To be determined)
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,HIGH);
       digitalWrite(ctr_d,LOW);//D
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,LOW);//CD
       digitalWrite(ctr_d,LOW);
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,LOW);//C
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,LOW);
       digitalWrite(ctr_c,LOW);//BC
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,HIGH);
       digitalWrite(ctr_b,LOW);//B
       digitalWrite(ctr_c,HIGH);
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,LOW);
       digitalWrite(ctr_b,LOW);//AB
       digitalWrite(ctr_c,HIGH);
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,LOW);//A
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,HIGH);
       digitalWrite(ctr_d,HIGH);
       delayMicroseconds(t);
       digitalWrite(ctr_a,LOW);
       digitalWrite(ctr_b,HIGH);
       digitalWrite(ctr_c,HIGH);//DA
       digitalWrite(ctr_d,LOW);
       delayMicroseconds(t);      
    }
  }

}
