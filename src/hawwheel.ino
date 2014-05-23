/*
 * hawwheel.ino
 * 已合并 on May 23, 2014
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
	if (ifbarn())  {
		pullup();
	} else if (forward_barrier()) {
		stop();
	} else if (up_red()) {
		stop();
	}
		
	else toRight();
}
