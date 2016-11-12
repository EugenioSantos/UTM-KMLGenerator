#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "UTM2LatLon.c"
#include "UTM-KMLGenerator.c"

int main()
{
    int navigation_selected_item;
    int navigation_exit = 0;

    do
    {
        navigation_clear();

        puts("[1] Ler arquivo com dados da poligonal");
        puts("[6] Sair");

        printf("Opcao escolhida: ");

        scanf("%i", &navigation_selected_item);

        switch(navigation_selected_item)
        {
            case 1 :
                navigation_read_file();
            break;
            case 6 :
                navigation_exit = navigation_console_exit();
                break;
            default :
                navigation_error("Opcao inválida");
                break;

        }
    }
    while(!navigation_exit);
}

void navigation_read_file()
{
    int navigation_selected_item;
    int navigation_exit = 0;

    read_coordinates_file("Informe o caminho do arquivo: ");
    convert_utm_coordinates_to_latlon();

    do
    {
        navigation_clear();

        puts("[1] Calcular Área da Poligonal");
        puts("[2] Calcular Perímetro da Poligonal");
        puts("[3] Testar se um ponto (em coordenadas geográficas) pertence a poligonal");
        puts("[4] Gerar Arquivo de Saída em KML");
        puts("[5] Sair");

        printf("Opção escolhida: ");

        scanf("%i", &navigation_selected_item);

        switch(navigation_selected_item)
        {
            case 1:
                navigation_sum_polygon_area();
                break;
            case 2:
                navigation_sum_polygon_perimeter();
                break;
            case 3:
                navigation_check_coordinate_point();
                break;
            case 4:
                generate_kml_file();
                break;
            case 5:
                navigation_exit = navigation_console_exit();
                break;
            default:
                navigation_error("Opção inválida");
                break;
        }

    }
    while(!navigation_exit);

}

void navigation_sum_polygon_area()
{
    double area = sum_polygon_area(utm_coordinates);

    printf("Total Area: %lf\n", area);

    system("PAUSE");
}

void navigation_sum_polygon_perimeter()
{
    double perimeter = sum_polygon_perimeter(utm_coordinates);

    printf("Total Perimeter: %lf\n",perimeter);

    system("PAUSE");

}

void navigation_check_coordinate_point()
{
    UTM utm_entry;

    printf("[Northing]: ");
    scanf("%lf", &utm_entry.northing);

    printf("[Easting]: ");
    scanf("%lf", &utm_entry.easting);

    if(check_coordinate_point(utm_entry)) {
        puts("O ponto pertence a poligonal");
    }
    else {
        puts("O ponto NÃO pertence a poligonal");
    }

    system("PAUSE");
}


void navigation_clear()
{
    system("cls || pause");
}

void navigation_error(char message[])
{
    puts(message);
    system("PAUSE");
}

int navigation_console_exit()
{
    char navigation_exit;

    do
    {
        navigation_clear();

        puts("Digite S para sair e N para continuar...");
        scanf("%c", &navigation_exit);
    }
    while(navigation_exit != 'S' && navigation_exit != 's' && navigation_exit != 'N' && navigation_exit != 'n');

    if(navigation_exit == 'S' || navigation_exit == 's')
        return 1;

    return 0;
}
