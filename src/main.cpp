#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// const char *ssid = "TURKSAT-KABLONET-F5BF-2.4G"; // WiFi SSID
// const char *password = "Akman123.";              // WiFi Şifresi

// const char *ssid = "TURKSAT-KABLONET-5G-7N4I";
const char *ssid = "TURKSAT-KABLONET-2.4G-IGy2";
const char *password = "ENo2BWI5q3Pd";

const char *host = "192.168.0.18"; // Sunucu adresi
const int httpPort = 80;           // HTTP için port numarası

int count = 0;               // Okuma sayısı
const int maxReadings = 200; // Toplu gönderim için okuma sayısı
int16_t axData[maxReadings];
int16_t ayData[maxReadings];
int16_t azData[maxReadings];
int16_t gxData[maxReadings];
int16_t gyData[maxReadings];
int16_t gzData[maxReadings];
float tempData[maxReadings];

// PrepareData fonksiyonu int16_t dizileri için
String prepareData(int16_t *dataArray)
{
  String data = "";
  for (int i = 0; i < maxReadings; i++)
  {
    data += String(dataArray[i]);
    if (i < maxReadings - 1)
    {
      data += ",";
    }
  }
  return data;
}

// PrepareData fonksiyonu float dizileri için
String prepareData(float *dataArray)
{
  String data = "";
  for (int i = 0; i < maxReadings; i++)
  {
    data += String(dataArray[i]);
    if (i < maxReadings - 1)
    {
      data += ",";
    }
  }
  return data;
}

void sendData()
{
  WiFiClient client;

  // Sunucuya bağlanma
  Serial.print("Sunucuya bağlanılıyor: ");
  Serial.println(host);

  if (client.connect(host, httpPort))
  {
    Serial.println("Bağlantı başarılı!");

    // POST isteği için verileri hazırla
    String postData = "ax=" + prepareData(axData) +
                      "&ay=" + prepareData(ayData) +
                      "&az=" + prepareData(azData) +
                      "&gx=" + prepareData(gxData) +
                      "&gy=" + prepareData(gyData) +
                      "&gz=" + prepareData(gzData) +
                      "&temp=" + prepareData(tempData);

    // HTTP POST isteğini gönder
    client.print(String("POST /mpu6050/php/6050post.php HTTP/1.1\r\n") +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: " + postData.length() + "\r\n" +
                 "Connection: close\r\n\r\n" +
                 postData);

    // Serial.println("POST isteği gönderildi: " + postData);
    Serial.println("POST isteği gönderildi.");

    // Sunucudan gelen yanıtı oku
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }

    // Bağlantıyı kapat
    client.stop();
    Serial.println("Bağlantı kapatıldı.");
  }
  else
  {
    Serial.println("Bağlantı başarısız!");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  // MPU6050 başlatma
  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection())
  {
    Serial.println("MPU6050 bağlantısı başarılı.");
  }
  else
  {
    Serial.println("MPU6050 bağlantısı başarısız.");
    while (1)
      ;
  }

  // Wi-Fi'ye bağlanma
  Serial.println();
  Serial.println("WiFi'ye bağlanılıyor...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nBağlantı sağlandı!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // MPU6050'den verileri oku
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t tempRaw;
  float temperature;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Sıcaklık verisini al
  tempRaw = mpu.getTemperature();
  temperature = (tempRaw / 340.0) + 36.53; // MPU6050 sıcaklık hesaplaması

  char buffer[200]; // Verileri tutmak için bir buffer
  sprintf(buffer, "Okunan Veriler: ax: %d ay: %d az: %d gx: %d gy: %d gz: %d temp: %.2f", ax, ay, az, gx, gy, gz, temperature);
  Serial.println(buffer);

  // Verileri dizilere kaydet
  axData[count] = ax;
  ayData[count] = ay;
  azData[count] = az;
  gxData[count] = gx;
  gyData[count] = gy;
  gzData[count] = gz;
  tempData[count] = temperature;

  count++;

  // Eğer 10 okumada bir gönderim yapıyorsak
  if (count >= maxReadings)
  {
    sendData();
    count = 0; // Okuma sayısını sıfırla
  }

  delay(20); // 200 ms bekle
}
