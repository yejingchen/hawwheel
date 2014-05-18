/*
 * hawwheel.ino
 * 没什么好合并的 on May 18, 2014
 */

#include "defines.h"
#include "coreutils.h"

void setup()
{
	pinMode(IRLED_RF, OUTPUT);
	pinMode(IRLED_RB, OUTPUT);
	pinMode(IRLED_FORWARD, OUTPUT);
	Serial.begin(9600);

	pinMode(LED, OUTPUT);

	//run the motor

	pinMode(ML_DIR, OUTPUT);
	pinMode(MR_DIR, OUTPUT);
	pinMode(ML_SPEED, OUTPUT);
	pinMode(MR_SPEED, OUTPUT);

	digitalWrite(ML_DIR, FORWARD);
	digitalWrite(MR_DIR, FORWARD);
	analogWrite(ML_SPEED, STD_SPEED_L);
	analogWrite(MR_SPEED, STD_SPEED_R);
}

void loop()
{
	//toRight();
//	digitalWrite(LED, LOW);
//	motor(LEFT, FORWARD, STD_SPEED_L);
//	motor(RIGHT, FORWARD, STD_SPEED_R);
//	delay(2000);

//	digitalWrite(LED, HIGH);
//	motor(LEFT, BACK, STD_SPEED_L);
//	motor(RIGHT, FORWARD, STD_SPEED_R);
//	delay(2000);

	if (getSensor(SENSOR_FORWARD, IRLED_FORWARD) > STD_DST_FORWARD) {
		func_overtake();
	}

	toRight();

}
