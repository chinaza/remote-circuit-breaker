<?php

error_reporting(E_ALL);
ini_set('display_errors',1);

require('../../library/config.php');
require('../../library/library.php'); //Import library

$lib =new Library(); //Call a new instance (library)
$relayData = $lib->getRelayState();
if ($relayData){
  echo $relayData;
} else {
  echo 'failed';
}

?>
