package com.sngSM3.VillageControl;

import java.io.IOException;

import javax.jdo.PersistenceManager;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.sngSM3.PMF;
import com.sngSM3.DataStores.VillageInfo;

public class VillageCreator extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int costA=(int)new Integer(req.getParameter("costA"));    //get으로 전송된 내용 name(id)값으로 받는다.
		int costB=(int)new Integer(req.getParameter("costB"));  

		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		
		try{
			VillageInfo village = new VillageInfo(id, costA, costB);
				pm.makePersistent(village);
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
