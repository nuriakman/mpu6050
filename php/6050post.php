<?php
// Veritabanı bağlantısı için gerekli bilgiler
$servername = "localhost"; // Sunucu adresi
$username = "root"; // MySQL kullanıcı adı
$password = "root"; // MySQL şifresi
$dbname = "mpu6050"; // Veritabanı adı

// Veritabanı bağlantısı oluştur
$conn = new mysqli($servername, $username, $password, $dbname);

// Bağlantıyı kontrol et
if ($conn->connect_error) {
  die("Bağlantı hatası: " . $conn->connect_error);
}

// POST ile gelen verileri al
$ax = isset($_POST['ax']) ? $_POST['ax'] : '';
$ay = isset($_POST['ay']) ? $_POST['ay'] : '';
$az = isset($_POST['az']) ? $_POST['az'] : '';
$gx = isset($_POST['gx']) ? $_POST['gx'] : '';
$gy = isset($_POST['gy']) ? $_POST['gy'] : '';
$gz = isset($_POST['gz']) ? $_POST['gz'] : '';
$temp = isset($_POST['temp']) ? $_POST['temp'] : '';

// Gelen verileri virgüllere göre ayır
$axArray = explode(",", $ax);
$ayArray = explode(",", $ay);
$azArray = explode(",", $az);
$gxArray = explode(",", $gx);
$gyArray = explode(",", $gy);
$gzArray = explode(",", $gz);
$tempArray = explode(",", $temp);
$c = 0;
// Her bir dizi elemanını veritabanına kaydet
for ($i = 0; $i < count($axArray); $i++) {
  $axVal = $axArray[$i];
  $ayVal = $ayArray[$i];
  $azVal = $azArray[$i];
  $gxVal = $gxArray[$i];
  $gyVal = $gyArray[$i];
  $gzVal = $gzArray[$i];
  $tempVal = $tempArray[$i];
  $c++;

  // Veritabanına veri ekleme sorgusu
  $sql = "INSERT INTO mpu6050_data (ax, ay, az, gx, gy, gz, temp)
            VALUES ('$axVal', '$ayVal', '$azVal', '$gxVal', '$gyVal', '$gzVal', '$tempVal')";

  // Sorguyu çalıştır ve sonucu kontrol et
  if ($conn->query($sql) === TRUE) {
    // echo "Veri başarıyla kaydedildi! Kayıt No: " . $conn->insert_id . "<br>";
  } else {
    echo "Hata: " . $sql . "<br>" . $conn->error;
  }
}
echo "Toplam $c adet veri kaydedildi";
// Veritabanı bağlantısını kapat
$conn->close();
