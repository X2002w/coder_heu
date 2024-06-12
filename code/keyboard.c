/*
 * keyboard.c
 *
 *  Created on: 2022��5��20��
 *      Author: admin
 */


#include "zf_common_headfile.h"

#define FLASH_SECTION_INDEX       (0)                                 // �洢�����õ�����
#define FLASH_PAGE_INDEX          (11)                                // �洢�����õ�ҳ�� ������һ��ҳ��

uint8 dispagenum=0,dispagenum_last=0;  //��ʾ����
uint8 paraadjnum=0;      //����������Ŀ


//���뿪��״̬����
uint8 sw1_status;
uint8 sw2_status;

//����״̬����
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;


//��һ�ο���״̬����
uint8 key1_last_status = 1;
uint8 key2_last_status = 1;
uint8 key3_last_status = 1;
uint8 key4_last_status = 1;


//���ر�־λ
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;




void prompt(void)
{
  static uint8 i;
  for(i=0;i<=7;i++)
         { if(i==paraadjnum)
             ips200_show_string(140, 130+i*20, "->");
           else
             ips200_show_string(140, 130+i*20, " ");
         }
}
void dispage1(void) //��2�������ʾ
{
    //����С��ģʽ���ٶȣ�������
    ips200_show_string(1,130, "l_kp:");
    ips200_show_string(1,150, "l_ki:");
    ips200_show_string(1,170, "r_kp:");
    //�����������
    ips200_show_string(1,190, "r_ki:");//����kp
    ips200_show_string(1,210, "string:");//���������
    ips200_show_string(1,230, "string:");//ָ�������
    ips200_show_string(1,250, "string:");//��̬kd����
    ips200_show_string(1,270, "string:");//�����Ǽ��ٶȲ���

}
void dispage2(void) //��3�������ʾ
{

    //���,���ٵ���
        ips200_show_string(1,130, "string:");
        ips200_show_string(1,150, "string:");
        ips200_show_string(1,170, "string:");//�������ϵ��

        ips200_show_string(1,190, "string:");//�������ϵ��
        ips200_show_string(1,210, "string:");//���ٻ�����ֵ

        ips200_show_string(1,230, "string:");//���������ֵ
        ips200_show_string(1,250, "string:");//���ٻ�����ֵ
        ips200_show_string(1,270, "string:");//����ֱ��ϵ��
}
void dispage3(void) //��4�������ʾ
{
    //������Ϣ����
        ips200_show_string(1,130, "string:");
        ips200_show_string(1,150, "string:");
    //�ٶȲ��Ե���
        ips200_show_string(1,170, "string:");
        ips200_show_string(1,190, "string:");
        ips200_show_string(1,210, "string:");
        ips200_show_string(1,230, "string:");
        ips200_show_string(1,250, "string:");
        ips200_show_string(1,270, "string:");
}
void number1(void)
{
    //����С��ģʽ���ٶȣ�������
    ips200_show_float(160, 130, l_motor_kp, 3, 1);
    ips200_show_float(160, 150, l_motor_ki, 3, 1);
    ips200_show_float(160, 170, r_motor_kp, 3, 1);
    //�����������
    ips200_show_float(160, 190, r_motor_ki, 3, 2);
    ips200_show_float(160, 210, 0, 3, 2);
    ips200_show_float(160, 230, 0, 3, 2);
    ips200_show_float(160, 250, 0, 3, 2);
    ips200_show_float(160, 270, 0, 3, 2);






}

void number2(void)
{
    ips200_show_float(160, 130, 0, 3, 2);
       ips200_show_float(160, 150, 0, 3, 2);
       ips200_show_float(160, 170, 0, 3, 1);
       ips200_show_float(160, 190, 0, 2, 3);
       ips200_show_float(160, 210, 0, 3, 1);
       ips200_show_float(160, 230, 0, 2, 3);
       ips200_show_float(160, 250, 0, 2, 3);
       ips200_show_float(160, 270, 0, 2, 3);
}
void number3(void)
{
    ips200_show_float(160, 130, 0, 3, 2);
       ips200_show_float(160, 150, 0, 3, 2);
       ips200_show_float(160, 170, 0, 3, 1);
       ips200_show_float(160, 190, 0, 2, 3);
       ips200_show_float(160, 210, 0, 3, 1);
       ips200_show_float(160, 230, 0, 2, 3);
       ips200_show_float(160, 250, 0, 2, 3);
       ips200_show_float(160, 270, 0, 2, 3);
}





