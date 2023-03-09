<?php

 error_log(0);
 error_reporting(0);
 include "../conn.php";
 
 $local_img = $_GET["img"];
 //$contents = file_get_contents($local_img);
 $url = 'http://192.168.1.100/picture.jpg';
 $ch = curl_init();
 curl_setopt($ch, CURLOPT_URL, $url);
 curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
 $contents = curl_exec($ch);
 curl_close($ch);
 $dosya_ad = uniqid();

 $sql = "INSERT INTO `img` (`img_url`) VALUES ('".$dosya_ad.".jpg"."');";
 if ($baglan->query($sql) === TRUE) {
    $file_path = '../img/' . $dosya_ad .".jpg";
    file_put_contents($file_path, $contents);
 }//http://localhost/esp32/api/add_img.php?img=http://192.168.1.100/picture.jpg
?>

 <img src="<?php echo $local_img; ?>"/>