package com.sngSM3.SystemControl;

import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.TimeZone;

import javax.jdo.PersistenceManager;
import javax.jdo.Query;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.api.datastore.PhoneNumber;
import com.sngSM3.PMF;
import com.sngSM3.TimeChecker;
import com.sngSM3.DataStores.MemberInfo;
import com.sngSM3.DataStores.SystemInfo;
@SuppressWarnings("serial")
public class SystemRegister extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String version= req.getParameter("ver");  //get으로 전송된 내용 name(name)값으로 받는다.
		String log=req.getParameter("log");    //get으로 전송된 내용 name(id)값으로 받는다.
		
		Boolean inputChk=false;
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
			try{
					Date updateddate = TimeChecker.GetNowDate();
					
					int iver = new Integer(version);
					SystemInfo p = new SystemInfo(iver,updateddate,log);
					pm.makePersistent(p);
					inputChk=true;
				
			}finally{
				
				pm.close();
			}
			
			req.setAttribute("result", inputChk);
			RequestDispatcher dip = getServletContext().getRequestDispatcher("/result.jsp");
			dip.forward(req, resp);
		  
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
	
}
