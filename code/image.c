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
const int standard_road_wide[MT9V03X_H]={

        18,19,19,20,21,23,25,27,27,29,
        31,32,34,36,37,39,41,42,44,45,
        47,49,49,51,53,54,56,57,58,60,
        62,63,64,66,68,69,70,72,73,75,
        77,78,80,82,82,84,86,87,89,90,
        91,92,94,95,96,98,100,102,103,104,
        106,108,109,111,112,114,115,117,118,120

};//��׼�������
const uint8 right_map[MT9V03X_H]={

        89,90,90,90,91,91,91,91,91,92,
        92,92,93,93,93,94,94,94,94,95,
        95,95,96,96,96,97,97,97,97,98,
        98,98,99,99,99,99,100,100,100,100,
        101,101,101,102,102,102,102,103,103,103,
        103,103,104,104,104,104,104,105,105,105,
        105,106,106,106,106,107,107,107,107,108,

};//����ӳ��
const uint8 left_map[MT9V03X_H]={

        86,86,85,85,84,84,84,83,83,83,
        82,82,82,81,81,81,80,80,80,79,
        79,79,79,78,78,78,78,77,77,77,
        76,76,76,76,76,75,75,75,74,74,
        74,74,73,73,73,73,72,72,72,72,
        71,71,71,71,70,70,70,69,69,69,
        69,68,68,68,68,67,67,67,66,66,

};//����ӳ��
//


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
int cross_flag=0;//ʮ�ֱ�־λ
int left_up_flag;//���Ϲյ��־λ
int left_down_flag;//���¹յ��־λ
int right_down_flag;//
int right_up_flag;

//�µ�
int ramp_flag=0;//�µ���־λ

//ֱ��
int straight_flag;//ֱ����־λ

//����


//������
int zebra_line_flag;//�����߱�־λ

//Сש��
int black_block_flag;//Сש���־λ

