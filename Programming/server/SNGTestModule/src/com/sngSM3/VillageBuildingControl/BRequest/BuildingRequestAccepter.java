package com.sngSM3.VillageBuildingControl.BRequest;

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
import com.sngSM3.DataStores.BuildingRequestInfo;
import com.sngSM3.DataStores.VillageBuilding;

public class BuildingRequestAccepter extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String owner=req.getParameter("owner");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); //고유 번호

		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(BuildingRequestInfo.class);
		List<BuildingRequestInfo> results;
		Boolean inputChk=false;
		q1.setFilter("memberId == id && VBIndex == index");
		q1.declareParameters("String id,int index");
		
		Query q2 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> Vresults;
		q2.setFilter("memberId == id && VBIndex == index");
		q2.declareParameters("String id,int index");
		
		try{
				results = (List<BuildingRequestInfo>) q1.execute(owner, index);
				if(results.size()>0)
				{	
					BuildingRequestInfo Temp=results.get(0);
					
					if(Temp.getRequestState()==BuildingRequestInfo.REQUEST_WAITING)
					{
						Vresults = (List<VillageBuilding>) q2.execute(owner, index);
						if(Vresults.get(0).getVBState() == VillageBuilding.BUILDING_REQUEST_WAITING)
						{
							Temp.setRequestState(BuildingRequestInfo.REQUEST_PRODUCTING);
							Vresults.get(0).setVBState(VillageBuilding.BUILDING_PRODUCTING);
							Vresults.get(0).UpdateDate();
							inputChk = true;
						}
					}
				}

			}finally{
				q1.closeAll();
				q2.closeAll();
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