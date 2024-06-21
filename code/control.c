/*
 * control.c
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */
#include "zf_common_headfile.h"
//电机驱动相关信息



//小车速度相关信息
int set_mode=1; //小车模式设置，给予不同速度
int target_speed=340;//小车车身目标速度
int center_speed;//小车车身左右编码加权实际速度
int left_encoder,right_encoder;//左右编码器读数
int left_speed,right_speed;//左右轮差速目标速度
int Target_Speed_l,Target_Speed_r;//左右轮实际速度
int speed_ratio=110;//差速系数
float duty_ratio=0.12;//电机增量误差系数
int duty;//电机差速增量


//速度策略相关,差速，不降速
int straight_jia;
int island_jia;
int ramp_jia;



int left_white_num=0;
int right_white_num=0;
int speed_map;//最终用于映射的白点
float straight_dis;//现实实际距离映射


void encoder_get(void){
    left_encoder=encoder_get_count(TIM6_ENCODER)*-1;
    right_encoder=encoder_get_count(TIM2_ENCODER);
    center_speed = (left_encoder + right_encoder) / 2;
  //  printf("%d,%d\n\r",left_encoder,right_encoder);

    encoder_clear_count(TIM6_ENCODER);
    encoder_clear_count(TIM2_ENCODER);
}

