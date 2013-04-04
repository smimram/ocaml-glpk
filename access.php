<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=ISO-8859-1"/>
<meta name="robots" content="noindex,nofollow,noarchive"/>
<link rel="stylesheet" type="text/css" href="main.css"/>
<title>Qui a vu ce site ?</title>
</head>

<body>
<h1>Qui a vu ce site ?</h1>

<div style="text-align: center">
<p>
<a class="button" href="access.php?arg=withbots">Toutes les visites</a> <a class="button" href="access.php">Tout sauf les bots</a> <a class="button" href="access.php?arg=files">Les fichiers seulement</a> <a class="button" href="access.php?arg=referext">Les pointeurs extérieurs</a>
</p>
</div>

<ol>
<?php
$accesslog = "/home/groups/o/oc/ocaml-glpk/htdocs/access.log";

$arg = $_GET["arg"];

if ($arg == "files") { }
elseif ($arg == "referext")
{
  $show_referext = 1;
}
else
{
  $show_host = 1;
  $show_date = 1;
  $show_agent = 1;
  $show_refer = 1;
  if ($arg = "withbots")
    $show_bots = 1;
}

$access = file($accesslog);
for ($i = sizeof($access) - 1; $i >= 0; $i--)
{
  list($time, $host, $ip, $ua, $uri, $ref) = split("\|", $access[$i]);
  $time = strftime("%d-%m-%y %H:%M:%S", $time);
  $ref = chop($ref);

  if (!$show_referext && ($show_bots || (!preg_match("/bot/i", $ua) && !preg_match("/NG/", $ua) && !preg_match("/ia_archiver/", $ua))))
    {
      echo "<li><b>$uri</b>\n";
      echo "<ul>\n";
      if ($show_host)
	echo "<li>Host: <a href=\"http://www.generic-nic.net/dyn/whois/ask?query=$ip\">$host</a></li>\n";
      if ($show_date)
	echo "<li>Date: $time</li>\n";
      if ($show_agent)
	echo "<li>Agent: $ua</li>\n";
      if (($show_refer || $show_referext) && $ref != "")
	{
	  $r = htmlspecialchars($ref);
	  echo "<li>Refer: <a href=\"$r\">$r</a></li>";
	}
      echo "</ul>\n</li>\n";
    }
}
?>
</ol>
</body>
</html>
