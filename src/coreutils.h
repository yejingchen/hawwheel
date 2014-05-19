/*
 * coreutils.h
 * 常用的核心函数
 * 已合并 on May 18, 2014
 */
int toRight();

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
	if (getSensor(SENSOR_FORWARD, IRLED_FORWARD) > STD_DST_FORWARD)
		return true;
	else 
		return false;
}

/* 超车模块，当检测到车前方有障碍物时向左转，绕过障碍物。
   返回 0 */
bool rotating = true;
bool changeToLeft = false;
bool changeToRight = false;
bool overtook = false;
int tmpRB;
bool forwardOnLeft;
int func_overtake()
{
	while (!overtook) {
		if (rotating) {
			// 状态：在障碍物后面调整车身姿态，准备换到左车道
			motor(LEFT, FORWARD, 0);
			motor(RIGHT, FORWARD, STD_SPEED_R);

			if (getSensor(SENSOR_FORWARD, IRLED_FORWARD) < FORWARD_TOO_FAR) {
				// 车身已转动到位，换到左车道
				rotating = false;
				changeToLeft = true;
			} else {
				rotating = true;
				changeToLeft = false;
			}
		} else if (!rotating && changeToLeft) {
			// 状态：换往左车道
			digitalWrite(LED, HIGH); // debug
			delay(100);
			motor(LEFT, FORWARD, STD_SPEED_L);
			motor(RIGHT, FORWARD, STD_SPEED_R);

			tmpRB = getSensor(SENSOR_RB, IRLED_RB);
			delay(5);
			tmpRB = getSensor(SENSOR_RB, IRLED_RB) - tmpRB;

			if (tmpRB < 0) {
				// 右后灯强度开始变小，准备在左车道直行
				changeToLeft = false;
				forwardOnLeft = true;
			} else {
				changeToLeft = true;
			}
		} else if (!changeToLeft && forwardOnLeft) {
			// 状态：在左车道直行，最后回到右车道
			toRight();
			
			if (getSensor(SENSOR_FORWARD, IRLED_FORWARD) > FORWARD_CRITICAL) {
				// 当右转到遇到右边的墙壁时，左转成遇道路平行
				forwardOnLeft = false;
				changeToRight = true;
			} else {
				forwardOnLeft = true;
			}
		} else if (!forwardOnLeft && changeToRight) {
			// 已经右转到右边的墙壁，将车身调节成与墙壁平行
			motor(LEFT, FORWARD, 0);
			motor(RIGHT, FORWARD, STD_SPEED_R);
		}
		
		if (
			(!forwardOnLeft && changeToRight) &&
			(getSensor(SENSOR_FORWARD, IRLED_FORWARD) < FORWARD_TOO_FAR)
			) {
			// 退出超车模式
			overtook = true;
		}

	} // while end 

	return 0;
}

// 靠右行驶模块，在此状态下车子会尝试寻找右边的墙壁并沿着它前行。
// 返回 0
int RF_shift; /* 偏移量 */
int RB_shift;
float SP_L = STD_SPEED_L;
float SP_R = STD_SPEED_R;
int toRight()
{
	// 把 //Serial 替换成 Serial 来取消串口通信的注释
	// RF_shift: 离墙壁越近，数值越大
	RF_shift = (getSensor(SENSOR_RF, IRLED_RF) - STD_DST_RF) / STD_JITTER;
	RB_shift = (getSensor(SENSOR_RB, IRLED_RB) - STD_DST_RB) / STD_JITTER;

	// 综合 SENSOR_RF 和 SENSOR_RB 的数据来判断何时转弯
	// 很多 if 的版本

	if (RB_shift - RF_shift > 0)
	{
		if (RB_shift < 0)
		{
			// 此时车头、车尾都离墙较远，且车头偏向左，应向右转
			//Serial.println("Turn right");
			//Serial.print("B-F: ");
			//Serial.println(RB_shift - RF_shift);
			//Serial.print("B: ");
			//Serial.print(RB_shift);
			//Serial.print("    F: ");
			//Serial.println(RF_shift);
			SP_R = STD_SPEED_R + RF_shift*K_SPEED;
			//analogWrite(MR_SPEED, STD_SPEED_R - (RB_shift + RF_shift)*1.5); //右轮减速
		} 
		else 
		{
			//Serial.println("keep");
			//Serial.print("B-F: ");
			//Serial.println(RB_shift - RF_shift);
			//Serial.print("B: ");
			//Serial.print(RB_shift);
			//Serial.print("    F: ");
			//Serial.println(RF_shift);
			SP_R = STD_SPEED_R;
			SP_L = STD_SPEED_L;

		}
	} 
	else if (RB_shift - RF_shift < 0)
	{
		if (RF_shift >= 0) 
		{
			// 此时车头离墙较近，且车头偏向右，应向左转 
			//Serial.println("Turn left");
			//Serial.print("B-F: ");
			//Serial.println(RB_shift - RF_shift);
			//Serial.print("B: ");
			//Serial.print(RB_shift);
			//Serial.print("    F: ");
			//Serial.println(RF_shift);
			SP_L = STD_SPEED_L - RF_shift*K_SPEED;
			//analogWrite(ML_SPEED, STD_SPEED_L + (RB_shift - RF_shift)*1.5); //左轮减速
		} 
		else 
		{
			//Serial.println("keep");
			//Serial.print("B-F: ");
			//Serial.println(RB_shift - RF_shift);
			//Serial.print("B: ");
			//Serial.print(RB_shift);
			//Serial.print("    F: ");
			//Serial.println(RF_shift);
			SP_R = STD_SPEED_R;
			SP_L = STD_SPEED_L;
		}
	} 
	else if (RF_shift == RB_shift)
	{
		if (RB_shift > 0) 
		{
			// 车头直向前，且车离墙较近，应向左转
			//Serial.println("Turn left");
			//Serial.print("B-F: ");
			//Serial.println(RB_shift - RF_shift);
			//Serial.print("B: ");
			//Serial.print(RB_shift);
			//Serial.print("    F: ");
			//Serial.println(RF_shift);
			SP_L = STD_SPEED_L - (RF_shift)*K_SPEED;
			//analogWrite(ML_SPEED, STD_SPEED_L - (RF_shift)*1.5); //左轮减速
		} 
		else if (RB_shift < 0) 
		{
			// 车头直向前，且车离墙较远，应向右转 
			//Serial.println("Turn right");
			//Serial.print("B-F: ");
			//Serial.println(RB_shift - RF_shift);
			//Serial.print("B: ");
			//Serial.print(RB_shift);
			//Serial.print("    F: ");
			//Serial.println(RF_shift);
			SP_R = STD_SPEED_R + (RF_shift)*K_SPEED;
			//analogWrite(MR_SPEED, STD_SPEED_R + (RF_shift)*1.5); //右轮减速
		} 
		else 
		{
			//Serial.println("keep");
			//Serial.print("B-F: ");
			//Serial.println(RB_shift - RF_shift);
			//Serial.print("B: ");
			//Serial.print(RB_shift);
			//Serial.print("    F: ");
			//Serial.println(RF_shift);
			SP_R = STD_SPEED_R;
			SP_L = STD_SPEED_L;
		}
	}

	//Serial.print("L : ");
	//Serial.print(SP_L);
	//Serial.print("     R : ");
	//Serial.println(SP_R);
	//Serial.println("\n---------------------------\n");

	analogWrite(ML_SPEED, SP_L);
	analogWrite(MR_SPEED, SP_R);
	// delay(500);
}
