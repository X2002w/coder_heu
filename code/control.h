/*
 * control.h
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */

#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_


extern int target_speed;//С������Ŀ���ٶ�
extern int left_white_num;
extern int right_white_num;
extern float straight_dis;//��ʵʵ�ʾ���ӳ��

//������������Ϣ
void motor_driver(int speed_l,int speed_r);//�����������

#endif /* CODE_CONTROL_H_ */
