/*
 * control.c
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */
#include "zf_common_headfile.h"
//������������Ϣ



//С���ٶ������Ϣ
int set_mode=0; //С��ģʽ���ã����費ͬ�ٶ�
int target_speed=0;//С������Ŀ���ٶ�
int center_speed;//С���������ұ����Ȩʵ���ٶ�
int left_encoder,right_encoder;//���ұ���������
int left_speed,right_speed;//�����ֲ���Ŀ���ٶ�
int Target_Speed_l,Target_Speed_r;//������ʵ���ٶ�



/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����Vofa������
  @param     data1��data2��data3��data4��data5��data6��Ҫ�������ݷŽ���μ���
  @return    null
  Sample     Vofa_data(Steer_Angle,Err,Speed_Left_Real,Speed_Left_Set,Speed_Right_Real,Speed_Right_Set,dl1a_distance_mm);
  @note      ��������ã������������μ��ɣ������Ӳ����ο�ԭ�����ע��
-------------------------------------------------------------------------------------------------------------------*/
void Vofa_data(int data1,int data2,int data3,int data4,int data5,int data6,int data7)
{
    int data[7];
    float tempFloat[7];   //�������ʱ����
    uint8 tempData[32];   //����Ĵ���Buffer��һ��int��float����ռ4���ֽڣ������7����4*7=28�����滹���ĸ�У��λ��28+4=32

    data[0] = data1;
    data[1] = data2;
    data[2] = data3;
    data[3] = data4;
    data[4] = data5;
    data[5] = data6;
    data[6] = data7;

    tempFloat[0] = (float)data[0];     //ת�ɸ�����
    tempFloat[1] = (float)data[1];
    tempFloat[2] = (float)data[2];
    tempFloat[3] = (float)data[3];
    tempFloat[4] = (float)data[4];
    tempFloat[5] = (float)data[5];
    tempFloat[6] = (float)data[6];

    memcpy(tempData, (uint8 *)tempFloat, sizeof(tempFloat));  //ͨ��������������������

    tempData[28] = 0x00;//д���β����
    tempData[39] = 0x00;
    tempData[30] = 0x80;
    tempData[31] = 0x7f;

    uart_write_buffer(UART_3, tempData, 32);//ͨ�����ڴ������ݣ�ǰ�濪�������飬����ռ��󳤶�
}


void encoder_get(void){
    left_encoder=encoder_get_count(TIM6_ENCODER)*-1;
    right_encoder=encoder_get_count(TIM2_ENCODER);
  //  printf("%d,%d\n\r",left_encoder,right_encoder);

    encoder_clear_count(TIM6_ENCODER);
    encoder_clear_count(TIM2_ENCODER);
}

//�ٶȾ���
void set_speed(void)
{
    target_speed=340;
    if(chujie_flag)
        target_speed=0;
}
//����
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




void motor_driver(int speed_l,int speed_r)//�����������
{
      if(speed_l>=0)//��
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


