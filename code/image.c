/*
 * image.c
 *
 *  Created on: 2024��6��7��
 *      Author: 2002
 */
#include "zf_common_headfile.h"


//��ֵ��ͼ��
uint8 bin_image[MT9V03X_H][MT9V03X_W];
int Threshold=0;
//��͸��ͼ��
uint8 per_image[MT9V03X_H][MT9V03X_W];
//�����������
int l_border[MT9V03X_H];
int r_border[MT9V03X_H];
uint8 l_border_fill[MT9V03X_H];
uint8 r_border_fill[MT9V03X_H];
uint8 center_line[MT9V03X_H];

//�������������Ϣ����
int road_wide[MT9V03X_H];//ʵʱ�������
int standard_road_wide[MT9V03X_H]={0};//��׼�������

//������Ұ�����Ϣ
int white_col_num[MT9V03X_W];//ÿ�а�����
int longest_white_column_left[2]; //�����,[0]������еĳ���
int longest_white_column_right[2];//�����,[0]������еĳ���
int hightest;//������ֹ��
//��������������Ϣ
int l_start;
int r_start;
int l_lost_num;
int r_lost_num;
int l_r_lostnum;
int l_lost_flag[MT9V03X_H] ; //�������飬������1
int r_lost_flag[MT9V03X_H]; //�Ҷ������飬������1


//����Ԫ�ر�־λ��Ϣ

//ʮ��
int cross_flag;//ʮ�ֱ�־λ
int left_up_flag;//���Ϲյ��־λ
int left_down_flag;//���¹յ��־λ
int right_down_flag;//
int right_up_flag;

//�µ�
int ramp_flag;//�µ���־λ

//ֱ��
int straight_flag;//ֱ����־λ

//����
int Island_State;//������־λ
int Left_Island_Flag;//�󻷵���־
int Right_Island_Flag;//�һ�����־

//������
int zebra_line_flag;//�����߱�־λ

//Сש��
int black_block_flag;//Сש���־λ

//����
int chujie_flag;

//�����Ϣ

float err;//����ͷ���
int angle=0;

const uint8 Weight[MT9V03X_H]=//����Ȩ����
{
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //ͼ����Զ��00 ����09 ��Ȩ��
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //ͼ����Զ��10 ����19 ��Ȩ��
        1, 1, 1, 1, 1, 1, 1, 3, 4, 5,              //ͼ����Զ��20 ����29 ��Ȩ��
        6, 7, 9,11,13,15,17,19,20,20,              //ͼ����Զ��30 ����39 ��Ȩ��
       19,17,15,13,11, 9, 7, 5, 3, 1,              //ͼ����Զ��40 ����49 ��Ȩ��
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //ͼ����Զ��50 ����59 ��Ȩ��
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //ͼ����Զ��60 ����69 ��Ȩ��

};



/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���ٴ������ֵ������ɽ��
  @param     image       ͼ������
             col         �� �����
             row         �У�����
  @return    null
  Sample     threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);//ɽ�����ٴ��
  @note      ��˵�ȴ�ͳ��򷨿�һ�㣬ʹ��Ч�����
-------------------------------------------------------------------------------------------------------------------*/
int my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j;
    int pixelSum = width * height/4;
    int threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }
    //����ÿ������ֵ�ĵ�������ͼ���еı���
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }
    //�����Ҷȼ�[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ
        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;
        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
    }
    return threshold;
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     ͼ���ֵ��������
  @param     ԭʼͼ�����飬�߶ȣ���ȣ���ֵ����ֵ
  @return    ��ֵ��֮���ͼ������
  Sample     Image_Binarization(Threshold);//ͼ���ֵ��
  @note      ��ֵ������0x00�ڣ�0xff�ǰף��Ѿ��ú궨���滻����
