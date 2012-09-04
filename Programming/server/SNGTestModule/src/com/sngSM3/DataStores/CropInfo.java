package com.sngSM3.DataStores;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;

@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class CropInfo {


	@PrimaryKey
	@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
	private Key key;
	@Persistent 
	private int SystemVersion;
	@Persistent 
	private int CropIndex;
	@Persistent 
	private String CropName;
	@Persistent 
	private int CropCostA;
	@Persistent 
	private int CropCostB;
	
	@Persistent 
	private int CropProfitA;
	@Persistent 
	private int CropProfitB;
	
	@Persistent 
	private int CropBTime; // 자라는 시 시간
	@Persistent 
	private int CropSTime; //농작물이 생존해있는 시	
	
	@Persistent 
	private int CropLevel;
	
	//0.5 추가
	@Persistent 
	private float CropPExp;
	
	

	public float getCropPExp() {
		return CropPExp;
	}
	public void setCropPExp(float cropPExp) {
		CropPExp = cropPExp;
	}
	public void setCropIndex(int cropIndex) {
		CropIndex = cropIndex;
	}
	public CropInfo(int systemVersion, int cropIndex, String cropName,
			int cropCostA, int cropCostB, int cropProfitA, int cropProfitB,
			int cropBTime, int cropSTime, int cropLevel, float cropPExp) {
		super();
		SystemVersion = systemVersion;
		CropIndex = cropIndex;
		CropName = cropName;
		CropCostA = cropCostA;
		CropCostB = cropCostB;
		CropProfitA = cropProfitA;
		CropProfitB = cropProfitB;
		CropBTime = cropBTime;
		CropSTime = cropSTime;
		CropLevel = cropLevel;
		CropPExp = cropPExp;
	}
	public int getSystemVersion() {
		return SystemVersion;
	}
	public void setSystemVersion(int systemVersion) {
		SystemVersion = systemVersion;
	}
	public String getCropName() {
		return CropName;
	}
	public void setCropName(String cropName) {
		CropName = cropName;
	}
	public int getCropCostA() {
		return CropCostA;
	}
	public void setCropCostA(int cropCostA) {
		CropCostA = cropCostA;
	}
	public int getCropCostB() {
		return CropCostB;
	}
	public void setCropCostB(int cropCostB) {
		CropCostB = cropCostB;
	}
	public int getCropProfitA() {
		return CropProfitA;
	}
	public void setCropProfitA(int cropProfitA) {
		CropProfitA = cropProfitA;
	}
	public int getCropProfitB() {
		return CropProfitB;
	}
	public void setCropProfitB(int cropProfitB) {
		CropProfitB = cropProfitB;
	}
	public int getCropBTime() {
		return CropBTime;
	}
	public void setCropBTime(int cropBTime) {
		CropBTime = cropBTime;
	}
	public int getCropSTime() {
		return CropSTime;
	}
	public void setCropSTime(int cropSTime) {
		CropSTime = cropSTime;
	}
	public int getCropLevel() {
		return CropLevel;
	}
	public void setCropLevel(int cropLevel) {
		CropLevel = cropLevel;
	}
	public int getCropIndex() {
		return CropIndex;
	}
	
	
}
