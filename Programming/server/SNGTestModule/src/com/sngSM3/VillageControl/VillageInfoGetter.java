package com.sngSM3.VillageControl;

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
import com.sngSM3.DataStores.VillageInfo;

public class VillageInfoGetter extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.

	
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		Query q1 = pm.newQuery(VillageInfo.class);
		List<VillageInfo> results;
		
		q1.setFilter("memberId == id");
		q1.declareParameters("String id");
		try{
				results = (List<VillageInfo>) q1.execute(id);

				req.setAttribute("result", (java.util.Iterator)results.iterator());
				req.setAttribute("count", new Integer(results.size()));
				RequestDispatcher dip = getServletContext().getRequestDispatcher("/VillageControl/VillageInfoData.jsp");//위치 수정하기 
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
