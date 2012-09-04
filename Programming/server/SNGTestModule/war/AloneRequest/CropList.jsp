<%@ page language="java" contentType="text/xml; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import = "com.sngSM3.*" %>
<%@ page import = "com.sngSM3.DataStores.*" %>
<%
	Integer ver = (Integer)request.getAttribute("count");
	java.util.Iterator Temp = (java.util.Iterator)request.getAttribute("result");
	CropRequestInfo TestData=null;
%>   
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<swmaestro>
	<count><%=ver.toString()%></count>
	<% while(Temp.hasNext()) {%><%TestData= (CropRequestInfo)Temp.next(); %>
	<crop>
		<index><%=TestData.getVBIndex()%></index>
		<cindex><%=TestData.getCropIndex()%></cindex>
	</crop>
	<%} %>
</swmaestro>

