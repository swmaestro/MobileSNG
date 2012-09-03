package com.sngSM3.VillageBuildingControl.AloneRequest;

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

public class BuildingAloneRequestEnd extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); //고유 번호
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
	
		Query q1 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> results;
		q1.setFilter("memberId == id && VBIndex == index");
		q1.declareParameters("String id,int index");		
		Query q2 = pm.newQuery(BuildingInfo.class);
		List<BuildingInfo> Biresults;
		q2.setFilter("BuildingIndex == index ");
		q2.declareParameters("String index");	
		Query q3 = pm.newQuery(VillageInfo.class);
		List<VillageInfo> Mresults;
		q3.setFilter("memberId == id ");
		q3.declareParameters("String id");		
		try{
			if(index/10000 == 0)	//건물이 아니면 스킵 
			{
				Mresults= (List<VillageInfo>) q3.execute(id);
				if(Mresults.size()>0)
				{
					results = (List<VillageBuilding>) q1.execute(id,index);
					if(results.size()>0)//위치에 대한 정보가 없어도 스킵
					{
						if(results.get(0).getVBState()==VillageBuilding.BUILDING_ALONE_ENDPRODUCT)	//건물은 실패가 없다
						{
							Biresults= (List<BuildingInfo>) q2.execute(results.get(0).getBuildingIndex());
						
							Mresults.get(0).AddCostA(Biresults.get(0).getBuildingProfitA());
							Mresults.get(0).AddCostB(Biresults.get(0).getBuildingProfitB());
							Mresults.get(0).AddExp(Biresults.get(0).getBuildingPExp());
							
							results.get(0).StartAloneProductingMode();
							inputChk=true;
						}
					}
					
				}
			}
			

			}finally{
				q1.closeAll();
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