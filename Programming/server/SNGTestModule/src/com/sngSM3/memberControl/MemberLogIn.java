package com.sngSM3.memberControl;

import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.TimeZone;

import javax.jdo.PersistenceManager;
import javax.jdo.Query;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.sngSM3.PMF;
import com.sngSM3.TimeChecker;
import com.sngSM3.DataStores.MemberInfo;
import com.sngSM3.DataStores.SystemInfo;

public class MemberLogIn extends HttpServlet {
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		
		String id= req.getParameter("id"); 
		String pw= req.getParameter("password"); 
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(MemberInfo.class);
		boolean resultData=false;

		q1.setFilter("memberId == Idparm");
		q1.declareParameters("String Idparm");
		
		try{
				List<MemberInfo> execute = (List<MemberInfo>) q1.execute(id);
				
				
				if(execute.size()>=1)
				{
					MemberInfo Temp = (MemberInfo)execute.get(0);
					
					Date updateddate = TimeChecker.GetNowDate();
					if(Temp.ChkPassWord(pw))
					{
						Temp.SetDate(updateddate);
						resultData= true;
					}
					else
					{
						resultData= false;
					}
					
				}
		
				
			
		}finally{
			q1.closeAll();
			pm.close();
			req.setAttribute("result", new Boolean(resultData));
			RequestDispatcher dip = getServletContext().getRequestDispatcher("/result.jsp");
			dip.forward(req, resp);
		}

			
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
}


