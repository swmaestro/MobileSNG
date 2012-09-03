package com.sngSM3.DataStores;

import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;
import com.sngSM3.TimeChecker;
// 마을의 건물을 저장하는 데이
@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class VillageBuilding {
	@PrimaryKey
	@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
	private Key key;
	
	@Persistent 
	private String memberId;	
	
	@Persistent 
	private int BuildingIndex;
	
	@Persistent 
	private int VBIndex;
	
	@Persistent 
	private int VBState;
	
	@Persistent 
	private int VBLocation;

	@Persistent 
	private boolean VBDirection;
	
	@Persistent 
	private Date VBUpdateDate;
	
	public static final int BUILDING_BUILD = 0;
	public static final int BUILDING_WAITING = 1;//밭을 위한 기능 
	public static final int BUILDING_PRODUCTING = 2;
	public static final int BUILDING_ENDPRODUCT = 3;//생산 완
	public static final int BUILDING_PRODUCTFAIL = 4;//생산 실패 
	public static final int BUILDING_REQUEST_WAITING = 5; //다른 유저의 요청을 받아 유저가 승인하기 전에 대기
	public static final int BUILDING_ALONE_PRODUCTING =6; //자가 생
	public static final int BUILDING_ALONE_ENDPRODUCT =7; //자가 생
	
	
	
	public VillageBuilding(String memberId, int vbIndex, int buildingIndex, int vBLocation, boolean vBDirection) {
		super();
		this.memberId = memberId;
		VBIndex=vbIndex;
		BuildingIndex = buildingIndex;
		VBState = BUILDING_BUILD;
		VBLocation = vBLocation;
		VBDirection = vBDirection;
		VBUpdateDate=TimeChecker.GetNowDate();
		
	}

	public void StartAloneProductingMode()
	{
		UpdateDate();
		VBState=BUILDING_ALONE_PRODUCTING;
	}
	public void EndAloneProductingMode()
	{
		UpdateDate();
		VBState=BUILDING_WAITING;
	}
	public void UpdateDate()
	{
		VBUpdateDate=TimeChecker.GetNowDate();
	}
	
	public Date getVBUpdateDate() {
		return VBUpdateDate;
	}

	public int getVBState() {
		return VBState;
	}
	public void setVBState(int vBComplete) {
		VBState = vBComplete;
		UpdateDate();
	}

	public int getVBIndex() {
		return VBIndex;
	}

	public int getVBLocation() {
		return VBLocation;
	}

	public void setVBLocation(int vBLocation) {
		VBLocation = vBLocation;
	}

	public boolean getVBDirection() {
		return VBDirection;
	}

	public void setVBDirection(boolean vBDirection) {
		VBDirection = vBDirection;
	}

	public String getMemberId() {
		return memberId;
	}

	public int getBuildingIndex() {
		return BuildingIndex;
	}
	
	public String GetUpdatedDate()
	{
		@SuppressWarnings("deprecation")
		String timeString=String.format("%04d-%02d-%02d-%02d-%02d-%02d",VBUpdateDate.getYear()+1900,VBUpdateDate.getMonth()+1,VBUpdateDate.getDate(),VBUpdateDate.getHours(),VBUpdateDate.getMinutes(),VBUpdateDate.getSeconds());
		
		return timeString;
		
	}
	
}
