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
import com.sngSM3.DataStores.BuildingInfo;


public class BuildingOutStream extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		int ver=(int)new Integer(req.getParameter("ver"));    //get으로 전송된 내용 name(name)값으로 받는다.

		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		
		Query q1 = pm.newQuery(BuildingInfo.class);
		List<BuildingInfo> results;
		
		q1.setFilter("SystemVersion > ver");
		q1.declareParameters("int ver");
		try{
				results = (List<BuildingInfo>) q1.execute(ver);
				req.setAttribute("result", (java.util.Iterator)results.iterator());
				req.setAttribute("count", new Integer(results.size()));

			
			}finally{
				q1.closeAll();
				pm.close();
				RequestDispatcher dip = getServletContext().getRequestDispatcher("/SystemControl/BuildingData.jsp");
				dip.forward(req, resp);
			}
		
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
	
}