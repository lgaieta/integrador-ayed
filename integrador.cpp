#include <iostream>
#include <cstring>
#include <cstdio>

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
    int cantidadFinalizados;
    char promedioTiempo[11];
};

struct Localidad
{
    char nombre[40];
    int cantidadCorredores;
    int cantidadFinalizados;
    char tiempoPromedio[11];
    int cantidadCiudades;
    ReporteCiudad ciudades[1000];
};

bool esCorredorEnNonstop(RegistroCorredor);
void cargarCorredores(RegistroCorredor registrosCorredoresClasica[],
                      int &cantidadCorredoresClasica,
                      RegistroCorredor registrosCorredoresNonstop[],
                      int &cantidadCorredoresNonstop,
                      char rutaArchivo[]);

bool esDescalificado(RegistroCorredor);
ReporteCorredor inicializarReporteCorredor(RegistroCorredor);
void generarReporte(RegistroCorredor[], ReporteCorredor[], int cantidadCorredores);
void mostrarReporteCorredores(ReporteCorredor[], int cantidadCorredores);
void guardarReporteEnArchivo(ReporteCorredor[], int cantidadCorredores, char nombreArchivo[]);
void guardarPodiosEnArchivo(ReporteCorredor[], int cantidadCorredores, char nombreArchivo[]);
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
void guardarLocalidadesEnArchivo(Localidad localidades[], int cantidadLocalidades, char rutaArchivo[]);

// HH:MM:SS.D
float horarioCadenaASegundos(char cadena[11]);
const char *horarioSegundosACadena(float segundos);

int main()
{
    RegistroCorredor registrosCorredoresClasica[1000];
    int cantidadCorredoresClasica = 0;
    RegistroCorredor registrosCorredoresNonstop[1000];
    int cantidadCorredoresNonstop = 0;
    cargarCorredores(registrosCorredoresClasica,
                     cantidadCorredoresClasica,
                     registrosCorredoresNonstop,
                     cantidadCorredoresNonstop,
                     "C:/programming/utn/integrador/Archivo corredores 4Refugios.bin");

    ReporteCorredor reportesCorredoresClasica[1000];
    ReporteCorredor reportesCorredoresNonstop[1000];

    generarReporte(registrosCorredoresClasica, reportesCorredoresClasica, cantidadCorredoresClasica);
    generarReporte(registrosCorredoresNonstop, reportesCorredoresNonstop, cantidadCorredoresNonstop);

    guardarReporteEnArchivo(reportesCorredoresClasica, cantidadCorredoresClasica, "C:/programming/utn/integrador/ReporteCorredoresClasica.bin");
    guardarReporteEnArchivo(reportesCorredoresNonstop, cantidadCorredoresNonstop, "C:/programming/utn/integrador/ReporteCorredoresNonstop.bin");
    mostrarReporteCorredores(reportesCorredoresClasica, cantidadCorredoresClasica);
    mostrarReporteCorredores(reportesCorredoresNonstop, cantidadCorredoresNonstop);
    guardarPodiosEnArchivo(reportesCorredoresClasica, cantidadCorredoresClasica, "C:/programming/utn/integrador/PodiosClasica.bin");
    guardarPodiosEnArchivo(reportesCorredoresNonstop, cantidadCorredoresNonstop, "C:/programming/utn/integrador/PodiosNonstop.bin");

    CorredorCiudad corredoresCiudad[1000];
    int cantidadCorredoresCiudad = 0;
    cargarCorredoresCiudad(corredoresCiudad, cantidadCorredoresCiudad);

    ReporteCorredor corredoresCarreraTotal[1000];
    unirCorredores(reportesCorredoresClasica,
                   cantidadCorredoresClasica,
                   reportesCorredoresNonstop,
                   cantidadCorredoresNonstop,
                   corredoresCarreraTotal);

    static Localidad localidades[1000];
    int cantidadLocalidades = 0;
    generarReporteLocalidades(localidades,
                              cantidadLocalidades,
                              corredoresCarreraTotal,
                              cantidadCorredoresClasica + cantidadCorredoresNonstop,
                              corredoresCiudad,
                              cantidadCorredoresCiudad);
    ordenarLocalidades(localidades, cantidadLocalidades);
    mostrarLocalidades(localidades, cantidadLocalidades);
    guardarLocalidadesEnArchivo(localidades, cantidadLocalidades, "C:/programming/utn/integrador/ReporteLocalidades.bin");

    return 0;
}

