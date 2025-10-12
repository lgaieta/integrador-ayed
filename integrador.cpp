#include <iostream>
#include <cstring>

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

struct ContadorCategoria
{
    char categoria[50];
    int cantidad;
};

struct ContadorGenero
{
    char genero;
    int cantidad;
};

struct PodioCategoria
{
    char categoria[50];
    int cantidadCargada;
    ReporteCorredor corredores[3];
};

struct CorredorCiudad
{
    int numero;
    char nombreApellido[50];
    char localidad[40];
    char ciudad[11];
};

struct ReporteCiudad
{
    char nombre[11];
    int cantidadCorredores;
    char promedioTiempo[11];
};

struct Localidad
{
    char nombre[40];
    int cantidadCorredores;
    char tiempoPromedio[11];
    int cantidadCiudades;
    ReporteCiudad ciudades[1000];
};

void cargarHorarioSalida(char[]);
void cargarCorredores(RegistroCorredor registrosCorredoresClasica[],
                      int &cantidadCorredoresClasica,
                      RegistroCorredor registrosCorredoresNonstop[],
                      int &cantidadCorredoresNonstop);

bool esDescalificado(ReporteCorredor);
ReporteCorredor calcularTiempo(RegistroCorredor, char horarioSalida[]);

void calcularTiempos(RegistroCorredor[],
                     ReporteCorredor[],
                     int cantidadCorredores,
                     char horarioSalida[]);

void generarReporte(ReporteCorredor[], int cantidadCorredores);
void guardarReporteEnArchivo(ReporteCorredor[], int cantidadCorredores);
void guardarPodiosEnArchivo(ReporteCorredor[], int cantidadCorredores);
void cargarCorredoresCiudad(CorredorCiudad[], int &cantidadCorredoresCiudad);

void unirCorredores(ReporteCorredor corredoresCarrera1[],
                    int cantidadCorredores1,
                    ReporteCorredor corredoresCarrera2[],
                    int cantidadCorredores2,
                    ReporteCorredor corredoresCarreraTotal[]);

void generarReporteLocalidades(Localidad[],
                               int &cantidadLocalidades,
                               ReporteCorredor[],
                               int cantidadCorredores,
                               CorredorCiudad[],
                               int cantidadCorredoresCiudad);

void ordenarCiudadesPorNombre(Localidad &localidad);
void ordenarLocalidades(Localidad localidades[], int cantidadLocalidades);
void mostrarLocalidades(Localidad localidades[], int cantidadLocalidades);

// HH:MM:SS.D
float horarioCadenaASegundos(char cadena[11]);
char *horarioSegundosACadena(float segundos);

int main()
{
    char horarioSalida[11];
    cargarHorarioSalida(horarioSalida);

    RegistroCorredor registrosCorredoresClasica[1000];
    int cantidadCorredoresClasica = 0;
    RegistroCorredor registrosCorredoresNonstop[1000];
    int cantidadCorredoresNonstop = 0;
    cargarCorredores(registrosCorredoresClasica,
                     cantidadCorredoresClasica,
                     registrosCorredoresNonstop,
                     cantidadCorredoresNonstop);

    ReporteCorredor reportesCorredoresClasica[1000];
    ReporteCorredor reportesCorredoresNonstop[1000];

    calcularTiempos(registrosCorredoresClasica,
                    reportesCorredoresClasica,
                    cantidadCorredoresClasica,
                    horarioSalida);
    calcularTiempos(registrosCorredoresNonstop,
                    reportesCorredoresNonstop,
                    cantidadCorredoresNonstop,
                    horarioSalida);

    generarReporte(reportesCorredoresClasica, cantidadCorredoresClasica);
    generarReporte(reportesCorredoresNonstop, cantidadCorredoresNonstop);

    guardarReporteEnArchivo(reportesCorredoresClasica, cantidadCorredoresClasica);
    guardarReporteEnArchivo(reportesCorredoresNonstop, cantidadCorredoresNonstop);
    guardarPodiosEnArchivo(reportesCorredoresClasica, cantidadCorredoresClasica);
    guardarPodiosEnArchivo(reportesCorredoresNonstop, cantidadCorredoresNonstop);

    CorredorCiudad corredoresCiudad[1000];
    int cantidadCorredoresCiudad = 0;
    cargarCorredoresCiudad(corredoresCiudad, cantidadCorredoresCiudad);

    ReporteCorredor corredoresCarreraTotal[1000];
    unirCorredores(reportesCorredoresClasica,
                   cantidadCorredoresClasica,
                   reportesCorredoresNonstop,
                   cantidadCorredoresNonstop,
                   corredoresCarreraTotal);

    Localidad localidades[1000];
    int cantidadLocalidades = 0;
    generarReporteLocalidades(localidades,
                              cantidadLocalidades,
                              corredoresCarreraTotal,
                              cantidadCorredoresClasica + cantidadCorredoresNonstop,
                              corredoresCiudad,
                              cantidadCorredoresCiudad);
    ordenarLocalidades(localidades, cantidadLocalidades);
    mostrarLocalidades(localidades, cantidadLocalidades);

    return 0;
}

