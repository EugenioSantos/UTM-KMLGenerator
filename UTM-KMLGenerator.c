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

// Ler arquivo com dados da poligonal
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

// Calcular Área da Poligonal
double sum_polygon_area(UTM utm_coordinates[])
{
    double area = 0;

    int i;
   	for (i = 0; i < vertices; i++)
   	    area +=((utm_coordinates[i].easting * utm_coordinates[i+1].northing)-(utm_coordinates[i+1].easting * utm_coordinates[i].northing))/2;

    return (area < 0) ? (area * (-1)) : area;
}

// Calcular Perímetro da Poligonal
double sum_polygon_perimeter(UTM utm_coordinates[])
{
    double perimeter = 0;

    int i;
   	for (i = 0; i < vertices-1; i++)
          perimeter += (sqrt(pow((utm_coordinates[i+1].easting - utm_coordinates[i].easting),2) + pow((utm_coordinates[i+1].northing - utm_coordinates[i].northing),2)));

    return (perimeter < 0) ? (perimeter * (-1)) : perimeter;
}

// Testar se um ponto (em coordenadas geográficas) pertence a poligonal
int check_coordinate_point(UTM point)
{

	int i, intersecoes = 0;

	for(i = 0; i < vertices-1; i++)
	{
		UTM p1, p2, temp;

		p1 = utm_coordinates[i];
		p2 = utm_coordinates[i+1];

		if(p1.easting > p2.easting)
		{
			temp = p1;
			p1 = p2;
			p2 = temp;
		}

		double northingMin, northingMax;

		if(p1.northing < p2.northing)
		{
			northingMin = p1.northing;
			northingMax = p2.northing;
		}
		else
		{
			northingMin = p2.northing;
			northingMax = p1.northing;
		}

		if(point.northing > northingMin && point.northing < northingMax)
		{
			if(point.easting > p2.easting)
			{
				intersecoes++;
			}
			else
			{
				if(point.easting > p1.easting)
				{
					double tanalfa1, tanalfa2, alfa1, alfa2;

					tanalfa1 = (p1.northing - point.northing) / (p1.easting - point.easting);
					tanalfa2 = (p1.northing - p2.northing) / (p1.easting - p2.easting);

					alfa1 = atan(tanalfa1);
					alfa2 = atan(tanalfa2);

					printf("a1 = %lf a2 = %lf", alfa1, alfa2);

					if(alfa1 < alfa2)
						intersecoes++;
				}
			}
		}
	}

	printf("inter = %i", intersecoes);

	return ((intersecoes % 2) == 0);
}

// Gerar Arquivo de Saída em KML
void generate_kml_file()
{
    FILE *input, *output;
    char name[140], description[140];

    fflush(stdin);

    printf("Nome: ");
    scanf("%[A-Z a-z]", name);

    fflush(stdin);

    printf("Descrição: ");
    scanf("%[A-Z a-z]", description);

    fflush(stdin);


	input = fopen("./sample/Exemplo.kml", "r");

	if(input == NULL)
    {
		puts("Erro ao abrir o arquivo KML base");
		return;
	}

	output = fopen("./output/Auto-KML.kml", "w");

	char line[300];

	while(fgets(line, 300, input))
    {
        fseek(output, 0, SEEK_END);

        if(strcmp(line, "{coordinates}\n") == 0)
        {
        	int i;
        	for(i = 0; i < vertices; i++)
        	{
        	    char new_line[300];
        		sprintf(new_line, "%lf, %lf\n", latlon_coordinates[i].latitude, latlon_coordinates[i].longitude);
        		fputs(new_line, output);
        	}

        }
        else if(strcmp(line, "{name}\n") == 0)
        {
            fputs(name, output);
        }
        else if(strcmp(line, "{description}\n") == 0)
        {
                fputs(description, output);
        }
        else
            fputs(line, output);
    }

    fclose(input);
    fclose(output);
}

int main(){

   read_coordinates_file("Informe o nome do arquivo: ");
   convert_utm_coordinates_to_latlon();

   generate_kml_file();

   printf("ZONA: %i\nVERTICES: %i\nHEMISFERIO: %c\nEST: %lf\nNOR: %lf\n", utm_coordinates[0].zona, vertices, utm_coordinates[0].letra, utm_coordinates[0].easting, utm_coordinates[0].northing);

    system("PAUSE");
}