bool esCorredorEnNonstop(RegistroCorredor corredor)
{
    return strncmp(corredor.categoria, "4 Refugios NonStop", 18) == 0;
}

bool esDescalificado(char tiempo[])
{
    if (strncmp(tiempo, "DNF", 3) == 0)
        return true;

    if (strncmp(tiempo, "DNF (NL)", 8) == 0)
        return true;

    if (strncmp(tiempo, "DSQ (FE)", 8) == 0)
        return true;

    if (strncmp(tiempo, "No Termino", 10) == 0)
        return true;

    return false;
}

void cargarCorredores(RegistroCorredor registrosCorredoresClasica[],
                      int &cantidadCorredoresClasica,
                      RegistroCorredor registrosCorredoresNonstop[],
                      int &cantidadCorredoresNonstop,
                      char rutaArchivo[])
{
    RegistroCorredor corredorActual;
    FILE *archivo = fopen(rutaArchivo, "rb");
    if (archivo == NULL)
    {
        cout << "Error al abrir el archivo de corredores." << endl;
        return;
    }

    cantidadCorredoresClasica = 0;
    cantidadCorredoresNonstop = 0;

    while (fread(&corredorActual, sizeof(RegistroCorredor), 1, archivo) == 1)
    {
        if (esCorredorEnNonstop(corredorActual))
        {
            registrosCorredoresNonstop[cantidadCorredoresNonstop] = corredorActual;
            cantidadCorredoresNonstop++;
        }
        else
        {
            registrosCorredoresClasica[cantidadCorredoresClasica] = corredorActual;
            cantidadCorredoresClasica++;
        }
    }

    std::fclose(archivo);
}

ReporteCorredor inicializarReporteCorredor(RegistroCorredor registro)
{
    ReporteCorredor reporte;
    reporte.numero = registro.numero;
    std::strcpy(reporte.nombreApellido, registro.nombreApellido);
    std::strcpy(reporte.categoria, registro.categoria);
    reporte.genero = registro.genero;
    std::strcpy(reporte.localidad, registro.localidad);

    if (esDescalificado(registro.llegada))
    {
        std::strcpy(reporte.tiempoTotal, "No Termino");
    }
    else
    {
        std::strcpy(reporte.tiempoTotal, registro.llegada);
    }

    // Inicializar diferencias y posiciones (se completan luego en generarReporte)
    std::strcpy(reporte.diferenciaPrimero, "00:00:00,0");
    std::strcpy(reporte.diferenciaAnterior, "00:00:00,0");
    reporte.posicionGeneral = 0;
    reporte.posicionGenero = 0;
    reporte.posicionCategoria = 0;
    return reporte;
}

