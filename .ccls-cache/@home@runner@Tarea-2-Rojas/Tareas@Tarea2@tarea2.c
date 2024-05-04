#include "Menu.c"
#include "Peliculas.c"



//////////////////////////////////////////////////////////////////
int main() {
  char opcion;
  Map *pelis_byid = map_create(is_equal_str);
  Map *MapDire;
  Map *MapGen;
  Map *MapDeca;
  Map *MapCali;
  // Recuerde usar un mapa por criterio de búsqueda

  do {
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
      buscar_por_director(pelis_byid);
      break;
    case '4':
      buscar_por_genero(MapGen);
      break;
    case '5':
      buscar_por_decada(pelis_byid);
      break;
    case '6':
      buscar_por_calificacion(pelis_byid);
      break;
    case '7':
      
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
