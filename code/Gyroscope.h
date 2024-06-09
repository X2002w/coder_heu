#ifndef CODE_GYROSCOPE_H_
#define CODE_GYROSCOPE_H_



/* ������ */
#define   GYRO_SENS             1/16.4
#define   ACCE_SENS             90.0/4096
#define   DT                    0.01

/**********��������*************/
extern volatile float FJ_Pitch;//ƫ����
extern volatile float FJ_Angle;//������

void Gyroscope_Init(void);
void Gyroscope_GetData(void);
void Zero_Point_Detect(void);
void Get_Gyroscope_Angle(void);
void Clear_Gyroscope_Angle(void);
void Get_Gyroscope_Pitch(void);
void Clear_Gyroscope_Pitch(void);

#endif
