/*
 * image.h
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */

#ifndef CODE_IMAGE_H_
#define CODE_IMAGE_H_


#define IMG_BLACK     0X00      //0x00是黑
#define IMG_WHITE     0Xff      //0xff为白




//二值化图像
extern uint8 bin_image[MT9V03X_H][MT9V03X_W];


//定义边线数组
extern int l_border[MT9V03X_H];
extern int r_border[MT9V03X_H];
extern uint8 l_border_fill[MT9V03X_H];//图传用
extern uint8 r_border_fill[MT9V03X_H];
extern int l_border_repair[MT9V03X_H];//最终边线
extern int r_border_repair[MT9V03X_H];
extern uint8 center_line[MT9V03X_H];//图传用
extern int center_line_repair[MT9V03X_H];

extern int hightest;//搜索截止行


//定义赛道丢线信息
extern int l_start;
extern int r_start;
extern int l_lost_num;
extern int r_lost_num;
extern int l_r_lostnum;


//环岛
extern int Island_State;//环岛标志位
extern int Left_Island_Flag;//左环岛标志
extern int Right_Island_Flag;//右环岛标志


extern float err;//摄像头误差
extern int angle;

//坡道
extern int ramp_flag;//坡道标志位

//十字
extern int cross_flag;//十字标志位

//直道
extern int straight_flag;//直道标志位

//斑马线
extern int zebra_line_flag;//斑马线标志位

//出界
extern int chujie_flag;

extern const uint8 right_map[MT9V03X_H];
extern const uint8 left_map[MT9V03X_H];

void Left_Add_Line(int x1, int y1, int x2, int y2);//左补线,补的是边界
void Right_Add_Line(int x1, int y1, int x2, int y2);//右补线,补的是边界
void Draw_Line(int startX, int startY, int endX, int endY);
void center_repair(void);
void straight_detect(void);
void buzzer(void);
void process(void);
#endif /* CODE_IMAGE_H_ */
