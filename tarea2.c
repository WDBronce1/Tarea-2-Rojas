#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[300];
  float rating;
  int year;
} Film;

struct Node{
  void *data;
  struct Node *next;
};

typedef struct Node Node;

//******************************************************************
void FuncionAux(int aux)
{
  printf("\n");
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) 
{
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}
//***********************************************
List *split_string(const char *str, const char *delim) 
{
  List *result = list_create();
  char *token = strtok((char *)str, delim);

  while (token != NULL) 
  {
    // Eliminar espacios en blanco al inicio del token
    while (*token == ' ') 
    {
      token++;
    }

    // Eliminar espacios en blanco al final del token
    char *end = token + strlen(token) - 1;
    while (*end == ' ' && end > token) 
    {
      *end = '\0';
      end--;
    }

    // Copiar el token en un nuevo string
    char *new_token = strdup(token);

    // Agregar el nuevo string a la lista
    list_pushBack(result, new_token);

    // Obtener el siguiente token
    token = strtok(NULL, delim);
  }

  return result;
}

 void cargar_peliculas(Map *pelis_byid, Map *MapDire, Map *MapGen, Map *MapDeca, Map *MapCali) 
{

  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); 
    return;
  }
  char **campos;

  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV
  char *Aux;
  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) 
  {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));

    strcpy(peli->id, campos[1]);        
    strcpy(peli->title, campos[5]);     
    strcpy(peli->director, campos[14]); 
    peli->genres = split_string(campos[11], ",");

    peli->rating = atof(campos[8]);
    peli->year = atoi(campos[10]); 

    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);

    map_insert(MapDire, peli->title, peli->director);
    map_insert(MapGen, peli->title, peli->genres);
    map_insert(MapDeca, peli->title, (void*)&peli->year);
    map_insert(MapCali, peli->title, (void*)&peli->rating);

  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas

  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) 
  {
    Film *peli = pair->value;
    List *Generos = peli->genres;

    printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
    printf("Director: %s, Rating: %.1lf\n",peli->director, peli->rating);
    printf("Géneros: ");

    char *Genero = list_first(Generos);
    while(Genero != NULL)
    {
        printf("%s ", Genero);
        Genero = list_next(Generos);
    }


    printf("\n ---------------------------------\n");
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
void buscar_por_director(Map *MapDire) 
{
  char Dire[50];
  printf("Ingrese al director que desea buscar --> ");
  getchar(); // Consumir el salto de línea pendiente
  fgets(Dire, sizeof(Dire), stdin);
  Dire[strcspn(Dire, "\n")] = '\0'; // Eliminar el salto de línea

  MapPair *Director = map_first(MapDire);

  while (Director != NULL) 
  {
    if (strcmp(Dire, (char*)Director->value) == 0) 
    {
      printf("Nombre: %s\n", (char*)Director->key);
    }
    Director = map_next(MapDire);
  }
}
//******************************************************************
void buscar_por_genero(Map *MapGeneros) 
{
  char genero[50];
  printf("Ingrese el género que desea buscar --> ");
  scanf("%s", genero);

  MapPair *Pelicula = map_first(MapGeneros);

  while (Pelicula != NULL) 
  {
    List *generos = (List *)Pelicula->value;
    char *genNodo = list_first(generos);

    while (genNodo != NULL) 
    {
      if (strcmp(genero, genNodo) == 0) 
      {
        printf("Nombre: %s\n", (char *)Pelicula->key);
        break; 
      }
      genNodo = list_next(generos);
    }
    Pelicula = map_next(MapGeneros);
  }
}
//******************************************************************
void buscar_por_decada(Map *MapDecada)
{
  int Decada;
  int aux;
  MapPair *Pelicula = map_first(MapDecada);

  do
  {
    limpiarPantalla();
    printf("Elija una decada\n");
    printf("1) 1940'\n");
    printf("2) 1950'\n");
    printf("3) 1960'\n");
    printf("4) 1970'\n");
    printf("5) 1980'\n");
    printf("6) 1990'\n");
    printf("7) 2000'\n");
    printf("8) 2010'\n");
    printf("9) 2020'\n");
    printf("Ingrese una opción --> ");
    scanf("%d",&Decada);

    switch (Decada) 
      {
        case 1:
          while(Pelicula != NULL)
            {
              aux = *(int*)Pelicula->value;
              if(aux >= 1940 && aux <= 1949)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapDecada);
            }
          break;
        case 2:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 1950 && aux <= 1959)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }

        case 3:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 1960 && aux <= 1969)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }
          FuncionAux(Decada);
          break;
        case 4:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 1970 && aux <= 1979)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }
          break;
        case 5:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 1980 && aux <= 1989)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }

        case 6:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 1990 && aux <= 1999)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }

        case 7:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 2000 && aux <= 2009)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }

        case 8:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 2010 && aux <= 2019)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }
          FuncionAux(Decada);
          break;
        case 9:
          while(Pelicula != NULL)
          {
            aux = *(int*)Pelicula->value;
            if(aux >= 2020 && aux <= 2025)
            {
              printf("Nombre: %s\n", (char*)Pelicula->key);
            }
            Pelicula = map_next(MapDecada);
          }
          break;
        default:
          printf("Ingrese una opcion valida");
      }
  }while(Decada < 1 || Decada > 9);

}