void generarReporte(RegistroCorredor registrosCorredores[], ReporteCorredor reportesCorredores[], int cantidadCorredores)
{
    ContadorCategoria contadoresCategoria[1000];
    int cantidadCategorias = 0;
    ContadorGenero contadoresGenero[1000];
    int cantidadGeneros = 0;

    for (int i = 0; i < cantidadCorredores; i++)
    {
        reportesCorredores[i] = inicializarReporteCorredor(registrosCorredores[i]);
    }

    for (int i = 0; i < cantidadCorredores; i++)
    {
        // Obtener el mayor actual por tiempo total
        int posicionMenorActual = i;
        for (int j = i + 1; j < cantidadCorredores; j++)
        {
            bool jDNF = strcmp(reportesCorredores[j].tiempoTotal, "No Termino") == 0;
            bool menorActualDNF = strcmp(reportesCorredores[posicionMenorActual].tiempoTotal, "No Termino") == 0;
            if (!jDNF && menorActualDNF)
            {
                posicionMenorActual = j;
            }
            else if (!jDNF && !menorActualDNF &&
                     horarioCadenaASegundos(reportesCorredores[j].tiempoTotal) < horarioCadenaASegundos(reportesCorredores[posicionMenorActual].tiempoTotal))
            {
                posicionMenorActual = j;
            }
        }

        // Asignar posicion general
        reportesCorredores[posicionMenorActual].posicionGeneral = i + 1;

        // Asignar posicion por categoria
        bool categoriaEncontrada = false;
        for (int k = 0; k < cantidadCategorias; k++)
        {
            if (strcmp(reportesCorredores[posicionMenorActual].categoria, contadoresCategoria[k].categoria) == 0)
            {
                contadoresCategoria[k].cantidad++;
                reportesCorredores[posicionMenorActual].posicionCategoria = contadoresCategoria[k].cantidad;
                categoriaEncontrada = true;
                break;
            }
        }
        if (categoriaEncontrada == false)
        {
            std::strcpy(contadoresCategoria[cantidadCategorias].categoria, reportesCorredores[posicionMenorActual].categoria);
            contadoresCategoria[cantidadCategorias].cantidad = 1;
            reportesCorredores[posicionMenorActual].posicionCategoria = 1;
            cantidadCategorias++;
        }

        // Asignar posicion por genero
        bool generoEncontrado = false;
        for (int k = 0; k < cantidadGeneros; k++)
        {
            if (reportesCorredores[posicionMenorActual].genero == contadoresGenero[k].genero)
            {
                contadoresGenero[k].cantidad++;
                reportesCorredores[posicionMenorActual].posicionGenero = contadoresGenero[k].cantidad;
                generoEncontrado = true;
                break;
            }
        }
        if (generoEncontrado == false)
        {
            contadoresGenero[cantidadGeneros].genero = reportesCorredores[posicionMenorActual].genero;
            contadoresGenero[cantidadGeneros].cantidad = 1;
            reportesCorredores[posicionMenorActual].posicionGenero = 1;
            cantidadGeneros++;
        }

        // Intercambio de posiciones para ordenar por tiempo total
        ReporteCorredor aux = reportesCorredores[posicionMenorActual];
        reportesCorredores[posicionMenorActual] = reportesCorredores[i];
        reportesCorredores[i] = aux;
    }

    // Cargar campos de diferenciaPrimero y diferenciaAnterior
    float tiempoPrimero = horarioCadenaASegundos(reportesCorredores[0].tiempoTotal);
    for (int i = 1; i < cantidadCorredores; i++)
    {
        bool actualDNF = strcmp(reportesCorredores[i].tiempoTotal, "No Termino") == 0;
        if (!actualDNF)
        {
            float tiempoActual = horarioCadenaASegundos(reportesCorredores[i].tiempoTotal);
            std::strcpy(reportesCorredores[i].diferenciaPrimero, horarioSegundosACadena(tiempoActual - tiempoPrimero));
            float tiempoAnterior = horarioCadenaASegundos(reportesCorredores[i - 1].tiempoTotal);
            std::strcpy(reportesCorredores[i].diferenciaAnterior, horarioSegundosACadena(tiempoActual - tiempoAnterior));
        }
        else
        {
            std::strcpy(reportesCorredores[i].diferenciaAnterior, "No Termino");
            std::strcpy(reportesCorredores[i].diferenciaPrimero, "No Termino");
        }
    }
}

void guardarReporteEnArchivo(ReporteCorredor corredores[], int cantidadCorredores, char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (!archivo)
    {
        cout << "Error al abrir el archivo de salida " << nombreArchivo << endl;
        return;
    }
    fwrite(corredores, sizeof(ReporteCorredor), cantidadCorredores, archivo);
    fclose(archivo);
}

