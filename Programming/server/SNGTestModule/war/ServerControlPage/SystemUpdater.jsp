<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import = "com.sngSM3.*" %>
<%@ page import = "com.sngSM3.DataStores.*" %>
<%
	Integer ver = (Integer)request.getAttribute("count");
	java.util.Iterator Temp = (java.util.Iterator)request.getAttribute("result");
	BuildingInfo TestData=null;
%>   
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    <title>시스템 업데이트</title>
  </head>

  <body align="center">
    
	<br>
	<form action="../systemregister" method="get">  
		버젼  :<input type="text" name="ver"><br>
		로그  :<input type="text" name="log"><br>
 	<input type=reset value='취소'>
 	<input type=submit value='확인'>
 	</form>
 
   
  </body>
</html>
