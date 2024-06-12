/*
 * control.c
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */
#include "zf_common_headfile.h"
//电机驱动相关信息



//小车速度相关信息
int set_mode=0; //小车模式设置，给予不同速度
int target_speed=0;//小车车身目标速度
int center_speed;//小车车身左右编码加权实际速度
int left_encoder,right_encoder;//左右编码器读数
int left_speed,right_speed;//左右轮差速目标速度
int Target_Speed_l,Target_Speed_r;//左右轮实际速度
int speed_ratio=430;//差速系数
float duty_ratio=500;//电机增量误差系数
int duty;//电机差速增量



int left_white_num=0;
int right_white_num=0;
int speed_map;//最终用于映射的白点
float straight_dis;//现实实际距离映射
/*-------------------------------------------------------------------------------------------------------------------
  @brief     利用Vofa发数据
  @param     data1，data2，data3，data4，data5，data6，要发的数据放进入参即可
  @return    null
  Sample     Vofa_data(Steer_Angle,Err,Speed_Left_Real,Speed_Left_Set,Speed_Right_Real,Speed_Right_Set,dl1a_distance_mm);
  @note      如果不够用，继续往后加入参即可，继续加参数参考原则见下注释
-------------------------------------------------------------------------------------------------------------------*/
void Vofa_data(int data1,int data2,int data3,int data4,int data5,int data6,int data7)
{
    int data[7];
    float tempFloat[7];   //定义的临时变量
    uint8 tempData[32];   //定义的传输Buffer，一个int，float数字占4个字节，如果传7个，4*7=28，后面还有四个校验位，28+4=32

    data[0] = data1;
    data[1] = data2;
    data[2] = data3;
    data[3] = data4;
    data[4] = data5;
    data[5] = data6;
    data[6] = data7;

    tempFloat[0] = (float)data[0];     //转成浮点数
    tempFloat[1] = (float)data[1];
    tempFloat[2] = (float)data[2];
    tempFloat[3] = (float)data[3];
    tempFloat[4] = (float)data[4];
    tempFloat[5] = (float)data[5];
    tempFloat[6] = (float)data[6];

    memcpy(tempData, (uint8 *)tempFloat, sizeof(tempFloat));  //通过拷贝把数据重新整理

    tempData[28] = 0x00;//写入结尾数据
    tempData[39] = 0x00;
    tempData[30] = 0x80;
    tempData[31] = 0x7f;

    uart_write_buffer(UART_3, tempData, 32);//通过串口传输数据，前面开多大的数组，后面占多大长度
}


void encoder_get(void){
    left_encoder=encoder_get_count(TIM6_ENCODER)*-1;
    right_encoder=encoder_get_count(TIM2_ENCODER);
  //  printf("%d,%d\n\r",left_encoder,right_encoder);

    encoder_clear_count(TIM6_ENCODER);
    encoder_clear_count(TIM2_ENCODER);
}

//速度决策
void set_speed(void)
{
    int y, x;
    target_speed=360;
    //直道:560;

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

        /*     f(x) = p1*x^2 + p2*x + p3
Coefficients (with 95% confidence bounds):
       p1 =    0.003075  (-0.005807, 0.01196)
       p2 =      0.7278  (-0.8917, 2.347)
       p3 =       313.6  (253.6, 373.5)
         * */
        //target_speed=0.003075*straight_dis*straight_dis+0.7278*straight_dis+313.6;

    if(chujie_flag==1||zebra_line_flag>=2)
        target_speed=0;
}
//差速
void speed_contral(void)
{



   /* if (MT9V03X_H-hightest > 20)
        duty = (angle - servos_center) * speed_ratio / 100;
    else
        duty = (angle - servos_center) * (speed_ratio - 390) / 100;

    //SU400——duty分两种情况，即两种左转右转
    //计算车身实际速度*/
    center_speed = (left_encoder + right_encoder) / 2;
    duty = angle - servos_center;
    if (duty > 440)
        duty = 440;
    else if (duty <= -440)
        duty = -440;








    if(Island_State==4){

        duty_ratio=1000;
    }
    else {
        duty_ratio=700;
    }

    if (duty > 0) {
        left_speed = (int)(target_speed*(1 - ((float)duty_ratio / 1200)*tan(5.137*(float)duty/4.0f*3.14 / 1673) / 0.885));
        right_speed = target_speed;
    /*    //左转
        if (abs(duty_ratio * duty) < 35)
            right_speed = target_speed + duty_ratio * duty;
        else
            right_speed = target_speed + 35;
        left_speed = target_speed - duty;*/
    }
    else {
        //右转
        right_speed = (int)(target_speed*(1 + ((float)duty_ratio / 1200)*tan(5.137*(float)duty/4.0f*3.14 / 1673) / 0.885));
        left_speed = target_speed;
      /*  if (abs(duty_ratio * duty) < 35)
            left_speed = target_speed - duty_ratio * duty;//左加速
        else
            left_speed = target_speed + 35;//左加速
        right_speed = target_speed + duty;//右减速*/
    }








   // left_speed=target_speed;
   // right_speed=target_speed;

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


