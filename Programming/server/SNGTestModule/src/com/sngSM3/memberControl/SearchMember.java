package com.sngSM3.memberControl;

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

public class SearchMember extends HttpServlet {
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {

		
		String id= req.getParameter("id"); 
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(MemberInfo.class);
		List<MemberInfo> results;
		
		q1.setFilter("memberId == Idparm");
		q1.declareParameters("String Idparm");
		try{
				results = (List<MemberInfo>) q1.execute(id);
				req.setAttribute("result", (java.util.Iterator)results.iterator());
				req.setAttribute("count", new Integer(results.size()));

		}finally{
			q1.closeAll();
			pm.close();
			RequestDispatcher dip = getServletContext().getRequestDispatcher("/memberControl/memInfoList.jsp");
			dip.forward(req, resp);
		
		}

			
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
}
