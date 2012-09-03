<%@ page language="java" contentType="text/xml; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import = "com.sngSM3.*" %>
<%@ page import = "com.sngSM3.DataStores.*" %>
<%
	Integer ver = (Integer)request.getAttribute("count");
	java.util.Iterator Temp = (java.util.Iterator)request.getAttribute("result");
	BuildingRequestInfo TestData=null;
%>   
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<swmaestro>
	<count><%=ver.toString()%></count>
	<% while(Temp.hasNext()) {%><%TestData= (BuildingRequestInfo)Temp.next(); %>
	<RequestInfo>
		<owner><%=TestData.getMemberId()%></owner>
		<requester><%=TestData.getRequesterId()%></requester>
		<state><%=TestData.getRequestState()%></state>
		<index><%=TestData.getVBIndex()%></index>
		<bindex><%=TestData.getBuildingIndex()%></bindex>
	</RequestInfo>
	<%} %>
</swmaestro>

