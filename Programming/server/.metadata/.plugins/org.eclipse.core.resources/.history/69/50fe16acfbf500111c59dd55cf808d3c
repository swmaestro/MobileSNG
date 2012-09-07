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

//친구 페이지 목록 적
public class RequestBoard extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int state=(int)new Integer(req.getParameter("state")); //고유 번호
		boolean ord = (boolean)new Boolean(req.getParameter("ord"));// 부등호false 아니면 등 true
		boolean userState = (boolean)new Boolean(req.getParameter("usertype"));//true면 내가 요청 받은거, false면 내가 요청한
		int page = (int)new Integer(req.getParameter("page"));	//1번 부터
		
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Query q1 = pm.newQuery(BuildingRequestInfo.class);
		List<BuildingRequestInfo> results;
		if(userState)
		{
			if(ord)
				q1.setFilter("memberId == id && RequestState ==state");
			else
				q1.setFilter("memberId == id && RequestState <= state");
		}
		else
		{
			if(ord)
				q1.setFilter("RequesterId == id && RequestState ==state");
			else
				q1.setFilter("RequesterId == id && RequestState <= state");
		}
		q1.declareParameters("String id,int state");
		q1.setRange(0+(10*(page-1)),10+(10*(page-1)));
		try{
			results = (List<BuildingRequestInfo>) q1.execute(id,state);
			//Vresults.size();	//유저 마을이 존재하냐?

			req.setAttribute("result", results.iterator());
			req.setAttribute("count", results.size());

			}finally{
				q1.closeAll();
				pm.close();
			}
			
		//s	req.setAttribute("result", inputChk);
			RequestDispatcher dip = getServletContext().getRequestDispatcher("/BuildingRequest/RequestList.jsp");
			dip.forward(req, resp);
		  
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
	
}