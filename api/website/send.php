<?php

error_reporting(E_ALL);
ini_set('display_errors',1);

require('../../library/config.php');
require('../../library/library.php'); //Import library

if (isset($_POST['relay'])){
	$relay = $_POST['relay'];
	$state = $_POST['state'];
	$lib =new Library(); //Call a new instance (library)
	if ($lib->updateRelay($relay,$state) == true){
		echo "successful";
	}
	else {
		echo "failed";
	}
}

?>
