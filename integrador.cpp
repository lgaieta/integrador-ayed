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
    char llegada[11];
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

void cargarHorarioSalida(char[]);
void cargarCorredores(ReporteCorredor corredoresClasica[],
                      int &cantidadCorredoresClasica,
                      ReporteCorredor corredoresNonstop[],
                      int &cantidadCorredoresNonstop);
bool esDescalificado(ReporteCorredor);
void calcularTiempo(ReporteCorredor, char horarioSalida[]);
void calcularTiempos(ReporteCorredor[], int cantidadCorredores, char horarioSalida[]);
void generarReporte(ReporteCorredor[], int cantidadCorredores);
void guardarReporteEnArchivo(ReporteCorredor[], int cantidadCorredores);
void guardarPodiosEnArchivo(ReporteCorredor[], int cantidadCorredores);

// HH:MM:SS.D
float horarioCadenaASegundos(char cadena[11]);
char *horarioSegundosACadena(float segundos);

int main()
{
    char horarioSalida[11];
    cargarHorarioSalida(horarioSalida);

    ReporteCorredor corredoresClasica[1000];
    int cantidadCorredoresClasica = 0;
    ReporteCorredor corredoresNonstop[1000];
    int cantidadCorredoresNonstop = 0;
    cargarCorredores(corredoresClasica,
                     cantidadCorredoresClasica,
                     corredoresNonstop,
                     cantidadCorredoresNonstop);

    calcularTiempos(corredoresClasica,
                    cantidadCorredoresClasica,
                    horarioSalida);
    calcularTiempos(corredoresNonstop,
                    cantidadCorredoresNonstop,
                    horarioSalida);
                    
    generarReporte(corredoresClasica, cantidadCorredoresClasica);
    generarReporte(corredoresNonstop, cantidadCorredoresNonstop);

    guardarReporteEnArchivo(corredoresClasica, cantidadCorredoresClasica);
    guardarReporteEnArchivo(corredoresNonstop, cantidadCorredoresNonstop);
    guardarPodiosEnArchivo(corredoresClasica, cantidadCorredoresClasica);
    guardarPodiosEnArchivo(corredoresNonstop, cantidadCorredoresNonstop);

    return 0;
}

// HH:MM:SS.D
float horarioCadenaASegundos(char cadena[11])
{
    float acumulador = 0;
    acumulador += ((cadena[0] - 48) * 10 + (cadena[1] - 48)) * 3600;
    acumulador += ((cadena[3] - 48) * 10 + (cadena[4] - 48)) * 60;
    acumulador += ((cadena[6] - 48) * 10 + (cadena[7] - 48));
    acumulador += (cadena[9] - 48) * 0.1;

    return acumulador;
}

char *horarioSegundosACadena(float segundos)
{

    int horas = (segundos / 60) / 60;
    static char hours[11];

    hours[0] = (horas / 10) + 48;
    hours[1] = (horas % 10) + 48;
    segundos -= horas * 60 * 60;
    int mins = segundos / 60;
    float segs = segundos - mins * 60;

    hours[0] = (horas / 10) + 48;
    hours[1] = (horas % 10) + 48;
    hours[2] = ':';
    hours[3] = (mins / 10) + 48;
    hours[4] = (mins % 10) + 48;
    hours[5] = ':';
    hours[6] = ((int)segs / 10) + 48;
    hours[7] = ((int)segs % 10) + 48;
    hours[8] = ',';
    hours[9] = ((int)(segs * 10) % 10) + 48;
    hours[10] = '\0';
    return hours;
}