//******************************************************************
void buscar_por_calificacion(Map *MapCalificacion)
{

  float Calificacion;
  int opcion;
  MapPair *Pelicula = map_first(MapCalificacion);

  do
    {
      limpiarPantalla();
      printf("Elija un rango de calificaciones\n");
      printf("1)  0.0 - 0.9\n");
      printf("2)  1.0 - 2.0\n");
      printf("3)  2.1 - 3.0\n");
      printf("4)  3.1 - 4.0\n");
      printf("5)  4.1 - 5.0\n");
      printf("6)  5.1 - 6.0\n");
      printf("7)  6.1 - 7.0\n");
      printf("8)  7.1 - 8.0\n");
      printf("9)  8.1 - 9.0\n");
      printf("10) 9.1 - 10.0\n");
      printf("Ingrese una opción --> ");
      scanf("%d",&opcion);
      switch (opcion) 
        {
          case 1:
            while(Pelicula != NULL)
            {
              if ( *(float*)Pelicula->value >= 0.0 && *(float*)Pelicula->value <= 0.9)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }
            break;
          case 2:
            while(Pelicula != NULL)
              {
                Calificacion = *(float*)Pelicula->value;
                if (Calificacion >= 1.0 && Calificacion <= 2.0)
                {
                  printf("Nombre: %s\n", (char*)Pelicula->key);
                }
                Pelicula = map_next(MapCalificacion);
              }

            break;
          case 3:
            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 2.1 && Calificacion <= 3.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }

            break;
          case 4:
            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 3.1 && Calificacion <= 4.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }
            break;
          case 5:

            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 4.1 && Calificacion <= 5.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }
            break;
          case 6:
            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 5.1 && Calificacion <= 6.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }
            break;
          case 7:
            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 6.1 && Calificacion <= 7.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }

            break;
          case 8:
            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 7.1 && Calificacion <= 8.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }

            break;
          case 9:
            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 8.1 && Calificacion <= 9.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }
            break;
          case 10:
            while(Pelicula != NULL)
            {
              Calificacion = *(float*)Pelicula->value;
              if (Calificacion >= 9.1 && Calificacion <= 10.0)
              {
                printf("Nombre: %s\n", (char*)Pelicula->key);
              }
              Pelicula = map_next(MapCalificacion);
            }
            break;
          default:
            printf("Ingrese una opcion valida");
        }
    }while(opcion < 1 || opcion > 10);

}
//******************************************************************

