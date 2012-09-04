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
import com.sngSM3.DataStores.BuildingInfo;
import com.sngSM3.DataStores.BuildingRequestInfo;
import com.sngSM3.DataStores.VillageBuilding;
import com.sngSM3.DataStores.VillageInfo;

public class BRequestFullComplete extends HttpServlet{
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
		List<VillageBuilding> VBresults;
		q2.setFilter("memberId == id && VBIndex == index");
		q2.declareParameters("String id,int index");
		q2.setRange(0,1);
		
		Query q3 = pm.newQuery(BuildingInfo.class);
		List<BuildingInfo> Bresults;
		
		q3.setFilter("BuildingIndex == index");
		q3.declareParameters("int index");
		q3.setRange(0,1);
		
		Query q4 = pm.newQuery(VillageInfo.class);
		List<VillageInfo> Vresults;
		
		q4.setFilter("memberId == id || memberId == id2");
		q4.declareParameters("String id,String id2");
		q4.setRange(0,2);
		
		try{
				results = (List<BuildingRequestInfo>) q1.execute(owner, index);
				if(results.size()>0)
				{	
					BuildingRequestInfo Temp=results.get(0);
					
					if(Temp.getRequestState()==BuildingRequestInfo.REQUEST_PRODUCT_COMPLETE)
					{					
						VBresults = (List<VillageBuilding>) q2.execute(owner, index);
						if(VBresults.get(0).getVBState() == VillageBuilding.BUILDING_REQUEST_WAITING)
						{
							Bresults= (List<BuildingInfo>) q3.execute(VBresults.get(0).getBuildingIndex());
							Vresults= (List<VillageInfo>) q4.execute(Temp.getMemberId(),Temp.getRequesterId());
							Temp.setRequestState(BuildingRequestInfo.REQUEST_COMPLETE);
							//건물 상태 변경 
							VBresults.get(0).setVBState(VillageBuilding.BUILDING_WAITING);
							VBresults.get(0).UpdateDate();
							
							for(int i=0;i<Vresults.size();i++)
							{
								Vresults.get(i).AddCostA(Bresults.get(0).getBuildingProfitA());
								Vresults.get(i).AddCostB(Bresults.get(0).getBuildingProfitB());
								Vresults.get(i).AddExp(Bresults.get(0).getBuildingPExp());
							}
							
							inputChk = true;
						}
					}
				}

			}finally{
				q1.closeAll();
				q2.closeAll();
				q3.closeAll();
				q4.closeAll();
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