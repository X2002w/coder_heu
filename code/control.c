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
    target_speed=340;
    if(chujie_flag)
        target_speed=0;
}
//差速
void speed_contral(void)
{
    left_speed=target_speed;
    right_speed=target_speed;

printf("%d,%d,%d,%d,%d,%d\n",left_encoder,left_speed,Target_Speed_l,right_encoder,right_speed,Target_Speed_r);
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


