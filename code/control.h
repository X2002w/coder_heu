/*
 * control.h
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */

#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_




 //小车速度相关信息
extern int set_mode ; //小车模式设置，给予不同速度
extern int target_speed ;//小车车身目标速度
extern int center_speed;//小车车身左右编码加权实际速度
extern int left_encoder, right_encoder;//左右编码器读数
extern int left_speed, right_speed;//左右轮差速目标速度
extern int Target_Speed_l, Target_Speed_r;//左右轮实际速度
extern int speed_ratio ;//差速系数
extern float duty_ratio ;//电机增量误差系数
extern int duty;//电机差速增量



extern int left_white_num ;
extern int right_white_num ;
extern int speed_map;//最终用于映射的白点
extern float straight_dis;//现实实际距离映射



//电机驱动相关信息
void motor_driver(int speed_l,int speed_r);//电机驱动函数

#endif /* CODE_CONTROL_H_ */
