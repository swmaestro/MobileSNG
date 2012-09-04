package com.sngSM3.VillageControl;

import java.io.IOException;
import java.util.ArrayList;
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
import com.sngSM3.DataStores.VillageInfo;

public class FriendInfoUpdate extends HttpServlet{
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws IOException, ServletException {
		String id=req.getParameter("id");    //get으로 전송된 내용 name(name)값으로 받는다.
		int Type=(int)new Integer(req.getParameter("type"));  
		int page=(int)new Integer(req.getParameter("page")); 
		PersistenceManager pm = PMF.getPMF().getPersistenceManager();
		Boolean inputChk=false;
		//1은 자신이 등록한, 2는 자신을 등록
		Query q1 = pm.newQuery(FriendInfo.class);
		Query q2 = pm.newQuery(VillageInfo.class);
		List<FriendInfo> results;
		List<VillageInfo> resultFriends;
		
		switch(Type)
		{
		case 1:
			q1.setFilter("memberId == id");
			q1.declareParameters("String id");
			break;
		case 2:
			q1.setFilter("friendId == id");
			q1.declareParameters("String id");
			break;
		default:
			throw new IOException("지원하지 않는 타입");
		}

		
		q2.setFilter("memberId == id");
		q2.declareParameters("String id");
		q2.setRange(0+(10*(page-1)),10+(10*(page-1)));
		try{
				results = (List<FriendInfo>) q1.execute(id);
				List<VillageInfo> friendList = new ArrayList<VillageInfo>();
				for(int i=0;i<results.size();i++)
				{
					resultFriends = (List<VillageInfo>) q2.execute(results.get(i).getFriendId());
					if(resultFriends.size()>0)
						friendList.add(resultFriends.get(0));
				}

				inputChk=true;
			
				req.setAttribute("result", (java.util.Iterator)friendList.iterator());
				req.setAttribute("count", new Integer(friendList.size()));
				RequestDispatcher dip = getServletContext().getRequestDispatcher("/VillageControl/VillageInfoData.jsp");//위치 수정하기 
				dip.forward(req, resp);
				
			}finally{
				q1.closeAll();
				q2.closeAll();
				pm.close();
			}
			

		  
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        doGet(request, response);
    }
	
}
