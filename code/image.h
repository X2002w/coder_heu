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
extern uint8 l_border_fill[MT9V03X_H];
extern uint8 r_border_fill[MT9V03X_H];
extern uint8 center_line[MT9V03X_H];







//����
extern int chujie_flag;


void process(void);
#endif /* CODE_IMAGE_H_ */
