// NodeMCU (ESP8266)            MPU6050
// -------                      -------
// 3.3V       ------------>       VCC
// GND        ------------>       GND
// D1         ------------>       SCL
// D2         ------------>       SDA

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Settings for earthquake detection
const float gravitationalAccel = 9.80665; // Gravitational acceleration (m/s²)
float previousNetAccel = 0;               // Previous net acceleration value
float accelThreshold = 0.85;              // Threshold value (m/s²)
unsigned long durationThreshold = 500;    // Threshold for earthquake duration (ms)

// Settings for running variables
float readingSensitivity = 0.05; // Reading sensitivity (m/s²)
unsigned long startTime = 0;
int loopDelay = 200; // Loop delay (ms)

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  // Initialize the MPU6050 sensor
  if (!mpu.begin())
  {
    Serial.println("MPU6050 connection failed!");
    while (1)
      ;
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // Set the accelerometer range
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);      // Set the gyro range
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);  // Set the filter bandwidth

  Serial.println("MPU6050 ready, starting to read data...");
}

void checkEarthquake()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate total acceleration
  float totalAccel = sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2));
  float netAccel = abs(totalAccel - gravitationalAccel); // Calculate net acceleration

  // Check if the net acceleration value has changed by more than 5%
  if (abs(netAccel - previousNetAccel) > previousNetAccel * readingSensitivity)
  {
    Serial.print("Net Acceleration: ");
    Serial.print(netAccel);
    Serial.println(" m/s²");
    previousNetAccel = netAccel; // Update previous net acceleration value
  }

  // Earthquake detection
  if (netAccel > accelThreshold)
  {
    if (startTime == 0)
    {
      startTime = millis(); // Set the start time of the earthquake
    }
  }
  else
  {
    startTime = 0; // No movement
  }

  // Detect earthquake if high acceleration is detected for a certain duration
  if (startTime > 0 && millis() - startTime > durationThreshold)
  {
    Serial.println("Earthquake Detected!");
    // Alarm, logging, etc. can be done here
  }
}

void loop()
{
  checkEarthquake(); // Call the earthquake check function

  // Other operations or conditions can be added here
  delay(loopDelay); // Short delay
}