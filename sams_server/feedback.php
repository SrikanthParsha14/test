<?php
# Call by method post
# file=&feedback=...
$db = mysql_connect("localhost", "root", "qu-cg123");
if (!$db) {
	die('Could not connect: ' . mysql_error());
}
mysql_select_db("sams", $db);
$sql = "select status from customer where product_id='" . $_POST["product_id"] . "'";
$result = mysql_query($sql, $db);
if (mysql_num_rows($result) <= 0) {
	//echo "{'code':4}"; // no such product
	echo makeRetHtml("�����ύʧ�ܣ����Ĳ�Ʒ���кŲ���ȷ��", false);
} else if ( $_FILES["file"]["size"] > 300000 ) {
	//echo "{'code':1}"; // file too big
	echo makeRetHtml("�����ύʧ�ܣ�ͼƬ̫�����޸�ͼƬ��С��", true);
} else if ( $_FILES["file"]["error"] == 4 ) {
	$sql = "SET CHARACTER_SET_CONNECTION=utf8";
	$result = mysql_query($sql, $db);
	$sql = "SET CHARACTER_SET_CLIENT=utf8";
	$result = mysql_query($sql, $db);
	$sql = "insert into feedback (filename, feedback) values('<none>','" .
$_POST["feedback"] . "');";
	$result = mysql_query($sql, $db);
	//echo "{'code':0}";
	echo makeRetHtml("�����ύ�ɹ�����л����֧�֣�", false);
} else if ( $_FILES["file"]["error"] > 0 ) {
	//echo "{'code':2}"; // transfer error
	echo makeRetHtml("�����ύʧ�ܣ����ݴ�������뷵�����ԡ�", true);
} else if (($_FILES["file"]["type"] == "image/gif")
	|| ($_FILES["file"]["type"] == "image/png")
	|| ($_FILES["file"]["type"] == "image/x-png")
	|| ($_FILES["file"]["type"] == "image/jpeg")
	|| ($_FILES["file"]["type"] == "image/pjpeg")) {
	if (($_FILES["file"]["type"] == "image/jpeg")
		|| ($_FILES["file"]["type"] == "image/pjpeg")) {
		$ext = ".jpg";
	} else if (($_FILES["file"]["type"] == "image/png")
		|| ($_FILES["file"]["type"] == "image/x-png")) {
		$ext = ".png";
	}
	$filename = randString(12) . $ext;
	move_uploaded_file($_FILES["file"]["tmp_name"], "upload/" . $filename);
	$sql = "SET CHARACTER_SET_CONNECTION=utf8";
	$result = mysql_query($sql, $db);
	$sql = "SET CHARACTER_SET_CLIENT=utf8";
	$result = mysql_query($sql, $db);
	$sql = "insert into feedback (filename, feedback) values('" .
$filename . "','" .
$_POST["feedback"] . "');";
	$result = mysql_query($sql, $db);
	//echo "{'code':0}";
	echo makeRetHtml("�����ύ�ɹ�����л����֧�֣�", false);
} else {
	//echo "{'code':3}"; // not support file type
	echo makeRetHtml("�����ύʧ�ܣ��ϴ���ͼ��֧��PNG/JPG/GIF��ʽ��", true);
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

function makeRetHtml($strMsg, $bRetBak){
	$strRet = "<div id=\"divFeedBack\" style=\"margin: 0 auto; padding:10px; font-size:12px; color:#996600;\">";
	$strRet .= $strMsg;
	$strRet .= "</div>";
	if($bRetBak){
		//$strRet .= '<div style="height:52px; width:95px; padding: 8px 0px 0 8px; border:0px;"><a style="float:right; display:block; width:95px; height:31px; background: url("images/fh_hover.png") 0 0 no-repeat;" href="javascript:history.back()" ><<����</a></div>';
		$strRet .= '<div><a style="display:block;background:url(images/fh_hover.png) 0 0 no-repeat;height:31px; width:95px;" href="javascript:history.back()" a></div>';
		//$strRet .= '<div><a href="javascript:history.back()" ><<����</a></div>';
	}
	return $strRet;	
}

?>