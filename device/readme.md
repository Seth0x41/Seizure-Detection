# introduction
This code is an ESP32 script that utilizes various libraries to collect data from multiple sensors and send it to a remote server through HTTP POST requests. The script uses a MAX30105 heart rate sensor to measure the heart rate, an Adafruit MPU6050 accelerometer/gyroscope sensor to detect falls, and WiFiManager to connect to a wifi network. The data is sent to a remote server using the HTTPClient library. The code is divided into several functions, such as setup(), loop(), setup1(), loop1(), make_post_request() and POSTREQ(). The setup() function is used to initialize the sensors and WiFi, and the loop() function is used to read sensor data and perform calculations. The setup1() and loop1() functions are used to handle the sending of data to the server, and make_post_request() and POSTREQ() are used to construct the HTTP request and send it to the server. The script is designed to detect falls, heart problem, and send notifications to a remote server.

# Technologies
Built with ESP32, MPU6050, MAX30102, WiFiManager, ESP32 Processor's Dual Core, HTTPClient

# Features
- The code is designed to send notifications to a remote server, it could be used for remote monitoring and early warning systems.
- Reading data from a MAX30105 heart rate sensor and an Adafruit MPU6050 accelerometer/gyroscope sensor
- Calculating heart rate from the sensor data and detecting falls.
- Connecting to a WiFi network through WiFiManager.
- Sending data to a remote server using HTTP POST requests.
- Using task to handle sending requests in parallel with the main loop
- Detecting falls, heart problem, and sending notifications to a remote server.
- Utilizing libraries such as Wire, MAX30105, Adafruit_MPU6050, Adafruit_Sensor, WiFi, HTTPClient, and WiFiManager to simplify the code and make it easier to work with sensors and network communication.
- using the hardware of ESP32 board to measure heart rate, fall and detect heart problem

> **IMPORTANT NOTE: This code is for educational purposes and there are many security issues with it such as there is no authentication or encryption while sending data and the code doesn't handle many cases such as losing connection, etc.** 
