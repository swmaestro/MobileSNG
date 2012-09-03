package com.sngSM3.VillageBuildingControl;

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
import com.sngSM3.DataStores.VillageBuilding;
import com.sngSM3.DataStores.VillageInfo;

public class BuildingInfoList extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		Query q2 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> Vresults;
		
		q2.setFilter("memberId == id");
		q2.declareParameters("String id");
		q2.setOrdering("VBIndex ascending");

	
		try{
					Vresults = (List<VillageBuilding>) q2.execute(id);
					//Vresults.size();	//유저 마을이 존재하냐?
		
					req.setAttribute("result", Vresults.iterator());
					req.setAttribute("count", Vresults.size());
			}finally{
	
				q2.closeAll();
				pm.close();
			}
		

			RequestDispatcher dip = getServletContext().getRequestDispatcher("/VBControl/VillageBuildingInfo.jsp");
			dip.forward(req, resp);
		  
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
	
}