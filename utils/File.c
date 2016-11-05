
FILE * file_open (char prompt[])
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

    return file;
}
