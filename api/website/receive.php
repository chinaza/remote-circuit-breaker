<?php

error_reporting(E_ALL);
ini_set('display_errors',1);

require('../../library/config.php');
require('../../library/library.php'); //Import library

$relayData = $lib->getRelayState($relay)
if ($relayData){
  echo $relayData;
} else {
  echo 'failed';
}

?>
