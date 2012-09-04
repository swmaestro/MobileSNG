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

public class BuildingCropFailChecker extends HttpServlet{
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

		Query cr1 = pm.newQuery(CropRequestInfo.class);
		List<CropRequestInfo> CRresults;
		cr1.setFilter("VBIndex == index");
		cr1.declareParameters("int index");

		Query cr2 = pm.newQuery(CropInfo.class);
		List<CropInfo> Cresults;
		cr2.setFilter("CropIndex == index");
		cr2.declareParameters("int index");
		
		try{
						Vresults = (List<VillageBuilding>) q2.execute(id,index);
						
						VillageBuilding Temp = Vresults.get(0);
						Bresults = (List<BuildingInfo>) q3.execute(Temp.getBuildingIndex());
						BuildingInfo Binfo = Bresults.get(0);
						switch(Temp.getVBState())
						{
							case VillageBuilding.BUILDING_ALONE_ENDPRODUCT:
							{
								//단 건물은 실패 예외를 주자!!
								if(Temp.getVBIndex()/1000 == 0)
									break;
						
								CRresults = (List<CropRequestInfo>) cr1.execute(Temp.getVBIndex());	//건물의 지정된 생존 시간만큼 생존 
								Cresults = (List<CropInfo>) cr2.execute(CRresults.get(0).getCropIndex());
								CropInfo crin = Cresults.get(0);
								Date FailChk = TimeChecker.GetTimeAdder(Temp.getVBUpdateDate(), crin.getCropSTime());//1시간 반이 지나도록 확인안하면 식료품이 썩는다 ㅇ
								if(TimeChecker.CheckNowTime(FailChk))
								{
									Temp.setVBState(VillageBuilding.BUILDING_PRODUCTFAIL);
									Temp.UpdateDate();
									inputChk=true;
								
								}
							
							}break;
							case VillageBuilding.BUILDING_PRODUCTFAIL:
								inputChk=true;
								break;
						}
						
					
					
					
			}finally{
				q3.closeAll();
				q2.closeAll();
				cr1.closeAll();
				cr2.closeAll();
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