/*
 * image.c
 *
 *  Created on: 2024年6月7日
 *      Author: 2002
 */
#include "zf_common_headfile.h"


//二值化图像
uint8 bin_image[MT9V03X_H][MT9V03X_W];
int Threshold=0;
//逆透视图像
uint8 per_image[MT9V03X_H][MT9V03X_W];
//定义边线数组
int l_border[MT9V03X_H];
int r_border[MT9V03X_H];
uint8 l_border_fill[MT9V03X_H];//wifi图传用，元素补线，无边界丢线处理
uint8 r_border_fill[MT9V03X_H];
int l_border_repair[MT9V03X_H];//最终边线，元素补线，边界丢线处理
int r_border_repair[MT9V03X_H];
uint8 center_line[MT9V03X_H];
int center_line_repair[MT9V03X_H];

//定义赛道宽度信息数组
int road_wide[MT9V03X_H];//实时赛道宽度
const int standard_road_wide[MT9V03X_H]={

        18,19,19,20,21,23,25,27,27,29,
        31,32,34,36,37,39,41,42,44,45,
        47,49,49,51,53,54,56,57,58,60,
        62,63,64,66,68,69,70,72,73,75,
        77,78,80,82,82,84,86,87,89,90,
        91,92,94,95,96,98,100,102,103,104,
        106,108,109,111,112,114,115,117,118,120

};//标准赛道宽度
const uint8 right_map[MT9V03X_H]={

        89,90,90,90,91,91,91,91,91,92,
        92,92,93,93,93,94,94,94,94,95,
        95,95,96,96,96,97,97,97,97,98,
        98,98,99,99,99,99,100,100,100,100,
        101,101,101,102,102,102,102,103,103,103,
        103,103,104,104,104,104,104,105,105,105,
        105,106,106,106,106,107,107,107,107,108,

};//右轮映射
const uint8 left_map[MT9V03X_H]={

        86,86,85,85,84,84,84,83,83,83,
        82,82,82,81,81,81,80,80,80,79,
        79,79,79,78,78,78,78,77,77,77,
        76,76,76,76,76,75,75,75,74,74,
        74,74,73,73,73,73,72,72,72,72,
        71,71,71,71,70,70,70,69,69,69,
        69,68,68,68,68,67,67,67,66,66,

};//左轮映射
//


//定义查找白列信息
int white_col_num[MT9V03X_W];//每行白列数
int longest_white_column_left[2]; //最长白列,[0]是最长白列的长度
int longest_white_column_right[2];//最长白列,[0]是最长白列的长度
int hightest;//搜索截止行
//定义赛道丢线信息
int l_start;
int r_start;
int l_lost_start;//丢线开始的点
int r_lost_start;
int l_lost_num;
int r_lost_num;
int l_r_lostnum;
int l_lost_flag[MT9V03X_H] ; //左丢线数组，丢线置1
int r_lost_flag[MT9V03X_H]; //右丢线数组，丢线置1


//定义元素标志位信息

//十字
int cross_flag=0;//十字标志位
volatile int Left_Down_Find = 0; //十字使用，找到被置行数，没找到就是0
volatile int Left_Up_Find = 0;   //四个拐点标志
volatile int Right_Down_Find = 0;
volatile int Right_Up_Find = 0;

//坡道
int ramp_flag=0;//坡道标志位

//直道
int straight_flag;//直道标志位

//环岛


//斑马线
int zebra_line_flag;//斑马线标志位

//小砖块
int black_block_flag;//小砖块标志位

//出界
int chujie_flag=0;

//舵机信息

float err;//摄像头误差
float err_add;//摄像头误差补偿量
int angle=0;

 uint8 Weight[MT9V03X_H]=//误差加权控制
{
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端00 ——09 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端10 ——19 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端20 ——29 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端30 ——39 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端40 ——49 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端50 ——59 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端60 ——69 行权重

};
const uint8 weight_part[24]= //动态误差权重
{

    1,3,
    4,5,6,8,9,12,15,17,19,22,
    23,20,18,17,15,13,11,9,6,4,
    2,1,

};



