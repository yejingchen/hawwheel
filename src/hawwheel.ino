/*
 * hawwheel.ino
 * 没什么好合并的 on May 18, 2014
 */

#include "defines.h"
#include "coreutils.h"


int RF_shift; /* 偏移量 */
int RB_shift;
int SP_L = STD_SPEED_L;
int SP_R = STD_SPEED_R;


void setup()
{
	pinMode(SENSOR_RF, OUTPUT);
	pinMode(SENSOR_RB, OUTPUT);
	Serial.begin(9600);

	//run the motor

	pinMode(ML_DIR, OUTPUT);
	pinMode(MR_DIR, OUTPUT);
	pinMode(ML_SPEED, OUTPUT);
	pinMode(MR_SPEED, OUTPUT);

	digitalWrite(ML_DIR, FORWARD);
	digitalWrite(MR_DIR, FORWARD);
	analogWrite(ML_SPEED, STD_SPEED_L);
	analogWrite(MR_SPEED, STD_SPEED_R);

	/////////////////////////


}

void loop()
{
	/* RF_shift: 离墙壁越近，数值越大 */

	RF_shift = (getSensor(SENSOR_RF) - STD_DST_RF) / STD_JITTER;
	RB_shift = (getSensor(SENSOR_RB) - STD_DST_RB) / STD_JITTER;


	/*Serial.print("RF_shift: ");
	Serial.println(RF_shift);
	Serial.print("RB_shift: ");
	Serial.println(RB_shift);*

	/* 综合 SENSOR_RF 和 SENSOR_RB 的数据来判断何时转弯 */
	/* 很多 if 的版本 */


	if (RB_shift - RF_shift > 0)
	{
		if (RB_shift < 0)
		{
			// 此时车头、车尾都离墙较远，且车头偏向左，应向右转
			Serial.println("Turn right");
			Serial.print("B-F: ");
			Serial.println(RB_shift - RF_shift);
			Serial.print("B: ");
			Serial.print(RB_shift);
			Serial.print("    F: ");
			Serial.println(RF_shift);
			SP_R = STD_SPEED_R - (RB_shift - RF_shift)*K_SPEED;
			//analogWrite(MR_SPEED, STD_SPEED_R - (RB_shift + RF_shift)*1.5); //右轮减速
		} 
		else 
		{
			Serial.println("keep");
			Serial.print("B-F: ");
			Serial.println(RB_shift - RF_shift);
			Serial.print("B: ");
			Serial.print(RB_shift);
			Serial.print("    F: ");
			Serial.println(RF_shift);
			SP_R = STD_SPEED_R;
			SP_L = STD_SPEED_L;

		}
	} 
	else if (RB_shift - RF_shift < 0)
	{
		if (RF_shift >= 0) 
		{
			// 此时车头离墙较近，且车头偏向右，应向左转 
			Serial.println("Turn left");
			Serial.print("B-F: ");
			Serial.println(RB_shift - RF_shift);
			Serial.print("B: ");
			Serial.print(RB_shift);
			Serial.print("    F: ");
			Serial.println(RF_shift);
			SP_L = STD_SPEED_L + (RB_shift - RF_shift)*K_SPEED;
			//analogWrite(ML_SPEED, STD_SPEED_L + (RB_shift - RF_shift)*1.5); //左轮减速
		} 
		else 
		{
			Serial.println("keep");
			Serial.print("B-F: ");
			Serial.println(RB_shift - RF_shift);
			Serial.print("B: ");
			Serial.print(RB_shift);
			Serial.print("    F: ");
			Serial.println(RF_shift);
			SP_R = STD_SPEED_R;
			SP_L = STD_SPEED_L;
		}
	} 
	else if (RF_shift == RB_shift)
	{
		if (RB_shift > 0) 
		{
			// 车头直向前，且车离墙较近，应向左转
			Serial.println("Turn left");
			Serial.print("B-F: ");
			Serial.println(RB_shift - RF_shift);
			Serial.print("B: ");
			Serial.print(RB_shift);
			Serial.print("    F: ");
			Serial.println(RF_shift);
			SP_L = STD_SPEED_L - (RF_shift)*K_SPEED;
			//analogWrite(ML_SPEED, STD_SPEED_L - (RF_shift)*1.5); //左轮减速
		} 
		else if (RB_shift < 0) 
		{
			// 车头直向前，且车离墙较远，应向右转 
			Serial.println("Turn right");
			Serial.print("B-F: ");
			Serial.println(RB_shift - RF_shift);
			Serial.print("B: ");
			Serial.print(RB_shift);
			Serial.print("    F: ");
			Serial.println(RF_shift);
			SP_R = STD_SPEED_R + (RF_shift)*K_SPEED;
			//analogWrite(MR_SPEED, STD_SPEED_R + (RF_shift)*1.5); //右轮减速
		} 
		else 
		{
			Serial.println("keep");
			Serial.print("B-F: ");
			Serial.println(RB_shift - RF_shift);
			Serial.print("B: ");
			Serial.print(RB_shift);
			Serial.print("    F: ");
			Serial.println(RF_shift);
			SP_R = STD_SPEED_R;
			SP_L = STD_SPEED_L;
		}
	}

	Serial.print("L : ");
	Serial.print(SP_L);
	Serial.print("     R : ");
	Serial.println(SP_R);
	Serial.println("\n---------------------------\n");
	analogWrite(ML_SPEED, SP_L);
	analogWrite(MR_SPEED, SP_R);
	//delay(500);
	
}
