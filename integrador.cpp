#include <iostream>

using namespace std;

struct RegistroCorredor
{
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

struct ReporteCorredor
{
    int posicionGeneral;
    int posicionGenero;
    int posicionCategoria;
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char tiempoTotal[11];
    char diferenciaPrimero[11];
    char diferenciaAnterior[11];
};

struct CorredorCiudad
{
    int numero;
    char nombreApellido[50];
    char localidad[40];
    char ciudad[11];
};

// HH:MM:SS.D
float cadenaAHorario(char cadena[11])
{
    float acumulador = 0;
    acumulador += ((cadena[0] - 48) * 10 + (cadena[1] - 48)) * 3600;
    acumulador += ((cadena[3] - 48) * 10 + (cadena[4] - 48)) * 60;
    acumulador += ((cadena[6] - 48) * 10 + (cadena[7] - 48));
    acumulador += (cadena[9] - 48) * 0.1;

    return acumulador;
}

int main()
{
    FILE *archivoCorredores = fopen("C:/programming/utn/integrador/Archivo corredores 4Refugios.bin", "rb+");

    if (!archivoCorredores)
    {
        cout << "Falta el archivo de corredores." << endl;
        return 1;
    }

    RegistroCorredor corredores[1000];

    float horaSalida = 3000;

    int i = 0;
    while (fread(&corredores[i], sizeof(RegistroCorredor), 1, archivoCorredores) == 1)
    {
        cout << corredores[i].nombreApellido;
        cout << cadenaAHorario(corredores[i].llegada) << endl;
        i++;
    }

    return 0;
}