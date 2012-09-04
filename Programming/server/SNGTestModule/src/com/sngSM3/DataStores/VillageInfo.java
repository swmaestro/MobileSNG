package com.sngSM3.DataStores;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;
@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class VillageInfo {
	@PrimaryKey
	@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
	private Key key;
	@Persistent 
	private String memberId;	
	@Persistent 
	private int VillageLevel;
	@Persistent 
	private int VillageCostA;
	@Persistent 
	private int VillageCostB;
	@Persistent 
	private float VillageExp;
	

	public boolean AddExp(float e)	//True 레벨 업, False No 레벨
	{
		this.VillageExp+=e;
		if(this.VillageExp>=100.0)
		{
			this.VillageExp=0;
			this.VillageLevel+=1;
			return true;
		}
		else
			return false;
		
	}
	public void AddCostA(int c)
	{
		VillageCostA+=c;
	}
	public void AddCostB(int c)
	{
		VillageCostB+=c;
	}

	public VillageInfo(String memberId, int villageCostA,
			int villageCostB) {
		this.memberId = memberId;
		VillageLevel = 1;
		VillageCostA = villageCostA;
		VillageCostB = villageCostB;
		VillageExp = 0;
	}


	public String getMemberId() {
		return memberId;
	}

	public int getVillageLevel() {
		return VillageLevel;
	}


	public void setVillageLevel(int villageLevel) {
		VillageLevel = villageLevel;
	}


	public int getVillageCostA() {
		return VillageCostA;
	}


	public void setVillageCostA(int villageCostA) {
		VillageCostA = villageCostA;
	}


	public int getVillageCostB() {
		return VillageCostB;
	}


	public void setVillageCostB(int villageCostB) {
		VillageCostB = villageCostB;
	}


	public float getVillageExp() {
		return VillageExp;
	}


}
