<%@ page language="java" contentType="text/xml; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import = "com.sngSM3.*" %>
<%
	Boolean res = (Boolean)request.getAttribute("result");

%>   
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<swmaestro>
		<result><%=res.toString()%></result>
</swmaestro>

