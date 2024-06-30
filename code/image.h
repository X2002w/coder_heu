/*
 * image.h
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */

#ifndef CODE_IMAGE_H_
#define CODE_IMAGE_H_


#define IMG_BLACK     0X00      //0x00�Ǻ�
#define IMG_WHITE     0Xff      //0xffΪ��




//��ֵ��ͼ��
extern uint8 bin_image[MT9V03X_H][MT9V03X_W];


//�����������
extern int l_border[MT9V03X_H];
extern int r_border[MT9V03X_H];
extern uint8 l_border_fill[MT9V03X_H];//ͼ����
extern uint8 r_border_fill[MT9V03X_H];
extern int l_border_repair[MT9V03X_H];//���ձ���
extern int r_border_repair[MT9V03X_H];
extern uint8 center_line[MT9V03X_H];//ͼ����
extern int center_line_repair[MT9V03X_H];

extern int hightest;//������ֹ��


//��������������Ϣ
extern int l_start;
extern int r_start;
extern int l_lost_num;
extern int r_lost_num;
extern int l_r_lostnum;


//����
extern int Island_State;//������־λ
extern int Left_Island_Flag;//�󻷵���־
extern int Right_Island_Flag;//�һ�����־


extern float err;//����ͷ���
extern int angle;

//�µ�
extern int ramp_flag;//�µ���־λ

//ʮ��
extern int cross_flag;//ʮ�ֱ�־λ

//ֱ��
extern int straight_flag;//ֱ����־λ

//������
extern int zebra_line_flag;//�����߱�־λ

//����
extern int chujie_flag;

extern const uint8 right_map[MT9V03X_H];
extern const uint8 left_map[MT9V03X_H];

void Left_Add_Line(int x1, int y1, int x2, int y2);//����,�����Ǳ߽�
void Right_Add_Line(int x1, int y1, int x2, int y2);//�Ҳ���,�����Ǳ߽�
void Draw_Line(int startX, int startY, int endX, int endY);
void center_repair(void);
void straight_detect(void);
void buzzer(void);
void process(void);
#endif /* CODE_IMAGE_H_ */