//����
int chujie_flag=0;

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
        err+=(MT9V03X_W/2-((l_border_fill[i]+r_border_fill[i])>>1))*Weight[i];//����1λ����Ч��2
        weight_count+=Weight[i];
    }
    err=err/weight_count;

    return err;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����
  @param     ���ߵ���㣬�յ�
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      ����ֱ���Ǳ߽磬������ǿ��Ŷȸߵ�,��Ҫ�Ҳ�
-------------------------------------------------------------------------------------------------------------------*/
void Left_Add_Line(int x1, int y1, int x2, int y2)//����,�����Ǳ߽�
{
    int i, max, a1, a2;
    uint8 hx;
    if (x1 >= MT9V03X_W - 1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
        x1 = MT9V03X_W - 1;
    else if (x1 <= 0)
        x1 = 0;
    if (y1 >= MT9V03X_H - 1)
        y1 = MT9V03X_H - 1;
    else if (y1 <= 0)
        y1 = 0;
    if (x2 >= MT9V03X_W - 1)
        x2 = MT9V03X_W - 1;
    else if (x2 <= 0)
        x2 = 0;
    if (y2 >= MT9V03X_H - 1)
        y2 = MT9V03X_H - 1;
    else if (y2 <= 0)
        y2 = 0;
    a1 = y1;
    a2 = y2;
    if (a1 > a2)//���껥��
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i <= a2; i++)//����б�ʲ��߼���
    {
        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= MT9V03X_W)
            hx = MT9V03X_W;
        else if (hx <= 0)
            hx = 0;
        l_border_fill[i] = hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     �Ҳ���
  @param     ���ߵ���㣬�յ�
  @return    null
  Sample     Right_Add_Line(int x1,int y1,int x2,int y2);
  @note      ����ֱ���Ǳ߽磬������ǿ��Ŷȸߵģ���Ҫ�Ҳ�
-------------------------------------------------------------------------------------------------------------------*/
void Right_Add_Line(int x1, int y1, int x2, int y2)//�Ҳ���,�����Ǳ߽�
{
    int i, max, a1, a2;
    uint8 hx;
    if (x1 >= MT9V03X_W - 1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
        x1 = MT9V03X_W - 1;
    else if (x1 <= 0)
        x1 = 0;
    if (y1 >= MT9V03X_H - 1)
        y1 = MT9V03X_H - 1;
    else if (y1 <= 0)
        y1 = 0;
    if (x2 >= MT9V03X_W - 1)
        x2 = MT9V03X_W - 1;
    else if (x2 <= 0)
        x2 = 0;
    if (y2 >= MT9V03X_H - 1)
        y2 = MT9V03X_H - 1;
    else if (y2 <= 0)
        y2 = 0;
    a1 = y1;
    a2 = y2;
    if (a1 > a2)//���껥��
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i <= a2; i++)//����б�ʲ��߼���
    {
        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= MT9V03X_W)
            hx = MT9V03X_W;
        else if (hx <= 0)
            hx = 0;
        r_border_fill[i] = hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ��߽��ӳ�
  @param     �ӳ���ʼ�������ӳ���ĳ��
  @return    null
  Sample     Stop_Detect(void)
  @note      ����ʼ��������5���㣬���б�ʣ������ӳ���ֱ��������
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Left_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= MT9V03X_H - 1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
        start = MT9V03X_H - 1;
    else if (start <= 0)
        start = 0;
    if (end >= MT9V03X_H - 1)
        end = MT9V03X_H - 1;
    else if (end <= 0)
        end = 0;
    if (end < start)//++���ʣ����껥��
    {
        t = end;
        end = start;
        start = t;
    }

    if (start <= 5)//��Ϊ��Ҫ�ڿ�ʼ��������3���㣬������ʼ����ڿ��ϣ��������ӳ���ֻ��ֱ������
    {
        Left_Add_Line(l_border[start], start, l_border[end], end);
    }

    else
    {
        k = (float)(l_border[start] - l_border[start - 4]) / 5.0;//�����k��1/б��
        for (i = start; i <= end; i++)
        {
            l_border_fill[i] = (int)(i - start) * k + l_border_fill[start];//(x=(y-y1)*k+x1),��бʽ����
            if (l_border_fill[i] >= MT9V03X_W - 1)
            {
                l_border_fill[i] = MT9V03X_W - 1;
            }
            else if (l_border_fill[i] <= 0)
            {
                l_border_fill[i] = 0;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����߽��ӳ�
  @param     �ӳ���ʼ�������ӳ���ĳ��
  @return    null
  Sample     Stop_Detect(void)
  @note      ����ʼ��������3���㣬���б�ʣ������ӳ���ֱ��������
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Right_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= MT9V03X_H - 1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
        start = MT9V03X_H - 1;
    else if (start <= 0)
        start = 0;
    if (end >= MT9V03X_H - 1)
        end = MT9V03X_H - 1;
    else if (end <= 0)
        end = 0;
    if (end < start)//++���ʣ����껥��
    {
        t = end;
        end = start;
        start = t;
    }

    if (start <= 5)//��Ϊ��Ҫ�ڿ�ʼ��������3���㣬������ʼ����ڿ��ϣ��������ӳ���ֻ��ֱ������
    {
        Right_Add_Line(r_border[start], start, r_border[end], end);
    }
    else
    {
        k = (float)(r_border[start] - r_border[start - 4]) / 5.0;//�����k��1/б��
        for (i = start; i <= end; i++)
        {
            r_border_fill[i] = (int)(i - start) * k + r_border_fill[start];//(x=(y-y1)*k+x1),��бʽ����
            if (r_border_fill[i] >= MT9V03X_W - 1)
            {
                r_border_fill[i] = MT9V03X_W - 1;
            }
            else if (r_border_fill[i] <= 0)
            {
                r_border_fill[i] = 0;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����
  @param     ������ʼ�㣬�յ����꣬��һ�����Ϊ2�ĺ���
  @return    null
  Sample     Draw_Line(0, 0,MT9V03X_W-1,MT9V03X_H-1);
             Draw_Line(MT9V03X_W-1, 0,0,MT9V03X_H-1);
                                    ��һ�����
  @note     ���ľ���һ���ߣ���Ҫ����ɨ��
-------------------------------------------------------------------------------------------------------------------*/
void Draw_Line(int startX, int startY, int endX, int endY)
{
    int i, x, y;
    int start = 0, end = 0;
    if (startX >= MT9V03X_W - 1)//�޷�����
        startX = MT9V03X_W - 1;
    else if (startX <= 0)
        startX = 0;
    if (startY >= MT9V03X_H - 1)
        startY = MT9V03X_H - 1;
    else if (startY <= 0)
        startY = 0;
    if (endX >= MT9V03X_W - 1)
        endX = MT9V03X_W - 1;
    else if (endX <= 0)
        endX = 0;
    if (endY >= MT9V03X_H - 1)
        endY = MT9V03X_H - 1;
    else if (endY <= 0)
        endY = 0;
    if (startX == endX)//һ������
    {
        if (startY > endY)//����
        {
            start = endY;
            end = startY;
        }
        for (i = start; i <= end; i++)
        {
            if (i <= 1)
                i = 1;
            bin_image[i][startX] = IMG_BLACK;
            bin_image[i - 1][startX] = IMG_BLACK;
        }
    }
    else if (startY == endY)//��һ������
    {
        if (startX > endX)//����
        {
            start = endX;
            end = startX;
        }
        for (i = start; i <= end; i++)
        {
            if (startY <= 1)
                startY = 1;
            bin_image[startY][i] = IMG_BLACK;
            bin_image[startY - 1][i] = IMG_BLACK;
        }
    }
    else //����������ˮƽ����ֱ��������������ǳ������
    {
        if (startY > endY)//��ʼ�����
        {
            start = endY;
            end = startY;
        }
        else
        {
            start = startY;
            end = endY;
        }
        for (i = start; i <= end; i++)//�����ߣ���֤ÿһ�ж��кڵ�
        {
            x = (int)(startX + (endX - startX) * (i - startY) / (endY - startY));//����ʽ����
            if (x >= MT9V03X_W - 1)
                x = MT9V03X_W - 1;
            else if (x <= 1)
                x = 1;
            bin_image[i][x] = IMG_BLACK;
            bin_image[i][x - 1] = IMG_BLACK;
        }
        if (startX > endX)
        {
            start = endX;
            end = startX;
        }
        else
        {
            start = startX;
            end = endX;
        }
        for (i = start; i <= end; i++)//�����ߣ���֤ÿһ�ж��кڵ�
        {

            y = (int)(startY + (endY - startY) * (i - startX) / (endX - startX));//����ʽ����
            if (y >= MT9V03X_H - 1)
                y = MT9V03X_H - 1;
            else if (y <= 0)
                y = 0;
            bin_image[y][i] = IMG_BLACK;
        }
    }
}
/*
float process_curvity(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 x3, uint8 y3)
{
    float K;
    uint8 S_of_ABC = ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2;
    //����ķ��ű�ʾ����
    uint8 q1 = (uint8)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    uint8 AB = my_sqrt(q1);
    q1 = (uint8)((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));
    uint8 BC = my_sqrt(q1);
    q1 = (uint8)((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
    uint8 AC = my_sqrt(q1);
    if (AB * BC * AC == 0)
    {
        K = 0;
    }
    else
        K = (float)4 * S_of_ABC / (AB * BC * AC);
    return K;
}
*/
void center_repair(void){
    int x,y;
    //���������޸�
    if(Island_State&&cross_flag==0 && ramp_flag==0)
    {
        if(Island_State==2 || Island_State==5 || Island_State==6 || Island_State==7 || Island_State==8)
        {
            if(Left_Island_Flag)
            {
                for(y=0;y<MT9V03X_H;y++)
                {
                    l_border_fill[y]=r_border_fill[y]-standard_road_wide[y]/2;
                }
            }
            else if(Right_Island_Flag)
            {
                for(y=0;y<MT9V03X_H;y++)
                {
                    r_border_fill[y]=l_border_fill[y]+standard_road_wide[y]/2;
                }
            }
        }
        if (Island_State == 4) 
        {
            if (Left_Island_Flag)
            {
                for (y = 0; y < MT9V03X_H; y++)
                {
                    l_border_fill[y] = r_border_fill[y] - standard_road_wide[y] / 2;
                }
            }
            if (Right_Island_Flag)
            {
                for (y = 0; y < MT9V03X_H; y++)
                {
                    r_border_fill[y] = l_border_fill[y] + standard_road_wide[y] / 2;
                }
            }
        }
    }

    else if (straight_flag==0&& Island_State==0&&cross_flag==0 && ramp_flag==0)
    {
        //��ͨ���
        //��ת��
        if(l_lost_num>10 &&  r_lost_num<5)
        {
            
        }
        //��ת��
        if (l_lost_num<5 && r_lost_num>10) 
        {


        }

    }

}
void Zebra_detect(void)
{
    int j;
    int count=0;
    int start_column=20;//����е���������
    int end_column=MT9V03X_W-20;
    if(hightest>=60)
    {
        for(j=start_column;j<=end_column;j++)
        {
            if(abs(white_col_num[j]-white_col_num[j+1])>=30)
            {
                count++;
            }
        }

    }
    if(count>=8)
   {
       zebra_line_flag+=1;
       if(zebra_line_flag>30)
       {
       //zebra_line_flag=0;
       }
   }


}

void project(void){
    int sum=0;
    int i;
    for(i=0;i<188;i++){
        if(bin_image[MT9V03X_H-3][i]==0){
            sum++;
            if (sum > 160 && chujie_flag == 0/*&& park_flag!=2 */) {
                chujie_flag = 1;
            }

    }
    //if((left_encoder+right_encoder)/2>400 && chujie_flag==0/*&& park_flag!=2 */){
    //    chujie_flag=1;
   //     Ok=0;
   // }
}
}


void process(void)
{
    int y;

    Threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);
    Image_Binarization(Threshold);//ͼ���ֵ��
    Longest_White_Column();
    Island_Detect();
//��ʾ��
    for(y=0;y<MT9V03X_H;y++){
        center_line[y]=(r_border_fill[y]+l_border_fill[y])/2;
    }

   // center_repair();
    Zebra_detect();
    set_speed();
    err=Err_Sum();
    angle=pid_servo(err);
    pwm_set_duty(ATOM0_CH1_P33_9, angle);
    project();
}
