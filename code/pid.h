/*
 * pid.h
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#define servos_center 4710//440,4545������ֵ
#define servos_max 5100
#define servos_min 4320

//���pid����
extern float l_motor_kp;
extern float l_motor_ki;
extern float l_motor_kd;

extern float r_motor_kp;
extern float r_motor_ki;
extern float r_motor_kd;


int pid_servo(float Err);
int pid_l_motor(int actual_speed,int set_speed);
int pid_r_motor(int actual_speed,int set_speed);

#endif /* CODE_PID_H_ */
