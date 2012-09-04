package com.sngSM3;

import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;

public class TimeChecker {


	public static Date GetNowDate()
	{
		TimeZone.setDefault(TimeZone.getTimeZone("Asia/Seoul"));
	
		Calendar Temp = Calendar.getInstance();
	
		return Temp.getTime();
	}
	
	
	
	public static Date GetTimeAdder(Date a, int tA) //HHMMSS int형태의 타입은 저렇게 나온
	{
		int h= (tA/10000);
		int m = (tA-h*10000)/100;
		int s = tA%100;
		Calendar Adder = Calendar.getInstance();
		Adder.setTime(a);
		Adder.add(Calendar.HOUR, h);
		Adder.add(Calendar.MINUTE, m);
		Adder.add(Calendar.SECOND, s);
		return Adder.getTime();
	}
	
	public static boolean CheckNowTime(Date a)
	{
		Date nowTime =GetNowDate();
	//	System.out.println("A :" + a.toString());
	//	System.out.println("B :" + nowTime.toString());
		return a.before(nowTime);
	}
	
	public static boolean CompareTime(Date a, Date b) //a가 b보다 이후이면 true아니면 false 
	{
			return a.after(b);
	}
}
