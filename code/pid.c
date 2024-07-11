/*
 * pid.c
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */
#include "zf_common_headfile.h"
//电机pid参数
float l_motor_kp=15;//17,3.2
float l_motor_ki=3.7;
float l_motor_kd=0;
int l_motor_l_err=0;
int l_motor_duty=0;

float r_motor_kp=18;//23,3.4
float r_motor_ki=3.7;
float r_motor_kd=0;
int r_motor_l_err=0;
int r_motor_duty=0;

//舵机pid参数
float servo_kp=8.2;
//float servo_kp=1.9;
float servo_ki=0;
float servo_kd=0;
float servo_kp1=0;
float servo_l_err=1.3;

float kd_ratio=0;//1.9;

//float kd_ratio=1.6;

int pid_servo(float Err)
{
    int duty;
    float err;
    err=Err;


    //二次项动态kp
   // servo_kp1=err*err/740+servo_kp;
    //指数动态kp3
   // servo_kp1=(float)(abs((exp(-abs(err))-1)/(exp(-abs(err))+1))/2+0.5)*servo_kp;
   // servo_kp1=(float)(abs((exp(-abs(err))-1)/(exp(-abs(err))+1))/2+servo_kp)*1.07;

  /*  if(servo_kp1>=10.3)
        servo_kp1=10.3;*/
    servo_kd=servo_kp*kd_ratio;

    duty=servo_kp*err+/*0.0015*err*abs(err)*/+servo_kd*(err-servo_l_err)/*-imu660ra_gyro_z*0.03*/;
   // printf("%d,%d,%d\n",imu660ra_gyro_x,imu660ra_gyro_y,imu660ra_gyro_z);
    // duty=-im u660ra_gyro_z*0.12;
    servo_l_err=err;
    duty=servos_center+duty;
    if(duty>=servos_max)
        duty=servos_max;
    if(duty<=servos_min)
        duty=servos_min;

    return duty;
}





int pid_l_motor(int actual_speed,int set_speed)
{

    int err;
    err=set_speed-actual_speed;
    l_motor_duty+=(int)(l_motor_kp*(err-l_motor_l_err)+l_motor_ki*err);
    l_motor_l_err=err;

   /* if(straight_flag)
    {
        if(err>150)
        {
            duty+=4000;
        }

        /*if(err>10)
            duty+=200;
        else if(err>20)
            duty+=400;
        else if(err>40)
            duty+=800;
        else if(err>70)
            duty+=1200;
        else if(err>80)
            duty+=1500;
        else if(err>100)
            duty+=3000;
        else
            duty+=4000;
        */
  /*  }
    if(jisha_flag)
    {
        if(err<-50)
            duty-=8000;



    }*/



    if(l_motor_duty>=8000)
        l_motor_duty=8000;
    else if(l_motor_duty<=-8000)
        l_motor_duty=-8000;


    return l_motor_duty;
}

int pid_r_motor(int actual_speed,int set_speed)
{

    int err;
    err=set_speed-actual_speed;
    r_motor_duty+=(int)(r_motor_kp*(err-r_motor_l_err)+r_motor_ki*err);
    r_motor_l_err=err;

   /* if(straight_flag)
    {
            if(err>150)
            {
                duty+=4000;
            }*/

            /*if(err>10)
                duty+=200;
            else if(err>20)
                duty+=400;
            else if(err>40)
                duty+=800;
            else if(err>70)
                duty+=1200;
            else if(err>80)
                duty+=1500;
            else if(err>100)
                duty+=3000;
            else
                duty+=4000;
            */
       /* }
        if(jisha_flag)
        {
            if(err<-50)
                duty-=8000;
        }*/

    if(r_motor_duty>=8000)
        r_motor_duty=8000;
    else if(r_motor_duty<=-8000)
        r_motor_duty=-8000;


    return r_motor_duty;
}
