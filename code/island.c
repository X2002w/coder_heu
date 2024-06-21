/*
 * island.c
 *
 *  Created on: 2024��6��8��
 *      Author: 2002
 */

#include "zf_common_headfile.h"

 //����
int Island_State=0;//������־λ
int Left_Island_Flag=0;//�󻷵���־
int Right_Island_Flag=0;//�һ�����־
int left_up_point[2];
int right_up_point[2];




/*-------------------------------------------------------------------------------------------------------------------
  @brief     �������
  @param     null
  @return    null
  Sample     Island_Detect(void);
  @note      �����ĸ��յ��б����������Ըı亯����������˺�ѵ㣬��Ϊ8��
-------------------------------------------------------------------------------------------------------------------*/
void Island_Detect()
{
    static float k = 0;//3��5״̬��k
    static int island_state_5_down[2] = { 0 };//״̬5ʱ�����뿪���������ұ߽����͵㣬[0]��y����ĳ�У�{1}��x����ĳ��
    static int island_state_3_up[2] = { 0 };//״̬3ʱ�������뻷���ã���������ǵ�[0]��y����ĳ�У�{1}��x����ĳ��
    static int left_down_guai[2] = { 0 };//�ĸ��յ������洢��[0]��y����ĳ�У�{1}��x����ĳ��
    static int right_down_guai[2] = { 0 };//�ĸ��յ������洢��[0]��y����ĳ�У�{1}��x����ĳ��
    int monotonicity_change_line[2];//�����Ըı�����꣬[0]��ĳ�У�[1]��ĳ��
    int monotonicity_change_left_flag = 0;//��ת����0
    int monotonicity_change_right_flag = 0;//��ת����0
    int continuity_change_right_flag = 0; //������0
    int continuity_change_left_flag = 0;  //������0
    //�����ǳ����жϷ�
    continuity_change_left_flag = Continuity_Change_Left(MT9V03X_H - 1 - 5, 10);//�������ж�
    continuity_change_right_flag = Continuity_Change_Right(MT9V03X_H - 1 - 5, 10);
    monotonicity_change_right_flag = Monotonicity_Change_Right(MT9V03X_H - 1 - 10, 10);
    monotonicity_change_left_flag = Monotonicity_Change_Left(MT9V03X_H - 1 - 10, 10);
    if (cross_flag == 0 && Island_State == 0 && ramp_flag == 0)//&&Ramp_Flag==0
    {
        continuity_change_left_flag = Continuity_Change_Left(MT9V03X_H - 1 - 5, 10);//�������ж�
        continuity_change_right_flag = Continuity_Change_Right(MT9V03X_H - 1 - 5, 10);
        if (Left_Island_Flag == 0)//��
        {
            if (monotonicity_change_right_flag == 0 && //�ұ��ǵ�����
                continuity_change_left_flag != 0 && //����ǲ�������
                continuity_change_right_flag == 0 && //�󻷵��ұ���������
                l_lost_num >= 10 && //��߶��ߺܶ�
                l_lost_num <= 50 && //Ҳ����ȫ����
                r_lost_num <= 10 &&//�ұ߶��߽���
                hightest >= MT9V03X_H * 0.95 && //������ֹ�п�����Զ
                l_start >= MT9V03X_H - 20 && r_start >= MT9V03X_H - 20 && //�߽���ʼ�㿿��
                l_r_lostnum <= 10)//˫�߶�����
            {
                left_down_guai[0] = Find_Left_Down_Point(MT9V03X_H - 1, 20);//�����½ǵ�
                if (left_down_guai[0] >= 30)//����1���ɣ��������жϹյ㣬λ�ò��ԣ��������У�����
                {
                    Island_State = 1;
                    Left_Island_Flag = 1;
                }
                else//���У�����
                {
                    Island_State = 0;
                    Left_Island_Flag = 0;
                }
            }
        }
        if (Right_Island_Flag == 0)//�һ�
        {
            if (monotonicity_change_left_flag == 0 &&
                continuity_change_left_flag == 0 && //�һ��������������
                continuity_change_right_flag != 1 && //�ұ��ǲ�������
                r_lost_num >= 10 &&           //�Ҷ��߶�
                r_lost_num <= 50 &&           //�Ҷ��߲���̫��
                l_lost_num <= 10 &&            //������
                hightest >= MT9V03X_H * 0.95 && //������ֹ�п�����Զ
                l_start >= MT9V03X_H - 20 && r_start >= MT9V03X_H - 20 && //�߽���ʼ�㿿��
                l_r_lostnum <= 10)
            {
                right_down_guai[0] = Find_Right_Down_Point(MT9V03X_H - 1, 20);//���µ�
                if (right_down_guai[0] >= 30)//����1���ɣ���������йյ㣬λ�ò��ԣ��������У�����
                {
                    Island_State = 1;
                    Right_Island_Flag = 1;
                }
                else
                {
                    Island_State = 0;
                    Right_Island_Flag = 0;
                }
            }
        }
    }
    //    ips200_show_int(0,7*16,continuity_change_left_flag,2);
    //    ips200_show_int(0,8*16,continuity_change_right_flag,2);
    //    ips200_show_int(0,9*16,Left_Lost_Time,2);
    //    ips200_show_int(0,10*16,Right_Lost_Time,2);
    //    ips200_show_int(0,11*16,Search_Stop_Line,2);
    //    ips200_show_int(0,12*16,Boundry_Start_Left,2);

    //    //��ŷ���ǿ��3״̬��Ĭ��ע�͵�
        // if(Cross_Flag==0&&(Island_State==0||Island_State==2||Island_State==1)&&Zebra_State==0&&Ramp_Flag==0)
        // {
        //     if(ADC_Nor_Value[2]>=65)//���ĵ��ֵ��
        //     {
        //         if (continuity_change_left_flag==0&&Right_Lost_Time>=20&&Both_Lost_Time<5&&Boundry_Start_Left>MT9V03X_H-10)
        //         {//��������������ʼ�㣬�����٣�˫�߶����٣��ұ߶��߶�ֱ���һ�����״̬
        //             Right_Island_Flag=1;
        //             Island_State=3;
        //         }
        //         else if(continuity_change_right_flag==0&&Left_Lost_Time>=20&&Both_Lost_Time<5&&Boundry_Start_Right>MT9V03X_H-10)
        //         {
        //             Left_Island_Flag=1;
        //             Island_State=3;
        //         }
        //     }
        // }

    if (Left_Island_Flag == 1)//1״̬�¹յ㻹�ڣ�û����
    {
        if (Island_State == 1)
        {
            monotonicity_change_line[0] = Monotonicity_Change_Left(30, 5);//Ѱ�ҵ����Ըı��
            monotonicity_change_line[1] = l_border[monotonicity_change_line[0]];
            Left_Add_Line((int)(monotonicity_change_line[1] * 0.15), MT9V03X_H - 1, monotonicity_change_line[1], monotonicity_change_line[0]);



            if ((Island_State == 1) && (l_start < 30))//�·�������ʱ���2
            {
                Island_State = 2;
            }
        }


        else if (Island_State == 2)//�·��ǵ���ʧ��2״̬ʱ�·�Ӧ���Ƕ��ߣ������ǻ���
        {
            monotonicity_change_line[0] = Monotonicity_Change_Left(70, 10);//Ѱ�ҵ����Ըı��
            monotonicity_change_line[1] = l_border[monotonicity_change_line[0]];
            Left_Add_Line((int)(monotonicity_change_line[1] * 0.1), MT9V03X_H - 1, monotonicity_change_line[1], monotonicity_change_line[0]);
            if (Island_State == 2 && (l_start >= MT9V03X_H - 5 || monotonicity_change_line[0] > 50))//��Բ������ʱ�򣬽�3
            {
                Island_State = 3;//�����Ѱ�ҷ�ΧҲҪ�ģ���camera.c
                Left_Island_Flag = 1;
            }
        }
        else if (Island_State == 3)//3״̬׼��������Ѱ���Ϲյ㣬����
        {
            if (k != 0)
            {
                K_Draw_Line(k, MT9V03X_W - 30, MT9V03X_H - 1, 0);//k�Ǹո�������ģ���̬��������
                Longest_White_Column();//ˢ�±߽�����
            }
            else
            {
                left_up_point[0] = Find_Left_Up_Point(40, 5);//�����Ϲյ�
                left_up_point[1] = l_border[left_up_point[0]];

               /* if (left_up_point[0] < 5)//�˴�Ϊ�˷�ֹ���У����������3״̬���㣬�����޸Ĵ˴��ж�����
                {
                    Island_State = 0;
                    Left_Island_Flag = 0;
                }*/

                if (k == 0 && (15 <= left_up_point[0] && left_up_point[0] < 50) && (50 < left_up_point[1] && left_up_point[1] < 110))//�յ������һ����Χ�ڣ���Ϊ�ǹյ����
                {
                    island_state_3_up[0] = left_up_point[0];
                    island_state_3_up[1] = left_up_point[1];
                    k = (float)((float)(MT9V03X_H - island_state_3_up[0]) / (float)(MT9V03X_W - 20 - island_state_3_up[1]));
                    K_Draw_Line(k, MT9V03X_W - 30, MT9V03X_H - 1, 0);//��¼�µ�һ���ϵ����ʱλ�ã�������������һ�����ߣ��뻷
                    Longest_White_Column();//ˢ�±߽�����
                }
            }
            if ((Island_State == 3) && (abs(FJ_Angle) >= 60))//���������ǻ����뻷
            {
                k = 0;//б������
                Island_State = 4;//��һ��ʱ��˳�����������е�������Χ
                Longest_White_Column();//�����
            }
        }
        else if (Island_State == 4)//״̬4�Ѿ�������
        {
            if (abs(FJ_Angle) > 200)//����200���Ժ��ڴ򿪳����ж�
            {
                monotonicity_change_line[0] = Monotonicity_Change_Right(MT9V03X_H - 10, 10);//�����Ըı�
                monotonicity_change_line[1] = r_border[monotonicity_change_line[0]];
                if ((Island_State == 4) && (35 <= monotonicity_change_line[0] && monotonicity_change_line[0] <= 55 && monotonicity_change_line[1] >= 10))//�����㿿�£���ȥ5
                {//monotonicity_change_line[1]>=90&&
                    island_state_5_down[0] = MT9V03X_H - 1;
                    island_state_5_down[1] = r_border[MT9V03X_H - 1];
                    k = (float)((float)(MT9V03X_H - monotonicity_change_line[0]) / (float)(island_state_5_down[1] - monotonicity_change_line[1]));
                    K_Add_Boundry_Right(k, island_state_5_down[1], island_state_5_down[0], 0);//��״̬3һ������סб��
                    Island_State = 5;
                }
            }
        }
        else if (Island_State == 5)//����
        {
            K_Add_Boundry_Right(k, island_state_5_down[1], island_state_5_down[0], 0);
            if ((Island_State == 5) && (r_start < MT9V03X_H - 20))//�ұ��ȶ���
            {
                Island_State = 6;
            }
        }
        else if (Island_State == 6)//���ڳ�
        {
            K_Add_Boundry_Right(k, island_state_5_down[1], island_state_5_down[0], 0);
            if ((Island_State == 6) && ((r_start > MT9V03X_H - 10) || (abs(FJ_Angle) >= 320)))//�ұ߲�����
            {//
                k = 0;
                Island_State = 7;
            }
        }
        else if (Island_State == 7)//������ȥ�ˣ���Ѱ�ҹյ㣬׼���뿪����״̬
        {
            left_up_point[0] = Find_Left_Up_Point(MT9V03X_H - 10, 0);//��ȡ���ϵ����꣬��������ȥ8
            left_up_point[1] = l_border[left_up_point[0]];
            if ((Island_State == 7) && (left_up_point[1] <= 100) && (5 <= left_up_point[0] && left_up_point[0] <= MT9V03X_H - 20))//ע������Ժ������궼��Ҫ��
            {
                Island_State = 8;//�������ҵ��յ��ȥ8
            }
        }
        else if (Island_State == 8)//���ߣ��������һ��
        {
            left_up_point[0] = Find_Left_Up_Point(MT9V03X_H - 1, 10);//��ȡ���ϵ�����
            left_up_point[1] = l_border[left_up_point[0]];
            Lengthen_Left_Boundry(left_up_point[0] - 1, MT9V03X_H - 1);
            if ((Island_State == 8) && (left_up_point[0] >= MT9V03X_H - 20 || (left_up_point[0] < 10 && l_start >= MT9V03X_H - 10)))//���յ㿿��ʱ����Ϊ�����ˣ���������
            {//Ҫô�յ㿿�£�Ҫô�յ㶪�ˣ����·������ߣ���Ϊ����������
                FJ_Angle = 0;//��������
                Island_State = 0;//8ʱ�򻷵����������ˣ�Ϊ�˷�ֹ�����л���8����9����ż�ʮ�������㣬
                Left_Island_Flag = 0;
            }
        }
    }

    else if (Right_Island_Flag == 1)
    {
        if (Island_State == 1)//1״̬�¹յ㻹�ڣ�û����
        {
            monotonicity_change_line[0] = Monotonicity_Change_Right(30, 5);//�����Ըı�
            monotonicity_change_line[1] = r_border[monotonicity_change_line[0]];
            Right_Add_Line((int)(MT9V03X_W - 1 - (monotonicity_change_line[1] * 0.15)), MT9V03X_H - 1, monotonicity_change_line[1], monotonicity_change_line[0]);
            if (r_start <= 30)//���½��ȶ���
            {
                Island_State = 2;
            }
        }
        else if (Island_State == 2)//2״̬�·����ߣ��Ϸ��������ִ���
        {
            monotonicity_change_line[0] = Monotonicity_Change_Right(70, 5);//�����Ըı�
            monotonicity_change_line[1] = r_border[monotonicity_change_line[0]];
            Right_Add_Line((int)(MT9V03X_W - 1 - (monotonicity_change_line[1] * 0.15)), MT9V03X_H - 1, monotonicity_change_line[1], monotonicity_change_line[0]);
            //            if(Island_State==2&&(Boundry_Start_Right>=MT9V03X_H-10))//���½��ٲ����߽�3
            if (Island_State == 2 && (r_start >= MT9V03X_H - 5 || monotonicity_change_line[0] > 50))//���½��ٲ����߽�3
            {
                Island_State = 3;//�·����ߣ�˵�������Ѿ�������
                Right_Island_Flag = 1;
            }
        }
        else if (Island_State == 3)//�����Ѿ����ִ��ߣ����Ϸ����ֽǵ�
        {
            if (k != 0)//���ǵ����·����ӣ���һ������
            {
                K_Draw_Line(k, 30, MT9V03X_H - 1, 0);
                Longest_White_Column();//ˢ�������
            }
            else
            {
                right_up_point[0] = Find_Right_Up_Point(40, 10);//�����Ϲյ�
                right_up_point[1] = r_border[right_up_point[0]];

                /*if (right_up_point[0] < 10)//����Ĺ����˴�Ϊ�˷�ֹ�������У�����������ֻ���3���㣬���޸Ĵ˴��ж�����
                {
                    Island_State = 0;
                    Right_Island_Flag = 0;
                }*/

                if (k == 0 && (15 <= right_up_point[0] && right_up_point[0] < 50) && (70 < right_up_point[1] && right_up_point[1] < 150))//�ҵ�һ�����������Ľǵ㣬����
                {
                    island_state_3_up[0] = right_up_point[0];
                    island_state_3_up[1] = right_up_point[1];
                    k = (float)((float)(MT9V03X_H - island_state_3_up[0]) / (float)(20 - island_state_3_up[1]));
                    K_Draw_Line(k, 30, MT9V03X_H - 1, 0);
                    Longest_White_Column();//ˢ����������
                }
            }
            if ((Island_State == 3) && (abs(FJ_Angle) >= 60))//ֻ���������ǻ���
            {
                k = 0;//б������
                Island_State = 4;
                Longest_White_Column();//�����
            }//�ǵ�ȥ������Ǳ߸�һ�£����������һ���
        }
        else if (Island_State == 4)//4״̬��ȫ��ȥ������
        {
            if (FJ_Angle > 200)//��������200�Ⱥ��ٴ򿪵���ת���ж�
            {
                monotonicity_change_line[0] = Monotonicity_Change_Left(90, 10);//�����Ըı�
                monotonicity_change_line[1] = l_border[monotonicity_change_line[0]];
                if ((Island_State == 4) && (35 <= monotonicity_change_line[0] && monotonicity_change_line[0] <= 55 && monotonicity_change_line[1] < MT9V03X_W - 10))//�����㿿�£���ȥ5
                {//monotonicity_change_line[1]<=120&&
                    island_state_5_down[0] = MT9V03X_H - 1;
                    island_state_5_down[1] = l_border[MT9V03X_H - 1] - 5;//ץס��һ�γ��ֵ�б�ʣ�����
                    k = (float)((float)(MT9V03X_H - monotonicity_change_line[0]) / (float)(island_state_5_down[1] - monotonicity_change_line[1]));
                    K_Add_Boundry_Left(k, island_state_5_down[1], island_state_5_down[0], 0);
                    Island_State = 5;
                }
            }
        }
        else if (Island_State == 5)//׼��������
        {
            K_Add_Boundry_Left(k, island_state_5_down[1], island_state_5_down[0], 0);
            if (Island_State == 5 && l_start < MT9V03X_H - 20)//����ȶ���
            {
                Island_State = 6;
            }
        }
        else if (Island_State == 6)//������
        {
            K_Add_Boundry_Left(k, island_state_5_down[1], island_state_5_down[0], 0);
            if ((Island_State == 6) && (l_start > MT9V03X_H - 10 || abs(FJ_Angle) >= 320))//����ȶ���
            {//
                k = 0;
                Island_State = 7;
            }
        }
        else if (Island_State == 7)//�������������ҽǵ�
        {
            right_up_point[0] = Find_Right_Up_Point(MT9V03X_H - 10, 0);//��ȡ���ϵ����꣬�ҵ���ȥ8
            right_up_point[1] = r_border[right_up_point[0]];
            if ((Island_State == 7) && ((right_up_point[1] >= MT9V03X_W - 88 && (5 <= right_up_point[0] && right_up_point[0] <= MT9V03X_H - 20))))//ע������Ժ������궼��Ҫ����Ϊ������һ������������ֹյ�
            {//���ǵ�λ�ú���ʱ����8
                Island_State = 8;
            }
        }
        else if (Island_State == 8)//����8
        {
            right_up_point[0] = Find_Right_Up_Point(MT9V03X_H - 1, 10);//��ȡ���ϵ�����
            right_up_point[1] = r_border[right_up_point[0]];
            Lengthen_Right_Boundry(right_up_point[0] - 1, MT9V03X_H - 1);
            if ((Island_State == 8) && (right_up_point[0] >= MT9V03X_H - 20 || (right_up_point[0] < 10 && l_start >= MT9V03X_H - 10)))//���յ㿿��ʱ����Ϊ�����ˣ���������
            {//�ǵ㿿�£������¶˲����ߣ���Ϊ������
                FJ_Angle = 0;
                Island_State = 0;
                Right_Island_Flag = 0;
            }
        }
    }

}

    






