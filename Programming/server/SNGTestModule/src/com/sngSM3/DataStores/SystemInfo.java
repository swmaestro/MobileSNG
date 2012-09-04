package com.sngSM3.DataStores;

import java.util.Date;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;

@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class SystemInfo {
	@PrimaryKey
	@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
	private Key key;

	@Persistent 
	private int SystemVersion;
	
	@Persistent 
	private Date SystemDate;
	
	@Persistent 
	private String SystemLog;
	
	public SystemInfo(int ver, Date d, String lg){
		this.SystemVersion=ver;
		this.SystemLog=lg;
		this.SystemDate=d;
	}
	
	public String GetUpdateDay()
	{
		String timeString=String.format("%04d-%02d-%02d-%02d-%02d-%02d",SystemDate.getYear()+1900,SystemDate.getMonth()+1,SystemDate.getDate(),SystemDate.getHours(),SystemDate.getMinutes(),SystemDate.getSeconds());
		
		return timeString;
	}
	
	public int GetVersion()
	{
		return SystemVersion;
	}
	
	public String GetLog()
	{
		return SystemLog;
	}
	
}
