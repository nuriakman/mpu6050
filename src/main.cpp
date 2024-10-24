#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
}

unsigned long previousMillis = 0; // Son işlem zamanı
const long interval = 1000;       // 1 saniye (1000 ms)
int c = 0;

char buffer[100];
int16_t ax, ay, az, gx, gy, gz;

void loop()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // sprintf(buffer, "%d:%d:%d:%d:%d:%d", ax, ay, az, gx, gy, gz);
  sprintf(buffer, "AX: %d AY: %d AZ: %d GX: %d GY: %d GZ: %d", ax, ay, az, gx, gy, gz);
  Serial.println(buffer);

  delay(100); // Veri okuma sıklığını ayarlayın
}
