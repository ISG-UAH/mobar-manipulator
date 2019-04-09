/* 
 * File:   clampStruct.h
 * Author: Juan Alejandro Mora Prieto
 *
 * Created on June 17th, 2014
 */

#ifndef CLAMPSTRUCT_H
#define	CLAMPSTRUCT_H


//enum colorname {BLACK, BLUE, BROWN, CYAN, DARK_GREEN, GREY, GREEN, LIGHT_BLUE, ORANGE, PINK, RED, YELLOW, WHITE};
typedef struct GRIP_STATE{
	double open;
	int barrier;
}GRIP_STATE;

typedef struct COLORRGB{
	int red;
	int green;
	int blue;
	double tolerance;
	int idname;
	char code[7];
}COLORRGB;

#endif	/* CLAMPSTRUCT_H */
