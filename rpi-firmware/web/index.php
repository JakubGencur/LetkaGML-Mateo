<!DOCTYPE HTML>
<html>
	<head>
		<meta charset=utf-8>
		<title>Mateo</title>
		<style>
			h1{
font-size:35pt; color:rgb(6, 18, 122); text-align:center; font-family:lovelo; margin:30px;
}
p{
font-size:25pt; color:rgb(6, 18, 122); text-align:left; font-family:draft; margin:5px;
}
a{
font-size:25pt; color:#0000EE; text-align:left; font-family:draft; margin:5px;
}





@font-face {
font-family: draft;
src: url(./css/draftb.otf);
}

@font-face {
font-family: lovelo;
src: url(./css/Lovelo.otf);
}
		</style>
	</head>
	<body>
		<h1>Mateo - meteorologická stanice GML</h1>
		<p>Toto je stránka meteorologické stanice GML. Na této stránce můžete stáhnout data ze stanice. Pro stažení dat použijte odkaz níže. Data se zaznamenávají jednou za 10 min. Pro získání aktuálních dat obnovte stránku.</p>
<?php
$file = fopen("./data/actual.txt", "r");
$cas = fgets($file);
$teplota = fgets($file);
$tlak = fgets($file);
fclose($file);
echo "		<p>Čas posledni aktualizace: " . $cas . "</p>";
echo "		<p>Aktuální teplota: " . $teplota . " °C</p>";
echo "		<p>Aktuální tlak: " . $tlak . " hPa</p>";
?>

		<a href="https://letkagml.space">Letka GML</a></p>
		<a href="data/data.txt" download>Stáhni txt soubor s daty</a>
	</body>
</html>