//������ʼ��
void keyboard_init(void)
{
    //���뿪�س�ʼ��
    gpio_init(SW1, GPI, 1, GPI_PULL_UP);
    gpio_init(SW2, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY1, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY2, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY3, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY4, GPI, 1, GPI_PULL_UP);


}


  void  key_operate(void)//��ȡ���뿪��״̬
  {

        sw1_status = gpio_get_level(KEY1);
       /* sw2_status = gpio_get(SW2);
**/
        //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
        //���水��״̬
        key1_last_status = key1_status;
        key2_last_status = key2_status;
        key3_last_status = key3_status;
        key4_last_status = key4_status;

        //��ȡ��ǰ����״̬
        key1_status = gpio_get_level(KEY1);
        key2_status = gpio_get_level(KEY2);
        key3_status = gpio_get_level(KEY3);
        key4_status = gpio_get_level(KEY4);


        //��⵽��������֮��  ���ſ���λ��־λ
        if(key1_status && !key1_last_status)    key1_flag = 1;
        if(key2_status && !key2_last_status)    key2_flag = 1;
        if(key3_status && !key3_last_status)    key3_flag = 1;
        if(key4_status && !key4_last_status)    key4_flag = 1;


        //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
        if(key1_flag)
        {
         key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
         key1_function();
       //  pit_disable(CCU60_CH0);
       //  if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
        //     flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // ������һҳ

    //     flash_deal();
     //    system_delay_ms(30);
      //   pit_enable(CCU60_CH0);



        }

        else if(key2_flag)
        {
            key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ

            prompt();
         key2_function();



        }

        else if(key3_flag)
        {
            key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ

         key3_function();
        }

        else if(key4_flag)
        {

           key4_flag = 0;//ʹ�ð���֮��Ӧ�������־λ

         key4_function();
        }
        else if(sw1_status){
          //  Ok=1;
        }


    }



void interface_display(void)
{

            key_operate();

       if(dispagenum==0) //��ʾ��һҳ��ͼ����ʾ����ز�������
       {
          ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);

          ips200_show_string(1,130, "Island:");  //�����ٶȵ�
          ips200_show_float(160, 130, Island_State, 3, 2);

          ips200_show_string(1,150, "l_huan:");
          ips200_show_float(160, 150, Left_Island_Flag, 3, 2);

          ips200_show_string(1,170, "r_huan:");
          ips200_show_float(160, 170, Right_Island_Flag, 5, 1);

          ips200_show_string(1,190, "FJ_Angle:");
          ips200_show_float(160, 190, FJ_Angle, 3, 3);

          ips200_show_string(1,210, "straight_dis:");
          ips200_show_float(160, 210, straight_dis, 4, 2);

          ips200_show_string(1,230, "zebra_line_flag:");
          ips200_show_float(160, 230, zebra_line_flag, 3, 1);

          ips200_show_string(1,250, "target_speed:");
          ips200_show_float(160, 250, target_speed, 3, 1);


          ips200_show_string(1,270, "chujie_flag:");
          ips200_show_float(160, 270, chujie_flag, 3, 1);

        }
          else if(dispagenum==1) //��ʾС�����Ʋ���������
       {


              ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);

              //����ϵ�в���
                       ips200_show_string(1,130, "string:");
                       ips200_show_float(160, 130, 0, 3, 2);

                       ips200_show_string(1,150, "string:");
                       ips200_show_float(160, 150, 0, 3, 1);

                       ips200_show_string(1,170, "string:");
                       ips200_show_float(160, 170, 0, 3, 1);

                       ips200_show_string(1,190, "string:");
                       ips200_show_float(160, 190, 0, 3, 1);

                       ips200_show_string(1,210, "string:");
                       ips200_show_float(160, 210, 0, 3, 1);

                       ips200_show_string(1,230, "string:");
                       ips200_show_float(50, 230, 0, 3, 1);
                       ips200_show_string(110,230, "string:");
                       ips200_show_float(170, 230, 0, 3, 1);

                       ips200_show_string(1,250, "string:");
                       ips200_show_float(160, 250, 0, 3, 2);

                       ips200_show_string(1,270, "string:");
                       ips200_show_float(50, 270, 0, 3, 1);
                       ips200_show_string(110,270, "string:");
                       ips200_show_float(170, 270, 0, 3, 1);

        }
        else if(dispagenum==2) //��ʾС�����Ʋ���������
       {
            ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);
         number1();
         dispage1();
          prompt();
        }
        else if(dispagenum==3) //��ʾС�����Ʋ���������
              {
            ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);
                number2();
                dispage2();
                 prompt();
               }
        else if(dispagenum==4) //��ʾС�����Ʋ���������
              {
            ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);
                number3();
                dispage3();
                 prompt();
               }
}



