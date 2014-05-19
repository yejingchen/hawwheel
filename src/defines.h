/*
 * defines.h
 * 已合并 on May 17, 2014
 */

// 所有值为 11111111 的值都表示还未真实测定

// 传感器、发射管管脚
#define IRLED_RF 4
#define SENSOR_RF 0

#define IRLED_RB 5
#define SENSOR_RB 1

#define IRLED_FORWARD 6
#define SENSOR_FORWARD 2

// 普通的 LED
#define LED 13

// 接车轮的管脚
#define ML_DIR 7
#define MR_DIR	8
#define ML_SPEED 9
#define MR_SPEED 10

#define STD_DST_RF 100	// 标准距离
#define STD_DST_RB 100
#define STD_DST_FORWARD 48 // 用于启动超车的标准距离

#define STD_SPEED_L 255
#define STD_SPEED_R 250
#define TURNING_SPEED_L 11111111 // 转弯时两轮的速度，用于 func_overtake()
#define TURNING_SPEED_R 11111111
#define HALF_SPEED 125 // 半速

#define STD_JITTER 3 // 正常数据抖动

#define LEFT true // 用于 motor()
#define RIGHT false
#define FORWARD true
#define BACK false

#define K_SPEED 15 // 速度的系数

// func_overtake() 所用到的数据
#define FORWARD_TOO_FAR 30 // 车身自转时，逆时针到角度最大时前灯的强度
#define FORWARD_CRITICAL 48 // 超车完回到右车道时，遇到右墙壁的强度。