void cargarHorarioSalida(char horarioSalida[])
{
    cout << "Ingrese horario de salida (HH:MM:SS.D): ";
    cin >> horarioSalida;
    cout << endl;
}

void generarReporte(ReporteCorredor corredores[], int cantidadCorredores)
{
    ContadorCategoria contadoresCategoria[1000];
    int cantidadCategorias = 0;
    ContadorGenero contadoresGenero[1000];
    int cantidadGeneros = 0;

    for (int i = 0; i < cantidadCorredores; i++)
    {
        // Obtener el mayor actual por tiempo total
        int posicionMayorActual = i;
        for (int j = i + 1; j < cantidadCorredores; j++)
        {
            if (horarioCadenaASegundos(corredores[j].tiempoTotal) > horarioCadenaASegundos(corredores[posicionMayorActual].tiempoTotal))
            {
                posicionMayorActual = j;
            }
        }

        // Asignar posicion general
        corredores[posicionMayorActual].posicionGeneral = i + 1;

        // Asignar posicion por categoria
        bool categoriaEncontrada = false;
        for (int k = 0; k < cantidadCategorias; k++)
        {
            if (strcmp(corredores[posicionMayorActual].categoria, contadoresCategoria[k].categoria) == 0)
            {
                contadoresCategoria[k].cantidad++;
                corredores[posicionMayorActual].posicionCategoria = contadoresCategoria[k].cantidad;
                categoriaEncontrada = true;
                break;
            }
        }
        if (categoriaEncontrada == false)
        {
            strcpy(contadoresCategoria[cantidadCategorias].categoria, corredores[posicionMayorActual].categoria);
            contadoresCategoria[cantidadCategorias].cantidad = 1;
            corredores[posicionMayorActual].posicionCategoria = 1;
            cantidadCategorias++;
        }

        // Asignar posicion por genero
        bool generoEncontrado = false;
        for (int k = 0; k < cantidadGeneros; k++)
        {
            if (corredores[posicionMayorActual].genero == contadoresGenero[k].genero)
            {
                contadoresGenero[k].cantidad++;
                corredores[posicionMayorActual].posicionGenero = contadoresGenero[k].cantidad;
                generoEncontrado = true;
                break;
            }
        }
        if (generoEncontrado == false)
        {
            contadoresGenero[cantidadGeneros].genero = corredores[posicionMayorActual].genero;
            contadoresGenero[cantidadGeneros].cantidad = 1;
            corredores[posicionMayorActual].posicionGenero = 1;
            cantidadGeneros++;
        }

        // Intercambio de posiciones para ordenar por tiempo total
        ReporteCorredor aux = corredores[posicionMayorActual];
        corredores[posicionMayorActual] = corredores[i];
        corredores[i] = aux;
    }
}

