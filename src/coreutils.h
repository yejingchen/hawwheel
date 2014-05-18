/*
 * coreutils.h
 * 常用的核心函数
 * 已合并 on May 18, 2014
 */

// 获取自然光，返回自然光的值
int getNL(int sensorPin, int irPin)
{
	int ret;
	digitalWrite(irPin,0);
	delay(2);
	ret = analogRead(sensorPin);
	digitalWrite(irPin,1);
	return ret;
}

inline void turnOffthelight()
{
	for(int i=2; i<=6; i++)
	{
	    digitalWrite(i, 0);
	}
	delay(1);
}

// 获取传感器的状态，返回已减去自然光的传感值
inline int getSensor(int sensorPin, int irPin)
{
	int ret = 0;
	turnOffthelight();
	int NL = getNL(sensorPin, irPin);
	delay(1);
	for (int i=0;i<=5;i++)
	{
		ret += NL - analogRead(sensorPin);
		delay(1);
	}
	
	return ret/6;
}

// 控制车轮速度、方向
void motor(boolean left, boolean forward, int speed)
{
	if (left) 
	{
		digitalWrite(ML_DIR, forward);
		analogWrite(ML_SPEED, speed);
	} 
	else
	{
		digitalWrite(MR_DIR, forward);
		analogWrite(MR_SPEED, speed);
	}
} 

// 遇障停车的主函数，当车前方检测到障碍，即停下。与超车函数不兼容
int stopForwardObstacle()
{
	motor(LEFT, FORWARD, 0);
	motor(RIGHT, FORWARD, 0);
	Serial.println("Left right motor stopped!");

	return 0;
}

// 检测是否有障碍物，返回 true 当前方有障碍物，否则返回 false
bool barrier()
{
	if (getSensor(SENSOR_FORWARD) > 55)
		return true;
	else 
		return false;
}

/* 超车模块，当检测到车前方有障碍物时向左转，绕过障碍物。
   返回 0 */
bool rotating = true;
bool changeToLeft = true;
bool changeToRight = false;
bool overtook = false;
int tmpRB;
bool forwardOnLeft;
int func_overtake()
{
	while (!overtook) {
		if (rotating) {
			/* 状态：在障碍物后面调整车身姿态，准备换到左车道 */
			motor(LEFT, FORWARD, TURNING_SPEED_L);
			motor(RIGHT, FORWARD, HALF_SPEED);

			if (getSensor(SENSOR_FORWARD) < FORWARD_TOO_FAR) {
				/* 车身已转动到位，换到左车道 */
				rotating = false;
				changeToLeft = true;
			} else {
				rotating = true;
			}
		} else if (!rotating && changeToLeft) {
			/* 状态：换往左车道 */
			motor(LEFT, FORWARD, STD_SPEED_L);
			motor(RIGHT, FORWARD, STD_SPEED_R);

			tmpRB = getSensor(SENSOR_RB);
			delay(5);
			tmpRB = getSensor(SENSOR_RB) - tmpRB;

			if (tmpRB < 0) {
				/* 右后灯强度开始变小，准备在左车道直行 */
				changeToLeft = false;
				forwardOnLeft = true;
			} else {
				changeToLeft = true;
			}
		} else if (!changeToLeft && forwardOnLeft) {
			/* 状态：在左车道直行，最后回到右车道 */
			toRight();
			turnedLeft = true;
			turningLeft = true;
			
			if (getSensor(SENSOR_FORWARD) > FORWARD_CRITICAL) {
				/* 当右转到遇到右边的墙壁时，左转成遇道路平行 */
				forwardOnLeft = false;
				changeToRight = true;
			} else {
				forwardOnLeft = true;
			}
		} else if (!forwardOnLeft && changeToRight) {
			/* 已经右转到右边的墙壁，将车身调节成与墙壁平行 */
			motor(LEFT, FORWARD, 0);
			motor(RIGHT, FORWARD, STD_SPEED_R);
		} else if (getSensor(SENSOR_FORWARD) < FORWARD_TOO_FAR) {
			/* 退出超车模式 */
			overtook = true;
		}

	} /* while end */ 

	return 0;
}
