<?php
/**
*
*/
class Library
{

	function __construct()
	{
		//Database Configuration
		$env = new Env();
		$dbhost = $env->dbhost;
		$dbuser = $env->dbuser;
		$dbpass = $env->dbpass;
		$dbname= $env->dbname;
    	//Database Connection
		$con = new mysqli($dbhost,$dbuser,$dbpass,$dbname);
    	//Check Connection
		if($con->connect_error)
		{
      		die("Failed to connect with MySQL: ".$con->connect_error); //Show error and end script
  		}
  		else
  		{
      		$this->connect = $con; //Make connection variable global
  		}
	}

	public function updateRelay ($relay, $state)
	{
		$sql = "UPDATE breaker_table SET state=$state WHERE relay='$relay'";
		if ($this->connect->query($sql) === FALSE)
		{
		    return false;
      	}
      	else
      	{
      		return true;
      	}

	}

	public function getRelayState()
	{
		$sql = "SELECT * FROM breaker_table WHERE 1";
		$RelayData = $this->connect->query($sql);
		$state = "cct";
		$count = 0;
		$totalData = $RelayData->num_rows;
		if ($totalData > 0)
		{
			while ($row = $RelayData->fetch_assoc())
			{
				if ($totalData - $count > 1){
					$state .= $row['relay'] . "=" . $row['state'] . "&";
				}else{
					$state .= $row['relay'] . "=" . $row['state'];
				}
				$count++;
			}
			$state .= "\n";
			return $state;
		}
		else
		{
			return false;
		}
	}

}
?>
