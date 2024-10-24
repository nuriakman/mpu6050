#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection())
  {
    Serial.println("MPU6050 bağlantı başarısız");
    while (1)
      ;
  }
}

void loop()
{
  // Hızlanma ve jiroskop verilerini oku
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Sensör verilerini float değerlerine dönüştür
  float AccX = ax / 16384.0;
  float AccY = ay / 16384.0;
  float AccZ = az / 16384.0;

  float GyroX = gx / 131.0;
  float GyroY = gy / 131.0;
  float GyroZ = gz / 131.0;

  // Roll ve Pitch hesapla (Radyan cinsinden)
  float roll = atan2(AccY, AccZ) * 180 / PI;
  float pitch = atan2(-AccX, sqrt(AccY * AccY + AccZ * AccZ)) * 180 / PI;

  // Yaw'u entegre ile hesaplayın (cihazın başlangıç açısına göre)
  static float yaw = 0;
  float dt = 0.01;   // Örnekleme süresi (10ms)
  yaw += GyroZ * dt; // Yaw jiroskop verisinden hesaplanır

  // Verileri serial ile yazdır
  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print(" Pitch: ");
  Serial.print(pitch);
  Serial.print(" Yaw: ");
  Serial.println(yaw);

  delay(10); // Ölçüm hızı
}