void  key1_function(void)
{
    dispagenum_last=dispagenum;
    dispagenum++;

      if(dispagenum>4)
      dispagenum=0;
      ips200_clear();
}

 void  key3_function(void)
 {
      if(dispagenum==0)  //������ʾ
      {
            if(paraadjnum==7) //����ֱ��ʱ���KP +
       {

       }
      }

    else  if(dispagenum==2){  //������ʾ�����
        if(paraadjnum==0){

            l_motor_kp+=1;

        }
        else  if(paraadjnum==1){

            l_motor_ki+=0.2;
        }
        else if(paraadjnum==2) {
            r_motor_kp+=1;
        }
        else if(paraadjnum==3) {

            r_motor_ki+=0.2;
        }
        else if(paraadjnum==4) {

        }
        else if(paraadjnum==5) {


        }
        else if(paraadjnum==6) {

        }
        else if(paraadjnum==7) {
        }

    }
    else  if(dispagenum==3){ //������ʾ
       if(paraadjnum==0){
       }
       else  if(paraadjnum==1){
       }
       else if(paraadjnum==2) {

       }
       else if(paraadjnum==3) {


       }
       else if(paraadjnum==4) {

       }
       else if(paraadjnum==5) {

       }
       else if(paraadjnum==6) {

       }
       else if(paraadjnum==7) {

       }
    }
   else  if(dispagenum==4){ //������ʾ
      if(paraadjnum==0){

      }
      else  if(paraadjnum==1){

      }
      else if(paraadjnum==2) {


      }
      else if(paraadjnum==3) {


      }
      else if(paraadjnum==4) {

      }
      else if(paraadjnum==5) {

      }
      else if(paraadjnum==6) {

      }
      else if(paraadjnum==7) {

      }

 }

  }
 void  key4_function(void)
 {

    if(dispagenum==0){//������ʾ
        if(paraadjnum==7){

         }
    }
    else  if(dispagenum==2){
          if(paraadjnum==0){//�ٶȵ�λ����-
              l_motor_kp-=1;
          }
          else  if(paraadjnum==1){//��ֱ���ٶ�-
              l_motor_ki-=0.2;
          }
          else if(paraadjnum==2) {//��ֱ���ٶ�-
              r_motor_kp-=1;
          }
          else if(paraadjnum==3) {//�µ����ٶ�-
              r_motor_ki-=0.2;
          }
          else if(paraadjnum==4) {//�������ٶ�-

          }
          else if(paraadjnum==5) {//����S����ٶ�-


          }
          else if(paraadjnum==6) {


          }
          else if(paraadjnum==7) {

          }

      }
      else  if(dispagenum==3){ //������ʾ
         if(paraadjnum==0){


         }
         else  if(paraadjnum==1){

         }
         else if(paraadjnum==2) {


         }
         else if(paraadjnum==3) {


         }
         else if(paraadjnum==4) {

         }
         else if(paraadjnum==5) {

         }
         else if(paraadjnum==6) {

         }
         else if(paraadjnum==7) {

         }
      }
     else  if(dispagenum==4){ //������ʾ
        if(paraadjnum==0){

        }
        else  if(paraadjnum==1){

        }
        else if(paraadjnum==2) {


        }
        else if(paraadjnum==3) {


        }
        else if(paraadjnum==4) {

        }
        else if(paraadjnum==5) {

        }
        else if(paraadjnum==6) {

        }
        else if(paraadjnum==7) {

        }

   }
  }

 void  key2_function(void)
 {
             if(dispagenum==0)
             {
                paraadjnum=7;
                prompt();
             }
            else if(dispagenum==2) //������ʾ
            {   paraadjnum++;
                if(paraadjnum>7) paraadjnum=0;
                prompt();
            }
            else if(dispagenum==3) //������ʾ
            {   paraadjnum++;
                if(paraadjnum>7) paraadjnum=0;
                prompt();
            }
            else if(dispagenum==4) //������ʾ
            {   paraadjnum++;
                if(paraadjnum>7) paraadjnum=0;
                prompt();
            }

}
     void flash_deal(void)
     {
       //  flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // �����ݴ� flash ��ȡ��������

         flash_buffer_clear();  // ��ջ�����
         //һ���Կɻ���1024������
         //set_mode,Ok,target_speed,SU400[0],SU400[3],SU400[4],kd1,k_acc,speed_ratio,duty_ratio,

         flash_union_buffer[0].int16_type  = 0;        // �򻺳����� 0 ��λ��д�� float  ����
         flash_union_buffer[1].int16_type = 0;        // �򻺳����� 1 ��λ��д�� uint32 ����
         flash_union_buffer[2].int16_type  = 0;     // �򻺳����� 2 ��λ��д�� int32  ����
         flash_union_buffer[3].float_type = 0;       // �򻺳����� 3 ��λ��д�� uint16 ����
         flash_union_buffer[4].float_type  = 0;        // �򻺳����� 4 ��λ��д�� int16  ����
         flash_union_buffer[5].float_type  = 0;      // �򻺳����� 5 ��λ��д�� uint8  ����
         flash_union_buffer[6].float_type   = 0;  // �򻺳����� 6 ��λ��д�� int8   ����
         flash_union_buffer[6].float_type   = 0;
         flash_union_buffer[6].int16_type   = 0;
         flash_union_buffer[6].float_type   = 0;


         flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // ��ָ�� Flash ������ҳ��д�뻺��������

         flash_buffer_clear();// ��ջ�����

     }
     void flash_read(void)
     {
         flash_buffer_clear();// ��ջ�����
         flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // �����ݴ� flash ��ȡ��������
         //��ȡ1024������
/*
         set_mode=flash_union_buffer[0].int16_type;
         Ok=flash_union_buffer[1].int16_type;
         target_speed=flash_union_buffer[2].int16_type;
         SU400[0]=flash_union_buffer[3].float_type;
         SU400[3]=flash_union_buffer[4].float_type;
         SU400[4]=flash_union_buffer[5].float_type;
         kd1=flash_union_buffer[6].float_type;
         k_acc=flash_union_buffer[7].float_type;
         speed_ratio=flash_union_buffer[8].int16_type;
         duty_ratio=flash_union_buffer[9].float_type;
*/

     }



