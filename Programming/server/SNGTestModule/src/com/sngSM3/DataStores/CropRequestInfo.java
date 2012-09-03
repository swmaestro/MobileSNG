package com.sngSM3.DataStores;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;

@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class CropRequestInfo {
	@PrimaryKey
	@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
	private Key key;
	
	@Persistent 
	private String memberId;
	
	@Persistent 
	private int VBIndex;	//요청한 건물의 주인의 고유 번호
	
	@Persistent 
	private int CropIndex;

	public CropRequestInfo(String memberId, int vBIndex, int cropIndex) {
		super();
		this.memberId = memberId;
		VBIndex = vBIndex;
		CropIndex = cropIndex;
	}

	public String getMemberId() {
		return memberId;
	}

	public int getVBIndex() {
		return VBIndex;
	}


	public int getCropIndex() {
		return CropIndex;
	}

	
	
}