void buscar_DecadaGenero(Map *MapDecada, Map *MapGenero)
{
  int Decada;
  char Genero[50];
  int aux;


  do
    {
    limpiarPantalla();
      printf("Elija una decada\n");
      printf("1) 1940'\n");
      printf("2) 1950'\n");
      printf("3) 1960'\n");
      printf("4) 1970'\n");
      printf("5) 1980'\n");
      printf("6) 1990'\n");
      printf("7) 2000'\n");
      printf("8) 2010'\n");
      printf("9) 2020'\n");
      printf("Ingrese una opción --> ");
      scanf("%d",&Decada);
      printf("Ingrese un genero --> ");
      getchar();
      fgets(Genero,sizeof(Genero),stdin);
      MapPair *PeliculaDecada = map_first(MapDecada);
      MapPair *PeliculaGenero = map_first(MapGenero);

      switch (Decada) 
        {
          case 1:
            while(PeliculaDecada != NULL)
              {
                aux = *(int*)PeliculaDecada->value;
                if(aux >= 1940 && aux <= 1949)
                {
                  List *generos = (List *)PeliculaGenero->value;
                  char *genNodo = list_first(generos);
                  while (genNodo != NULL) 
                  {
                    if (strcmp(Genero, genNodo) == 0) 
                    {
                      printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                      break;
                    }
                    genNodo = list_next(generos);
                  }
                }
                PeliculaDecada = map_next(MapDecada);
              }
            break;
          case 2:
            while(PeliculaDecada != NULL)
            {
              aux = *(int*)PeliculaDecada->value;
              if(aux >= 1950 && aux <= 1959)
              {
                List *generos = (List *)PeliculaGenero->value;
                char *genNodo = list_first(generos);
                while (genNodo != NULL) 
                {
                  if (strcmp(Genero, genNodo) == 0) 
                  {
                    printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                    break;
                  }
                  genNodo = list_next(generos);
                }
              }
              PeliculaDecada = map_next(MapDecada);
            }
            break;
          case 3:
            while(PeliculaDecada != NULL)
            {
              aux = *(int*)PeliculaDecada->value;
              if(aux >= 1960 && aux <= 1969)
              {
                List *generos = (List *)PeliculaGenero->value;
                char *genNodo = list_first(generos);
                while (genNodo != NULL) 
                {
                  if (strcmp(Genero, genNodo) == 0) 
                  {
                    printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                    break;
                  }
                  genNodo = list_next(generos);
                }
              }
              PeliculaDecada = map_next(MapDecada);
            }
            break;
          case 4:
          while(PeliculaDecada != NULL)
          {
            aux = *(int*)PeliculaDecada->value;
            if(aux >= 1970 && aux <= 1979)
            {
              List *generos = (List *)PeliculaGenero->value;
              char *genNodo = list_first(generos);
              while (genNodo != NULL) 
              {
                if (strcmp(Genero, genNodo) == 0) 
                {
                  printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                  break;
                }
                genNodo = list_next(generos);
              }
            }
            PeliculaDecada = map_next(MapDecada);
          }
          break;
          case 5:
            while(PeliculaDecada != NULL)
              {
                aux = *(int*)PeliculaDecada->value;
                if(aux >= 1980 && aux <= 1989)
                {
                  List *generos = (List *)PeliculaGenero->value;
                  char *genNodo = list_first(generos);
                  while (genNodo != NULL) 
                  {
                    if (strcmp(Genero, genNodo) == 0) 
                    {
                      printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                      break;
                    }
                    genNodo = list_next(generos);
                  }
                }
                PeliculaDecada = map_next(MapDecada);
              }
              break;
          case 6:
            while(PeliculaDecada != NULL)
              {
                aux = *(int*)PeliculaDecada->value;
                if(aux >= 1990 && aux <= 1999)
                {
                  List *generos = (List *)PeliculaGenero->value;
                  char *genNodo = list_first(generos);
                  while (genNodo != NULL) 
                  {
                    if (strcmp(Genero, genNodo) == 0) 
                    {
                      printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                      break;
                    }
                    genNodo = list_next(generos);
                  }
                }
                PeliculaDecada = map_next(MapDecada);
              }
              break;
          case 7:
            while(PeliculaDecada != NULL)
              {
                aux = *(int*)PeliculaDecada->value;
                if(aux >= 2000 && aux <= 2009)
                {
                  List *generos = (List *)PeliculaGenero->value;
                  char *genNodo = list_first(generos);
                  while (genNodo != NULL) 
                  {
                    if (strcmp(Genero, genNodo) == 0) 
                    {
                      printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                      break;
                    }
                    genNodo = list_next(generos);
                  }
                }
                PeliculaDecada = map_next(MapDecada);
              }
              break;
          case 8:
            while(PeliculaDecada != NULL)
              {
                aux = *(int*)PeliculaDecada->value;
                if(aux >= 2010 && aux <= 2019)
                {
                  List *generos = (List *)PeliculaGenero->value;
                  char *genNodo = list_first(generos);
                  while (genNodo != NULL) 
                  {
                    if (strcmp(Genero, genNodo) == 0) 
                    {
                      printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                      break;
                    }
                    genNodo = list_next(generos);
                  }
                }
                PeliculaDecada = map_next(MapDecada);
              }
              break;
          case 9:
            while(PeliculaDecada != NULL)
              {
                aux = *(int*)PeliculaDecada->value;
                if(aux >= 2020 && aux <= 2029)
                {
                  List *generos = (List *)PeliculaGenero->value;
                  char *genNodo = list_first(generos);
                  while (genNodo != NULL) 
                  {
                    if (strcmp(Genero, genNodo) == 0) 
                    {
                      printf("Nombre: %s\n", (char *)PeliculaGenero->key);
                      break;
                    }
                    genNodo = list_next(generos);
                  }
                }
                PeliculaDecada = map_next(MapDecada);
              }
              break;
          default:
            printf("Ingrese una opcion valida");

        }
    }while(Decada < 1 || Decada > 9);
}
////////////////////////////////////////////////////////////

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *MapDire = map_create(is_equal_str);
  Map *MapGen = map_create(is_equal_str);
  Map *MapDeca = map_create(is_equal_str);
  Map *MapCali = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda

  do 
  {
  mostrarMenuPrincipal();
  printf("Ingrese su opción: ");
  scanf(" %c", &opcion);

  switch (opcion) {
  case '1':
    cargar_peliculas(pelis_byid, MapDire, MapGen, MapDeca, MapCali);
    break;
  case '2':
    buscar_por_id(pelis_byid);
    break;
  case '3':
    buscar_por_director(MapDire);
    break;
  case '4':
    buscar_por_genero(MapGen);
    break;
  case '5':
    buscar_por_decada(MapDeca);
    break;
  case '6':
    buscar_por_calificacion(MapCali);
    break;
  case '7':
    buscar_DecadaGenero(MapDeca, MapGen);
    break;
  default:
  }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
