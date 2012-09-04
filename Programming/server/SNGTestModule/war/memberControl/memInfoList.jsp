<%@ page language="java" contentType="text/xml; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import = "com.sngSM3.*" %>
<%@ page import = "com.sngSM3.DataStores.*" %>
<%
	java.util.Iterator Temp = (java.util.Iterator)request.getAttribute("result");
	Integer count = (Integer)request.getAttribute("count");
	MemberInfo TestData;
%>   
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<swmaestro>
		<count><%=count.toString() %></count>
		<% while(Temp.hasNext()) {%><%TestData= (MemberInfo)Temp.next(); %>
		<userinfo>
			<id><%= TestData.GetId() %></id>
			<phone><%=TestData.GetPhone() %></phone>
			<date><%=TestData.GetUpdatedDate() %></date>
		</userinfo><%} %>
</swmaestro>

