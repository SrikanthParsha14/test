<?php
# Call by method post
# productid=pid
$db = mysql_connect("localhost", "root", "qu-cg123");
if (!$db) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db("sams", $db);
$sql = "select status from product where product_id='" . $_POST["product_id"] . "'";
$result = mysql_query($sql, $db);
if ($myrow = mysql_fetch_array($result)) {
	if ($myrow["status"] == 0){
		$random = randString(16);
		$sql = "update product set status=0,random='" . $random .
			"' where product_id='" . $_POST["product_id"] . "'";
		$result = mysql_query($sql, $db);
		if(!$result) {
			echo "{'code':6}"; # update status error
		} else {
			echo "{'code':0,'random':'" . $random . "'}"; # random code
		}
	} else {
		echo "{'code':2}"; # status error
	}
} else {
	echo "{'code':1}"; # no such product
}
mysql_close($db);

function randString($len)
{
	$chars='ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz'; // characters to build the password from
	$string='';
	for(; $len>=1; $len--) {
		$position=rand()%strlen($chars);
		$string.=substr($chars, $position, 1);
	}
	return $string;
}
?>
