# max okuma testi

## 5 sn periyyotta max kaç okuma yapılıyor

```cpp
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int period = 5000; // 5 saniye
unsigned long previousMillis = 0;
int readingCount = 0;

struct SensorData
{
  int16_t accelX, accelY, accelZ;
  int16_t gyroX, gyroY, gyroZ;
};

const int maxReadings = 1000; // Dizi boyutunu küçült
SensorData *sensorReadings;   // Dinamik bellek

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection())
  {
    Serial.println("MPU6050 bağlantı hatası!");
    while (1)
      ;
  }
  sensorReadings = new SensorData[maxReadings]; // Dinamik bellek ayır
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= period)
  {
    Serial.print("5 saniyede okunan değer sayısı: ");
    Serial.println(readingCount);
    readingCount = 0;
    previousMillis = currentMillis;
  }

  if (readingCount < maxReadings)
  {
    sensorReadings[readingCount].accelX = mpu.getAccelerationX();
    sensorReadings[readingCount].accelY = mpu.getAccelerationY();
    sensorReadings[readingCount].accelZ = mpu.getAccelerationZ();
    sensorReadings[readingCount].gyroX = mpu.getRotationX();
    sensorReadings[readingCount].gyroY = mpu.getRotationY();
    sensorReadings[readingCount].gyroZ = mpu.getRotationZ();
    readingCount++;
  }
}

void cleanup()
{
  delete[] sensorReadings; // Bellek serbest bırak
}
```

## Bellek dolana kadar dizileri doldurmaya devam et ve her saniye okunan veri adedini, ve dolu/boş bellek miktarını ekrana yazdır.

```cpp
// #include <Arduino.h>
#include <MPU6050.h>
// #include <Wire.h>
// #include <vector> // Dinamik dizi için gerekli kütüphane

MPU6050 mpu;

const int period = 1000; // 1 saniye
unsigned long previousMillis = 0;

struct SensorData
{
  int16_t accelX, accelY, accelZ;
  int16_t gyroX, gyroY, gyroZ;
};

std::vector<SensorData> sensorReadings; // Dinamik dizi

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  unsigned long currentMillis = millis();

  // 1 saniye kontrolü
  if (currentMillis - previousMillis >= period)
  {
    // Okunan değer sayısını ve dolu/boş bellek miktarını yazdır
    Serial.print("Toplam okunan değer sayısı: ");
    Serial.println(sensorReadings.size());

    Serial.print("Dolu bellek: ");
    Serial.print(sensorReadings.size() * sizeof(SensorData)); // Dolu bellek miktarını yazdır
    Serial.print(" | Boş bellek: ");
    Serial.println((ESP.getFreeHeap() - (sensorReadings.size() * sizeof(SensorData)))); // Boş bellek miktarını yazdır

    previousMillis = currentMillis; // Zaman damgasını güncelle
  }

  // Bellek dolana kadar veri oku
  if (ESP.getFreeHeap() > sizeof(SensorData))
  { // Yeterli bellek kontrolü
    SensorData newReading;
    newReading.accelX = mpu.getAccelerationX();
    newReading.accelY = mpu.getAccelerationY();
    newReading.accelZ = mpu.getAccelerationZ();
    newReading.gyroX = mpu.getRotationX();
    newReading.gyroY = mpu.getRotationY();
    newReading.gyroZ = mpu.getRotationZ();

    sensorReadings.push_back(newReading); // Yeni veriyi ekle
  }
  else
  {
    // Bellek dolduğunda yazdır
    Serial.println("Bellek doldu! Daha fazla veri okunamaz.");
    while (true)
      ; // Bellek dolduğunda sonsuz döngüde kal
  }
  delay(20);
}

```
