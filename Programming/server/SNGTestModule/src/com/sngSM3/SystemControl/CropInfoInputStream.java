package com.sngSM3.SystemControl;

import java.io.IOException;

import javax.jdo.PersistenceManager;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.sngSM3.PMF;
import com.sngSM3.DataStores.CropInfo;

public class CropInfoInputStream extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		int ver=(int)new Integer(req.getParameter("ver"));    //get으로 전송된 내용 name(name)값으로 받는다.
		int index=(int)new Integer(req.getParameter("index"));    //get으로 전송된 내용 name(id)값으로 받는다.
		String name = req.getParameter("name");
		int costA=(int)new Integer(req.getParameter("costA"));  
		int profitA=(int)new Integer(req.getParameter("profitA"));  
		int costB=(int)new Integer(req.getParameter("costB"));  
		int profitB=(int)new Integer(req.getParameter("profitB"));  
		int btime=(int)new Integer(req.getParameter("btime"));  
		int stime=(int)new Integer(req.getParameter("stime"));   
		int level=(int)new Integer(req.getParameter("level"));  
		float pexp=(Float)new Float(req.getParameter("PExp")); 
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		
		try{
				CropInfo building = new CropInfo(ver, index, name, costA,costB,profitA,profitB,btime,stime,level,pexp);
				pm.makePersistent(building);
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
