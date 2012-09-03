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

// 친구가 요청 -> 요청 대기 -> 유저가 승인 ->생산 시작 -> 생산 완료(유저가 확인을 해야함) -> 그 후 요청한 친구가 확인(모두 이 때 돈 및 경험치 증가) 

@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class BuildingRequestInfo {
		@PrimaryKey
		@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
		private Key key;
		
		@Persistent 
		private String memberId;
		
		@Persistent 
		private String RequesterId;
		
		@Persistent 
		private Date RequestDate;
		//처음 요청 시간, 두번째 승인 시간으로 변경
		
		@Persistent 
		private int RequestState;		

		@Persistent 
		private int VBIndex;	//요청한 건물의 주인의 고유 번호
		
		@Persistent 
		private int BuildingIndex;//요청한 건물의 타입 
		
		public static final int REQUEST_WAITING = 0;
		public static final int REQUEST_PRODUCTING = 1;
		public static final int REQUEST_PRODUCT_COMPLETE = 2;
		public static final int REQUEST_COMPLETE = 3;// 여기서 파

	//	public static final int REQUEST_CANCEL = -1; //주인 또는 상대가 취소
		
		public Date getRequestDate() {
			return RequestDate;
		}

		public void SetRequestDate() {
			RequestDate =TimeChecker.GetNowDate();
		}

		public int getRequestState() {
			return RequestState;
		}

		public void setRequestState(int requestState) {
			RequestState = requestState;
		}

		public String getMemberId() {
			return memberId;
		}

		public String getRequesterId() {
			return RequesterId;
		}

		public int getBuildingIndex() {
			return BuildingIndex;
		}

		public int getVBIndex() {
			return VBIndex;
		}

		public BuildingRequestInfo(String memberId, String requesterId,
			int vBIndex, int buildingIndex) {
			super();
			this.memberId = memberId;
			RequesterId = requesterId;
			RequestDate =TimeChecker.GetNowDate();
			RequestState = REQUEST_WAITING;
			VBIndex = vBIndex;
			BuildingIndex = buildingIndex;
		}
		
		
}
