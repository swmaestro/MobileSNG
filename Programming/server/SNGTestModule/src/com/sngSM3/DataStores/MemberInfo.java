package com.sngSM3.DataStores;

import java.util.Date;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;

@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class MemberInfo {
		@PrimaryKey
		@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
		private Key key;
		
		@Persistent 
		private String memberId;	//유저 아이디 기본적으로 영문 지
		
		@Persistent 
		private String memberPhone;
		
		@Persistent
		private String memberPd;
		
		@Persistent
		private Date memberLoginDate;
	

		
		public MemberInfo(String Id,String phone, String pass, Date Updatedate)
		{
			this.memberId=Id;
			this.memberPd=pass;
			this.memberPhone=phone;
			this.memberLoginDate=Updatedate;
		
		}
		
		public String GetId()	{	return memberId;	}
		public String GetPhone() 	{
			return memberPhone;
		}
		public boolean ChkPassWord(String Pass){
			return Pass.equals(this.memberPd);
		}
		public Date GetUpdateDate(){
			return this.memberLoginDate;
		}
		public String GetUpdatedDate()
		{
			@SuppressWarnings("deprecation")
			String timeString=String.format("%04d-%02d-%02d-%02d-%02d-%02d",memberLoginDate.getYear()+1900,memberLoginDate.getMonth()+1,memberLoginDate.getDate(),memberLoginDate.getHours(),memberLoginDate.getMinutes(),memberLoginDate.getSeconds());
			
			return timeString;
			
		}
		public void SetPhone(String p){
			this.memberPhone = p;
		}
		public void SetDate(Date d){
			this.memberLoginDate= d;
		}

}
