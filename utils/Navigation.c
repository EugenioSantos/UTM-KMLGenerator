void navigation_clear()
{
	system("cls || pause");
}

void navigation_error(char message[])
{
	puts(message);
    system("PAUSE");
}

void navigation_read_file()
{
    FILE * file = file_open("Informe o caminho completo do arquivo: ");

    int navigation_selected_item;
    int navigation_exit = 0;

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
    }
    while(!navigation_exit);

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


void navigation_main()
{
    setlocale(LC_ALL,"");

	int navigation_selected_item;
    int navigation_exit = 0;

    do
    {
        navigation_clear();

        puts("[1] Ler arquivo com dados da poligonal");
        puts("[6] Sair");

        printf("Opção escolhida: ");

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
        		navigation_error("Opção inválida");
        		break;

        }
    }
    while(!navigation_exit);
}

