package com.sngSM3.VillageControl;

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
import com.sngSM3.DataStores.FriendInfo;

public class FriendDelete extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		String friend=req.getParameter("friend");    //get으로 전송된 내용 name(name)값으로 받는다.


		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		Query q1 = pm.newQuery(FriendInfo.class);
		List<FriendInfo> results;
		q1.setFilter("memberId == id && friendId == friend");
		q1.declareParameters("String id, String friend");
		try{
				results = (List<FriendInfo>) q1.execute(id,friend);
				if(results.size()>0)
				{
					FriendInfo friendr = results.get(0);
					pm.deletePersistent(friendr);
					inputChk=true;
				}
				
			}finally{
				
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