void guardarPodiosEnArchivo(ReporteCorredor corredores[], int cantidadCorredores, char nombreArchivo[])
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
            std::strcpy(podios[cantidadPodios].categoria, corredores[i].categoria);
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
            if (strcmp(podios[j].categoria, podios[posicionMenorActual].categoria) < 0)
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

    // Mostrar por consola los podios por categoria
    printf("\nPodios por categoria (%s)\n", nombreArchivo);
    printf("%-50s %-6s %-6s %-30s %-12s\n",
           "Categoria", "Puesto", "Numero", "Nombre y Apellido", "Tiempo");
    for (int k = 0; k < 106; ++k)
        putchar('-');
    putchar('\n');

    for (int i = 0; i < indiceReportes; ++i)
    {
        // Asegurar terminación nula por seguridad
        reportesPodios[i].categoria[49] = '\0';
        reportesPodios[i].nombreApellido[49] = '\0';
        reportesPodios[i].tiempoTotal[10] = '\0';

        printf("%-50.50s %-6d %-6d %-30.30s %-12.12s\n",
               reportesPodios[i].categoria,
               reportesPodios[i].posicionCategoria,
               reportesPodios[i].numero,
               reportesPodios[i].nombreApellido,
               reportesPodios[i].tiempoTotal);
    }
    for (int k = 0; k < 106; ++k)
        putchar('-');
    putchar('\n');
    printf("Total de entradas de podio: %d\n\n", indiceReportes);

    FILE *archivoPodios = fopen(nombreArchivo, "wb");
    if (!archivoPodios)
    {
        cout << "Error al abrir el archivo " << nombreArchivo << endl;
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
    cantidadCorredoresCiudad = i;
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
            continue;

        // Busco si ya hay una localidad guardada que coincida con el corredor i
        int posicionLocalidad = -1;
        for (int j = 0; j < cantidadLocalidades; j++)
        {
            if (strncmp(reportesCorredor[i].localidad, localidades[j].nombre, 40) == 0)
            {
                posicionLocalidad = j;
                break;
            }
        }

        bool esCorredorDNF = esDescalificado(reportesCorredor[i].tiempoTotal);

        // Si no hay localidad guardada, la creo
        if (posicionLocalidad == -1)
        {
            Localidad &localidadActual = localidades[cantidadLocalidades];
            std::strcpy(localidadActual.nombre, reportesCorredor[i].localidad);
            localidadActual.cantidadCorredores = 1;
            std::strcpy(localidadActual.ciudades[0].nombre, corredoresCiudad[posicionCorredorCiudad].ciudad);
            localidadActual.ciudades[0].cantidadCorredores = 1;
            localidadActual.ciudades[0].cantidadFinalizados = esCorredorDNF ? 0 : 1;
            std::strcpy(localidadActual.ciudades[0].promedioTiempo, esCorredorDNF ? "00:00:00" : reportesCorredor[i].tiempoTotal);

            localidadActual.cantidadCiudades = 1;
            localidadActual.cantidadFinalizados = esCorredorDNF ? 0 : 1;
            std::strcpy(localidadActual.tiempoPromedio, esCorredorDNF ? "00:00:00" : reportesCorredor[i].tiempoTotal);

            cantidadLocalidades++;
            continue;
        }

        Localidad &localidadActual = localidades[posicionLocalidad];
        localidadActual.cantidadCorredores++;

        // Busco si ya hay una ciudad guardada en la localidad que coincida con el corredor i
        int posicionCiudad = -1;
        for (int j = 0; j < localidadActual.cantidadCiudades; j++)
        {
            if (strncmp(localidadActual.ciudades[j].nombre, corredoresCiudad[posicionCorredorCiudad].ciudad, 11) == 0)
            {
                posicionCiudad = j;
                break;
            }
        }

        // Si no hay ciudad guardada, la creo
        if (posicionCiudad == -1)
        {
            ReporteCiudad &ciudadActual = localidadActual.ciudades[localidadActual.cantidadCiudades];
            std::strcpy(ciudadActual.nombre, corredoresCiudad[posicionCorredorCiudad].ciudad);
            ciudadActual.cantidadCorredores = 1;
            ciudadActual.cantidadFinalizados = esCorredorDNF ? 0 : 1;
            std::strcpy(ciudadActual.promedioTiempo, esCorredorDNF ? "00:00:00" : reportesCorredor[i].tiempoTotal);
            localidadActual.cantidadCiudades++;
            continue;
        }

        ReporteCiudad &ciudadActual = localidadActual.ciudades[posicionCiudad];
        ciudadActual.cantidadCorredores++;

        if (esCorredorDNF)
            continue;

        // Actualizar localidad: promedio ponderado por cantidad de corredores
        float tiempoTotalCorredorActual = horarioCadenaASegundos(reportesCorredor[i].tiempoTotal);
        float actualPromedioLocalidad = horarioCadenaASegundos(localidadActual.tiempoPromedio);
        float nuevoPromedioLocalidad = (actualPromedioLocalidad * localidadActual.cantidadFinalizados + tiempoTotalCorredorActual) / (localidadActual.cantidadFinalizados + 1);
        std::strcpy(localidadActual.tiempoPromedio, horarioSegundosACadena(nuevoPromedioLocalidad));
        localidadActual.cantidadFinalizados++;

        // Actualizar ciudad: promedio ponderado por cantidad de corredores
        float actualPromedioCiudad = horarioCadenaASegundos(ciudadActual.promedioTiempo);
        float nuevoPromedioCiudad = (actualPromedioCiudad * ciudadActual.cantidadFinalizados + tiempoTotalCorredorActual) / (ciudadActual.cantidadFinalizados + 1);
        std::strcpy(ciudadActual.promedioTiempo, horarioSegundosACadena(nuevoPromedioCiudad));
        ciudadActual.cantidadFinalizados++;
    }
};