void guardarPodiosEnArchivo(ReporteCorredor corredores[], int cantidadCorredores)
{
    PodioCategoria podios[1000];
    int cantidadPodios = 0;

    // Guardar podios por categoria
    for (int i = 0; i < cantidadCorredores; i++)
    {
        bool categoriaEncontrada = false;
        for (int j = 0; j < cantidadPodios; j++)
        {
            if (strcmp(corredores[i].categoria, podios[j].categoria) == 0)
            {
                if (podios[j].cantidadCargada < 3)
                {
                    podios[j].corredores[podios[j].cantidadCargada] = corredores[i];
                    podios[j].cantidadCargada++;
                }
                categoriaEncontrada = true;
                break;
            }
        }

        if (categoriaEncontrada == false)
        {
            strcpy(podios[cantidadPodios].categoria, corredores[i].categoria);
            podios[cantidadPodios].corredores[0] = corredores[i];
            podios[cantidadPodios].cantidadCargada = 1;
            cantidadPodios++;
        }
    }

    // Ordenar podios por categoria
    for (int i = 0; i < cantidadPodios; i++)
    {
        int posicionMenorActual = i;
        for (int j = i + 1; j < cantidadPodios; j++)
        {
            if (strcmp(podios[i].categoria, podios[j].categoria) < 0)
            {
                posicionMenorActual = j;
            }
        }
        PodioCategoria aux = podios[i];
        podios[i] = podios[posicionMenorActual];
        podios[posicionMenorActual] = aux;
    }

    ReporteCorredor reportesPodios[1000];
    int indiceReportes = 0;

    for (int i = 0; i < cantidadPodios; i++)
    {
        for (int j = 0; j < podios[i].cantidadCargada; j++)
        {
            reportesPodios[indiceReportes] = podios[i].corredores[j];
            indiceReportes++;
        }
    }

    FILE *archivoPodios = fopen("C:/programming/utn/integrador/podios.bin", "wb");
    if (!archivoPodios)
    {
        cout << "Error al abrir el archivo podios.bin";
        return;
    }

    fwrite(reportesPodios, sizeof(ReporteCorredor), indiceReportes, archivoPodios);
    fclose(archivoPodios);
}

void unirCorredores(ReporteCorredor corredoresCarrera1[],
                    int cantidadCorredores1,
                    ReporteCorredor corredoresCarrera2[],
                    int cantidadCorredores2,
                    ReporteCorredor corredoresCarreraTotal[])
{
    for (int i = 0; i < cantidadCorredores1; i++)
    {
        corredoresCarreraTotal[i] = corredoresCarrera1[i];
    }
    for (int i = 0; i < cantidadCorredores2; i++)
    {
        corredoresCarreraTotal[i + cantidadCorredores1] = corredoresCarrera2[i];
    }
}

void cargarCorredoresCiudad(CorredorCiudad corredoresCiudad[], int &cantidadCorredoresCiudad)
{
    FILE *archivoCorredoresCiudad = fopen("C:/programming/utn/integrador/Ciudades.bin", "rb");
    if (!archivoCorredoresCiudad)
    {
        cout << "Error al abrir el archivo Ciudades.bin";
        return;
    }

    int i = 0;
    while (fread(&corredoresCiudad[i], sizeof(CorredorCiudad), 1, archivoCorredoresCiudad) == 1)
    {
        i++;
    }
    fclose(archivoCorredoresCiudad);
    cantidadCorredoresCiudad = i + 1;
}

