package com.sngSM3.DataStores;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

import com.google.appengine.api.datastore.Key;

@PersistenceCapable ( identityType = IdentityType.APPLICATION)
public class FriendInfo {
	@PrimaryKey
	@Persistent (valueStrategy = IdGeneratorStrategy.IDENTITY )
	private Key key;

	@Persistent 
	private String memberId;
	@Persistent 
	private String friendId;
	
	public FriendInfo(String memberId, String friendId) {
		super();
		this.memberId = memberId;
		this.friendId = friendId;
	}

	public String getMemberId() {
		return memberId;
	}

	public String getFriendId() {
		return friendId;
	}

	
	
}
