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

public class BuildingSuccess extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); 
		//int state=(int)new Integer(req.getParameter("state")); 
	
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> results;
		Boolean inputChk=false;
		q1.setFilter("memberId == id && VBIndex == index");
		q1.declareParameters("String id,int index");

		Query q2 = pm.newQuery(VillageInfo.class);
		List<VillageInfo> Vresults;		
		q2.setFilter("memberId == id");
		q2.declareParameters("String id");
		
		Query q3 = pm.newQuery(BuildingInfo.class);
		List<BuildingInfo> Bresults;
		q3.setFilter("BuildingIndex == index");
		q3.declareParameters("int index");

		try{
				results = (List<VillageBuilding>) q1.execute(id, index);
				if(results.size()>0)
				{	
					VillageBuilding Temp=results.get(0);
					if(Temp.getVBState() == VillageBuilding.BUILDING_BUILD)
					{
						Vresults=(List<VillageInfo>) q2.execute(id);
						Bresults=(List<BuildingInfo>) q3.execute(Temp.getBuildingIndex());
						BuildingInfo binfo = Bresults.get(0);
						VillageInfo userVInfo = Vresults.get(0);
						Temp.setVBState(VillageBuilding.BUILDING_WAITING);
						userVInfo.AddExp(binfo.getBuildingCExp());					
						inputChk=true;
					}
					
				}

			}finally{
				q1.closeAll();
				q2.closeAll();
				q3.closeAll();
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