#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 108
#define HAND_SIZE 7
#define MAX_NAME_LENGTH 20

typedef enum {
    ROJO, VERDE, AZUL, AMARILLO
} Color;

typedef struct {
    Color color;
    int value;
} Card;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Card hand[HAND_SIZE];
    int handSize;
} Player;

Card deck[DECK_SIZE];
int deckIndex = 0;

void copyString(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0' && i < MAX_NAME_LENGTH - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void initializeDeck() {
    int i, j, index = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 10; j++) {
            deck[index].color = i;
            deck[index].value = j;
            index++;
            if (j != 0) {
                deck[index].color = i;
                deck[index].value = j;
                index++;
            }
        }
    }
}

void shuffleDeck() {
    int i, j;
    Card temp;
    for (i = DECK_SIZE - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

Card drawCard() {
    return deck[deckIndex++];
}

void printCard(Card card) {
    const char* colors[] = {"Rojo", "Verde", "Azul", "Amarillo"};
    const char* values[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    printf("%s %s", colors[card.color], values[card.value]);
}

void initializePlayers(Player players[2]) {
    // Asignar nombres genéricos a los jugadores
    copyString(players[0].name, "Jugador 1");
    copyString(players[1].name, "Jugador 2");

    initializeDeck();
    shuffleDeck();

    int i, j;
    for (i = 0; i < 2; i++) {
        players[i].handSize = HAND_SIZE;
        for (j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = drawCard();
        }
    }
}

void switchPlayer(Player* currentPlayer, Player players[2]) {
    if (currentPlayer == &players[0]) {
        currentPlayer = &players[1];
    } else {
        currentPlayer = &players[0];
    }
}

int isValidMove(Card topCard, Card playedCard) {
    return (playedCard.color == topCard.color) || (playedCard.value == topCard.value);
}

void drawCardFromDeck(Player* player, int numCards) {
    int i;
    for (i = 0; i < numCards; i++) {
        if (deckIndex < DECK_SIZE && player->handSize < HAND_SIZE) {
            // Solo dibujar si quedan cartas en el mazo y la mano no está llena
            player->hand[player->handSize++] = drawCard();
        } else if (deckIndex >= DECK_SIZE) {
            printf("¡Se acabaron las cartas para robar!\n");
            break;
        } else {
            printf("No se pueden dibujar más cartas, la mano está llena.\n");
            break;
        }
    }
}

void printHand(Player player) {
    printf("Mano de %s:\n", player.name);
    for (int i = 0; i < player.handSize; i++) {
        printf("[%d] ", i + 1);
        printCard(player.hand[i]);
        printf("\n");
    }
}

void printMenu(Card topCard, Player currentPlayer) {
    printf("\n******************************************************\n");
    printf("                   JUEGO DE CARTAS UNO               \n");
    printf("******************************************************\n");
    printf("\nCarta Actual ");
    printCard(topCard);
    printf("\n\nTU MANO:\n");
    printHand(currentPlayer);
    printf("\n");
    printf("******************************************************\n");
    printf("\n");
}

void endOfGameMenu(int* playAgain) {
    printf("\n******************************************************\n");
    printf("                   JUEGO DE CARTAS UNO               \n");
    printf("******************************************************\n");
    printf("\n1. Jugar otra vez\n");
    printf("2. Salir\n\n");
    printf("Ingresa el número de tu elección: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        *playAgain = 1;
    } else if (choice == 2) {
        *playAgain = 0;
    } else {
        printf("Opción inválida. Saliendo del juego.\n");
        *playAgain = 0;
    }
}

void playGame(Player players[2]) {
    srand(time(NULL));
    Player* currentPlayer = &players[0];
    Card topCard = drawCard(); // Dibujar la primera carta para comenzar el juego

    while (1) {
        printMenu(topCard, *currentPlayer);

        if (currentPlayer->handSize == 0) {
            printf("¡%s gana!\n", currentPlayer->name);
        }

        if (deckIndex >= DECK_SIZE) {
            printf("¡Se acabaron las cartas para robar!\n");
        }

        if (currentPlayer->handSize == 0 || deckIndex >= DECK_SIZE) {
            // Menú de fin de juego
            int playAgain;
            endOfGameMenu(&playAgain);

            if (playAgain) {
                // Reiniciar el juego para una nueva ronda
                initializeDeck();
                shuffleDeck();
                initializePlayers(players);
                currentPlayer = &players[0];
                topCard = drawCard();
                printf("¡Vamos a jugar UNO!\n");
            } else {
                // Salir del juego
                break;
            }
        }

        printf("Ingresa el número de la carta para jugar (o 0 para robar una carta): ");
        int choice;
        scanf("%d", &choice);
        getchar(); // Consumir el carácter de nueva línea

        if (choice == 0) {
            drawCardFromDeck(currentPlayer, 1);
        } else if (choice >= 1 && choice <= currentPlayer->handSize) {
            Card playedCard = currentPlayer->hand[choice - 1];
            if (isValidMove(topCard, playedCard)) {
                topCard = playedCard;
                currentPlayer->hand[choice - 1] = currentPlayer->hand[--currentPlayer->handSize]; // Quitar la carta jugada de la mano
            } else {
                printf("¡Movimiento inválido! Inténtalo de nuevo.\n");
                continue;
            }
        } else {
            printf("¡Elección inválida! Inténtalo de nuevo.\n");
            continue;
        }

        if (currentPlayer->handSize > 0) {
            switchPlayer(currentPlayer, players);
        }
    }
}

int main() {
    srand(time(NULL));
    Player players[2];
    initializePlayers(players);
    playGame(players);
    return 0;
}
