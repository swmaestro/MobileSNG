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
import com.sngSM3.DataStores.FriendInfo;
import com.sngSM3.DataStores.VillageBuilding;
import com.sngSM3.DataStores.VillageInfo;

public class BuildingInsert extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); 
		int bindex=(int)new Integer(req.getParameter("bindex")); 
		int location=(int)new Integer(req.getParameter("location")); 
		boolean direction=(boolean)new Boolean(req.getParameter("direction")); 
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		Query q2 = pm.newQuery(VillageInfo.class);
		List<VillageInfo> Vresults;
		
		q2.setFilter("memberId == id");
		q2.declareParameters("String id");
		q2.setRange(0,1);
		
		Query q3 = pm.newQuery(BuildingInfo.class);
		List<BuildingInfo> Bresults;
		
		q3.setFilter("BuildingIndex == index");
		q3.declareParameters("int index");
	
		try{
					Vresults = (List<VillageInfo>) q2.execute(id);
					if(Vresults.size()>0)	//유저 마을이 존재하냐?
					{
						Bresults = (List<BuildingInfo>) q3.execute(bindex);
						if(Bresults.size()>0)	//건물정보가 존재하냐? 
						{
								VillageInfo user = Vresults.get(0);
								BuildingInfo binfo = Bresults.get(0);
								
								user.AddCostA(-1*binfo.getBuildingProfitA());
								user.AddCostB(-1*binfo.getBuildingProfitB());
								user.AddExp(binfo.getBuildingCExp());
								VillageBuilding vb = new VillageBuilding(id, index, bindex,location,direction);
								pm.makePersistent(vb);
								inputChk=true;
						}
						else
						{
							throw new IOException("에러는B "+Bresults.size());
						}
					}
					else{
					throw new IOException("에러는A "+Vresults.size());
					}

			}finally{
	
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
