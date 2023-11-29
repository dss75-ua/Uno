#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    ROJO,
    AMARILLO,
    VERDE,
    AZUL,
    COMODIN
} Color;

typedef enum {
    CERO,
    UNO,
    DOS,
    TRES,
    CUATRO,
    CINCO,
    SEIS,
    SIETE,
    OCHO,
    NUEVE,
    SALTA,
    REVERSA,
    TOMA_DOS,
    COMODIN_CARTA,
    COMODIN_TOMA_CUATRO
} Valor;

typedef struct {
    Color color;
    Valor valor;
} Carta;

const char* colorToString(Color color) {
    switch (color) {
        case ROJO: return "Rojo";
        case AMARILLO: return "Amarillo";
        case VERDE: return "Verde";
        case AZUL: return "Azul";
        case COMODIN: return "Comodin";
        default: return "Uknown";
    }
}

const char* valorToString(Valor valor) {
    switch (valor) {
        case CERO: return "0";
        case UNO: return "1";
        case DOS: return "2";
        case TRES: return "3";
        case CUATRO: return "4";
        case CINCO: return "5";
        case SEIS: return "6";
        case SIETE: return "7";
        case OCHO: return "8";
        case NUEVE: return "9";
        case SALTA: return "Salta";
        case REVERSA: return "Reversa";
        case TOMA_DOS: return "Toma Dos";
        case COMODIN_CARTA: return "Comodin";
        case COMODIN_TOMA_CUATRO: return "Comodin toma cuatro";
        default: return "Unknown";
    }
}

void generarMano(Carta mano[], int cantidad) {
    srand(time(NULL));

    for (int i = 0; i < cantidad; ++i) {
        mano[i].color = (Color)(rand() % 4);
        mano[i].valor = (Valor)(rand() % 15);
    }
}

void mostrarMano(Carta mano[], int cantidad) {
    printf("Tu mano:\n");

    for (int i = 0; i < cantidad; ++i) {
        printf("[%s %s] ", colorToString(mano[i].color), valorToString(mano[i].valor));
    }
}

int cuantosPersonas() {
    int numeroDePersonas;
    int leidos;

    do {
        printf("Pon el número de personas con las que quieres jugar (máximo 4): ");
        leidos = scanf("%d", &numeroDePersonas);

        if (leidos != 1 || numeroDePersonas < 1 || numeroDePersonas > 4) {
            printf("Entrada no válida. Debe ser un número entre 1 y 4.\n");
            while (getchar() != '\n');
        }
    } while (leidos != 1 || numeroDePersonas < 1 || numeroDePersonas > 4);

    return numeroDePersonas;
}

void jugarContraComputadora() {    
    int maxHand = 7;
    int maxCards = 108;

    printf("Ejecutando función para jugar contra la computadora...\n");
    int numPersonas = cuantosPersonas();
    printf("Num de personas: %d\n", numPersonas);

    Carta mano[maxHand];
    generarMano(mano, maxHand);

    mostrarMano(mano, maxHand);
}


void jugarContraAmigos() {
    printf("Ejecutando función para jugar contra amigos...\n");
}

void salirDelJuego() {
    printf("Saliendo del juego...\n");
}

void menu(int *eleccion) {
    printf("******************************\n");
    printf("      MENÚ DEL JUEGO\n");
    printf("******************************\n\n");
    printf("1. Jugar contra la computadora\n2. Jugar contra amigos\n3. Salir\n\n");

    do {
        printf("Ingresa el número de tu elección: ");
        scanf("%d", eleccion);
    } while (*eleccion < 1 || *eleccion > 3);
}

int main() {
    int eleccion;
    
    menu(&eleccion);

    switch (eleccion) {
        case 1:
            jugarContraComputadora();
            break;
        case 2:
            jugarContraAmigos();
            break;
        case 3:
            salirDelJuego();
            break;
        default:
            printf("Opción no válida.\n");
    }

    return 0;
}
