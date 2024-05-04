#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "tdas/list.h"


typedef struct Persona{
  char nombre[60];
  int edad;
  char sintomas[150];
}Persona;

typedef struct Paciente{
  Persona persona;
  int Prioridad;
  int HoraLlegada;
}Paciente;

typedef struct Filas{
  List *FilaAlta;
  List *FilaMedia;
  List *FilaBaja;
}Filas;

Filas* create_Filas(void)
{
  Filas* newFila = malloc(sizeof(Filas));
  newFila->FilaAlta = list_create();
  newFila->FilaMedia = list_create();
  newFila->FilaBaja = list_create();
  return newFila;
}

// Funciones ****************************************************************
void registrar_paciente(List *lista_pacientes, Filas *fila)
{
  Paciente *paciente = malloc(sizeof(Paciente));
  getchar();
  printf("Ingrese el nombre del paciente: ");
  fgets(paciente->persona.nombre, sizeof(paciente->persona.nombre), stdin);
  printf("\n");
  printf("Ingrese la edad del paciente: ");
  scanf("%d", &paciente->persona.edad);
  printf("\n");
  getchar();
  printf("Ingrese los sintomas del paciente: ");
  fgets(paciente->persona.sintomas, sizeof(paciente->persona.sintomas), stdin);
  printf("\n");
  printf("Ingrese la hora de registro del paciente: ");
  scanf("%d", &paciente->HoraLlegada);
  printf("\n");
  paciente->Prioridad = 3;
  list_pushBack(lista_pacientes, paciente);
}


void asignar_prioridad(List *lista_pacientes, Filas *fila)
{
    char PacienteBuscado[60];
    int NuevaPrioridad = 0;
    Paciente *PacienteActual = list_first(lista_pacientes);
    printf("Ingrese el nombre del paciente a buscar: ");
    getchar();
    fgets(PacienteBuscado, sizeof(PacienteBuscado), stdin);

    while (PacienteActual != NULL)
    {
        if (strcmp(PacienteActual->persona.nombre, PacienteBuscado) == 0)
        {
            break;  // Si se encuentra el paciente, salir del bucle
        }
        PacienteActual = list_next(lista_pacientes);
    }

    if (PacienteActual == NULL)
    {
        printf("El paciente no existe\n");
        return;
    }

    switch (PacienteActual->Prioridad)
    {
    case 1:
        list_popCurrent(fila->FilaAlta);
        break;
    case 2:
        list_popCurrent(fila->FilaMedia);
        break;
    case 3:
        list_popCurrent(fila->FilaBaja);
        break;
    }

    while (NuevaPrioridad < 1 || NuevaPrioridad > 3)
    {
        printf("Ingrese la prioridad a asignar (Alta 1| Media 2| Baja 3): ");
        scanf("%d", &NuevaPrioridad);
        getchar();
    }

    switch (NuevaPrioridad)
    {
    case 1:
        list_pushBack(fila->FilaAlta, PacienteActual);
        break;
    case 2:
        list_pushBack(fila->FilaMedia, PacienteActual);
        break;
    case 3:
        list_pushBack(fila->FilaBaja, PacienteActual);
        break;
    }

    PacienteActual->Prioridad = NuevaPrioridad;

    printf("Prioridad actualizada con éxito\n");
}

void mostrar_lista_pacientes(List *lista_pacientes, Filas *fila, int pacientes)
{
  // Mostrar la cantidad total de pacientes en espera
  printf("Pacientes en espera: %d\n", pacientes);

  // Recorrer cada fila y mostrar los pacientes en espera
  List *filas[] = {fila->FilaAlta, fila->FilaMedia, fila->FilaBaja};
  char *prioridades[] = {"Alta", "Media", "Baja"};

  for (int i = 0; i < 3; ++i)
  {
    printf("Prioridad: %s-----------------\n", prioridades[i]);
    printf("\n");
    Paciente *PacienteActual = list_first(filas[i]);
    while (PacienteActual != NULL)
    {
      printf("Nombre: %s\n", PacienteActual->persona.nombre);
      printf("    Hora de llegada: %d\n", PacienteActual->HoraLlegada);
      printf("\n");
      PacienteActual = list_next(filas[i]);
    }
  }

  printf("Lista de pacientes que no an sido asignados a ninguna fila:\n");
  Paciente *PacienteActual = list_first(lista_pacientes);
  while (PacienteActual != NULL)
    {
      printf("Nombre: %s\n", PacienteActual->persona.nombre);
      printf("    Hora de llegada: %d\n", PacienteActual->HoraLlegada);
      printf("\n");
      PacienteActual = list_next(lista_pacientes);
    }
}