void ordenarCiudadesPorNombre(Localidad &localidad)
{
    // Selection sort ascendente por nombre de ciudad
    for (int i = 0; i < localidad.cantidadCiudades - 1; i++)
    {
        int posicionMenor = i;
        for (int j = i + 1; j < localidad.cantidadCiudades; j++)
        {
            if (strncmp(localidad.ciudades[j].nombre, localidad.ciudades[posicionMenor].nombre, 11) < 0)
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
    // Selection sort ascendente por nombre de localidad
    for (int i = 0; i < cantidadLocalidades - 1; i++)
    {
        ordenarCiudadesPorNombre(localidades[i]);
        int posicionMenor = i;
        for (int j = i + 1; j < cantidadLocalidades; j++)
        {
            if (strncmp(localidades[j].nombre, localidades[posicionMenor].nombre, 40) < 0)
            {
                posicionMenor = j;
            }
        }
        Localidad temp = localidades[i];
        localidades[i] = localidades[posicionMenor];
        localidades[posicionMenor] = temp;
    }
}

void mostrarReporteCorredores(ReporteCorredor corredores[], int cantidadCorredores)
{
    // Encabezado de la tabla
    printf("%-8s %-10s %-8s %-6s %-30s %-30s %-8s %-20s %-12s %-12s %-12s\n",
           "PosGen", "PosGenero", "PosCat", "Numero",
           "Nombre y Apellido", "Categoria", "Genero",
           "Localidad", "Tiempo", "DifPrimero", "DifAnterior");
    for (int k = 0; k < 160; ++k)
        putchar('-');
    putchar('\n');

    // Filas de datos
    for (int i = 0; i < cantidadCorredores; ++i)
    {
        printf("%-8d %-10d %-8d %-6d %-30.30s %-30.30s %-8c %-20.20s %-12.12s %-12.12s %-12.12s\n",
               corredores[i].posicionGeneral,
               corredores[i].posicionGenero,
               corredores[i].posicionCategoria,
               corredores[i].numero,
               corredores[i].nombreApellido,
               corredores[i].categoria,
               corredores[i].genero,
               corredores[i].localidad,
               corredores[i].tiempoTotal,
               corredores[i].diferenciaPrimero,
               corredores[i].diferenciaAnterior);
    }
}

void mostrarLocalidades(Localidad localidades[], int cantidadLocalidades)
{
    // Encabezado tabla de localidades
    printf("%-32s %-12s %-12s\n", "Localidad", "Promedio", "Corredores");
    for (int k = 0; k < 56; ++k)
        putchar('-');
    putchar('\n');

    for (int i = 0; i < cantidadLocalidades; i++)
    {
        printf("%-32s %-12s %-12d\n",
               localidades[i].nombre,
               localidades[i].tiempoPromedio,
               localidades[i].cantidadCorredores);

        // Encabezado subtabla de ciudades
        printf("  %-14s %-12s %-12s\n", "Ciudad", "Promedio", "Corredores");
        printf("  ");
        for (int k = 0; k < 42; ++k)
            putchar('-');
        putchar('\n');

        for (int j = 0; j < localidades[i].cantidadCiudades; j++)
        {
            printf("  %-14s %-12s %-12d\n",
                   localidades[i].ciudades[j].nombre,
                   localidades[i].ciudades[j].promedioTiempo,
                   localidades[i].ciudades[j].cantidadCorredores);
        }

        printf("\n");
    }
}

void guardarLocalidadesEnArchivo(Localidad localidades[], int cantidadLocalidades, char rutaArchivo[])
{
    FILE *archivo = fopen(rutaArchivo, "wb");
    if (!archivo)
    {
        cout << "Error al abrir el archivo para escritura: " << rutaArchivo << endl;
        return;
    }

    fwrite(localidades, sizeof(Localidad), cantidadLocalidades, archivo);

    fclose(archivo);
}

// HH:MM:SS.D
float horarioCadenaASegundos(char cadena[11])
{
    if (strcmp(cadena, "No Termino") == 0)
        return 0;

    float acumulador = 0;
    acumulador += ((cadena[0] - 48) * 10 + (cadena[1] - 48)) * 3600;
    acumulador += ((cadena[3] - 48) * 10 + (cadena[4] - 48)) * 60;
    acumulador += ((cadena[6] - 48) * 10 + (cadena[7] - 48));
    acumulador += (cadena[9] - 48) * 0.1;

    return acumulador;
}

const char *horarioSegundosACadena(float segundos)
{
    if (segundos == 0)
        return "No Termino";

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
    hours[8] = '.';
    hours[9] = ((int)(segs * 10) % 10) + 48;
    hours[10] = '\0';
    return hours;
}