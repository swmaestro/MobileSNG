package com.sngSM3.DataStores;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;
@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class BuildingInfo {
		@PrimaryKey
		@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
		private Key key;
		@Persistent 
		private int SystemVersion;
		@Persistent 
		private int BuildingIndex;
		@Persistent 
		private String BuildingName;
		@Persistent 
		private int BuildingCostA;
		@Persistent 
		private int BuildingCostB;
		
		@Persistent 
		private int BuildingProfitA;
		@Persistent 
		private int BuildingProfitB;
		
		@Persistent 
		private int BuildingBTime; // 생성 시간
		@Persistent 
		private int BuildingPTime;
		@Persistent 
		private int BuildingSize;
		@Persistent 
		private int BuildingLevel;
		
		//0.5 추가 버젼
		@Persistent 
		private float BuildingCExp;
		@Persistent 
		private float BuildingPExp;
	
		



		public float getBuildingCExp() {
			return BuildingCExp;
		}


		public void setBuildingCExp(float buildingCExp) {
			BuildingCExp = buildingCExp;
		}


		public float getBuildingPExp() {
			return BuildingPExp;
		}


		public void setBuildingPExp(float buildingPExp) {
			BuildingPExp = buildingPExp;
		}


		public BuildingInfo(int systemVersion, int buildingIndex,
				String buildingName, int buildingCostA, int buildingCostB,
				int buildingProfitA, int buildingProfitB, int buildingBTime,
				int buildingPTime, int buildingSize, int buildingLevel,
				float buildingCExp, float buildingPExp) {
			super();
			SystemVersion = systemVersion;
			BuildingIndex = buildingIndex;
			BuildingName = buildingName;
			BuildingCostA = buildingCostA;
			BuildingCostB = buildingCostB;
			BuildingProfitA = buildingProfitA;
			BuildingProfitB = buildingProfitB;
			BuildingBTime = buildingBTime;
			BuildingPTime = buildingPTime;
			BuildingSize = buildingSize;
			BuildingLevel = buildingLevel;
			BuildingCExp = buildingCExp;
			BuildingPExp = buildingPExp;
		}


		public int getSystemVersion() {
			return SystemVersion;
		}


		public void setSystemVersion(int systemVersion) {
			SystemVersion = systemVersion;
		}


		public String getBuildingName() {
			return BuildingName;
		}


		public void setBuildingName(String buildingName) {
			BuildingName = buildingName;
		}


		public int getBuildingBTime() {
			return BuildingBTime;
		}


		public void setBuildingBTime(int buildingBTime) {
			BuildingBTime = buildingBTime;
		}


		public int getBuildingPTime() {
			return BuildingPTime;
		}


		public void setBuildingPTime(int buildingPTime) {
			BuildingPTime = buildingPTime;
		}


		public int getBuildingSize() {
			return BuildingSize;
		}


		public void setBuildingSize(int buildingSize) {
			BuildingSize = buildingSize;
		}


		public int getBuildingLevel() {
			return BuildingLevel;
		}


		public void setBuildingLevel(int buildingLevel) {
			BuildingLevel = buildingLevel;
		}


		public int getBuildingIndex() {
			return BuildingIndex;
		}


		public int getBuildingCostA() {
			return BuildingCostA;
		}


		public void setBuildingCostA(int buildingCostA) {
			BuildingCostA = buildingCostA;
		}


		public int getBuildingCostB() {
			return BuildingCostB;
		}


		public void setBuildingCostB(int buildingCostB) {
			BuildingCostB = buildingCostB;
		}


		public int getBuildingProfitA() {
			return BuildingProfitA;
		}


		public void setBuildingProfitA(int buildingProfitA) {
			BuildingProfitA = buildingProfitA;
		}


		public int getBuildingProfitB() {
			return BuildingProfitB;
		}


		public void setBuildingProfitB(int buildingProfitB) {
			BuildingProfitB = buildingProfitB;
		}	
		
}
