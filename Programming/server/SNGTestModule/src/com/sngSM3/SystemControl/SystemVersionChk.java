package com.sngSM3.SystemControl;

import java.io.IOException;
import java.util.List;

import javax.jdo.PersistenceManager;
import javax.jdo.Query;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.sngSM3.PMF;
import com.sngSM3.DataStores.MemberInfo;
import com.sngSM3.DataStores.SystemInfo;

public class SystemVersionChk extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {

		
		int ver= new Integer(req.getParameter("ver")); 
	
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(SystemInfo.class);
		List<SystemInfo> results;
		
		q1.setFilter("SystemVersion > now_ver");
		q1.declareParameters("int now_ver");
	
		try{
				Boolean verChker;
				results = (List<SystemInfo>) q1.execute(ver);
					verChker=(results.size()>0) ? false: true;
				req.setAttribute("result", verChker);
				RequestDispatcher dip = getServletContext().getRequestDispatcher("/result.jsp");
				dip.forward(req, resp);
		}finally{
			q1.closeAll();
			pm.close();
			
		
		
		}

			
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
}
