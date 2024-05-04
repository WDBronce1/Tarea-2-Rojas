#include "../../tdas/list.h"
#include "../../tdas/map.h"
#include "../../data/Top1500.csv"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 100

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[100];
  float rating;
  int year;
} Film;

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
char **leer_linea_csv(FILE *archivo, char separador) {
  static char linea[MAX_LINE_LENGTH];
  static char *campos[MAX_FIELDS];
  char *ptr, *start;
  int idx = 0;

  if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL) {
    return NULL; // No hay más líneas para leer
  }

  ptr = start = linea;
  while (*ptr) {
    if (idx >= MAX_FIELDS - 1)
      break;

    if (*ptr == '\"') { // Inicio de un campo entrecomillado
      start = ++ptr;    // Saltar la comilla inicial
      while (*ptr && !(*ptr == '\"' && *(ptr + 1) == separador))
        ptr++;
    } else { // Campo sin comillas
      start = ptr;
      while (*ptr && *ptr != separador)
        ptr++;
    }

    if (*ptr) {
      *ptr = '\0'; // Reemplazar comilla final o separador por terminación
      ptr++;
      if (*ptr == separador)
        ptr++;
    }

    // Quitar comilla final si existe
    if (*(ptr - 2) == '\"') {
      *(ptr - 2) = '\0';
    }

    campos[idx++] = start;
  }

  campos[idx] = NULL; // Marcar el final del array
  return campos;
}
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//*************************************************************
void cargar_peliculas(Map *pelis_byid) 
{
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos;
  campos = leer_linea_csv(archivo, ','); // encabezados
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) 
  {
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);
    strcpy(peli->title, campos[5]);
    peli->genres = list_create();
    peli->year = atoi(campos[10]);

    map_insert(pelis_byid, peli->id, peli);
  }
  fclose(archivo);

  // se muestran las peliculas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
    pair = map_next(pelis_byid);
  }
}
//******************************************************************
void buscar_por_id(Map *pelis_byid) 
{
  char id[10];
  printf("Ingrese el id de la película: ");
  scanf("%s", id);

  MapPair *pair = map_search(pelis_byid, id);
  if (pair != NULL) 
  {
    Film *peli = pair->value;
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } 
  else
  {
    printf("La película con id %s no existe\n", id);
  }
}
//******************************************************************
void buscar_por_genero(Map *pelis_byid)
{
  char genero[50];
  printf("Ingrese el género que desea -->");
  scanf("%s", genero);

  
  
}

//******************************************************************
void buscar_por_director(Map *pelis_byid, char *director)
{
  
}
//******************************************************************
void buscar_por_decada(Map *pelis_byid, int decada)
{
  
}
//******************************************************************
void buscar_por_calificacion(Map *pelis_byid, float calificacion)
{
  
}
//******************************************************************
void buscar_DecadaGenero(Map *pelis_byid, int decada, char *genero)
{

}