void fengmingqi(void){
    uint16 count = 0;
    while(count<=20){
           if(count < 10)
               gpio_toggle_level(P33_10);
           else if(count < 20)
               gpio_set_level(P33_10, GPIO_LOW);
           count ++;
           system_delay_ms(1);
    }
    count = 0;
}
void fenglingqi_use(void)
{
    if(Left_Island_Flag||Right_Island_Flag){
        fengmingqi();
      //  Island_State=0;
    }
}
void wifi_spi(void){

    while(wifi_spi_init("2163_WIFI_STA", "21632163"))
    {
        printf("\r\n connect wifi failed. \r\n");
        system_delay_ms(100);                                                   // ��ʼ��ʧ�� �ȴ� 100ms
    }

    printf("\r\n module version:%s",wifi_spi_version);                          // ģ��̼��汾
    printf("\r\n module mac    :%s",wifi_spi_mac_addr);                         // ģ�� MAC ��Ϣ
    printf("\r\n module ip     :%s",wifi_spi_ip_addr_port);                     // ģ�� IP ��ַ

    // zf_device_wifi_spi.h �ļ��ڵĺ궨����Ը���ģ������(����) WIFI ֮���Ƿ��Զ����� TCP ������������ UDP ����
    if(1 != WIFI_SPI_AUTO_CONNECT)                                              // ���û�п����Զ����� ����Ҫ�ֶ�����Ŀ�� IP
    {
        while(wifi_spi_socket_connect(                                          // ��ָ��Ŀ�� IP �Ķ˿ڽ��� TCP ����
            "TCP",                                                              // ָ��ʹ��TCP��ʽͨѶ
            WIFI_SPI_TARGET_IP,                                                 // ָ��Զ�˵�IP��ַ����д��λ����IP��ַ
            WIFI_SPI_TARGET_PORT,                                               // ָ��Զ�˵Ķ˿ںţ���д��λ���Ķ˿ںţ�ͨ����λ��Ĭ����8080
            WIFI_SPI_LOCAL_PORT))                                               // ָ�������Ķ˿ں�
        {
            // ���һֱ����ʧ�� ����һ���ǲ���û�н�Ӳ����λ
            printf("\r\n Connect TCP Servers error, try again.");
            system_delay_ms(100);                                               // ��������ʧ�� �ȴ� 100ms
        }
    }



    ips200_init(IPS200_TYPE_PARALLEL8);

    // ������ֳ�ʼ�� ���ݴ���ʹ�ø���WIFI SPI
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);

    // ���Ҫ����ͼ����Ϣ������ص���seekfree_assistant_camera_information_config�������б�Ҫ�Ĳ�������
    // �����Ҫ���ͱ����������seekfree_assistant_camera_boundary_config�������ñ��ߵ���Ϣ


    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, MT9V03X_H, center_line, r_border_fill, l_border_fill, NULL, NULL, NULL);     // ͼ����ʱ�����������ߣ�����ֻ�к�����

}


float tempFloat[12] = {0} ;
uint8 tempData[52] = {0} ;

void sending(void)
{
    tempFloat[0]=err;
    tempFloat[1]=left_encoder;
    tempFloat[2]=right_encoder;
  memcpy(tempData, (uint8 *)tempFloat, sizeof(tempFloat));

    tempData[48] = 0x00;
    tempData[49] = 0x00;
    tempData[50] = 0x80;
    tempData[51] = 0x7f;
    wireless_uart_send_buffer(&tempData[0],52);
}
