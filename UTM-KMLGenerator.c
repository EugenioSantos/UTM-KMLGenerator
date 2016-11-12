int zona;
char hemisferio;
int vertices;

UTM *utm_coordinates;
LatLon *latlon_coordinates;

void read_coordinates_file(char prompt[]);
void convert_utm_coordinates_to_latlon();
double sum_polygon_area(UTM utm_coordinates[]);
double sum_polygon_perimeter(UTM utm_coordinates[]);
bool check_coordinate_point(LatLon point);
void generate_kml_file();
void navigation_clear();
void navigation_error(char message[]);
void navigation_read_file();
int navigation_console_exit();

void navigation_sum_polygon_area();
void navigation_sum_polygon_perimeter();
void navigation_check_coordinate_point();

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
bool check_coordinate_point(LatLon point)
{
	int i, j = vertices-1;
    bool nodes;

     for (i=0; i<vertices; i++) {
    if (latlon_coordinates[i].longitude<point.longitude && latlon_coordinates[j].longitude>=point.longitude
    ||  latlon_coordinates[j].longitude<point.longitude && latlon_coordinates[i].longitude>=point.longitude) {
      if (latlon_coordinates[i].latitude+(point.longitude-latlon_coordinates[i].longitude)/(latlon_coordinates[j].longitude-latlon_coordinates[i].longitude)*(latlon_coordinates[j].latitude-latlon_coordinates[i].latitude)<point.latitude) {
        nodes=!nodes; }}
    j=i; }

  return nodes;
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



