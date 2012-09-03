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

public class IsBuildingRequest extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String owner=req.getParameter("owner");    //get으로 전송된 내용 name(name)값으로 받는다.
		String requester=req.getParameter("requester");  

	
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(BuildingRequestInfo.class);
		List<BuildingRequestInfo> results;
		Boolean inputChk=false;
		q1.setFilter("memberId == id && RequesterId ==  rid &&RequestState < 3");
		q1.declareParameters("String id,String rid");
		q1.setRange(0,1);
		try{
			results = (List<BuildingRequestInfo>) q1.execute(owner,requester);
			if(results.size()==0)
			{
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