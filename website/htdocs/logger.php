<?php
$accesslog = "/home/groups/o/oc/ocaml-glpk/htdocs/access.log";

$uri = substr(getenv("REQUEST_URI"), 14);
$ip = getenv("REMOTE_ADDR");
$host = gethostbyaddr($ip);

if (true)
{
  $fp = fopen($accesslog, "a");
  $time = time();
  $ua = getenv("HTTP_USER_AGENT");
  $ref = getenv("HTTP_REFERER");
  flock($fp, LOCK_EX);
  fwrite($fp, "$time|$host|$ip|$ua|$uri|$ref\n");
  flock($fp, LOCK_UN);
  fclose($fp);
}
?>
