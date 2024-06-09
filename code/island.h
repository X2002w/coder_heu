/*
 * island.h
 *
 *  Created on: 2024Äê6ÔÂ8ÈÕ
 *      Author: 2002
 */

#ifndef CODE_ISLAND_H_
#define CODE_ISLAND_H_










int Continuity_Change_Left(int start, int end);
int Continuity_Change_Right(int start, int end);
int Find_Left_Down_Point(int start, int end);
int Find_Left_Up_Point(int start, int end);
int Find_Right_Down_Point(int start, int end);
int Find_Right_Up_Point(int start, int end);
int Monotonicity_Change_Left(int start, int end);
int Monotonicity_Change_Right(int start, int end);
void K_Add_Boundry_Left(float k, int startX, int startY, int endY);
void K_Add_Boundry_Right(float k, int startX, int startY, int endY);
void K_Draw_Line(float k, int startX, int startY, int endY);
float Get_Left_K(int start_line, int end_line);
float Get_Right_K(int start_line, int end_line);



#endif /* CODE_ISLAND_H_ */
