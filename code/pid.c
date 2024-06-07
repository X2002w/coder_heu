/*
 * pid.c
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */
#include "zf_common_headfile.h"
//���pid����
float l_motor_kp=9;
float l_motor_ki=1.3;
float l_motor_kd=0;

float r_motor_kp=9;
float r_motor_ki=1.3;
float r_motor_kd=0;

//���pid����
float servo_kp=6.5;
float servo_ki=0;
float servo_kd=0;
float servo_kp1;

int a;
int pid_servo(float Err)
{
    int duty;
    float err,l_err;
    err=Err;

    //�����̬kp
    servo_kp1=err*err/740+servo_kp;
    //ָ����̬kp
    servo_kp1=(double)(abs((exp(-abs(err))-1)/(exp(-abs(err))+1))/2+servo_kp)*2;

    duty=servo_kp*err+servo_kd*(err-l_err);
    l_err=err;
    duty=servos_center+duty;
    if(duty>=servos_max)
        duty=servos_max;
    if(duty<=servos_min)
        duty=servos_min;

    return duty;
}





int pid_l_motor(int actual_speed,int set_speed)
{
    volatile static int duty,l_err;
    volatile static int err;
    err=set_speed-actual_speed;
    duty+=(int)(l_motor_kp*(err-l_err)+l_motor_ki*err);
    l_err=err;

    if(duty>=8000)
        duty=8000;
    else if(duty<=-8000)
        duty=-8000;


    return (int)duty;
}

int pid_r_motor(int actual_speed,int set_speed)
{
    volatile  static int duty,l_err;
    volatile  static int err;
    err=set_speed-actual_speed;
    duty+=(int)(r_motor_kp*(err-l_err)+r_motor_ki*err);
    l_err=err;


    if(duty>=8000)
        duty=8000;
    else if(duty<=-8000)
        duty=-8000;


    return (int)duty;
}
