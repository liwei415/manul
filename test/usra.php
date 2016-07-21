<?php
date_default_timezone_set('Asia/Shanghai');
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

$url  = "http://127.0.0.1:9528/usra";

$data = json_encode(array('plat_date'=>date('Ymd'),'plat_time'=>date('His'),'plat_no'=>'txn'.time(),'plat_code'=>'8888021655','uid'=>'888888', 'username' => '招商银行', 'type' => 'P', 'cert' => 'P01', 'cert_no' => '522221198511061255', 'mobile' => '13120607077', 'spare' => ''));
$r = http_post_data($url, $data);

var_dump($data);
var_dump($r);
