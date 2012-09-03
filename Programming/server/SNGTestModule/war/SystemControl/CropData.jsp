<%@ page language="java" contentType="text/xml; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import = "com.sngSM3.*" %>
<%@ page import = "com.sngSM3.DataStores.*" %>
<%
	Integer ver = (Integer)request.getAttribute("count");
	java.util.Iterator Temp = (java.util.Iterator)request.getAttribute("result");
	CropInfo TestData=null;
%>   
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<swmaestro>
	<count><%=ver.toString()%></count>
	<% while(Temp.hasNext()) {%><%TestData= (CropInfo)Temp.next(); %>
	<Crop>
		<index><%=TestData.getCropIndex()%></index>
		<name><%=TestData.getCropName()%></name>
		<costA><%=TestData.getCropCostA()%></costA>
		<costB><%=TestData.getCropCostB()%></costB>
		<profitA><%=TestData.getCropProfitA()%></profitA>
		<profitB><%=TestData.getCropProfitB()%></profitB>
		<PExp><%=TestData.getCropPExp()%></PExp>
		<b_time><%=TestData.getCropBTime()%></b_time>
		<s_time><%=TestData.getCropSTime()%></s_time>
		<level><%=TestData.getCropLevel()%></level>
	</Crop>
	<%} %>
</swmaestro>