void atender_paciente(List *lista_pacientes, Filas *fila)
{
  Paciente *PacienteActual = NULL;
  Paciente *Auxiliar = malloc(sizeof(Paciente));
  if (list_first(fila->FilaAlta) != NULL)
  {
    PacienteActual = list_first(fila->FilaAlta);
    Auxiliar = PacienteActual;
    list_popFront(fila->FilaAlta);
  }
  else if (list_first(fila->FilaMedia) != NULL)
  {
    PacienteActual = list_first(fila->FilaMedia);
    Auxiliar = PacienteActual;
    list_popFront(fila->FilaMedia);
  }
  else if (list_first(fila->FilaBaja) != NULL)
  {
    PacienteActual = list_first(fila->FilaBaja);
    Auxiliar = PacienteActual;
    list_popFront(fila->FilaBaja);
  }
  else
  {
    printf("NO HAY PACIENTES EN ESPERA\n");
    return;
  }
  printf("Datos del Paciente atendido:\n");
  printf("Nombre: %s\n", Auxiliar->persona.nombre);
  printf("Edad: %d\n", Auxiliar->persona.edad);
  printf("Sintomas: %s\n", Auxiliar->persona.sintomas);
  printf("Hora de llegada: %d\n", Auxiliar->HoraLlegada);
  if(Auxiliar->Prioridad == 1)
  {
    printf("Prioridad: Alta\n");
  }
  else
  {
    if(Auxiliar->Prioridad == 2)
    {
      printf("Prioridad: Media\n");
    }
    else
    {
      printf("Prioridad: Baja\n");
    }
  }

  list_popCurrent(lista_pacientes);
}

void mostrar_pacientes_por_prioridad(List *lista_pacientes, Filas *fila)
{
  Paciente *PacienteActual = malloc(sizeof(Paciente));
  int PrioridadBuscada;
  while (PrioridadBuscada < 1 || PrioridadBuscada > 3)
  {
      printf("Ingrese la prioridad a buscar (Alta 1| Media 2| Baja 3): ");
      scanf("%d", &PrioridadBuscada);
      getchar();
  }

  if(PrioridadBuscada == 1)
  {
    PacienteActual = list_first(fila->FilaAlta);
    while(PacienteActual != NULL)
    {
      printf("Nombre: %s\n", PacienteActual->persona.nombre);
      printf("                         Hora de llegada : %d", PacienteActual->HoraLlegada);
      printf("\n");
      PacienteActual = list_next(fila->FilaAlta);
    }
  }
  else if(PrioridadBuscada== 2)
  {
    PacienteActual = list_first(fila->FilaMedia);
    while(PacienteActual != NULL)
    {
      printf("Nombre: %s\n", PacienteActual->persona.nombre);
      printf("                         Hora de llegada : %d", PacienteActual->HoraLlegada);
      printf("\n");
      PacienteActual = list_next(fila->FilaMedia);
    }
  }
  else if(PrioridadBuscada == 3)
  {
    PacienteActual = list_first(fila->FilaBaja);
    while(PacienteActual != NULL)
    {
      printf("Nombre: %s\n", PacienteActual->persona.nombre);
      printf("                         Hora de llegada : %d", PacienteActual->HoraLlegada);
      printf("\n");
      PacienteActual = list_next(fila->FilaBaja);
    }
  }
}
//****************************************************************************

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}



int main() {
  char opcion;
  List *pacientes = list_create(); // puedes usar una lista para gestionar los pacientes
  Filas *filas = create_Filas();
  int CantidadPacientes = 0;

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) 
    {
      case '1':
        registrar_paciente(pacientes, filas);
        CantidadPacientes++;
        break;
      case '2':
        // Lógica para asignar prioridad
        asignar_prioridad(pacientes, filas);
        break;
      case '3':
        mostrar_lista_pacientes(pacientes, filas, CantidadPacientes);
        break;
      case '4':
        // Lógica para atender al siguiente paciente
        atender_paciente(pacientes, filas);
        CantidadPacientes--;
        break;
      case '5':
        // Lógica para mostrar pacientes por prioridad
        mostrar_pacientes_por_prioridad(pacientes, filas);
        break;
      case '6':
        puts("Saliendo del sistema de gestión hospitalaria...");
        break;
      default:
        puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos, si es necesario
  list_clean(pacientes);


  return 0;
}
