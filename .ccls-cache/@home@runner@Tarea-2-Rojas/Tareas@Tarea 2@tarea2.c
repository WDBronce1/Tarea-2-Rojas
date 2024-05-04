#include "Peliculas.c"
#include "Menu.c"



//////////////////////////////////////////////////////////////////
int main() {
  char opcion;
  Map *pelis_byid = map_create(is_equal_str);
  // Recuerde usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      
      break;
    case '4':
      
      break;
    case '5':
      
      break;
    case '6':
      
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
