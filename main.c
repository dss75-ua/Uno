#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HAND_SIZE 7 // Tamaño de la mano
#define MAX_NAME_LENGTH 20 // Longitud máxima del nombre
#define MAX_CARDS 20 // Número máximo de cartas

typedef enum {
    ROJO, VERDE, AZUL, AMARILLO // Colores de las cartas
} Color;

typedef struct {
    Color color; // Color de la carta
    int value; // Valor de la carta
} Card;

typedef struct {
    char name[MAX_NAME_LENGTH]; // Nombre del jugador
    Card hand[MAX_CARDS]; // Mano del jugador
    int handSize; // Tamaño de la mano del jugador
} Player;

typedef struct {
    Card cards[MAX_CARDS]; // Cartas en la pila
    int top; // Carta superior de la pila
} CardPile;

Card getRandomCard() {
    Card randomCard;
    randomCard.color = rand() % 4; // Selecciona un color aleatorio
    randomCard.value = rand() % 10; // Selecciona un valor aleatorio
    return randomCard;
}

int customStringCompare(const char *str1, const char *str2) {
    // Compara dos cadenas de caracteres
    while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }

    if (*str1 == '\0' && *str2 == '\0') {
        return 0; // Las cadenas son iguales
    } else {
        return 1; // Las cadenas son diferentes
    }
}

