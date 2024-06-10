/*
 * control.h
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */

#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_




 //С���ٶ������Ϣ
extern int set_mode = 0; //С��ģʽ���ã����費ͬ�ٶ�
extern int target_speed = 0;//С������Ŀ���ٶ�
extern int center_speed;//С���������ұ����Ȩʵ���ٶ�
extern int left_encoder, right_encoder;//���ұ���������
extern int left_speed, right_speed;//�����ֲ���Ŀ���ٶ�
extern int Target_Speed_l, Target_Speed_r;//������ʵ���ٶ�
extern int speed_ratio = 430;//����ϵ��
extern float duty_ratio = 0.3;//����������ϵ��
extern int duty;//�����������



extern int left_white_num = 0;
extern int right_white_num = 0;
extern int speed_map;//��������ӳ��İ׵�
extern float straight_dis;//��ʵʵ�ʾ���ӳ��



//������������Ϣ
void motor_driver(int speed_l,int speed_r);//�����������

#endif /* CODE_CONTROL_H_ */
