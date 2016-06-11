<?php
function http_post_data($url, $data_string) {
  $ch = curl_init();  
  curl_setopt($ch, CURLOPT_POST, 1);  
  curl_setopt($ch, CURLOPT_URL, $url);  
  curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);  
  curl_setopt($ch, CURLOPT_HTTPHEADER, array(  
      'Content-Type: application/json; charset=utf-8',  
      'Content-Length: ' . strlen($data_string))  
  );  
  ob_start();  
  curl_exec($ch);  
  $return_content = ob_get_contents();  
  ob_end_clean();  
  $return_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);  
  return array($return_code, $return_content);  
}  
      
$url  = "http://127.0.0.1:9528/test";

$data = json_encode(array('md5'=>'ad0971ef4fe355c4b05d3e99d94f0cf1'));
$r = http_post_data($url, $data);  

var_dump($data);
var_dump($r);
