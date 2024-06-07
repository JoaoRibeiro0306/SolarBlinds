#include <SerialUtil.h>

#include <TimeLib.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);

#include <SolarCalculator.h>

#define ANALOG_IN_PIN A0


//Global Variables
double latitude = 38.736860;     // Latitude do técnico
double longitude = -9.137392;  // Longitude do técnico
double current_angle=0; //Current prependicular angle of the panel
int ctr_a =9;
int ctr_b =8;
int ctr_c =11;
int ctr_d =10;
int t=1500;
 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
float in_current = 0.0;
float in_power = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value = 0;

int a = 0;

String mode_rec = "1";
String elevation_rec;

//Gets the rest for the elevation while there is not a rotating notch 
double elevation_rest = 0;

double elevation=0;

SerialUtil mySerial_bool;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(ctr_a,OUTPUT);
  pinMode(ctr_b,OUTPUT);
  pinMode(ctr_c,OUTPUT);
  pinMode(ctr_d,OUTPUT);

  // Configure the baudrate of the serial communication.
  mySerial_bool.setBaudrate(9600);
  // Configure the wait time between the different data sends (a little pause).
  mySerial_bool.changeWaitTime(20);
  // Configure the start communication mode.
  // You can't define the same start mode in both Arduino sketches.
  mySerial_bool.startMode(MODE_SEND);
  // Configure the communication type: send, receive or send and receive.
  // .comType(enableSend, enableRecieve)
  mySerial_bool.comType(true,true);
  // Configure the timeout time for a new request.
  mySerial_bool.setTimeOut(2000);
  // Configure the time when the communication mode change from send to receive or from receive to send.
  mySerial_bool.setTimeChangeCom(5000);

  while(mySerial.available()==0){
    Serial.println("Connecting");
    delay(1000);
  }

  int index = 0;
  String Time = mySerial.readString();
  while(Time.indexOf("TIME") == -1){
    String Time = mySerial.readString();
  }

  index = Time.indexOf("TIME");
  String Time_prnt = Time.substring(index, index+4);
  Serial.print("Time = ");
  Serial.println(Time_prnt);
  String hr_s = Time.substring(index+5, index+7);
  Serial.print("Hora = ");
  Serial.println(hr_s);
  String min_s = Time.substring(index+8, index+10);
  Serial.print("Minuto = ");
  Serial.println(min_s);
  String sec_s = Time.substring(index+11, index+13);
  Serial.print("Segundo = ");
  Serial.println(sec_s);

  String dy_s;
  String mnth_s;
  String yr_s;

  if(Time.indexOf("DATE") == -1){
    
    while(mySerial.available()==0){
      Serial.println("Waiting");
      delay(1000);
    }
    String Date = 'D' + mySerial.readStringUntil(':');
    Serial.println(Date);
    dy_s = mySerial.readStringUntil('/');
    Serial.println(dy_s);
    mnth_s = mySerial.readStringUntil('/');
    Serial.println(mnth_s);
    yr_s = mySerial.readStringUntil('/');
    Serial.println(yr_s);
  }

  else{
    index = Time.indexOf("DATE");
    String Date_prnt = Time.substring(index, index+4);
    Serial.print("Date = ");
    Serial.println(Date_prnt);
    dy_s = Time.substring(index+5, index+7);
    Serial.print("Dia = ");
    Serial.println(dy_s);
    mnth_s = Time.substring(index+8, index+10);
    Serial.print("Mês = ");
    Serial.println(mnth_s);
    yr_s = Time.substring(index+11, index+15);
    Serial.print("Ano = ");
    Serial.println(yr_s);
  }
  

  // Set time manually (hr, min, sec, day, mo, yr)
  setTime(hr_s.toInt(), min_s.toInt(), sec_s.toInt(), dy_s.toInt(), mnth_s.toInt(), yr_s.toInt());
  
  delay(1);
}

void loop() {

  if(mySerial.available()!= 0){
    mode_rec = mySerial.readStringUntil('\n');
    mode_rec = mode_rec.substring(1,2);
    elevation_rec = mySerial.readStringUntil('\n');
    elevation_rec = elevation_rec.substring(2,4);  
  }
  
  if(mode_rec != "0" && mode_rec != "1"){
    mode_rec = "1";
  }

  else{
     elevation = elevation_rec.toFloat();
  }
 
   Serial.print("Mode selector = ");
   Serial.println(mode_rec); 
   

   Serial.print("Elevation = ");
   Serial.println(elevation_rec);
   
  if(mode_rec=="1"){
    automatic_mode();
  }
  else{
    rotate_blinds(elevation);
  }
  delay(1000);
}

void rotate_downwards(int rotating_notches){
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

void rotate_upwards(int rotating_notches){
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

void calc_power(){
  // Read the Analog Input
   adc_value = analogRead(ANALOG_IN_PIN);
   
   // Determine voltage at ADC input
   adc_voltage  = (adc_value * ref_voltage) / 1024.0; 
   
   // Calculate voltage, current and power at divider input
   in_voltage = adc_voltage / (R2/(R1+R2)); 
   in_current = adc_voltage / R2;
   in_power = in_voltage * in_current;
   
   // Print results to Serial Monitor to 2 decimal places
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);
  Serial.print("Input Current = ");
  Serial.println(in_current, 2);
  Serial.print("Input Power = ");
  Serial.println(in_power, 2);
}

void automatic_mode(){
  time_t utc = now(); //Get the time and date of the measurement
  double azimuth = 0, elevation = 0; //Gamma is the degrees that the panel has to turn to maximize its power production

  calcHorizontalCoordinates(utc, latitude, longitude, azimuth, elevation); //Calculate the azimuth and elevation of the sun
  Serial.print("Elevation = ");
  Serial.println(elevation);
  rotate_blinds(elevation); 
}

void rotate_blinds(double elevation){
  double gamma=0, gamma_aux=0;
  int rotating_notches = 0; //Amount of notches that need to be rotated to turn gamma degrees
  double not_rotating_notches = 0;

  gamma = elevation - current_angle;
  Serial.print("Gamma = ");
  Serial.println(gamma);
  
  if(abs(gamma) < 0.71){
    return;
  }
  current_angle = elevation; 

  if(gamma > 0){ //The angle of the panel is getting smaller by minus gamma
    gamma_aux = gamma;
    
    if(gamma > 60){
      gamma = gamma/2;  
    }

    rotating_notches = round((gamma*512)/360); //Calculating what are the number of notches that need to be turned for the panel to rotate gamma degrees
    
    rotate_upwards(rotating_notches);

    if(gamma_aux > 60){
      rotating_notches = round((gamma*512)/360); //Calculating what are the number of notches that need to be turned for the panel to rotate gamma degrees

      rotate_upwards(rotating_notches);
    }
  }

  if(gamma < 0){ //The angle of the panel is getting bigger by gamma
    gamma = abs(gamma); //Getting the absolute value of gamma
    gamma_aux = gamma;

    if(gamma > 60){
      gamma = gamma/2;
    }

    rotating_notches = round((gamma*512)/360); //Calculating what are the number of notches that need to be turned for the panel to rotate gamma degrees

    rotate_downwards(rotating_notches);

    if(gamma_aux > 60){
      rotating_notches = round((gamma*512)/360); //Calculating what are the number of notches that need to be turned for the panel to rotate gamma degrees
  
      rotate_downwards(rotating_notches);
    }
  }
}