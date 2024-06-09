#include "zf_common_headfile.h"

int ZeroDrift_gyro_y=0;
int ZeroDrift_gyro_z=0;

int16 FJ_gyro_z = 0,FJ_gyro_y = 0;
volatile float FJ_Angle = 0;//��������ı���
volatile float FJ_Pitch = 0;//��������ı���
volatile float FJ_PitchSpeed = 0,FJ_LastPitchSpeed = 0;
volatile float FJ_LastAngleSpeed = 0,FJ_AngleSpeed = 0;
volatile float FJ_Angle_Max =  360;
volatile float FJ_Angle_Min = -360;
volatile float AngleSpeed = 0;



void Zero_Point_Detect(void)
{
    uint8 i;
    int zero_point_y_accu=0;
    int zero_point_z_accu=0;
    for(i=0;i<=100;i++)//����100�Σ���ȡƽ��ֵ����ȡ��ǰ��Ʈ
    {
        imu660ra_get_gyro();
        zero_point_y_accu+=imu660ra_gyro_y;//
        zero_point_z_accu+=imu660ra_gyro_z;
        system_delay_ms(3);//�ǵû��ɱ�ϵͳ��delay
    }
    ZeroDrift_gyro_y=zero_point_y_accu/100.0;
    ZeroDrift_gyro_z=zero_point_z_accu/100.0;
}

void Gyroscope_GetData(void)
{
    int16 gyro_z =0;
    imu660ra_get_gyro();
    gyro_z=imu660ra_gyro_z;
    AngleSpeed = ((gyro_z-ZeroDrift_gyro_z) * GYRO_SENS)*DT;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����Ǽ��㺽ƫ�ǽǶ�
//  @param
//  @return     void
//-------------------------------------------------------------------------------------------------------------------
void Get_Gyroscope_Angle(void)
{
    float K=0.7;
    FJ_gyro_z = imu660ra_gyro_z;
    FJ_LastAngleSpeed=FJ_AngleSpeed;
    FJ_AngleSpeed += ((FJ_gyro_z-ZeroDrift_gyro_z) * GYRO_SENS)*DT;
    FJ_Angle = FJ_AngleSpeed*K+FJ_LastAngleSpeed*(1-K);                //����Ϊ��
    FJ_Angle = FJ_Angle > FJ_Angle_Max ? FJ_Angle_Max : FJ_Angle;
    FJ_Angle = FJ_Angle < FJ_Angle_Min ? FJ_Angle_Min : FJ_Angle;
    FJ_Angle = -FJ_Angle;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����Ǽ��㸩���ǽǶ�
//  @param
//  @return     void
//-------------------------------------------------------------------------------------------------------------------
void Get_Gyroscope_Pitch(void)
{
    float K=0.7;
    FJ_LastPitchSpeed=FJ_PitchSpeed;
    FJ_gyro_y = imu660ra_gyro_y;
    FJ_PitchSpeed += ((FJ_gyro_y-ZeroDrift_gyro_y) * GYRO_SENS)*DT;
    FJ_Pitch = FJ_PitchSpeed*K+FJ_LastPitchSpeed*(1-K);

    FJ_Pitch = FJ_Pitch > 40 ? 40 : FJ_Pitch;
    FJ_Pitch = FJ_Pitch < (-40) ? (-40) : FJ_Pitch;

    if(FJ_Pitch>=5&&ramp_flag==0)
    {//�����Ǳ����Ϊ���µ���ǿ�н�1״̬������ע�⣬��������Ʈ�ᵼ�¸������Լ���Ʈ����Ī������Ľ�ȥ�µ�
    //    ramp_flag=1;
    }

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ǽǶ�����
//  @param
//  @return     void
//  @note
//-------------------------------------------------------------------------------------------------------------------
void Clear_Gyroscope_Pitch(void)
{
    FJ_Pitch = 0;
    FJ_gyro_y = 0;
    FJ_PitchSpeed = 0;
    FJ_LastPitchSpeed = 0;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ƫ���ǽǶ�����
//  @param
//  @return     void
//  @note
//-------------------------------------------------------------------------------------------------------------------
void Clear_Gyroscope_Angle(void)
{
    FJ_Angle = 0;
    FJ_gyro_z = 0;
    FJ_AngleSpeed = 0;
    FJ_LastAngleSpeed = 0;
}
