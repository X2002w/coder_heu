/*
 * ips200.c
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */
#include "zf_common_headfile.h"

#define x_limited(x)    (x<MT9V03X_W?(x>0?x:0):MT9V03X_W-1)
#define y_limited(y)    (y<MT9V03X_H?(y>0?y:0):MT9V03X_H-1)


void ips200(void)
{
    int i;


    ips200_show_gray_image(0, 0, bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
    ips200_draw_line(93, 0, 93, 69, RGB565_PURPLE);
    for(i=0;i<MT9V03X_H;i++)
    {
        ips200_draw_point(x_limited(center_line_repair[i]),i, RGB565_GREEN);//显示起点 显示中线
               ips200_draw_point(x_limited(center_line_repair[i]+1),i, RGB565_GREEN);
               ips200_draw_point(x_limited(center_line_repair[i]+2),i, RGB565_GREEN);



               ips200_draw_point(x_limited(r_border_repair[i]), i,RGB565_RED);//显示起点 显示右边
               ips200_draw_point(x_limited(r_border_repair[i]+1), i,RGB565_RED);
               ips200_draw_point(x_limited(r_border_repair[i]+2), i,RGB565_RED);


                ips200_draw_point(x_limited(l_border_repair[i]), i, RGB565_BLUE);//显示起点 显示左边线
                ips200_draw_point(x_limited(l_border_repair[i]+1), i, RGB565_BLUE);
                ips200_draw_point(x_limited(l_border_repair[i]+2), i, RGB565_BLUE);
    }

}

