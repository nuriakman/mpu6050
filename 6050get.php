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
