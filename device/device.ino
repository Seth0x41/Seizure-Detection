
// import libs
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "WiFi.h" 
#include "HTTPClient.h" 
#include <WiFiManager.h> 


// instance of sensors
MAX30105 particleSensor;
Adafruit_MPU6050 mpu;
WiFiManager wifiManager;


// initalize Send Req Task Handler
TaskHandle_t SendRequests;

// heart Rate Sensor
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;


bool iswearing=true;
// if heart problem triggerd
bool triggerd = false;
// if the wifi connected
bool res;

// MPU and Fall detection 

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;

boolean fall = false; //stores if a fall has occurred

boolean trigger1=false; //stores if first trigger (lower threshold) has occurred
boolean trigger2=false; //stores if second trigger (upper threshold) has occurred
boolean trigger3=false; //stores if third trigger (orientation change) has occurred

byte trigger1count=0; //stores the counts past since trigger 1 was set true
byte trigger2count=0; //stores the counts past since trigger 2 was set true
byte trigger3count=0; //stores the counts past since trigger 3 was set true

int angleChange=0;
long mpu_t=0;

int counter=0;



int make_post_request(String url, String request_body) {
  
   HTTPClient http;
  http.begin(url); //Specify destination for HTTP request
  http.addHeader("Content-Type", "application/json");


  delay(1000);
  int httpResponseCode = http.POST(request_body);
    if(httpResponseCode>0){
    String response = http.getString();  //Get the response to the request
   }else{
  
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  
   }
  http.end();

}


// Task Function
void POSTREQ(void* pvParameters){
  setup1();

  for(;;)
    loop1();
}




void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    POSTREQ,               /* Task function. */
    "loop1",                /* name of task. */
    10000,                  /* Stack size of task */
    NULL,                   /* parameter of the task */
    1,                      /* priority of the task */
    &SendRequests,            /* Task handle to keep track of created task */
    0); /* pin task to core */

//  initialize heart rate sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED


// initialize MPU sensor

 Wire.begin();
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);  // PWR_MGMT_1 register
 Wire.write(0);     // set to zero (wakes up the MPU-6050)
 Wire.endTransmission(true);

}


void setup1(){

     WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    WiFiManager wm;
  //first parameter is name of access point, second is the password
  
  res = wm.autoConnect("Seizure-Detection","12345678"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi
        res = true;
        Serial.println("connected...yeey :)");
    }

}



void loop(){

    // income MAX30102 Data

  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  if (irValue < 50000){
    beatsPerMinute = 0;
    beatAvg=0;
    iswearing=false;} else{
      iswearing =true;
    }

if (millis()-mpu_t>100){
  mpu_t=  millis();



  // income MPU6050 Data
 mpu_read();
 ax = (AcX-2050)/16384.00;
 ay = (AcY-77)/16384.00;
 az = (AcZ-1947)/16384.00;
 gx = (GyX+270)/131.07;
 gy = (GyY-351)/131.07;
 gz = (GyZ+136)/131.07;
 // calculating Amplitute vactor for 3 axis
 float Raw_Amp = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
 int Amp = Raw_Amp * 10;  // Mulitiplied by 10 bcz values are between 0 to 1
 if (Amp<=2 && trigger2==false){ //if AM breaks lower threshold (0.4g)
   trigger1=true;
   Serial.println("TRIGGER 1 ACTIVATED");
   }
 if (trigger1==true){
   trigger1count++;
   if (Amp>=12){ //if AM breaks upper threshold (3g)
     trigger2=true;
     Serial.println("TRIGGER 2 ACTIVATED");
     trigger1=false; trigger1count=0;
     }
 }
 if (trigger2==true){
   trigger2count++;
   angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5); Serial.println(angleChange);
   if (angleChange>=30 && angleChange<=400){ //if orientation changes by between 80-100 degrees
     trigger3=true; trigger2=false; trigger2count=0;
     Serial.println(angleChange);
     Serial.println("TRIGGER 3 ACTIVATED");
       }
   }
 if (trigger3==true){
    trigger3count++;
    if (trigger3count>=10){ 
       angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);
       Serial.println(angleChange); 
       if ((angleChange>=0) && (angleChange<=10)){ //if orientation changes remains between 0-10 degrees
           fall=true; trigger3=false; trigger3count=0;
           Serial.println(angleChange);
             }
       else{ //user regained normal orientation
          trigger3=false; trigger3count=0;
          Serial.println("TRIGGER 3 DEACTIVATED");
       }
     }
  }

 if (trigger2count>=6){ //allow 0.5s for orientation change
   trigger2=false; trigger2count=0;
   Serial.println("TRIGGER 2 DECACTIVATED");
   }
 if (trigger1count>=6){ //allow 0.5s for AM to break upper threshold
   trigger1=false; trigger1count=0;
   Serial.println("TRIGGER 1 DECACTIVATED");
   }
}  
 }


void loop1(){


  // Send the BPM data to the server via the API
  if(res==true){   //Check WiFi connection status

    if (iswearing == false){
    beatAvg = 0;
    ax=0;
    ay=0;
    az=0;
  }
    String url = "http://192.168.1.9:5000/api/send-data";
    String data = "{\"BPM\": " + String(beatAvg) + ",\"iswearing\": " + String(iswearing) + "}";
    make_post_request(url,data);
 
    // Conditions for Detection
    if (iswearing == true && (beatAvg > 100 || beatAvg < 60)){
      counter += 1;
      delay(500);
      if (counter >= 2){
        triggerd = true;
          String url = "http://192.168.1.9:5000/webhook/triggerd";
          String data = "{\"message\": \"Heart Problem!\"}";
          make_post_request(url,data);
          counter = 0;
          if (fall != true){
            triggerd = false;
          }

      }
    }
        if(fall==true && iswearing == true && (triggerd == true) ){
          String url = "http://192.168.1.9:5000/webhook/triggerd";
          String data = "{\"message\": \"Seizure Detected!\"}";
          make_post_request(url,data);
          triggerd = false;
          delay (60000)

      }

 if (fall==true){ //in event of a fall detection
   Serial.println("FALL DETECTED");
        String url = "http://192.168.1.9:5000/webhook/triggerd";
        String data = "{\"message\": \"FALL DETECTED!\"}";
          make_post_request(url,data);
   fall=false;
   }
  }
  else{
    Serial.println("Error in WiFi connection");   
 }
  
}



void mpu_read(){
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom((uint16_t)MPU_addr,(uint8_t)14,true);  // request a total of 14 registers
 AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
 AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
 AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
 Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
 GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
 GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
 GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 }
