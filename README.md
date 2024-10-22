# NodeMCU 1.0, ESP8266-12E

- USB bağlantısı için için cp2102 Kullanır
- **.h Dosyası:** Tanımlar (prototipler ve sınıf bildirimleri) içerir; başka dosyalarda kullanılmak üzere içe aktarılır.
- **.cpp Dosyası:** Fonksiyonların ve sınıfların tanımlarını içerir; derleme aşamasında çalıştırılabilir dosya oluşturmak için kullanılır.

## İlk yapılacak

Bu komut çalıştırıldıktan sonra bilgisayar yeniden başlatılmalıdır.

```bash
sudo usermod -aG dialout $USER
```

## Bağlantı kontrolü

ESP8266'nın USB kablosu ile bilgisayara bağlanmasının ardından şu işlemler yapılmalı:

- İlk bağlantı ile birlikte ESP8266 üzerindeki led 1 defa yanıp söner
- Bu olmuyorsa ESP'de veya Kabloda sorun var demektir
- Bağlantının ve portun aşağıdaki komutlar ile kontrol edilmesi

```bash
lsusb
# Örnek çıktı: Bus 002 Device 004: ID 10c4:ea60 Silicon Labs CP210x UART Bridge

ls /dev/ttyUSB*
# Örnek çıktı: /dev/ttyUSB0
```

## Örnek platformio.ini

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 115200
```

## Örnek config.h

```cpp
// config.h
#ifndef CONFIG_H
#define CONFIG_H

const char *ssid = "TURKSAT-KABLONET-F5BF-2.4G"; // WiFi SSID
const char *password = "Akman123."; // WiFi Şifresi

#endif // CONFIG_H
```

## Örnek wifi_utils.cpp

```cpp
// wifi_utils.cpp
#include "wifi_utils.h"
#include <Arduino.h>

