/*
 * keyboard.c
 *
 *  Created on: 2022年5月20日
 *      Author: admin
 */


#include "zf_common_headfile.h"

#define FLASH_SECTION_INDEX       (0)                                 // 存储数据用的扇区
#define FLASH_PAGE_INDEX          (11)                                // 存储数据用的页码 倒数第一个页码

uint8 dispagenum=0,dispagenum_last=0;  //显示内容
uint8 paraadjnum=0;      //参数调节项目


//拨码开关状态变量
uint8 sw1_status;
uint8 sw2_status;

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;


//上一次开关状态变量
uint8 key1_last_status = 1;
uint8 key2_last_status = 1;
uint8 key3_last_status = 1;
uint8 key4_last_status = 1;


//开关标志位
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
void dispage1(void) //第2面参数显示
{
    //设置小车模式（速度），发车
    ips200_show_string(1,130, "l_kp:");
    ips200_show_string(1,150, "l_ki:");
    ips200_show_string(1,170, "r_kp:");
    //舵机参数调节
    ips200_show_string(1,190, "r_ki:");//基础kp
    ips200_show_string(1,210, "string:");//二次项参数
    ips200_show_string(1,230, "string:");//指数项参数
    ips200_show_string(1,250, "string:");//动态kd参数
    ips200_show_string(1,270, "string:");//陀螺仪加速度参数

}
void dispage2(void) //第3面参数显示
{

    //电机,差速调节
        ips200_show_string(1,130, "string:");
        ips200_show_string(1,150, "string:");
        ips200_show_string(1,170, "string:");//差速弯道系数

        ips200_show_string(1,190, "string:");//差速误差系数
        ips200_show_string(1,210, "string:");//差速环岛阈值

        ips200_show_string(1,230, "string:");//差速弯道阈值
        ips200_show_string(1,250, "string:");//差速环岛阈值
        ips200_show_string(1,270, "string:");//差速直道系数
}
void dispage3(void) //第4面参数显示
{
    //赛道信息调节
        ips200_show_string(1,130, "string:");
        ips200_show_string(1,150, "string:");
    //速度策略调节
        ips200_show_string(1,170, "string:");
        ips200_show_string(1,190, "string:");
        ips200_show_string(1,210, "string:");
        ips200_show_string(1,230, "string:");
        ips200_show_string(1,250, "string:");
        ips200_show_string(1,270, "string:");
}
void number1(void)
{
    //设置小车模式（速度），发车
    ips200_show_float(160, 130, l_motor_kp, 3, 1);
    ips200_show_float(160, 150, l_motor_ki, 3, 1);
    ips200_show_float(160, 170, r_motor_kp, 3, 1);
    //舵机参数调节
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





//按键初始化
void keyboard_init(void)
{
    //拨码开关初始化
    gpio_init(SW1, GPI, 1, GPI_PULL_UP);
    gpio_init(SW2, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY1, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY2, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY3, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY4, GPI, 1, GPI_PULL_UP);


}


  void  key_operate(void)//获取拨码开关状态
  {

        sw1_status = gpio_get_level(KEY1);
       /* sw2_status = gpio_get(SW2);
**/
        //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
        //保存按键状态
        key1_last_status = key1_status;
        key2_last_status = key2_status;
        key3_last_status = key3_status;
        key4_last_status = key4_status;

        //读取当前按键状态
        key1_status = gpio_get_level(KEY1);
        key2_status = gpio_get_level(KEY2);
        key3_status = gpio_get_level(KEY3);
        key4_status = gpio_get_level(KEY4);


        //检测到按键按下之后  并放开置位标志位
        if(key1_status && !key1_last_status)    key1_flag = 1;
        if(key2_status && !key2_last_status)    key2_flag = 1;
        if(key3_status && !key3_last_status)    key3_flag = 1;
        if(key4_status && !key4_last_status)    key4_flag = 1;


        //标志位置位之后，可以使用标志位执行自己想要做的事件
        if(key1_flag)
        {
         key1_flag = 0;//使用按键之后，应该清除标志位
         key1_function();
       //  pit_disable(CCU60_CH0);
       //  if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // 判断是否有数据
        //     flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // 擦除这一页

    //     flash_deal();
     //    system_delay_ms(30);
      //   pit_enable(CCU60_CH0);



        }

        else if(key2_flag)
        {
            key2_flag = 0;//使用按键之后，应该清除标志位

            prompt();
         key2_function();



        }

        else if(key3_flag)
        {
            key3_flag = 0;//使用按键之后，应该清除标志位

         key3_function();
        }

        else if(key4_flag)
        {

           key4_flag = 0;//使用按键之后，应该清除标志位

         key4_function();
        }
        else if(sw1_status){
          //  Ok=1;
        }


    }



void interface_display(void)
{

            key_operate();

       if(dispagenum==0) //显示第一页，图像显示及相关参数调节
       {
          ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);

          ips200_show_string(1,130, "Island:");  //设置速度档
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
          else if(dispagenum==1) //显示小车控制参数及调节
       {


              ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);

              //环岛系列参数
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
        else if(dispagenum==2) //显示小车控制参数及调节
       {
            ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);
         number1();
         dispage1();
          prompt();
        }
        else if(dispagenum==3) //显示小车控制参数及调节
              {
            ips200_show_gray_image(0,0,bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);
                number2();
                dispage2();
                 prompt();
               }
        else if(dispagenum==4) //显示小车控制参数及调节
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
      if(dispagenum==0)  //参数显示
      {
            if(paraadjnum==7) //调节直道时舵机KP +
       {

       }
      }

    else  if(dispagenum==2){  //参数显示与调节
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
    else  if(dispagenum==3){ //参数显示
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
   else  if(dispagenum==4){ //参数显示
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

    if(dispagenum==0){//参数显示
        if(paraadjnum==7){

         }
    }
    else  if(dispagenum==2){
          if(paraadjnum==0){//速度档位调节-
              l_motor_kp-=1;
          }
          else  if(paraadjnum==1){//长直道速度-
              l_motor_ki-=0.2;
          }
          else if(paraadjnum==2) {//短直道速度-
              r_motor_kp-=1;
          }
          else if(paraadjnum==3) {//坡道道速度-
              r_motor_ki-=0.2;
          }
          else if(paraadjnum==4) {//环岛道速度-

          }
          else if(paraadjnum==5) {//连续S弯道速度-


          }
          else if(paraadjnum==6) {


          }
          else if(paraadjnum==7) {

          }

      }
      else  if(dispagenum==3){ //参数显示
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
     else  if(dispagenum==4){ //参数显示
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
            else if(dispagenum==2) //参数显示
            {   paraadjnum++;
                if(paraadjnum>7) paraadjnum=0;
                prompt();
            }
            else if(dispagenum==3) //参数显示
            {   paraadjnum++;
                if(paraadjnum>7) paraadjnum=0;
                prompt();
            }
            else if(dispagenum==4) //参数显示
            {   paraadjnum++;
                if(paraadjnum>7) paraadjnum=0;
                prompt();
            }

}
     void flash_deal(void)
     {
       //  flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // 将数据从 flash 读取到缓冲区

         flash_buffer_clear();  // 清空缓冲区
         //一次性可缓冲1024个数据
         //set_mode,Ok,target_speed,SU400[0],SU400[3],SU400[4],kd1,k_acc,speed_ratio,duty_ratio,

         flash_union_buffer[0].int16_type  = 0;        // 向缓冲区第 0 个位置写入 float  数据
         flash_union_buffer[1].int16_type = 0;        // 向缓冲区第 1 个位置写入 uint32 数据
         flash_union_buffer[2].int16_type  = 0;     // 向缓冲区第 2 个位置写入 int32  数据
         flash_union_buffer[3].float_type = 0;       // 向缓冲区第 3 个位置写入 uint16 数据
         flash_union_buffer[4].float_type  = 0;        // 向缓冲区第 4 个位置写入 int16  数据
         flash_union_buffer[5].float_type  = 0;      // 向缓冲区第 5 个位置写入 uint8  数据
         flash_union_buffer[6].float_type   = 0;  // 向缓冲区第 6 个位置写入 int8   数据
         flash_union_buffer[6].float_type   = 0;
         flash_union_buffer[6].int16_type   = 0;
         flash_union_buffer[6].float_type   = 0;


         flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // 向指定 Flash 扇区的页码写入缓冲区数据

         flash_buffer_clear();// 清空缓冲区

     }
     void flash_read(void)
     {
         flash_buffer_clear();// 清空缓冲区
         flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // 将数据从 flash 读取到缓冲区
         //读取1024个数据
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
        system_delay_ms(100);                                                   // 初始化失败 等待 100ms
    }

    printf("\r\n module version:%s",wifi_spi_version);                          // 模块固件版本
    printf("\r\n module mac    :%s",wifi_spi_mac_addr);                         // 模块 MAC 信息
    printf("\r\n module ip     :%s",wifi_spi_ip_addr_port);                     // 模块 IP 地址

    // zf_device_wifi_spi.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接
    if(1 != WIFI_SPI_AUTO_CONNECT)                                              // 如果没有开启自动连接 就需要手动连接目标 IP
    {
        while(wifi_spi_socket_connect(                                          // 向指定目标 IP 的端口建立 TCP 连接
            "TCP",                                                              // 指定使用TCP方式通讯
            WIFI_SPI_TARGET_IP,                                                 // 指定远端的IP地址，填写上位机的IP地址
            WIFI_SPI_TARGET_PORT,                                               // 指定远端的端口号，填写上位机的端口号，通常上位机默认是8080
            WIFI_SPI_LOCAL_PORT))                                               // 指定本机的端口号
        {
            // 如果一直建立失败 考虑一下是不是没有接硬件复位
            printf("\r\n Connect TCP Servers error, try again.");
            system_delay_ms(100);                                               // 建立连接失败 等待 100ms
        }
    }



    ips200_init(IPS200_TYPE_PARALLEL8);

    // 逐飞助手初始化 数据传输使用高速WIFI SPI
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);

    // 如果要发送图像信息，则务必调用seekfree_assistant_camera_information_config函数进行必要的参数设置
    // 如果需要发送边线则还需调用seekfree_assistant_camera_boundary_config函数设置边线的信息


    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, MT9V03X_H, center_line, r_border_fill, l_border_fill, NULL, NULL, NULL);     // 图像发送时包含三条边线，边线只有横坐标

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
