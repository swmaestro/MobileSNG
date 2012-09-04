package com.sngSM3.VillageControl;

import java.io.IOException;

import javax.jdo.PersistenceManager;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.sngSM3.PMF;
import com.sngSM3.DataStores.FriendInfo;

public class FriendConect extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		String friend=req.getParameter("friend");    //get으로 전송된 내용 name(name)값으로 받는다.


		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		
		try{
				FriendInfo friendr = new FriendInfo(id, friend);
				pm.makePersistent(friendr);
				inputChk=true;
			
				
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
