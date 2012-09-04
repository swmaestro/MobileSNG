package com.sngSM3.VillageBuildingControl.StateUpdater;

import java.io.IOException;
import java.util.Date;
import java.util.List;

import javax.jdo.PersistenceManager;
import javax.jdo.Query;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.sngSM3.PMF;
import com.sngSM3.TimeChecker;
import com.sngSM3.DataStores.BuildingInfo;
import com.sngSM3.DataStores.CropInfo;
import com.sngSM3.DataStores.CropRequestInfo;
import com.sngSM3.DataStores.VillageBuilding;

public class BuildingBuildComplete  extends HttpServlet{
	//상태를 한번 자동 업그레이드 해준다.
	public final int limitedTime = 13000;
	
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); 
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;

		
		Query q2 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> Vresults;
		
		q2.setFilter("memberId == id && VBIndex == index");
		q2.declareParameters("String id, int index");
			
		Query q3 = pm.newQuery(BuildingInfo.class);
		List<BuildingInfo> Bresults;
		q3.setFilter("BuildingIndex == index");
		q3.declareParameters("int index");

		
		try{
				
			
						Vresults = (List<VillageBuilding>) q2.execute(id,index);
						
						VillageBuilding Temp = Vresults.get(0);
						Bresults = (List<BuildingInfo>) q3.execute(Temp.getBuildingIndex());
						BuildingInfo Binfo = Bresults.get(0);
						switch(Temp.getVBState())
						{
							case VillageBuilding.BUILDING_BUILD:
							{
								Date BuildingTime= TimeChecker.GetTimeAdder(Temp.getVBUpdateDate(),Binfo.getBuildingBTime());
							//	System.out.println(TimeChecker.CheckNowTime(BuildingTime));
									if(TimeChecker.CheckNowTime(BuildingTime))
									{
										if(Temp.getVBIndex()/1000 == 0)	//건물이면 자동 생산 모드 작동 
										{
											Temp.StartAloneProductingMode();
										}
										else{
											//밭처
										Temp.setVBState(VillageBuilding.BUILDING_WAITING);
										Temp.UpdateDate();
										}
										inputChk=true;
									}
									
							}		
									break;
							default:
								inputChk=true;
								break;
						}
						
					
					
					
			}finally{
				q3.closeAll();
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