package com.sngSM3.memberControl;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.TimeZone;

import javax.jdo.PersistenceManager;
import javax.jdo.Query;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.*;


import com.google.appengine.api.datastore.PhoneNumber;
import com.sngSM3.PMF;
import com.sngSM3.TimeChecker;
import com.sngSM3.DataStores.MemberInfo;

@SuppressWarnings("serial")
public class SNGTestModuleServlet extends HttpServlet {
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		//resp.setContentType("text/plain");
		//resp.getWriter().println("Hello, world");
		String id= req.getParameter("id");  //get으로 전송된 내용 name(name)값으로 받는다.
		String pass=req.getParameter("password");    //get으로 전송된 내용 name(id)값으로 받는다.
		String ph=req.getParameter("phone");
	
		Boolean inputChk=false;
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(MemberInfo.class);

		
		q1.setFilter("memberId == Idparm");
		q1.declareParameters("String Idparm");
			try{
				List<MemberInfo> results = (List<MemberInfo>) q1.execute(id);
				if(!results.isEmpty())
				{
					//resp.getWriter().println("아이디가 존재합니다.");
					inputChk=false;
				}
				else{
					inputChk=true;
					Date updateddate = TimeChecker.GetNowDate();
				
					MemberInfo p = new MemberInfo(id,ph,pass,updateddate);
					pm.makePersistent(p);
				}
			}finally{
				q1.closeAll();
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
