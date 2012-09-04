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
import com.sngSM3.DataStores.FriendInfo;
import com.sngSM3.DataStores.VillageBuilding;
import com.sngSM3.DataStores.VillageInfo;


public class BuildingLocationChange extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); 
		int location=(int)new Integer(req.getParameter("location")); 
		boolean direction=(boolean)new Boolean(req.getParameter("direction")); 
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> results;
		Boolean inputChk=false;
		q1.setFilter("memberId == id && VBIndex == index");
		q1.declareParameters("String id,int index");

		try{
				results = (List<VillageBuilding>) q1.execute(id, index);
				if(results.size()>0)
				{
					results.get(0).setVBLocation(location);
					results.get(0).setVBDirection(direction);
					inputChk=true;
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
