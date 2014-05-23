/*
 * defines.h
 * 已合并 on May 23, 2014
 */

// 传感器、发射管管脚
#define IRLED_RF 2
#define SENSOR_RF 0

#define IRLED_RB 3
#define SENSOR_RB 1

#define IRLED_FORWARD 6 // 车前、底部、后部的红外 LED 都使用此管脚
#define SENSOR_FORWARD 2

#define SENSOR_UP 3

#define SENSOR_BARN 4 //减速带检测
#define SENSOR_BACK 5 //后轮传感器 

// 普通的 LED
#define LED 13

// 接车轮的管脚
#define ML_DIR 7
#define MR_DIR	8
#define ML_SPEED 9
#define MR_SPEED 10

#define STD_DST_RF 100	// 标准距离!!!!!!!!
#define STD_DST_RB 100
#define STD_DST_FORWARD 60 // 用于启动超车的标准距离

#define STD_SLOWBAR 100 //!!!!!!!!
#define STD_FINDBARN 20//!!!!!!!!

#define STD_SPEED_L 225//!!!!!!!!
#define STD_SPEED_R 220//!!!!!!!!
#define STD_SPEED_L_SLOW 105//!!!!!!!!
#define STD_SPEED_R_SLOW 105//!!!!!!!!

#define HALF_SPEED 100 // 半速

#define STD_JITTER 3 // 正常数据抖动

#define LEFT true // 用于 motor()
#define RIGHT false
#define FORWARD true
#define BACK false

#define K_SPEED 15 // 速度的系数

// func_overtake() 所用到的数据
#define FORWARD_TOO_FAR 30 // 车身自转时，逆时针到角度最大时前灯的强度//!!!!!!!!
#define FORWARD_CRITICAL 48 // 超车完回到右车道时，遇到右墙壁的强度。//!!!!!!!!

// 遇障停车、红灯停用到
#define RED_LIGHT 100//!!!!!!!!