void generarReporteLocalidades(Localidad localidades[],
                               int &cantidadLocalidades,
                               ReporteCorredor reportesCorredor[],
                               int cantidadCorredores,
                               CorredorCiudad corredoresCiudad[],
                               int cantidadCorredoresCiudad)
{
    for (int i = 0; i < cantidadCorredores; i++)
    {
        // Busco si ya hay una localidad guardada que coincida con el corredor i
        int posicionLocalidad = -1;
        for (int j = 0; j < cantidadLocalidades; j++)
        {
            if (strcmp(reportesCorredor[i].localidad, localidades[j].nombre) == 0)
            {
                posicionLocalidad = j;
                break;
            }
        }

        // Busco el corredor en la lista de corredores con su ciudad
        int posicionCorredorCiudad = -1;
        for (int k = 0; k < cantidadCorredoresCiudad; k++)
        {
            if (reportesCorredor[i].numero == corredoresCiudad[k].numero)
            {
                posicionCorredorCiudad = k;
                break;
            }
        }

        if (posicionCorredorCiudad == -1)
        {
            continue;
        }

        // Si no hay localidad guardada, la creo
        if (posicionLocalidad == -1)
        {
            Localidad &localidadActual = localidades[cantidadLocalidades];
            strcpy(localidadActual.nombre, reportesCorredor[i].localidad);
            localidadActual.cantidadCorredores = 1;
            strcpy(localidadActual.ciudades[0].nombre, corredoresCiudad[posicionCorredorCiudad].ciudad);
            localidadActual.ciudades[0].cantidadCorredores = 1;
            strcpy(localidadActual.ciudades[0].promedioTiempo, reportesCorredor[i].tiempoTotal);
            localidadActual.cantidadCiudades = 1;
            strcpy(localidadActual.tiempoPromedio, reportesCorredor[i].tiempoTotal);
            cantidadLocalidades++;
            continue;
        }

        Localidad &localidadActual = localidades[posicionLocalidad];
        // Actualizar localidad: promedio ponderado por cantidad de corredores
        float tiempoTotalCorredorActual = horarioCadenaASegundos(reportesCorredor[i].tiempoTotal);
        float actualPromedioLocalidad = horarioCadenaASegundos(localidadActual.tiempoPromedio);
        float nuevoPromedioLocalidad = (actualPromedioLocalidad * localidadActual.cantidadCorredores + tiempoTotalCorredorActual) / (localidadActual.cantidadCorredores + 1);
        strcpy(localidadActual.tiempoPromedio, horarioSegundosACadena(nuevoPromedioLocalidad));
        localidadActual.cantidadCorredores++;

        // Busco si ya hay una ciudad guardada en la localidad que coincida con el corredor i
        int posicionCiudad = -1;
        for (int j = 0; j < localidadActual.cantidadCiudades; j++)
        {
            if (strcmp(localidadActual.ciudades[j].nombre, corredoresCiudad[posicionCorredorCiudad].ciudad) == 0)
            {
                posicionCiudad = j;
                break;
            }
        }

        // Si no hay ciudad guardada, la creo
        if (posicionCiudad == -1)
        {
            ReporteCiudad &ciudadActual = localidadActual.ciudades[localidadActual.cantidadCiudades];
            strcpy(ciudadActual.nombre, corredoresCiudad[posicionCorredorCiudad].ciudad);
            ciudadActual.cantidadCorredores = 1;
            strcpy(ciudadActual.promedioTiempo, reportesCorredor[i].tiempoTotal);
            localidadActual.cantidadCiudades++;
            continue;
        }

        // Actualizar ciudad: promedio ponderado por cantidad de corredores
        ReporteCiudad &ciudadActual = localidadActual.ciudades[posicionCiudad];
        float actualPromedioCiudad = horarioCadenaASegundos(ciudadActual.promedioTiempo);
        float nuevoPromedioCiudad = (actualPromedioCiudad * ciudadActual.cantidadCorredores + tiempoTotalCorredorActual) / (ciudadActual.cantidadCorredores + 1);
        strcpy(ciudadActual.promedioTiempo, horarioSegundosACadena(nuevoPromedioCiudad));
        ciudadActual.cantidadCorredores++;
    }
};

void ordenarCiudadesPorNombre(Localidad &localidad)
{
    for (int i = 0; i < localidad.cantidadCiudades - 1; i++)
    {
        int posicionMenor = i;
        for (int j = i; j < localidad.cantidadCiudades - i - 1; j++)
        {
            if (strcmp(localidad.ciudades[j].nombre, localidad.ciudades[posicionMenor].nombre) < 0)
            {
                posicionMenor = j;
            }
        }
        ReporteCiudad temp = localidad.ciudades[i];
        localidad.ciudades[i] = localidad.ciudades[posicionMenor];
        localidad.ciudades[posicionMenor] = temp;
    }
}

void ordenarLocalidades(Localidad localidades[], int cantidadLocalidades)
{
    for (int i = 0; i < cantidadLocalidades - 1; i++)
    {
        ordenarCiudadesPorNombre(localidades[i]);
        int posicionMenor = i;
        for (int j = i; j < cantidadLocalidades - i - 1; j++)
        {
            if (strcmp(localidades[j].nombre, localidades[posicionMenor].nombre) < 0)
            {
                posicionMenor = j;
            }
        }
        Localidad temp = localidades[i];
        localidades[i] = localidades[posicionMenor];
        localidades[posicionMenor] = temp;
    }
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
