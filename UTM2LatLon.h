#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
	double northing;
	double easting;
	int zona;
	char letra;
} UTM;

typedef struct
{
	double latitude;
	double longitude;
} LatLon;

LatLon convertUTM2LatLon (UTM utm, int zona, char hemisferio);
UTM converteLatLon2UTM(LatLon c);

double degree2rad(double deg);
double rad2degree(double rad);


#include "UTM2LatLon.c"