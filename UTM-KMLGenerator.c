#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "UTM2LatLon.h"
#include <string.h>
#include <math.h>

int zona;
char hemisferio;
int vertices;

UTM *utm_coordinates;
LatLon *latlon_coordinates;

void read_coordinates_file(char prompt[])
{
    char filename[300];
    FILE *file;

    do
    {
        printf(prompt);
        scanf("%s", &filename);

        file = fopen(filename, "r");

        if(file == NULL)
            puts("Arquivo não encontrado");

    }
    while(file == NULL);

    fscanf(file, "%i %c %i ", &zona, &hemisferio, &vertices);
    utm_coordinates = (UTM*) malloc(vertices * sizeof(UTM));

    int i;

    for(i = 0; i < vertices; i++)
    {
        fscanf(file, "%lf %lf", &utm_coordinates[i].easting, &utm_coordinates[i].northing);
        utm_coordinates[i].letra = hemisferio;
        utm_coordinates[i].zona = zona;
    }

    fclose(file);
}

void convert_utm_coordinates_to_latlon()
{
	latlon_coordinates = (LatLon*) malloc(vertices * sizeof(LatLon));

    int i;

	for(i = 0; i < vertices; i++)
        latlon_coordinates[i] = convertUTM2LatLon(utm_coordinates[i], zona, hemisferio);
}

double sum_polygon_area(UTM utm_coordinates[])
{
    double area = 0;

    int i;
   	for (i = 0; i < vertices; i++)
   	    area +=((utm_coordinates[i].easting * utm_coordinates[i+1].northing)-(utm_coordinates[i+1].easting * utm_coordinates[i].northing))/2;

    return (area < 0) ? (area * (-1)) : area;
}

double sum_polygon_perimeter(UTM utm_coordinates[])
{
    double perimeter = 0;

    int i;
   	for (i = 0; i < vertices-1; i++)
          perimeter += (sqrt(pow((utm_coordinates[i+1].easting - utm_coordinates[i].easting),2) + pow((utm_coordinates[i+1].northing - utm_coordinates[i].northing),2)));

    return (perimeter < 0) ? (perimeter * (-1)) : perimeter;
}

int main(){

   read_coordinates_file("Informe o nome do arquivo: ");
   convert_utm_coordinates_to_latlon();

   printf("ZONA: %i\nVERTICES: %i\nHEMISFERIO: %c\nEST: %lf\nNOR: %lf\n", utm_coordinates[0].zona, vertices, utm_coordinates[0].letra, utm_coordinates[0].easting, utm_coordinates[0].northing);

    system("PAUSE");
}

