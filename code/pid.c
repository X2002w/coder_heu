/*
 * pid.c
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */
#include "zf_common_headfile.h"
//电机pid参数
float l_motor_kp=17;
float l_motor_ki=3.2;
float l_motor_kd=0;

float r_motor_kp=23;
float r_motor_ki=3.4;
float r_motor_kd=0;


//舵机pid参数
float servo_kp=9.5;
float servo_ki=0;
float servo_kd=0;
float servo_kp1=0;

int pid_servo(float Err)
{
    int duty;
    float err,l_err;
    err=Err;


    //二次项动态kp
   // servo_kp1=err*err/740+servo_kp;
    //指数动态kp
    //servo_kp1=(double)(abs((exp(-abs(err))-1)/(exp(-abs(err))+1))/2+servo_kp)*1.06;

    if(servo_kp1>=11.5)
        servo_kp1=11.5;
    servo_kd=servo_kp1*0.11;
    duty=servo_kp*err+0.003*err*abs(err)+servo_kd*(err-l_err);//-imu660ra_gyro_z*0.03;
   // printf("%d,%d,%d\n",imu660ra_gyro_x,imu660ra_gyro_y,imu660ra_gyro_z);
    // duty=-imu660ra_gyro_z*0.12;
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
