/*
 * control.c
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */
#include "zf_common_headfile.h"
//������������Ϣ



//С���ٶ������Ϣ
int set_mode=1; //С��ģʽ���ã����費ͬ�ٶ�
int target_speed=340;//С������Ŀ���ٶ�
int center_speed;//С���������ұ����Ȩʵ���ٶ�
int left_encoder,right_encoder;//���ұ���������
int left_speed,right_speed;//�����ֲ���Ŀ���ٶ�
int Target_Speed_l,Target_Speed_r;//������ʵ���ٶ�
int speed_ratio=110;//����ϵ��
float duty_ratio=0.12;//����������ϵ��
int duty;//�����������


//�ٶȲ������,���٣�������
int straight_jia;
int island_jia;
int ramp_jia;



int left_white_num=0;
int right_white_num=0;
int speed_map;//��������ӳ��İ׵�
float straight_dis;//��ʵʵ�ʾ���ӳ��


void encoder_get(void){
    left_encoder=encoder_get_count(TIM6_ENCODER)*-1;
    right_encoder=encoder_get_count(TIM2_ENCODER);
    center_speed = (left_encoder + right_encoder) / 2;
  //  printf("%d,%d\n\r",left_encoder,right_encoder);

    encoder_clear_count(TIM6_ENCODER);
    encoder_clear_count(TIM2_ENCODER);
}

//�ٶȾ���
void set_speed(void)
{
    int y, x;
  
    //��������ٶ�Ϊ�����ٶ�

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



        //��ֱ���ٶ�
        if (straight_flag && Island_State==0&&ramp_flag==0&& zebra_line_flag==0)
        {
            target_speed += straight_jia;
        }
        //�����ٶ�
        else if (straight_flag==0&& Island_State&& ramp_flag == 0 && zebra_line_flag == 0)
        {
            target_speed += island_jia;
        }
        //�µ��ٶ�
        else if (straight_flag == 0 && Island_State==0 && ramp_flag && zebra_line_flag == 0)
        {
            target_speed += ramp_jia;
        }
        //���磬�������ٶ�
        else if ((straight_flag == 0 && Island_State == 0 && ramp_flag==0 && zebra_line_flag)|| chujie_flag == 1)
        {
            target_speed = 0;
        }
        //����������ٶ�ӳ��
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
//����
void speed_contral(void)
{



   //�����Χ������430
    if (straight_dis < 140 && hightest>40)//���ǳ�ֱ��
        duty = (angle - servos_center)*speed_ratio/100;
    else
        duty = (angle - servos_center) * (speed_ratio - 105)/100;
        
    //SU400����duty�������������������ת��ת
    //���㳵��ʵ���ٶ�
   // center_speed = (left_encoder + right_encoder) / 2;
   // duty = angle - servos_center;
    if (duty > 430)
        duty = 430;
    else if (duty <= -430)
        duty = -430;




    //���ٵĸ��ַ�ʽ��������

    //��ͨ���
   if (duty > 0) {
        //��ת
        if (abs(duty_ratio * duty) < 35)//�Ҽ����޷�����ֹ�෭
            right_speed = target_speed + duty_ratio * duty;
        else
            right_speed = target_speed + 35;
        left_speed = target_speed - duty;
    }
    else {
        //��ת
        if (abs(duty_ratio * duty) < 35)//������޷�����ֹ�෭
            left_speed = target_speed - duty_ratio * duty;//�����
        else
            left_speed = target_speed + 35;//�����
        right_speed = target_speed + duty;//�Ҽ���
    }

   //��������
    if (Left_Island_Flag || Left_Island_Flag)
    {
        if (Island_State == 4)
        {
            duty_ratio = 0.32;//�����Ƿ����
            if (duty > 0) {
                //��ת
                if (abs(duty_ratio * duty) < 80)//�Ҽ����޷�����ֹ�෭
                    right_speed = target_speed + duty_ratio * duty;
                else
                    right_speed = target_speed + 80;
                left_speed = target_speed - duty;
            }
            else {
                //��ת
                if (abs(duty_ratio * duty) < 80)//������޷�����ֹ�෭
                    left_speed = target_speed - duty_ratio * duty;//�����
                else
                    left_speed = target_speed + 80;//�����
                right_speed = target_speed +  duty;//�Ҽ���
            }
        }
        else if(Island_State==3|| Island_State==5|| Island_State==6|| Island_State==7|| Island_State==8)
        {//��������״̬�����ٳ��뻷
            duty = (angle - servos_center) * speed_ratio / 100;
            if (duty > 430)
                duty = 430;
            else if (duty <= -430)
                duty = -430;
            duty_ratio = 0;//�����Ƿ����
            if (duty > 0) {
                //��ת
                if (abs(duty_ratio * duty) < 60)//�Ҽ����޷�����ֹ�෭
                    right_speed = target_speed + duty_ratio * duty;
                else
                    right_speed = target_speed + 60;
                left_speed = target_speed -  duty;
            }
            else {
                //��ת
                if (abs(duty_ratio * duty) < 60)//������޷�����ֹ�෭
                    left_speed = target_speed - duty_ratio * duty;//�����
                else
                    left_speed = target_speed + 60;//�󲻱�
                right_speed = target_speed + duty;//�Ҽ���
            }
        }
        else
        {
            duty = (angle - servos_center) * (speed_ratio - 105) / 100;
            if (duty > 430)
                duty = 430;
            else if (duty <= -430)
                duty = -430;
            duty_ratio = 0;//�����Ƿ����
            if (duty > 0) {
                //��ת
                if (abs(duty_ratio * duty) < 60)//�Ҽ����޷�����ֹ�෭
                    right_speed = target_speed + duty_ratio * duty;
                else
                    right_speed = target_speed + 60;
                left_speed = target_speed - duty;
            }
            else {
                //��ת
                if (abs(duty_ratio * duty) < 60)//������޷�����ֹ�෭
                    left_speed = target_speed - duty_ratio * duty;//�����
                else
                    left_speed = target_speed + 60;//�󲻱�
                right_speed = target_speed + duty;//�Ҽ���
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