/*-------------------------------------------------------------------------------------------------------------------
  @brief     �����������Լ��
  @param     ��ʼ�㣬��ֹ��
  @return    ��������0�����������ض��߳�����
  Sample     Continuity_Change_Left(int start,int end);
  @note      �����Ե���ֵ����Ϊ5���ɸ���
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Left(int start, int end)//��������ֵ����Ϊ5
{
    int i;
    int t;
    int continuity_change_flag = 0;
    if (l_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û��Ҫ�ж���
        return 1;
    if (hightest <= 5)//������ֹ�кܰ�
        return 1;
    if (start >= MT9V03X_H - 1 - 5)//����Խ�籣��
        start = MT9V03X_H - 1 - 5;
    if (end <= 5)
        end = 5;
    if (start < end)//���Ǵ������ϼ���ģ����˾ͻ���һ��
    {
        t = start;
        start = end;
        end = t;
    }

    for (i = start; i >= end; i--)
    {
        if (abs(l_border[i] - l_border[i - 1]) >= 5)//�����ж���ֵ��5,�ɸ���
        {
            continuity_change_flag = i;
            break;
        }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     �����������Լ��
  @param     ��ʼ�㣬��ֹ��
  @return    ��������0�����������ض��߳�����
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      �����Ե���ֵ����Ϊ5���ɸ���
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Right(int start, int end)
{
    int i;
    int t;
    int continuity_change_flag = 0;
    if (r_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û��Ҫ�ж���
        return 1;
    if (start >= MT9V03X_H - 5)//����Խ�籣��
        start = MT9V03X_H - 5;
    if (end <= 5)
        end = 5;
    if (start < end)//���Ǵ������ϼ���ģ����˾ͻ���һ��
    {
        t = start;
        start = end;
        end = t;
    }

    for (i = start; i >= end; i--)
    {
        if (abs(r_border[i] - r_border[i - 1]) >= 5)//��������ֵ��5���ɸ���
        {
            continuity_change_flag = i;
            break;
        }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Left_Down_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Down_Point(int start, int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i, t;
    int left_down_line = 0;
    if (l_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
        return left_down_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (start >= MT9V03X_H - 1 - 5)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start = MT9V03X_H - 1 - 5;
    if (end <= MT9V03X_H - hightest)
        end = MT9V03X_H - hightest;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--)
    {
        if (left_down_line == 0 &&//ֻ�ҵ�һ�����������ĵ�
            abs(l_border[i] - l_border[i + 1]) <= 3 &&//�ǵ����ֵ���Ը���
            abs(l_border[i + 1] - l_border[i + 2]) <= 4 &&
            abs(l_border[i + 2] - l_border[i + 3]) <= 5 &&
            (l_border[i] - l_border[i - 2]) >= 3 &&
            (l_border[i] - l_border[i - 3]) >= 5 &&
            (l_border[i] - l_border[i - 4]) >= 7)
        {
            left_down_line = i;//��ȡ��������
            break;
        }
    }
    return left_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���Ͻǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Left_Up_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Up_Point(int start, int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i, t;
    int left_up_line = 0;
    if (l_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
        return left_up_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (end <= MT9V03X_H - hightest)//������ֹ�����ϵ�ȫ������
        end = MT9V03X_H - hightest;
    if (end <= 5)//��ʱ����зǳ�����ҲҪ�������ֵ㣬��ֹ����Խ��
        end = 5;
    if (start >= MT9V03X_H - 1 - 5)
        start = MT9V03X_H - 1 - 5;
    for (i = start; i >= end; i--)
    {
        if (left_up_line == 0 &&//ֻ�ҵ�һ�����������ĵ�
            abs(l_border[i] - l_border[i - 1]) <= 5 &&
            abs(l_border[i - 1] - l_border[i - 2]) <= 5 &&
            abs(l_border[i - 2] - l_border[i - 3]) <= 5 &&
            (l_border[i] - l_border[i + 2]) >= 8 &&
            (l_border[i] - l_border[i + 3]) >= 15 &&
            (l_border[i] - l_border[i + 4]) >= 15)
        {
            left_up_line = i;//��ȡ��������
            break;
        }
    }
    return left_up_line;//�����MT9V03X_H-1��˵��û����ô���յ�
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start, int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i, t;
    int right_down_line = 0;
    if (r_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
        return right_down_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (start >= MT9V03X_H - 1 - 5)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start = MT9V03X_H - 1 - 5;
    if (end <= MT9V03X_H - hightest)
        end = MT9V03X_H - hightest;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--)
    {
        if (right_down_line == 0 &&//ֻ�ҵ�һ�����������ĵ�
            abs(r_border[i] - r_border[i + 1]) <= 5 &&//�ǵ����ֵ���Ը���
            abs(r_border[i + 1] - r_border[i + 2]) <= 5 &&
            abs(r_border[i + 2] - r_border[i + 3]) <= 5 &&
            (r_border[i] - r_border[i - 2]) <= -5 &&
            (r_border[i] - r_border[i - 3]) <= -10 &&
            (r_border[i] - r_border[i - 4]) <= -10)
        {
            right_down_line = i;//��ȡ��������
            break;
        }
    }
    return right_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���Ͻǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Up_Point(int start, int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i, t;
    int right_up_line = 0;
    if (r_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
        return right_up_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (end <= MT9V03X_H - hightest)//������ֹ�����ϵ�ȫ������
        end = MT9V03X_H - hightest;
    if (end <= 5)//��ʱ����зǳ�����ҲҪ�������ֵ㣬��ֹ����Խ��
        end = 5;
    if (start >= MT9V03X_H - 1 - 5)
        start = MT9V03X_H - 1 - 5;
    for (i = start; i >= end; i--)
    {
        if (right_up_line == 0 &&//ֻ�ҵ�һ�����������ĵ�
            abs(r_border[i] - r_border[i - 1]) <= 5 &&//��������λ�ò��
            abs(r_border[i - 1] - r_border[i - 2]) <= 5 &&
            abs(r_border[i - 2] - r_border[i - 3]) <= 5 &&
            (r_border[i] - r_border[i + 2]) <= -8 &&
            (r_border[i] - r_border[i + 3]) <= -15 &&
            (r_border[i] - r_border[i + 4]) <= -15)
        {
            right_up_line = i;//��ȡ��������
            break;
        }
    }
    return right_up_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������ͻ����
  @param     ��ʼ�㣬��ֹ��
  @return    �����ڵ��������Ҳ�������0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      ǰ5��5�������С�����������ǽǵ�
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Left(int start, int end)//�����Ըı䣬����ֵ�ǵ����Ըı�����ڵ�����
{
    int i;
    int monotonicity_change_line = 0;
    if (l_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û�е������жϵ�����
        return monotonicity_change_line;
    if (start >= MT9V03X_H - 1 - 5)//����Խ�籣�������жϵ�i����ʱ
        start = MT9V03X_H - 1 - 5; //Ҫ������ǰ��5���㣬������ͷ�ĵ�Ҫ������Ϊ����յ�
    if (end <= 5)
        end = 5;
    if (start <= end)//�ݼ����㣬��ڷ��ˣ�ֱ�ӷ���0
        return monotonicity_change_line;
    for (i = start; i >= end; i--)//���ȡǰ5��5���ݣ�����ǰ������뷶Χ��Ҫ��
    {
        if (l_border[i] == l_border[i + 5] && l_border[i] == l_border[i - 5] &&
            l_border[i] == l_border[i + 4] && l_border[i] == l_border[i - 4] &&
            l_border[i] == l_border[i + 3] && l_border[i] == l_border[i - 3] &&
            l_border[i] == l_border[i + 2] && l_border[i] == l_border[i - 2] &&
            l_border[i] == l_border[i + 1] && l_border[i] == l_border[i - 1])
        {//һ������һ������Ȼ������Ϊ����ת�۵�
            continue;
        }
        else if (l_border[i] >= l_border[i + 5] && l_border[i] >= l_border[i - 5] &&
            l_border[i] >= l_border[i + 4] && l_border[i] >= l_border[i - 4] &&
            l_border[i] >= l_border[i + 3] && l_border[i] >= l_border[i - 3] &&
            l_border[i] >= l_border[i + 2] && l_border[i] >= l_border[i - 2] &&
            l_border[i] >= l_border[i + 1] && l_border[i] >= l_border[i - 1])
        {//�ͺܱ����������������ǰ5����5�����ģ�����ȡ�ȣ����Ǿ��ǵ���ͻ���
            monotonicity_change_line = i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������ͻ����
  @param     ��ʼ�㣬��ֹ��
  @return    �����ڵ��������Ҳ�������0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      ǰ5��5�������С�����������ǽǵ�
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Right(int start, int end)//�����Ըı䣬����ֵ�ǵ����Ըı�����ڵ�����
{
    int i;
    int monotonicity_change_line = 0;

    if (r_lost_num >= 0.9 * MT9V03X_H)//�󲿷ֶ����ߣ�û�е������жϵ�����
        return monotonicity_change_line;
    if (start >= MT9V03X_H - 1 - 5)//����Խ�籣��
        start = MT9V03X_H - 1 - 5;
    if (end <= 5)
        end = 5;
    if (start <= end)
        return monotonicity_change_line;
    for (i = start; i >= end; i--)//���ȡǰ5��5���ݣ�����ǰ������뷶Χ��Ҫ��
    {
        if (r_border[i] == r_border[i + 5] && r_border[i] == r_border[i - 5] &&
            r_border[i] == r_border[i + 4] && r_border[i] == r_border[i - 4] &&
            r_border[i] == r_border[i + 3] && r_border[i] == r_border[i - 3] &&
            r_border[i] == r_border[i + 2] && r_border[i] == r_border[i - 2] &&
            r_border[i] == r_border[i + 1] && r_border[i] == r_border[i - 1])
        {//һ������һ������Ȼ������Ϊ����ת�۵�
            continue;
        }
        else if (r_border[i] <= r_border[i + 5] && r_border[i] <= r_border[i - 5] &&
            r_border[i] <= r_border[i + 4] && r_border[i] <= r_border[i - 4] &&
            r_border[i] <= r_border[i + 3] && r_border[i] <= r_border[i - 3] &&
            r_border[i] <= r_border[i + 2] && r_border[i] <= r_border[i - 2] &&
            r_border[i] <= r_border[i + 1] && r_border[i] <= r_border[i - 1])
        {//�ͺܱ����������������ǰ5����5�����ģ��Ǿ��ǵ���ͻ���
            monotonicity_change_line = i;
            break;
        }
    }
    return monotonicity_change_line;
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     ͨ��б�ʣ����㲹��--
  @param     k       ����б��
             startY  ������ʼ��������
             endY    ������������
  @return    null
  Sample     K_Add_Boundry_Left(float k,int startY,int endY);
  @note      ������ֱ�����ڱ�����
-------------------------------------------------------------------------------------------------------------------*/
void K_Add_Boundry_Left(float k, int startX, int startY, int endY)
{
    int i, t;
    if (startY >= MT9V03X_H - 1)
        startY = MT9V03X_H - 1;
    else if (startY <= 0)
        startY = 0;
    if (endY >= MT9V03X_H - 1)
        endY = MT9V03X_H - 1;
    else if (endY <= 0)
        endY = 0;
    if (startY < endY)//--������start��Ҫ��
    {
        t = startY;
        startY = endY;
        endY = t;
    }
    for (i = startY; i >= endY; i--)
    {
        l_border_fill[i] = (int)((i - startY) / k + startX);//(y-y1)=k(x-x1)���Σ�x=(y-y1)/k+x1
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

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ͨ��б�ʣ����㲹��
  @param     k       ����б��
             startY  ������ʼ��������
             endY    ������������
  @return    null    ֱ�Ӳ�����
  Sample     K_Add_Boundry_Right(float k,int startY,int endY);
  @note      ������ֱ�����ڱ�����
-------------------------------------------------------------------------------------------------------------------*/
void K_Add_Boundry_Right(float k, int startX, int startY, int endY)
{
    int i, t;
    if (startY >= MT9V03X_H - 1)
        startY = MT9V03X_H - 1;
    else if (startY <= 0)
        startY = 0;
    if (endY >= MT9V03X_H - 1)
        endY = MT9V03X_H - 1;
    else if (endY <= 0)
        endY = 0;
    if (startY < endY)
    {
        t = startY;
        startY = endY;
        endY = t;
    }
    for (i = startY; i >= endY; i--)
    {
        r_border_fill[i] = (int)((i - startY) / k + startX);//(y-y1)=k(x-x1)���Σ�x=(y-y1)/k+x1
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

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����б�ʻ���
  @param     ����б�ʣ����㣬��һ������
  @return    null
  Sample     K_Draw_Line(k, 20,MT9V03X_H-1 ,0)
  @note      ���ľ���һ���ߣ���Ҫ����ɨ��
-------------------------------------------------------------------------------------------------------------------*/
void K_Draw_Line(float k, int startX, int startY, int endY)
{
    int endX = 0;

    if (startX >= MT9V03X_W - 1)//�޷�����
        startX = MT9V03X_W - 1;
    else if (startX <= 0)
        startX = 0;
    if (startY >= MT9V03X_H - 1)
        startY = MT9V03X_H - 1;
    else if (startY <= 0)
        startY = 0;
    if (endY >= MT9V03X_H - 1)
        endY = MT9V03X_H - 1;
    else if (endY <= 0)
        endY = 0;
    endX = (int)((endY - startY) / k + startX);//(y-y1)=k(x-x1)���Σ�x=(y-y1)/k+x1
    Draw_Line(startX, startY, endX, endY);
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     ��ȡ�������߽�б��
  @param     int start_line,int end_line����ʼ�У���ֹ��
  @return    ����֮���б��
  Sample     k=Get_Left_K(68,69);
  @note      ����֮��ó�б�ʣ�Ĭ�ϵ�һ������С���ڶ���������
-------------------------------------------------------------------------------------------------------------------*/
float Get_Left_K(int start_line, int end_line)
{
    if (start_line >= MT9V03X_H - 1)
        start_line = MT9V03X_H - 1;
    else if (start_line <= 0)
        start_line = 0;
    if (end_line >= MT9V03X_H - 1)
        end_line = MT9V03X_H - 1;
    else if (end_line <= 0)
        end_line = 0;
    float k = 0;
    int t = 0;
    if (start_line > end_line)//++���ʣ����귴�˻���
    {
        t = start_line;
        start_line = end_line;
        end_line = t;
    }
    k = (float)(((float)l_border[start_line] - (float)l_border[end_line]) / (end_line - start_line + 1));
    return k;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ��ȡ�������߽�б��
  @param     int start_line,int end_line����ʼ�У���ֹ��
  @return    ����֮���б��
  Sample     k=Get_Right_K(68,69);
  @note      ����֮��ó�б�ʣ�Ĭ�ϵ�һ������С���ڶ���������
-------------------------------------------------------------------------------------------------------------------*/
float Get_Right_K(int start_line, int end_line)
{
    if (start_line >= MT9V03X_H - 1)
        start_line = MT9V03X_H - 1;
    else if (start_line <= 0)
        start_line = 0;
    if (end_line >= MT9V03X_H - 1)
        end_line = MT9V03X_H - 1;
    else if (end_line <= 0)
        end_line = 0;
    float k = 0;
    int t = 0;
    if (start_line > end_line)//++���ʣ����귴�˻���
    {
        t = start_line;
        start_line = end_line;
        end_line = t;
    }
    k = (float)(((float)r_border[start_line] - (float)r_border[end_line]) / (end_line - start_line + 1));
    return k;
}

