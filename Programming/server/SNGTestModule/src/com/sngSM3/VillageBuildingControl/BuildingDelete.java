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

public class BuildingDelete extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index")); 
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;

		Query q3 = pm.newQuery(VillageBuilding.class);
		List<VillageBuilding> Vresults;
		
		q3.setFilter("memberId == id && VBIndex == index");
		q3.declareParameters("String id,int index");
		
	
		try{
					Vresults = (List<VillageBuilding>) q3.execute(id,index);
					VillageBuilding deleteData = Vresults.get(0);
					pm.deletePersistent(deleteData);
					inputChk=true;

			}finally{
	
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
