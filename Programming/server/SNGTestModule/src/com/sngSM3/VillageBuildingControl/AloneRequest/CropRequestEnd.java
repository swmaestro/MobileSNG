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
import com.sngSM3.DataStores.CropInfo;
import com.sngSM3.DataStores.CropRequestInfo;
import com.sngSM3.DataStores.VillageBuilding;
import com.sngSM3.DataStores.VillageInfo;

public class CropRequestEnd  extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); //고유 번호
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> results;
		Boolean inputChk=false;
		q1.setFilter("memberId == id && VBIndex == index");
		q1.declareParameters("String id,int index");
		Query q2 = pm.newQuery(CropInfo.class);
		List<CropInfo> Cresults;
		q2.setFilter("CropIndex == index");
		q2.declareParameters("int index");		
		Query q3 = pm.newQuery(VillageInfo.class);
		List<VillageInfo> Mresults;
		q3.setFilter("memberId == id ");
		q3.declareParameters("String id");		
		Query q4 = pm.newQuery(CropRequestInfo.class);
		List<CropRequestInfo> CRresults;
		q4.setFilter("memberId == id && VBIndex == index");
		q4.declareParameters("String id,int index");
		
		
		try{
			if(index/1000 != 0)	//밭이 아니면 스킵 
			{
				CRresults=(List<CropRequestInfo>) q4.execute(id,index);
				CropRequestInfo requestInfo = CRresults.get(0);
				Mresults= (List<VillageInfo>) q3.execute(id);//마을에 대한 정보 없어도 스킵
				if(Mresults.size()>0)
				{
					Cresults = (List<CropInfo>) q2.execute(requestInfo.getCropIndex());
					if(Cresults.size()>0)	//작물에 대한 정보가 없어도 스킵
					{
						results = (List<VillageBuilding>) q1.execute(id,index);
						if(results.size()>0)//위치에 대한 정보가 없어도 스킵
						{
							VillageBuilding Farm = results.get(0); //생산 모드로 변경
							if(Farm.getVBState() == VillageBuilding.BUILDING_ALONE_ENDPRODUCT)
							{	
								Mresults.get(0).AddCostA(Cresults.get(0).getCropCostA());
								Mresults.get(0).AddCostB(Cresults.get(0).getCropCostB());
								Mresults.get(0).AddExp(Cresults.get(0).getCropPExp());
								Farm.EndAloneProductingMode();//최근 시간 갱신 
								pm.deletePersistent(requestInfo);
								inputChk=true;
							}
							else if(Farm.getVBState() == VillageBuilding.BUILDING_PRODUCTFAIL)
							{

								Farm.EndAloneProductingMode();//최근 시간 갱신 
								pm.deletePersistent(requestInfo);
								inputChk=true;
							}
						}
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