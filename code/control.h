/*
 * control.h
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */

#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_


extern int target_speed;//小车车身目标速度
extern int left_white_num;
extern int right_white_num;
extern float straight_dis;//现实实际距离映射

//电机驱动相关信息
void motor_driver(int speed_l,int speed_r);//电机驱动函数

#endif /* CODE_CONTROL_H_ */
