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

public class BuildingRequest extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String owner=req.getParameter("owner");    //get으로 전송된 내용 name(name)값으로 받는다.
		String requester=req.getParameter("requester");  
		int index=(int)new Integer(req.getParameter("index")); //고유 번호
	//	int bindex=(int)new Integer(req.getParameter("bindex")); //빌딩번
		//int state=(int)new Integer(req.getParameter("state")); 
	
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> results;
		Boolean inputChk=false;
		q1.setFilter("memberId == id && VBIndex == index");
		q1.declareParameters("String id,int index");
		
		try{
			
				results = (List<VillageBuilding>) q1.execute(owner, index);
				if(results.size()>0)
				{	
					VillageBuilding Temp=results.get(0);
					if(Temp.getVBIndex()/1000 == 0) 
					{
					if((Temp.getVBState() == VillageBuilding.BUILDING_WAITING) ||(Temp.getVBState() == VillageBuilding.BUILDING_ALONE_PRODUCTING))
					{
						BuildingRequestInfo BRInfo = new BuildingRequestInfo(owner,requester,index,Temp.getBuildingIndex());
						Temp.setVBState(VillageBuilding.BUILDING_REQUEST_WAITING);
						pm.makePersistent(BRInfo);
						inputChk=true;
					}
					}//건물이 아니면 빠져나가기 
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