void scanWiFiNetworks() {
  WiFi.disconnect(); // Herhangi bir eski bağlantıyı kapat
  delay(100);

  Serial.println("");
  Serial.println("");
  Serial.println("Çevredeki WiFi ağları taranıyor...");

  int n = WiFi.scanNetworks(); // WiFi ağlarını tara
  if (n == 0) {
    Serial.println("Hiçbir WiFi ağı bulunamadı.");
  } else {
    Serial.println("Bulunan WiFi ağları:");
    for (int i = 0; i < n; ++i) {
      Serial.printf("%d: %s, Sinyal Gücü: %d dBm\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
  }
}

```

## Örnek main.cpp

```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h" // config.h dosyasını dahil et
#include "wifi_utils.h" // wifi_utils.h dosyasını dahil et
#include "wifi_utils.cpp" // wifi_utils.h dosyasını dahil et

// Diğer fonksiyon ve kodlar...

void loop() {
  checkConnection(); // Bağlantı durumunu kontrol et
  sendHelloWorld();  // "Hello World" mesajını gönder
  scanWiFiNetworks(); // WiFi ağlarını tara
  counter++;         // counter değerini artır
  delay(5000);      // 5 saniye bekle
}

```

## Bazı temel fonksiyonlar

```cpp
void scanWiFiNetworks()
{
  // WiFi.disconnect(); // Herhangi bir eski bağlantıyı kapat
  delay(100);

  Serial.println("");
  Serial.println("");
  Serial.println("Çevredeki WiFi ağları taranıyor...");

  int n = WiFi.scanNetworks(); // WiFi ağlarını tara
  if (n == 0)
  {
    Serial.println("Hiçbir WiFi ağı bulunamadı.");
  }
  else
  {
    Serial.println("Bulunan WiFi ağları:");
    for (int i = 0; i < n; ++i)
    {
      Serial.printf("%d: %s, Sinyal Gücü: %d dBm\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
  }
}

void connectToWiFi()
{
  Serial.printf("Bağlanıyor: %s\n", ssid);
  WiFi.begin(ssid, password); // WiFi'ye bağlan

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nBağlandı!");
}

void checkConnection()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Bağlantı kesildi, yeniden bağlanılıyor...");
    connectToWiFi(); // Yeniden bağlan
  }
  else
  {
    Serial.println("Bağlantı kuruldu.");
  }
}

```

## MPU6050 Kullanımı

platformio.ini

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 115200
lib_deps = MPU6050
```

```cpp
/*
VCC → NodeMCU 3.3V
GND → NodeMCU GND
SDA → NodeMCU D2 (GPIO 2)
SCL → NodeMCU D1 (GPIO 1)
*/
#include <Arduino.h>

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup()
{
  Serial.begin(115200);
  Wire.begin(D2, D1); // SDA -> D2, SCL -> D1
  mpu.initialize();   // MPU6050'i başlat

  Serial.println("MPU6050 Başlatıldı");
  Serial.print("MPU6050 Durumu: ");
  Serial.println(mpu.testConnection() ? "Bağlı" : "Bağlı Değil");
}

void loop()
{
  // Hız ve açı verilerini oku
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Verileri seri monitöre yazdır
  Serial.print("Hız: ");
  Serial.print("X: ");
  Serial.print(ax);
  Serial.print(" Y: ");
  Serial.print(ay);
  Serial.print(" Z: ");
  Serial.print(az);

  Serial.print("  Açı: ");
  Serial.print("X: ");
  Serial.print(gx);
  Serial.print(" Y: ");
  Serial.print(gy);
  Serial.print(" Z: ");
  Serial.println(gz);

  delay(100); // 1 saniye bekle
}
```

## Deprem Algılama

platformio.ini

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 115200
lib_deps = MPU6050
```

```cpp
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// İvme eşik değeri
const float threshold = 1.5;
const float hassasiyet = 4;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  // MPU6050'ün doğru bir şekilde çalıştığını kontrol et
  if (mpu.testConnection())
  {
    Serial.println("MPU6050 başarıyla bağlandı.");
  }
  else
  {
    Serial.println("MPU6050 bağlanamadı.");
  }
}

void loop()
{
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // İvme değerlerini hesapla (g cinsinden)
  float accelX = ax / 16384.0; // 16384.0, MPU6050'in duyarlılığı
  float accelY = ay / 16384.0;
  float accelZ = az / 16384.0;

  // Deprem algılama
  if (abs(accelX) > threshold || abs(accelY) > threshold || abs(accelZ) > threshold)
  {
    Serial.print("Deprem algılandı! ");
    Serial.print("Zaman: ");
    Serial.print(millis()); // Geçen zamanı yazdır

    // Sensör değerlerini yazdır, ##.####
    Serial.print(" | Accel X: ");
    Serial.print((accelX >= 0 ? "+" : "") + String(accelX, hassasiyet)); // Pozitifse + işareti ekle
    Serial.print(" | Accel Y: ");
    Serial.print((accelY >= 0 ? "+" : "") + String(accelY, hassasiyet)); // Pozitifse + işareti ekle
    Serial.print(" | Accel Z: ");
    Serial.println((accelZ >= 0 ? "+" : "") + String(accelZ, hassasiyet)); // Pozitifse + işareti ekle

    // Bir süre bekle (örneğin 1 saniye)
    // delay(1000);
  }

  delay(10); // CPU kullanımını azaltmak için biraz bekle
}
/*
ÖRNEK ÇIKTI:
Deprem algılandı! Zaman: 40844 | Accel X: +1.9999 | Accel Y: +1.4460 | Accel Z: -2.0000
Deprem algılandı! Zaman: 40856 | Accel X: +1.9999 | Accel Y: -0.3904 | Accel Z: -2.0000
Deprem algılandı! Zaman: 40924 | Accel X: -2.0000 | Accel Y: -0.1538 | Accel Z: +1.9999
Deprem algılandı! Zaman: 40946 | Accel X: -0.1777 | Accel Y: +0.2437 | Accel Z: +1.5254
*/
```

## Başarılı GET

```cpp
#include <ESP8266WiFi.h>

const char *ssid = "TURKSAT-KABLONET-F5BF-2.4G"; // WiFi SSID
const char *password = "Akman123.";              // WiFi Şifresi

const char *host = "192.168.0.27";                    // Sunucu adresi (https olmadan)
//const char *host = "73e7-94-54-60-90.ngrok-free.app"; // Sunucu adresi (https olmadan)

void setup()
{
  Serial.begin(115200);
  delay(10);

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
  WiFiClient client;

  // Sunucuya bağlanma
  Serial.print("Sunucuya bağlanılıyor: ");
  Serial.println(host);

  if (client.connect(host, 80))
  { // HTTP için port 80 kullanılır
    Serial.println("Bağlantı başarılı!");

    // HTTP GET isteğini gönder
    client.print(String("GET / HTTP/1.1\r\n") +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    Serial.println("GET isteği gönderildi.");

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

  delay(2000); // 2 saniye bekle
}
```

## Başarılı MySQL kaydı

```sql
SELECT created_at, COUNT(1) AS veri_adedi
FROM mpu6050_data
GROUP BY created_at
ORDER BY created_at DESC
LIMIT 10
```

```sql
CREATE TABLE mpu6050_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ax INT NOT NULL,
    ay INT NOT NULL,
    az INT NOT NULL,
    gx INT NOT NULL,
    gy INT NOT NULL,
    gz INT NOT NULL,
    temp INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

```

```php
<?php
$servername = "localhost"; // Sunucu adresi
$username = "root"; // MySQL kullanıcı adı
$password = "root"; // MySQL şifresi
$dbname = "mpu6050"; // Veritabanı adı

// MySQL bağlantısı oluştur
$conn = new mysqli($servername, $username, $password, $dbname);

// Bağlantıyı kontrol et
if ($conn->connect_error) {
  die("Bağlantı hatası: " . $conn->connect_error);
}

// GET isteğinden gelen verileri al
$ax = $_GET['ax'];
$ay = $_GET['ay'];
$az = $_GET['az'];
$gx = $_GET['gx'];
$gy = $_GET['gy'];
$gz = $_GET['gz'];
$temp = $_GET['temp'];

// SQL sorgusu oluştur ve veriyi kaydet
$sql = "INSERT INTO mpu6050_data (ax, ay, az, gx, gy, gz, temp)
VALUES ('$ax', '$ay', '$az', '$gx', '$gy', '$gz', '$temp')";

if ($conn->query($sql) === TRUE) {
  $last_id = $conn->insert_id;
  echo "Kayıt No: " . $last_id;
} else {
  echo "Hata: " . $sql . "<br>" . $conn->error;
}

// Bağlantıyı kapat
$conn->close();

```

```cpp
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const char *ssid = "TURKSAT-KABLONET-F5BF-2.4G"; // WiFi SSID
const char *password = "Akman123.";              // WiFi Şifresi

const char *host = "192.168.0.27"; // Sunucu adresi
const int httpPort = 80;           // HTTP için port numarası

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
  WiFiClient client;

  // MPU6050'den verileri oku
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t tempRaw;
  float temperature;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Sıcaklık verisini al
  tempRaw = mpu.getTemperature();
  temperature = (tempRaw / 340.0) + 36.53; // MPU6050 sıcaklık hesaplaması

  // Sunucuya bağlanma
  Serial.print("Sunucuya bağlanılıyor: ");
  Serial.println(host);

  if (client.connect(host, httpPort))
  {
    Serial.println("Bağlantı başarılı!");

    // GET isteği için verileri hazırla
    String url = "/6050.php?ax=" + String(ax) + "&ay=" + String(ay) + "&az=" + String(az) +
                 "&gx=" + String(gx) + "&gy=" + String(gy) + "&gz=" + String(gz) +
                 "&temp=" + String(temperature); // Sıcaklık verisi GET parametrelerine eklendi

    // HTTP GET isteğini gönder
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("GET isteği gönderildi: " + url);

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

  delay(200); // 2 saniye bekle
}

```