/*-------------------------------------------------------------------------------------------------------------------
  @brief     快速大津求阈值，来自山威
  @param     image       图像数组
             col         列 ，宽度
             row         行，长度
  @return    null
  Sample     threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);//山威快速大津
  @note      据说比传统大津法快一点，使用效果差不多
-------------------------------------------------------------------------------------------------------------------*/
int my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j;
    int pixelSum = width * height/4;
    int threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }
    //计算每个像素值的点在整幅图像中的比例
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }
    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值
        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;
        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
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
  @brief     图像二值化处理函数
  @param     原始图像数组，高度，宽度，二值化阈值
  @return    二值化之后的图像数组
  Sample     Image_Binarization(Threshold);//图像二值化
  @note      二值化处理，0x00黑，0xff是白，已经用宏定义替换过了
-------------------------------------------------------------------------------------------------------------------*/
void Image_Binarization(int threshold)//图像二值化
{
    uint16 i,j;
    for(i=0;i<MT9V03X_H;i++)
    {
        for(j=0;j<MT9V03X_W;j++)
        {
            if(mt9v03x_image[i][j]>=threshold)
                bin_image[i][j]=IMG_WHITE;//白
            else
                bin_image[i][j]=IMG_BLACK;//黑
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     双最长白列巡线
  @param     null
  @return    null
  Sample     Longest_White_Column_Left();
  @note      最长白列巡线，寻找初始边界，丢线，最长白列等基础元素，后续读取这些变量来进行赛道识别
-------------------------------------------------------------------------------------------------------------------*/
void Longest_White_Column(void)//最长白列巡线
{
    int i, j;
    int start_column=20;//最长白列的搜索区间
    int end_column=MT9V03X_W-20;
    int left_border = 0, right_border = 0;//临时存储赛道位置
    longest_white_column_left[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    longest_white_column_left[1] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    longest_white_column_right[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    longest_white_column_right[1] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    r_lost_num = 0;    //边界丢线数
    l_lost_num  = 0;
    l_start = 0;//第一个非丢线点,常规边界起始点
    r_start = 0;
    l_lost_start = 0;
    r_lost_start = 0;
    l_r_lostnum = 0;//两边同时丢线数

    for (i = 0; i <=MT9V03X_H-1; i++)//数据清零
    {
        r_lost_flag[i] = 0;
        l_lost_flag[i] = 0;
        l_border[i] = 0;
        l_border_fill[i] = 0;
        l_border_repair[i]=0;
        r_border[i] = MT9V03X_W-1;
        r_border_fill[i] = MT9V03X_W-1;
        r_border_repair[i]=MT9V03X_W-1;
    }
    for(i=0;i<=MT9V03X_W-1;i++)
    {
        white_col_num[i] = 0;
    }

//环岛需要对最长白列范围进行限定，环岛3状态找不到上角点，可以修改下述参数
    //环岛3状态需要改变最长白列寻找范围
    if(Right_Island_Flag==1)//右环
    {
        if(Island_State==3)
        {
            start_column=40;
            end_column=MT9V03X_W-20;
        }
    }
    else if(Left_Island_Flag==1)//左环
    {
        if(Island_State==3)
        {
            start_column=20;
            end_column=MT9V03X_W-40;
        }
    }

    //从左到右，从下往上，遍历全图记录范围内的每一列白点数量
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

    //从左到右找左边最长白列
    longest_white_column_left[0] =0;
    for(i=start_column;i<=end_column;i++)
    {
        if (longest_white_column_left[0] < white_col_num[i])//找最长的那一列
        {
            longest_white_column_left[0] = white_col_num[i];//【0】是白列长度
            longest_white_column_left[1] = i;              //【1】是下标，第j列
        }
    }
    //从右到左找右左边最长白列
    longest_white_column_right[0] = 0;//【0】是白列长度
    for(i=end_column;i>=start_column;i--)//从右往左，注意条件，找到左边最长白列位置就可以停了
    {
        if (longest_white_column_right[0] < white_col_num[i])//找最长的那一列
        {
            longest_white_column_right[0] = white_col_num[i];//【0】是白列长度
            longest_white_column_right[1] = i;              //【1】是下标，第j列
        }
    }

    hightest = longest_white_column_left[0];//搜索截止行选取左或者右区别不大，他们两个理论上是一样的
    for (i = MT9V03X_H - 1; i >=MT9V03X_H-hightest; i--)//常规巡线
    {
        for (j = longest_white_column_right[1]; j <= MT9V03X_W - 1 - 2; j++)
        {
            if (bin_image[i][j] ==IMG_WHITE && bin_image[i][j + 1] == IMG_BLACK && bin_image[i][j + 2] == IMG_BLACK)//白黑黑，找到右边界
            {
                right_border = j;

                r_border_fill[i] = (uint8)j;
                r_lost_flag[i] = 0; //右丢线数组，丢线置1，不丢线置0
                break;
            }
            else if(j>=MT9V03X_W-1-2)//没找到右边界，把屏幕最右赋值给右边界
            {
                right_border = j;
                r_border_fill[i] = (uint8)j;
                r_lost_flag[i] = 1; //右丢线数组，丢线置1，不丢线置0
                break;
            }
        }
        for (j = longest_white_column_left[1]; j >= 0 + 2; j--)//往左边扫描
        {
            if (bin_image[i][j] ==IMG_WHITE && bin_image[i][j - 1] == IMG_BLACK && bin_image[i][j - 2] == IMG_BLACK)//黑黑白认为到达左边界
            {
                left_border = j;
                l_border_fill [i]=(uint8)j;
                l_lost_flag[i] = 0; //左丢线数组，丢线置1，不丢线置0
                break;
            }
            else if(j<=0+2)
            {
                left_border = j;//找到头都没找到边，就把屏幕最左右当做边界
                l_border_fill [i]=(uint8)j;
                l_lost_flag[i] = 1; //左丢线数组，丢线置1，不丢线置0
                break;
            }
        }
        l_border [i] = left_border;       //左边线线数组
        l_border_repair[i]=left_border;
      //  l_border_fill [i] = left_border;       //左边线线数组
        r_border[i] = right_border;      //右边线线数组
        r_border_repair[i]=right_border;
       // r_border_fill[i] = right_border;      //右边线线数组

    }

    for (i = MT9V03X_H - 1; i >= 0; i--)//赛道数据初步分析
    {
        if (l_lost_flag[i]  == 1)//单边丢线数
            l_lost_num++;
        if (r_lost_flag[i] == 1)
            r_lost_num++;
        if (l_lost_flag[i] == 1 && r_lost_flag[i] == 1)//双边丢线数
            l_r_lostnum++;
        if (l_start ==  0 && l_lost_flag[i]  != 1)//记录第一个非丢线点，边界起始点
            l_start = i;
        if (r_start == 0 && r_lost_flag[i] != 1)
            r_start = i;
        if (l_lost_start == 0 && l_lost_flag[i] == 1 && l_lost_flag[i + 1] == 1 && l_lost_flag[i + 2] == 1)//记录第一个丢线点，
            l_lost_start = i;
        if (r_lost_start == 0 && r_lost_flag[i] == 1 && r_lost_flag[i + 1] == 1 && r_lost_flag[i + 2] == 1)
            r_lost_start = i;
        road_wide[i]=r_border[i]-l_border[i];
    }

}






/*-------------------------------------------------------------------------------------------------------------------
  @brief     摄像头误差获取
  @param     null
  @return    获取到的误差
  Sample     err=Err_Sum();
  @note      加权取平均
-------------------------------------------------------------------------------------------------------------------*/
float Err_Sum(void)
{
    int i;
    float err=0;
    float weight_count=0;
    int weight_num;//动态权重24行,主行加10
    //根据速度给予误差行，使用13行（17——100cm）以下控制
    //weight_num给予13-27
    center_speed = (left_encoder + right_encoder) / 2;
    if (center_speed >= 460)
        weight_num = 13;
    else if (center_speed < 460 && center_speed >= 430)
        weight_num = 15;
    else if (center_speed < 430 && center_speed >= 410)
        weight_num = 17;
    else if (center_speed < 410 && center_speed >= 380)
        weight_num = 20;
    else if (center_speed < 380 && center_speed >= 360)
        weight_num = 22;
    else if (center_speed < 360 && center_speed >= 340)
        weight_num = 25;
    else
        weight_num = 27;


    for (i = weight_num; i < weight_num + 24; i++)
    {
        Weight[i] = weight_part[i-weight_num];
    }

    //使用13行（17——100cm）以下控制
    if ((MT9V03X_H - hightest) <=13)
    {
        for (i = MT9V03X_H - 1; i >=13; i--)//常规误差计算
        {
            err += (MT9V03X_W / 2 - ((l_border_repair[i] + r_border_repair[i]) >> 1)) * Weight[i];//右移1位，等效除2
            weight_count += Weight[i];
        }
        err = err / weight_count;
    }



    else 
    {
            for (i = MT9V03X_H - 1; i >= MT9V03X_H - hightest - 1; i--)//常规误差计算
            {
                err += (MT9V03X_W / 2 - ((l_border_repair[i] + r_border_repair[i]) >> 1)) * Weight[i];//右移1位，等效除2
                weight_count += Weight[i];
            }
            err = err / weight_count;
    }


    return err;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左补线
  @param     补线的起点，终点
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      补的直接是边界，点最好是可信度高的,不要乱补
-------------------------------------------------------------------------------------------------------------------*/
void Left_Add_Line(int x1, int y1, int x2, int y2)//左补线,补的是边界
{
    int i, max, a1, a2;
    uint8 hx;
    if (x1 >= MT9V03X_W - 1)//起始点位置校正，排除数组越界的可能
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
    if (a1 > a2)//坐标互换
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i <= a2; i++)//根据斜率补线即可
    {
        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= MT9V03X_W)
            hx = MT9V03X_W;
        else if (hx <= 0)
            hx = 0;
        l_border_repair[i] = hx;
        l_border_fill[i] = hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右补线
  @param     补线的起点，终点
  @return    null
  Sample     Right_Add_Line(int x1,int y1,int x2,int y2);
  @note      补的直接是边界，点最好是可信度高的，不要乱补
-------------------------------------------------------------------------------------------------------------------*/
void Right_Add_Line(int x1, int y1, int x2, int y2)//右补线,补的是边界
{
    int i, max, a1, a2;
    uint8 hx;
    if (x1 >= MT9V03X_W - 1)//起始点位置校正，排除数组越界的可能
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
    if (a1 > a2)//坐标互换
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i <= a2; i++)//根据斜率补线即可
    {
        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= MT9V03X_W)
            hx = MT9V03X_W;
        else if (hx <= 0)
            hx = 0;
        r_border_repair[i] = hx;
        r_border_fill[i] = hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Stop_Detect(void)
  @note      从起始点向上找5个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Left_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= MT9V03X_H - 1)//起始点位置校正，排除数组越界的可能
        start = MT9V03X_H - 1;
    else if (start <= 0)
        start = 0;
    if (end >= MT9V03X_H - 1)
        end = MT9V03X_H - 1;
    else if (end <= 0)
        end = 0;
    if (end < start)//++访问，坐标互换
    {
        t = end;
        end = start;
        start = t;
    }

    if (start <= 5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Left_Add_Line(l_border[start], start, l_border[end], end);
    }

    else
    {
        k = (float)(l_border[start] - l_border[start - 4]) / 5.0;//这里的k是1/斜率
        for (i = start; i <= end; i++)
        {
            l_border_repair[i] = (int)(i - start) * k + l_border_repair[start];//(x=(y-y1)*k+x1),点斜式变形
            l_border_fill[i] = (int)(i - start) * k + l_border_fill[start];//(x=(y-y1)*k+x1),点斜式变形
            if (l_border_fill[i] >= MT9V03X_W - 1)
            {
                l_border_repair[i] = MT9V03X_W - 1;
                l_border_fill[i] = MT9V03X_W - 1;
            }
            else if (l_border_fill[i] <= 0)
            {
                l_border_repair[i] = 0;
                l_border_fill[i] = 0;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Stop_Detect(void)
  @note      从起始点向上找3个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Right_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= MT9V03X_H - 1)//起始点位置校正，排除数组越界的可能
        start = MT9V03X_H - 1;
    else if (start <= 0)
        start = 0;
    if (end >= MT9V03X_H - 1)
        end = MT9V03X_H - 1;
    else if (end <= 0)
        end = 0;
    if (end < start)//++访问，坐标互换
    {
        t = end;
        end = start;
        start = t;
    }

    if (start <= 5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Right_Add_Line(r_border[start], start, r_border[end], end);
    }
    else
    {
        k = (float)(r_border[start] - r_border[start - 4]) / 5.0;//这里的k是1/斜率
        for (i = start; i <= end; i++)
        {
            r_border_repair[i] = (int)(i - start) * k + r_border_repair[start];//(x=(y-y1)*k+x1),点斜式变形
            r_border_fill[i] = (int)(i - start) * k + r_border_fill[start];//(x=(y-y1)*k+x1),点斜式变形
            if (r_border_fill[i] >= MT9V03X_W - 1)
            {
                r_border_repair[i]= MT9V03X_W - 1;
                r_border_fill[i] = MT9V03X_W - 1;
            }
            else if (r_border_fill[i] <= 0)
            {
                r_border_repair[i] = 0;
                r_border_fill[i] = 0;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     画线
  @param     输入起始点，终点坐标，补一条宽度为2的黑线
  @return    null
  Sample     Draw_Line(0, 0,MT9V03X_W-1,MT9V03X_H-1);
             Draw_Line(MT9V03X_W-1, 0,0,MT9V03X_H-1);
                                    画一个大×
  @note     补的就是一条线，需要重新扫线
-------------------------------------------------------------------------------------------------------------------*/
void Draw_Line(int startX, int startY, int endX, int endY)
{
    int i, x, y;
    int start = 0, end = 0;
    if (startX >= MT9V03X_W - 1)//限幅处理
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
    if (startX == endX)//一条竖线
    {
        if (startY > endY)//互换
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
    else if (startY == endY)//补一条横线
    {
        if (startX > endX)//互换
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
    else //上面两个是水平，竖直特殊情况，下面是常见情况
    {
        if (startY > endY)//起始点矫正
        {
            start = endY;
            end = startY;
        }
        else
        {
            start = startY;
            end = endY;
        }
        for (i = start; i <= end; i++)//纵向补线，保证每一行都有黑点
        {
            x = (int)(startX + (endX - startX) * (i - startY) / (endY - startY));//两点式变形
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
        for (i = start; i <= end; i++)//横向补线，保证每一列都有黑点
        {

            y = (int)(startY + (endY - startY) * (i - startX) / (endX - startX));//两点式变形
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
    //面积的符号表示方向
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



/*-------------------------------------------------------------------------------------------------------------------
  @brief     找下面的两个拐点，供十字使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Right_Down_Find=0;
             Left_Down_Find=0;
  Sample     Find_Down_Point(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Down_Point(int start, int end)
{
    int i, t;
    Right_Down_Find = 0;
    Left_Down_Find = 0;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (start >= MT9V03X_H - 1 - 5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = MT9V03X_H - 1 - 5;
    if (end <= MT9V03X_H - hightest)
        end = MT9V03X_H - hightest;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--)
    {
        if (Left_Down_Find == 0 &&//只找第一个符合条件的点
            abs(l_border[i] - l_border[i + 1]) <= 3 &&//角点的阈值可以更改
            abs(l_border[i + 1] - l_border[i + 2]) <= 4 &&
            abs(l_border[i + 2] - l_border[i + 3]) <= 5 &&
            (l_border[i] - l_border[i - 2]) >= 7 &&
            (l_border[i] - l_border[i - 3]) >= 8 &&
            (l_border[i] - l_border[i - 4]) >= 9)
        {
            Left_Down_Find = i;//获取行数即可
        }
        if (Right_Down_Find == 0 &&//只找第一个符合条件的点
            abs(r_border[i] - r_border[i + 1]) <= 2 &&//角点的阈值可以更改
            abs(r_border[i + 1] - r_border[i + 2]) <= 3 &&
            abs(r_border[i + 2] - r_border[i + 3]) <= 5 &&
            (r_border[i] - r_border[i - 2]) <= -7 &&
            (r_border[i] - r_border[i - 3]) <= -8 &&
            (r_border[i] - r_border[i - 4]) <= -9)
        {
            Right_Down_Find = i;
        }
        if (Left_Down_Find != 0 && Right_Down_Find != 0)//两个找到就退出
        {
            break;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     找上面的两个拐点，供十字使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Left_Up_Find=0;
             Right_Up_Find=0;
  Sample     Find_Up_Point(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Up_Point(int start, int end)
{
    int i, t;
    Left_Up_Find = 0;
    Right_Up_Find = 0;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (end <= MT9V03X_H - hightest)
        end = MT9V03X_H - hightest;
    if (end <= 5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end = 5;
    if (start >= MT9V03X_H - 1 - 5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = MT9V03X_H - 1 - 5;
    for (i = start; i >= end; i--)
    {
        if (Left_Up_Find == 0 &&//只找第一个符合条件的点
            abs(l_border[i] - l_border[i - 1]) <= 2 &&
            abs(l_border[i - 1] - l_border[i - 2]) <= 3 &&
            abs(l_border[i - 2] - l_border[i - 3]) <= 5 &&
            (l_border[i] - l_border[i + 2]) >= 7 &&
            (l_border[i] - l_border[i + 3]) >= 8 &&
            (l_border[i] - l_border[i + 4]) >= 9)
        {
            Left_Up_Find = i;//获取行数即可
        }
        if (Right_Up_Find == 0 &&//只找第一个符合条件的点
            abs(r_border[i] - r_border[i - 1]) <= 2 &&//下面两行位置差不多
            abs(r_border[i - 1] - r_border[i - 2]) <= 3 &&
            abs(r_border[i - 2] - r_border[i - 3]) <= 5 &&
            (r_border[i] - r_border[i + 2]) <= -7 &&
            (r_border[i] - r_border[i + 3]) <= -8 &&
            (r_border[i] - r_border[i + 4]) <= -9)
        {
            Right_Up_Find = i;//获取行数即可
        }
        if (Left_Up_Find != 0 && Right_Up_Find != 0)//下面两个找到就出去
        {
            break;
        }
    }
    if (abs(Right_Up_Find - Left_Up_Find) >= 30)//纵向撕裂过大，视为误判
    {
        Right_Up_Find = 0;
        Left_Up_Find = 0;
    }
}



/*-------------------------------------------------------------------------------------------------------------------
  @brief     十字检测
  @param     null
  @return    null
  Sample     Cross_Detect(void);
  @note      利用四个拐点判别函数，查找四个角点，根据找到拐点的个数决定是否补线
-------------------------------------------------------------------------------------------------------------------*/
void Cross_Detect()
{
    int down_search_start = 0;//下点搜索开始行
    cross_flag = 0;
    if (Island_State == 0 && ramp_flag == 0)//与环岛互斥开
    {
        Left_Up_Find = 0;
        Right_Up_Find = 0;
        if (l_r_lostnum >= 10)//十字必定有双边丢线，在有双边丢线的情况下再开始找角点
        {
            Find_Up_Point(MT9V03X_H - 1, 0);
            if (Left_Up_Find == 0 && Right_Up_Find == 0)//只要没有同时找到两个上点，直接结束
            {
                return;
            }
        }
        if (Left_Up_Find != 0 && Right_Up_Find != 0)//找到两个上点，就找到十字了
        {
            cross_flag = 1;//对应标志位，便于各元素互斥掉
            down_search_start = Left_Up_Find > Right_Up_Find ? Left_Up_Find : Right_Up_Find;//用两个上拐点坐标靠下者作为下点的搜索上限
            Find_Down_Point(MT9V03X_H - 5, down_search_start + 2);//在上拐点下2行作为下点的截止行
            if (Left_Down_Find <= Left_Up_Find)
            {
                Left_Down_Find = 0;//下点不可能比上点还靠上
            }
            if (Right_Down_Find <= Right_Up_Find)
            {
                Right_Down_Find = 0;//下点不可能比上点还靠上
            }
            if (Left_Down_Find != 0 && Right_Down_Find != 0)
            {//四个点都在，无脑连线，这种情况显然很少
                Left_Add_Line(l_border[Left_Up_Find], Left_Up_Find, l_border[Left_Down_Find], Left_Down_Find);
                Right_Add_Line(r_border[Right_Up_Find], Right_Up_Find, r_border[Right_Down_Find], Right_Down_Find);
            }
            else if (Left_Down_Find == 0 && Right_Down_Find != 0)//11//这里使用的都是斜率补线
            {//三个点                                     //01
                Lengthen_Left_Boundry(Left_Up_Find - 1, MT9V03X_H - 1);
                Right_Add_Line(r_border[Right_Up_Find], Right_Up_Find, r_border[Right_Down_Find], Right_Down_Find);
            }
            else if (Left_Down_Find != 0 && Right_Down_Find == 0)//11
            {//三个点                                     //10
                Left_Add_Line(l_border[Left_Up_Find], Left_Up_Find, l_border[Left_Down_Find], Left_Down_Find);
                Lengthen_Right_Boundry(Right_Up_Find - 1, MT9V03X_H - 1);
            }
            else if (Left_Down_Find == 0 && Right_Down_Find == 0)//11
            {//就俩上点                                   //00
                Lengthen_Left_Boundry(Left_Up_Find - 1, MT9V03X_H - 1);
                Lengthen_Right_Boundry(Right_Up_Find - 1, MT9V03X_H - 1);
            }
        }
        else
        {
            cross_flag = 0;
        }
    }
    //角点相关变量，debug使用
    //ips200_showuint8(0,12,Cross_Flag);
//    ips200_showuint8(0,13,Island_State);
//    ips200_showuint8(50,12,Left_Up_Find);
//    ips200_showuint8(100,12,Right_Up_Find);
//    ips200_showuint8(50,13,Left_Down_Find);
//    ips200_showuint8(100,13,Right_Down_Find);
}





void center_repair(void){
    int x,y;


//对于丢线，查找未丢线边界的变化趋势，映射到丢线区域,
//直接检查双边丢线情况


    for (y = MT9V03X_H - 1; y > MT9V03X_H - hightest; y--)
    {
        //遍历过的必定为正确的边界
        //左不丢，右丢
        if (l_lost_flag[y - 1] == 0 && r_lost_flag[y - 1] == 1)
            r_border_repair[y - 1] = r_border_repair[y] + abs(l_border[y - 1] - l_border[y]);

        //右不丢，左丢
        else if (l_lost_flag[y + 1] == 1 && r_lost_flag[y + 1] == 0)
            l_border_repair[y - 1] = l_border_repair[y] - abs(r_border[y - 1] - r_border[y]);
    }

    //环岛中线修复
    if(Island_State&&cross_flag==0 && ramp_flag==0)
    {
        if(Island_State==1||Island_State==2 || Island_State==8)
        {
            if(Left_Island_Flag)
            {
                for(y=0;y<MT9V03X_H;y++)
                {
                    l_border_repair[y]=r_border_repair[y]-standard_road_wide[y];
                }
            }
            else if(Right_Island_Flag)
            {
                for(y=0;y<MT9V03X_H;y++)
                {
                    r_border_repair[y]= l_border_repair[y]+standard_road_wide[y];
                }
            }
        }
        if (Island_State == 4) 
        {
            //环岛误差补偿
            /*
             * f(x) = p1*x + p2
            Coefficients:
           p1 =      0.3333
           p2 =      -3.333*/
            if(l_lost_num>10 &&  r_lost_num<5)
                   {
                       err_add = l_lost_num * 0.3333 + -3.333;
                   }
                   //右转弯(-)
                   if (l_lost_num<5 && r_lost_num>10)
                   {

                       err_add = -(r_lost_num * 0.3333 + -3.333);
                   }
        }
    }

    //再次修补普通弯道的中线，使用丢线数来给予误差补偿
    else if (straight_flag==0&& Island_State==0&&cross_flag==0 && ramp_flag==0)
    {
        //普通弯道
        //左转弯
        //设置阈值

        /*   f(x) = p1*x + p2  （0-7）
         Coefficients:
       p1 =      0.1795
       p2 =      -1.795
       */
        if(l_lost_num>10 &&  r_lost_num<5)
        {
            err_add = l_lost_num * 0.1795 + -1.795;
        }
        //右转弯(-)
        if (l_lost_num<5 && r_lost_num>10) 
        {

            err_add = -(r_lost_num * 0.1795 + -1.795);
        }
    }









}
void Zebra_detect(void)
{
    int j;
    int count=0;
    int start_column=20;//最长白列的搜索区间
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


void buzzer(void)
{
    if (cross_flag || Island_State || ramp_flag || zebra_line_flag)
    {
        gpio_set_level(P33_10, 1);
    }
    else
    {
        gpio_set_level(P33_10, 0);
    }
}



void project(void){
    int sum=0;
    int i;
    for(i=0;i<188;i++){
        if(bin_image[MT9V03X_H-3][i]==0){
            sum++;
            if ((sum > 160) && chujie_flag == 0/*&& park_flag!=2 */) {
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
    Image_Binarization(Threshold);//图像二值化
    Longest_White_Column();
    Cross_Detect();
    Island_Detect();
//显示用
    for(y=0;y<MT9V03X_H;y++){
        center_line[y]=(r_border_fill[y]+l_border_fill[y])/2;
        center_line_repair[y]= (r_border_repair[y] + l_border_repair[y]) / 2;
    }

    center_repair();
    for(y=0;y<MT9V03X_H;y++){
        center_line[y]=(r_border_fill[y]+l_border_fill[y])/2;
        center_line_repair[y] = (r_border_repair[y] + l_border_repair[y]) / 2;
    }

    Zebra_detect();
    set_speed();
    err=Err_Sum();
    angle=pid_servo(err);
    pwm_set_duty(ATOM0_CH1_P33_9, angle);
    project();
}
