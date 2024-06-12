/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu1_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"

#define WIFI_SSID_TEST          "2163_WIFI_STA"
#define WIFI_PASSWORD_TEST      "21632163"

uint8 image_copy[MT9V03X_H][MT9V03X_W];
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中

// **************************** 代码区域 ****************************

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断.
    wifi_spi();
    mt9v03x_init();
    imu660ra_init();
    Zero_Point_Detect();
  //  while (dl1a_init());//测距

    //ips200_init(IPS200_TYPE_PARALLEL8);
    pwm_init(ATOM0_CH1_P33_9,300,4700 );
    pwm_init(ATOM0_CH5_P02_5, 17000, 2500);//you
    pwm_init(ATOM0_CH7_P02_7, 17000, 2500);//zuo

    gpio_init(P02_4, GPO, 0, GPO_PUSH_PULL);//you
    gpio_init(P02_6, GPO, 0, GPO_PUSH_PULL);//zuo
   // gpio_init(P33_10, GPO, 1, GPO_PUSH_PULL);

    encoder_dir_init(TIM6_ENCODER, TIM6_ENCODER_CH1_P20_3, TIM6_ENCODER_CH2_P20_0);
    encoder_dir_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);//右
    pit_ms_init(CCU60_CH0, 10);
    pit_ms_init(CCU60_CH1, 10);

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        ips200();
        interface_display();
        //fenglingqi_use();
         memcpy(image_copy[0], bin_image[0], MT9V03X_IMAGE_SIZE);
             // 发送图像
         seekfree_assistant_camera_send();



        // 此处编写需要循环执行的代码
    }
}
#pragma section all restore
// **************************** 代码区域 ****************************