//速度决策
void set_speed(void)
{
    int y, x;
  
    //设置弯道速度为基础速度

    if (set_mode == 0)
        target_speed = 320;
    else if (set_mode == 1)
        target_speed = 390;
    else if (set_mode == 2)
        target_speed = 410;
    else if (set_mode == 3)
        target_speed = 430;
    else if (set_mode == 4)
        target_speed = 450;
    else if (set_mode == 5)
        target_speed = 470;
    else if (set_mode == 6)
        target_speed = 490;
    else if (set_mode == 7)
        target_speed = 510;
    else if (set_mode == 8)
        target_speed = 530;
    else if (set_mode == 9)
        target_speed = 560;

    for (y= MT9V03X_H-1;y>=0;y--)
    {
        if (bin_image[y][left_map[y]]==0)
        {
            break;
        }
        else 
        {
            left_white_num++;
            //printf("%d\n",left_white_num);
        }
        //printf("%d    %d\n",y,left_white_num);
    }

    for (y= MT9V03X_H-1;y>=0;y--)
    {
        if (bin_image[y][right_map[y]] == 0)
        {
            break;
        }
        else
        {
            right_white_num++;
        }
    }
    if (left_white_num < right_white_num)
        speed_map = left_white_num;
    else 
        speed_map = right_white_num;

    left_white_num=0;
    right_white_num=0;



    straight_dis = -0.002036*(MT9V03X_H- speed_map)*(MT9V03X_H- speed_map)*(MT9V03X_H- speed_map)+0.2785*(MT9V03X_H- speed_map)*(MT9V03X_H- speed_map)+-12.74*(MT9V03X_H- speed_map)+232.2;
          /*  Linear model Poly3:
     f(x) = p1*x^3 + p2*x^2 + p3*x + p4
Coefficients (with 95% confidence bounds):
       p1 =   -0.002036  (-0.002525, -0.001547)
       p2 =      0.2785  (0.2291, 0.3279)
       p3 =      -12.74  (-14.09, -11.4)
       p4 =       232.2  (222.5, 241.8)

        */
        if(straight_dis>250)straight_dis=250;
        else if(straight_dis<1)straight_dis=1;



        //长直道速度
        if (straight_flag && Island_State==0&&ramp_flag==0&& zebra_line_flag==0)
        {
            target_speed += straight_jia;
        }
        //环岛速度
        else if (straight_flag==0&& Island_State&& ramp_flag == 0 && zebra_line_flag == 0)
        {
            target_speed += island_jia;
        }
        //坡道速度
        else if (straight_flag == 0 && Island_State==0 && ramp_flag && zebra_line_flag == 0)
        {
            target_speed += ramp_jia;
        }
        //出界，斑马线速度
        else if ((straight_flag == 0 && Island_State == 0 && ramp_flag==0 && zebra_line_flag)|| chujie_flag == 1)
        {
            target_speed = 0;
        }
        //其他情况，速度映射
        else
        {
            /*         f(x) = a*x*x+b
Coefficients (with 95% confidence bounds):
       a =    0.007708  (0.006841, 0.008575)
       b =       322.5  (310.4, 334.6)
         * */
          //  5:8
            target_speed= 0.007708 *straight_dis*straight_dis+ 322.5;
        }

    if(chujie_flag==1||zebra_line_flag>=2)
        target_speed=0;
}
//差速
void speed_contral(void)
{



   //舵机误差范围，正负430
    if (straight_dis < 140 && hightest>40)//不是长直道
        duty = (angle - servos_center)*speed_ratio/100;
    else
        duty = (angle - servos_center) * (speed_ratio - 105)/100;
        
    //SU400——duty分两种情况，即两种左转右转
    //计算车身实际速度
   // center_speed = (left_encoder + right_encoder) / 2;
   // duty = angle - servos_center;
    if (duty > 430)
        duty = 430;
    else if (duty <= -430)
        duty = -430;




    //差速的各种方式，待更改

    //普通弯道
   if (duty > 0) {
        //左转
        if (abs(duty_ratio * duty) < 35)//右加速限幅，防止侧翻
            right_speed = target_speed + duty_ratio * duty;
        else
            right_speed = target_speed + 35;
        left_speed = target_speed - duty;
    }
    else {
        //右转
        if (abs(duty_ratio * duty) < 35)//左加速限幅，防止侧翻
            left_speed = target_speed - duty_ratio * duty;//左加速
        else
            left_speed = target_speed + 35;//左加速
        right_speed = target_speed + duty;//右减速
    }

   //环岛差速
    if (Left_Island_Flag || Left_Island_Flag)
    {
        if (Island_State == 4)
        {
            duty_ratio = 0.32;//外轮是否加速
            if (duty > 0) {
                //左转
                if (abs(duty_ratio * duty) < 80)//右加速限幅，防止侧翻
                    right_speed = target_speed + duty_ratio * duty;
                else
                    right_speed = target_speed + 80;
                left_speed = target_speed - duty;
            }
            else {
                //右转
                if (abs(duty_ratio * duty) < 80)//左加速限幅，防止侧翻
                    left_speed = target_speed - duty_ratio * duty;//左加速
                else
                    left_speed = target_speed + 80;//左加速
                right_speed = target_speed +  duty;//右减速
            }
        }
        else if(Island_State==3|| Island_State==5|| Island_State==6|| Island_State==7|| Island_State==8)
        {//其他环岛状态，快速出入环
            duty = (angle - servos_center) * speed_ratio / 100;
            if (duty > 430)
                duty = 430;
            else if (duty <= -430)
                duty = -430;
            duty_ratio = 0;//外轮是否加速
            if (duty > 0) {
                //左转
                if (abs(duty_ratio * duty) < 60)//右加速限幅，防止侧翻
                    right_speed = target_speed + duty_ratio * duty;
                else
                    right_speed = target_speed + 60;
                left_speed = target_speed -  duty;
            }
            else {
                //右转
                if (abs(duty_ratio * duty) < 60)//左加速限幅，防止侧翻
                    left_speed = target_speed - duty_ratio * duty;//左加速
                else
                    left_speed = target_speed + 60;//左不变
                right_speed = target_speed + duty;//右减速
            }
        }
        else
        {
            duty = (angle - servos_center) * (speed_ratio - 105) / 100;
            if (duty > 430)
                duty = 430;
            else if (duty <= -430)
                duty = -430;
            duty_ratio = 0;//外轮是否加速
            if (duty > 0) {
                //左转
                if (abs(duty_ratio * duty) < 60)//右加速限幅，防止侧翻
                    right_speed = target_speed + duty_ratio * duty;
                else
                    right_speed = target_speed + 60;
                left_speed = target_speed - duty;
            }
            else {
                //右转
                if (abs(duty_ratio * duty) < 60)//左加速限幅，防止侧翻
                    left_speed = target_speed - duty_ratio * duty;//左加速
                else
                    left_speed = target_speed + 60;//左不变
                right_speed = target_speed + duty;//右减速
            }
        }

    }


  //  left_speed=target_speed;
    //right_speed=target_speed;

//printf("%d,%d,%d,%d,%d,%d\n",left_encoder,left_speed,Target_Speed_l,right_encoder,right_speed,Target_Speed_r);
//printf("%d,%d,%d\n\r",right_encoder,right_speed,Target_Speed_r);
    Target_Speed_l=pid_l_motor(left_encoder,left_speed);
    Target_Speed_r= pid_r_motor(right_encoder,right_speed);
   // Target_Speed_r=0;
    motor_driver(Target_Speed_l,Target_Speed_r);
}




void motor_driver(int speed_l,int speed_r)//电机驱动函数
{
      if(speed_l>=0)//左
      {
          gpio_set_level(P02_6, 0);
          pwm_set_duty(ATOM0_CH7_P02_7,speed_l);


      }
      else if(speed_l<0)
        {
          gpio_set_level(P02_6, 1);
          pwm_set_duty(ATOM0_CH7_P02_7,abs(speed_l));
        }

      if(speed_r>=0)
          {

          gpio_set_level(P02_4, 0);
          pwm_set_duty(ATOM0_CH5_P02_5,speed_r);
          }
          else if(speed_r<0)
            {
              gpio_set_level(P02_4, 1);
              pwm_set_duty(ATOM0_CH5_P02_5,abs(speed_r));
            }

    }


