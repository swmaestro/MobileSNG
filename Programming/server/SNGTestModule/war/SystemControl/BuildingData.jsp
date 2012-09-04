<%@ page language="java" contentType="text/xml; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import = "com.sngSM3.*" %>
<%@ page import = "com.sngSM3.DataStores.*" %>
<%
	Integer ver = (Integer)request.getAttribute("count");
	java.util.Iterator Temp = (java.util.Iterator)request.getAttribute("result");
	BuildingInfo TestData=null;
%>   
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<swmaestro>
	<count><%=ver.toString()%></count>
	<% while(Temp.hasNext()) {%><%TestData= (BuildingInfo)Temp.next(); %>
	<building>
		<index><%=TestData.getBuildingIndex()%></index>
		<name><%=TestData.getBuildingName()%></name>
		<costA><%=TestData.getBuildingCostA()%></costA>
		<costB><%=TestData.getBuildingCostB()%></costB>
		<profitA><%=TestData.getBuildingProfitA()%></profitA>
		<profitB><%=TestData.getBuildingProfitB()%></profitB>
		<CExp><%=TestData.getBuildingCExp()%></CExp>
		<PExp><%=TestData.getBuildingPExp()%></PExp>
		<b_time><%=TestData.getBuildingBTime()%></b_time>
		<p_time><%=TestData.getBuildingPTime()%></p_time>
		<size><%=TestData.getBuildingSize()%></size>
		<level><%=TestData.getBuildingLevel()%></level>
	</building>
	<%} %>
</swmaestro>

