<?php

 error_log(0);
 error_reporting(0);
 include "conn.php";
 
 // Veritabanından verileri çekme
 $sql = "SELECT * FROM img";
 $result = $baglan->query($sql);

 // Veritabanından çekilen verileri listeleme
 if ($result->num_rows > 0) {
     while($row = $result->fetch_assoc()) {
         echo"
         <img src='"."./img/".$row["img_url"]."'/>

         ";
     }
 } else {
     echo "Kayıt bulunamadı.";
 }

?>