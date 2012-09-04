package com.sngSM3;

import java.io.IOException;


import javax.jdo.PersistenceManager;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.sngSM3.DataStores.FriendInfo;

public class TimePrint  extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
					
	
		req.setAttribute("time", GetUpdatedDate());
		RequestDispatcher dip = getServletContext().getRequestDispatcher("/TimePrint.jsp");
		dip.forward(req, resp);
		  
	}
	
	public String GetUpdatedDate()
	{
		java.util.Date time = TimeChecker.GetNowDate();
		@SuppressWarnings("deprecation")
		String timeString=String.format("%04d-%02d-%02d-%02d-%02d-%02d",time.getYear()+1900,time.getMonth()+1,time.getDate(),time.getHours(),time.getMinutes(),time.getSeconds());
		
		return timeString;
		
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
	
}