-------------------------------------------------------------------------------------------------------------------*/
void Image_Binarization(int threshold)//ͼ���ֵ��
{
    uint16 i,j;
    for(i=0;i<MT9V03X_H;i++)
    {
        for(j=0;j<MT9V03X_W;j++)
        {
            if(mt9v03x_image[i][j]>=threshold)
                bin_image[i][j]=IMG_WHITE;//��
            else
                bin_image[i][j]=IMG_BLACK;//��
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     ˫�����Ѳ��
  @param     null
  @return    null
  Sample     Longest_White_Column_Left();
  @note      �����Ѳ�ߣ�Ѱ�ҳ�ʼ�߽磬���ߣ�����еȻ���Ԫ�أ�������ȡ��Щ��������������ʶ��
-------------------------------------------------------------------------------------------------------------------*/
void Longest_White_Column(void)//�����Ѳ��
{
    int i, j;
    int start_column=20;//����е���������
    int end_column=MT9V03X_W-20;
    int left_border = 0, right_border = 0;//��ʱ�洢����λ��
    longest_white_column_left[0] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    longest_white_column_left[1] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    longest_white_column_right[0] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    longest_white_column_right[1] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    r_lost_num = 0;    //�߽綪����
    l_lost_num  = 0;
    l_start = 0;//��һ���Ƕ��ߵ�,����߽���ʼ��
    r_start = 0;
    l_r_lostnum = 0;//����ͬʱ������

    for (i = 0; i <=MT9V03X_H-1; i++)//��������
    {
        r_lost_flag[i] = 0;
        l_lost_flag[i] = 0;
        l_border[i] = 0;
        l_border_fill[i] = 0;
        r_border[i] = MT9V03X_W-1;
        r_border_fill[i] = MT9V03X_W-1;
    }
    for(i=0;i<=MT9V03X_W-1;i++)
    {
        white_col_num[i] = 0;
    }

//������Ҫ������з�Χ�����޶�������3״̬�Ҳ����Ͻǵ㣬�����޸���������
    //����3״̬��Ҫ�ı������Ѱ�ҷ�Χ
    if(Right_Island_Flag==1)//�һ�
    {
        if(Island_State==3)
        {
            start_column=40;
            end_column=MT9V03X_W-20;
        }
    }
    else if(Left_Island_Flag==1)//��
    {
        if(Island_State==3)
        {
            start_column=20;
            end_column=MT9V03X_W-40;
        }
    }

    //�����ң��������ϣ�����ȫͼ��¼��Χ�ڵ�ÿһ�а׵�����
    for (j =start_column; j<=end_column; j++)
    {
        for (i = MT9V03X_H - 1; i >= 0; i--)
        {
            if(bin_image[i][j] == IMG_BLACK)
                break;
            else
                white_col_num[j]++;
        }
    }

    //����������������
    longest_white_column_left[0] =0;
    for(i=start_column;i<=end_column;i++)
    {
        if (longest_white_column_left[0] < white_col_num[i])//�������һ��
        {
            longest_white_column_left[0] = white_col_num[i];//��0���ǰ��г���
            longest_white_column_left[1] = i;              //��1�����±꣬��j��
        }
    }
    //���ҵ���������������
    longest_white_column_right[0] = 0;//��0���ǰ��г���
    for(i=end_column;i>=start_column;i--)//��������ע���������ҵ���������λ�þͿ���ͣ��
    {
        if (longest_white_column_right[0] < white_col_num[i])//�������һ��
        {
            longest_white_column_right[0] = white_col_num[i];//��0���ǰ��г���
            longest_white_column_right[1] = i;              //��1�����±꣬��j��
        }
    }

    hightest = longest_white_column_left[0];//������ֹ��ѡȡ����������𲻴�����������������һ����
    for (i = MT9V03X_H - 1; i >=MT9V03X_H-hightest; i--)//����Ѳ��
    {
        for (j = longest_white_column_right[1]; j <= MT9V03X_W - 1 - 2; j++)
        {
            if (bin_image[i][j] ==IMG_WHITE && bin_image[i][j + 1] == IMG_BLACK && bin_image[i][j + 2] == IMG_BLACK)//�׺ںڣ��ҵ��ұ߽�
            {
                right_border = j;
                r_border_fill[i] = (uint8)j;
                r_lost_flag[i] = 0; //�Ҷ������飬������1����������0
                break;
            }
            else if(j>=MT9V03X_W-1-2)//û�ҵ��ұ߽磬����Ļ���Ҹ�ֵ���ұ߽�
            {
                right_border = j;
                r_border_fill[i] = (uint8)j;
                r_lost_flag[i] = 1; //�Ҷ������飬������1����������0
                break;
            }
        }
        for (j = longest_white_column_left[1]; j >= 0 + 2; j--)//�����ɨ��
        {
            if (bin_image[i][j] ==IMG_WHITE && bin_image[i][j - 1] == IMG_BLACK && bin_image[i][j - 2] == IMG_BLACK)//�ںڰ���Ϊ������߽�
            {
                left_border = j;
                l_border_fill [i]=(uint8)j;
                l_lost_flag[i] = 0; //�������飬������1����������0
                break;
            }
            else if(j<=0+2)
            {
                left_border = j;//�ҵ�ͷ��û�ҵ��ߣ��Ͱ���Ļ�����ҵ����߽�
                l_border_fill [i]=(uint8)j;
                l_lost_flag[i] = 1; //�������飬������1����������0
                break;
            }
        }
        l_border [i] = left_border;       //�����������
      //  l_border_fill [i] = left_border;       //�����������
        r_border[i] = right_border;      //�ұ���������
       // r_border_fill[i] = right_border;      //�ұ���������
        center_line[i]=(r_border_fill[i]+l_border_fill[i])/2;
    }

    for (i = MT9V03X_H - 1; i >= 0; i--)//�������ݳ�������
    {
        if (l_lost_flag[i]  == 1)//���߶�����
            l_lost_num++;
        if (r_lost_flag[i] == 1)
            r_lost_num++;
        if (l_lost_flag[i] == 1 && r_lost_flag[i] == 1)//˫�߶�����
            l_r_lostnum++;
        if (l_start ==  0 && l_lost_flag[i]  != 1)//��¼��һ���Ƕ��ߵ㣬�߽���ʼ��
            l_start = i;
        if (r_start == 0 && r_lost_flag[i] != 1)
            r_start = i;
        road_wide[i]=r_border[i]-l_border[i];
    }

}




/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����ͷ����ȡ
  @param     null
  @return    ��ȡ�������
  Sample     err=Err_Sum();
  @note      ��Ȩȡƽ��
-------------------------------------------------------------------------------------------------------------------*/
float Err_Sum(void)
{
    int i;
    float err=0;
    float weight_count=0;
    //�������
    for(i=MT9V03X_H-1;i>=MT9V03X_H-hightest-1;i--)//����������
    {
        err+=(MT9V03X_W/2-((l_border[i]+r_border[i])>>1))*Weight[i];//����1λ����Ч��2
        weight_count+=Weight[i];
    }
    err=err/weight_count;

    return err;
}


void project(void){
    int sum=0;
    int i;
    for(i=0;i<188;i++){
        if(bin_image[MT9V03X_H-3][i]==0){
            sum++;
            if(sum>160&&chujie_flag==0/*&& park_flag!=2 */){
                chujie_flag=1;
            }
        }
    }
    //if((left_encoder+right_encoder)/2>400 && chujie_flag==0/*&& park_flag!=2 */){
    //    chujie_flag=1;
   //     Ok=0;
   // }
}


void process(void)
{
    Threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);
    Image_Binarization(Threshold);//ͼ���ֵ��
    Longest_White_Column();
    set_speed();
    err=Err_Sum();
    angle=pid_servo(err);
    pwm_set_duty(ATOM0_CH1_P33_9, angle);
    project();
}