void copyString(char dest[], const char src[]) {
    // Copia una cadena en otra
    int i = 0;
    while (src[i] != '\0' && i < MAX_NAME_LENGTH - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

Card drawInitialCard() {
    // Roba una carta inicial
    printf("¡Se ha robado una carta adicional: ");
    Card newCard = getRandomCard();
    switch (newCard.color) {
        case ROJO:
            printf("\033[31mRojo\033[0m");
            break;
        case VERDE:
            printf("\033[32mVerde\033[0m");
            break;
        case AZUL:
            printf("\033[34mAzul\033[0m");
            break;
        case AMARILLO:
            printf("\033[33mAmarillo\033[0m");
            break;
    }
    printf(" %d", newCard.value);
    printf("!\n");
    return newCard;
}

void initializePlayers(Player players[2]) {
    // Inicializa los jugadores
    copyString(players[0].name, "Jugador 1");
    copyString(players[1].name, "Jugador 2");

    int i, j;
    for (i = 0; i < 2; i++) {
        players[i].handSize = HAND_SIZE;
        for (j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = getRandomCard(); // Asigna una carta aleatoria a la mano del jugador
        }
    }
}

void switchPlayer(int* currentPlayer, int numPlayers) {
    // Cambia el jugador actual
    *currentPlayer = (*currentPlayer + 1) % numPlayers;
}

int isValidMove(Card topCard, Card playedCard) {
    register Color topCardColor = topCard.color;
    register int topCardValue = topCard.value;

    register Color playedCardColor = playedCard.color;
    register int playedCardValue = playedCard.value;

    return (playedCardColor == topCardColor) || (playedCardValue == topCardValue);
}


// Función para robar una carta del mazo
Card drawCardFromDeck(Player* player, int displayCard) {
    // Comprueba si el tamaño de la mano del jugador ha alcanzado el máximo
    if ((*player).handSize >= MAX_CARDS) {
        // Si es así, muestra un mensaje y devuelve una carta aleatoria
        printf("\n¡La mano está llena! No puedes robar más cartas.\n");
        return getRandomCard();
    }

    // Si displayCard es verdadero, muestra un mensaje indicando que se ha robado una carta
    if (displayCard) {
        printf("\n¡Se ha robado una carta adicional: ");
    }
    // Obtiene una carta aleatoria
    Card newCard = getRandomCard();
    // Si displayCard es verdadero, muestra el color y el valor de la nueva carta
    if (displayCard) {
        switch (newCard.color) {
            case ROJO:
                printf("\033[31mRojo\033[0m");
                break;
            case VERDE:
                printf("\033[32mVerde\033[0m");
                break;
            case AZUL:
                printf("\033[34mAzul\033[0m");
                break;
            case AMARILLO:
                printf("\033[33mAmarillo\033[0m");
                break;
        }
        printf(" %d", newCard.value);
        printf("!\n");
    }

    // Añade la nueva carta a la mano del jugador e incrementa el tamaño de la mano
    (*player).hand[(*player).handSize++] = newCard;

    // Devuelve la nueva carta
    return newCard;
}

void printCard(Card card) {
    // Imprime una carta
    const char *colors[] = {"\033[31mRojo\033[0m", "\033[32mVerde\033[0m", "\033[34mAzul\033[0m", "\033[33mAmarillo\033[0m"};
    const char *values[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    printf("%s %s", colors[card.color], values[card.value]); // Imprime el color y el valor de la carta
}

void printHand(Player player) {
    // Imprime la mano de un jugador
    printf("Mano de %s:\n\n", player.name);
    int i;
    for (i = 0; i < player.handSize; i++) {
        printf("[%d] ", i + 1);
        printCard(player.hand[i]); // Imprime cada carta de la mano del jugador
        printf("\n");
    }
}

void printPile(CardPile pile) {
    // Imprime la pila de cartas
    printf("Cartas en la pila:\n");
    int i;
    for (i = 0; i <= pile.top; i++) {
        printf(" - ");
        printCard(pile.cards[i]); // Imprime cada carta de la pila
        printf("\n");
    }
}

void printMenu(Player currentPlayer, CardPile *cardPile, Card initialCard, Card lastPlayedCard) {
    // Imprime el menú del juego
    printf("\n******************************************************\n");
    printf("                   JUEGO DE CARTAS UNO               \n");
    printf("******************************************************\n");

    printf("\nCarta en la pila: ");
    printCard(lastPlayedCard); // Imprime la última carta jugada
    printf("\n\n");

    if (currentPlayer.handSize > 0) {
        printHand(currentPlayer); // Imprime la mano del jugador actual si tiene cartas
    } else {
        printf("Mano de %s:\n", currentPlayer.name);
        printf("(Vacia)\n"); // Imprime que la mano del jugador está vacía si no tiene cartas
    }

    printf("\n******************************************************\n");
    printf("\n");
}

void endOfGameMenu(int *playAgain) {
    // Menú de fin de juego
    printf("\n******************************************************\n");
    printf("                   JUEGO DE CARTAS UNO               \n");
    printf("******************************************************\n");
    printf("\n1. Jugar otra vez\n"); // Opción para jugar otra vez
    printf("2. Salir\n\n"); // Opción para salir del juego
    printf("Ingresa el número de tu elección: ");

    int choice;
    scanf("%d", &choice); // Lee la elección del usuario

    if (choice == 1) {
        *playAgain = 1; // Si el usuario elige jugar otra vez, establece playAgain a 1
    } else if (choice == 2) {
        *playAgain = 0; // Si el usuario elige salir, establece playAgain a 0
    } else {
        printf("Opción inválida. Saliendo del juego.\n"); // Si el usuario elige una opción inválida, sale del juego
        *playAgain = 0;
    }
}

void playGame(Player players[2]) {
    srand(time(NULL)); // Inicializa la semilla del generador de números aleatorios
    int currentPlayer = 0; // Establece el jugador actual

    CardPile cardPile; // Crea una pila de cartas
    cardPile.top = -1; // Inicializa la cima de la pila

    Card initialCard = drawInitialCard(); // Roba una carta inicial
    cardPile.cards[++cardPile.top] = initialCard; // Coloca la carta inicial en la cima de la pila

    Card lastPlayedCard = initialCard; // Establece la última carta jugada como la carta inicial

    while (1) { // Comienza el juego
        printMenu(players[currentPlayer], &cardPile, initialCard, lastPlayedCard); // Imprime el menú del juego

        while (1) {
            printf("Ingresa el número de la carta para jugar (o 0 para robar una carta): ");
            int choice;
            scanf("%d", &choice); // Lee la elección del jugador
            getchar();

            if (choice == 0) { // Si el jugador elige robar una carta
                Card drawnCard = drawCardFromDeck(&players[currentPlayer], 0); // Roba una carta del mazo
                break;
            } else if (choice >= 1 && choice <= players[currentPlayer].handSize) { // Si el jugador elige jugar una carta
                Card playedCard = players[currentPlayer].hand[choice - 1]; // Obtiene la carta jugada
                if (isValidMove(lastPlayedCard, playedCard)) {
                    printf("%s juega la carta: ", players[currentPlayer].name);
                    printCard(playedCard); // Imprime la carta jugada
                    printf("\n");

                    cardPile.cards[++cardPile.top] = playedCard; // Coloca la carta jugada en la cima de la pila
                    lastPlayedCard = playedCard; // Actualiza la última carta jugada

                    players[currentPlayer].handSize--; // Disminuye el tamaño de la mano del jugador
                    int i;
                    for (i = choice - 1; i < players[currentPlayer].handSize; i++) {
                        players[currentPlayer].hand[i] = players[currentPlayer].hand[i + 1]; // Elimina la carta jugada de la mano del jugador
                    }

                    if (players[currentPlayer].handSize == 0) { // Si el jugador no tiene más cartas
                        printf("¡%s gana!\n", players[currentPlayer].name); // Anuncia que el jugador ha ganado
                        break; // Termina el juego
                    }
                    break;
                } else {
                    printf("¡Movimiento inválido! Inténtalo de nuevo.\n"); // Imprime un mensaje de error
                    continue; // Continúa con el siguiente turno
                }
            } else { // Si el jugador elige una opción inválida
                printf("¡Elección inválida! Inténtalo de nuevo.\n"); // Imprime un mensaje de error
                continue; // Continúa con el siguiente turno
            }
        }

        switchPlayer(&currentPlayer, 2); // Cambia al siguiente jugador
    }
}

// Función para jugar contra la computadora
void playAgainstComputer(Player players[2]) {
    // Asigna los nombres a los jugadores
    copyString(players[0].name, "Jugador 1");
    copyString(players[1].name, "Computadora");

    // Inicializa las manos de los jugadores con cartas aleatorias
    int i;
    for (i = 0; i < 2; i++) {
        players[i].handSize = HAND_SIZE;
        for (int j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = getRandomCard();
        }
    }

    // Establece el jugador actual
    int currentPlayer = 0;
    // Inicializa la pila de cartas
    CardPile cardPile;
    cardPile.top = -1;
    // Roba la carta inicial
    Card initialCard = drawInitialCard();
    // Coloca la carta inicial en la pila de cartas
    cardPile.cards[++cardPile.top] = initialCard;
    // Establece la última carta jugada como la carta inicial
    Card lastPlayedCard = initialCard;

    // Comienza el juego
    while (1) {
        // Si el jugador actual es la computadora
        if (customStringCompare(players[currentPlayer].name, "Computadora") == 0) {
            int played = 0;
            // La computadora intenta jugar una carta válida de su mano
            for (i = 0; i < players[currentPlayer].handSize; i++) {
                if (isValidMove(lastPlayedCard, players[currentPlayer].hand[i])) {
                    // Si encuentra una, la juega y la coloca en la pila de cartas
                    printf("\nComputadora juega la carta: ");
                    printCard(players[currentPlayer].hand[i]);
                    printf("\n");

                    cardPile.cards[++cardPile.top] = players[currentPlayer].hand[i];
                    lastPlayedCard = players[currentPlayer].hand[i];

                    // Elimina la carta jugada de la mano de la computadora
                    players[currentPlayer].handSize--;
                    int j;
                    for (j = i; j < players[currentPlayer].handSize; j++) {
                        players[currentPlayer].hand[j] = players[currentPlayer].hand[j + 1];
                    }

                    played = 1;
                    break;
                }
            }

            // Si la computadora no pudo jugar una carta, roba una
            if (!played) {
                printf("\nComputadora roba una carta.\n");
                drawCardFromDeck(&players[currentPlayer], 0);
            }
        } else {
            // Si el jugador actual es el usuario
            printMenu(players[currentPlayer], &cardPile, initialCard, lastPlayedCard);

            while (1) {
                // Solicita al usuario que elija una carta para jugar
                printf("Ingresa el número de la carta para jugar (o 0 para robar una carta): ");
                int choice;
                scanf("%d", &choice);
                getchar();

                // Si el usuario elige robar una carta
                if (choice == 0) {
                    Card drawnCard = drawCardFromDeck(&players[currentPlayer], 1);
                    break;
                } else if (choice >= 1 && choice <= players[currentPlayer].handSize) {
                    // Si el usuario elige jugar una carta
                    Card playedCard = players[currentPlayer].hand[choice - 1];
                    if (isValidMove(lastPlayedCard, playedCard)) {
                        printf("%s juega la carta: ", players[currentPlayer].name);
                        printCard(playedCard);
                        printf("\n");

                        // Coloca la carta jugada en la pila de cartas
                        cardPile.cards[++cardPile.top] = playedCard;
                        lastPlayedCard = playedCard;

                        // Elimina la carta jugada de la mano del usuario
                        players[currentPlayer].handSize--;
                        int i;
                        for (i = choice - 1; i < players[currentPlayer].handSize; i++) {
                            players[currentPlayer].hand[i] = players[currentPlayer].hand[i + 1];
                        }
                        break;
                    } else {
                        // Si el usuario intenta jugar una carta inválida
                        printf("¡Movimiento inválido! Inténtalo de nuevo.\n");
                        continue;
                    }
                } else {
                    // Si el usuario elige una opción inválida
                    printf("¡Elección inválida! Inténtalo de nuevo.\n");
                    continue;
                }
            }
        }

        // Si un jugador se queda sin cartas, gana el juego
        if (players[currentPlayer].handSize == 0) {
            if (customStringCompare(players[currentPlayer].name, "Computadora") == 0) {
                printf("\n¡La computadora gana!\n");
            } else {
                printf("¡%s gana!\n", players[currentPlayer].name);
            }
            break;
        }

        // Cambia el turno al otro jugador
        switchPlayer(&currentPlayer, 2);
    }
}

int main() {
    srand(time(NULL)); // Inicializa el generador de números aleatorios con la hora actual
    Player players[2]; // Crea un array para almacenar dos jugadores
    int playAgain = 1; // Variable para controlar si el juego debe continuar

    while (playAgain) { // Mientras el jugador quiera seguir jugando
        printf("\n******************************************************\n");
        printf("                   JUEGO DE CARTAS UNO               \n");
        printf("******************************************************\n");
        printf("\n1. Jugar contra computadora\n"); // Opción para jugar contra la computadora
        printf("2. Jugar contra amigo\n"); // Opción para jugar contra un amigo
        printf("3. Salir\n\n"); // Opción para salir del juego
        printf("Ingresa el número de tu elección: "); // Solicita al usuario que elija una opción

        int choice;
        scanf("%d", &choice); // Lee la elección del usuario

        switch (choice) { // Ejecuta una acción basada en la elección del usuario
            case 1:
                playAgainstComputer(players); // Juega contra la computadora
                break;

            case 2:
                initializePlayers(players); // Inicializa los jugadores
                playGame(players); // Juega el juego
                break;

            case 3:
                playAgain = 0; // Termina el juego
                break;

            default:
                printf("Opción inválida. Inténtalo de nuevo.\n"); // Mensaje de error si la elección no es válida
        }
    }

    return 0; // Termina el programa
}
