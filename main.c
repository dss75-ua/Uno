#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HAND_SIZE 7
#define MAX_NAME_LENGTH 20
#define MAX_CARDS 20  // Maximum number of cards a player can have

typedef enum {
    ROJO, VERDE, AZUL, AMARILLO
} Color;

typedef struct {
    Color color;
    int value;
} Card;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Card hand[MAX_CARDS];  // 2D array of Cards
    int handSize;
} Player;

void printCard(Card card);
void printHand(Player player);

Card getRandomCard() {
    Card randomCard;
    randomCard.color = rand() % 4;  // Random color (0 to 3)
    randomCard.value = rand() % 10; // Random value (0 to 9)
    return randomCard;
}

void copyString(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0' && i < MAX_NAME_LENGTH - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void initializePlayers(Player players[2]) {
    copyString(players[0].name, "Jugador 1");
    copyString(players[1].name, "Jugador 2");

    int i;
    for (i = 0; i < 2; i++) {
        players[i].handSize = HAND_SIZE;
        for (int j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = getRandomCard();
        }
    }
}

void switchPlayer(Player** currentPlayer, Player players[2]) {
    if (*currentPlayer == &players[0]) {
        *currentPlayer = &players[1];
    } else {
        *currentPlayer = &players[0];
    }
}

int isValidMove(Card topCard, Card playedCard) {
    return (playedCard.color == topCard.color) || (playedCard.value == topCard.value);
}

void drawCardFromDeck(Player* player) {
    printf("¡Se ha robado una carta adicional: ");
    Card newCard = getRandomCard();
    printCard(newCard);
    printf("!\n");

    // Check if the player's hand is full
    if (player->handSize >= MAX_CARDS) {
        printf("¡La mano está llena! No puedes robar más cartas.\n");
        return;
    }

    // Add the new card to the player's hand at the correct index
    (*(player)).hand[(*(player)).handSize++] = newCard;
    printHand(*player);  // Print the updated hand
}

void printCard(Card card) {
    const char* colors[] = {"Rojo", "Verde", "Azul", "Amarillo"};
    const char* values[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    printf("%s %s", colors[card.color], values[card.value]);
}

void printHand(Player player) {
    printf("Mano de %s:\n", player.name);
    for (int i = 0; i < player.handSize; i++) {
        printf("[%d] ", i + 1);
        printCard(player.hand[i]);
        printf("\n");
    }
}

void printMenu(Player currentPlayer) {
    printf("\n******************************************************\n");
    printf("                   JUEGO DE CARTAS UNO               \n");
    printf("******************************************************\n");

    if (currentPlayer.handSize > 0) {
        printHand(currentPlayer);
    } else {
        printf("Mano de %s:\n", currentPlayer.name);
        printf("(Vacia)\n");
    }

    printf("\n******************************************************\n");
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

    while (1) {
        printMenu(*currentPlayer);

        // Check for the end of the game
        if (currentPlayer->handSize == 0) {
            printf("¡%s gana!\n", currentPlayer->name);
            break;  // Exit the loop when the game ends
        }

        printf("Ingresa el número de la carta para jugar (o 0 para robar una carta): ");
        int choice;
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        if (choice == 0) {
            drawCardFromDeck(currentPlayer);
        } else if (choice >= 1 && choice <= currentPlayer->handSize) {
            // Play the selected card (no need for playCard function)
            currentPlayer->handSize--;
            for (int i = choice - 1; i < currentPlayer->handSize; i++) {
                currentPlayer->hand[i] = currentPlayer->hand[i + 1];
            }
        } else {
            printf("¡Elección inválida! Inténtalo de nuevo.\n");
            continue;  // Restart the loop to get a valid input
        }

        // After drawing a card or playing a card, switch to the other player
        switchPlayer(&currentPlayer, players);
    }
}

int main() {
    srand(time(NULL));
    Player players[2];
    int playAgain = 1;

    while (playAgain) {
        printf("\n******************************************************\n");
        printf("                   JUEGO DE CARTAS UNO               \n");
        printf("******************************************************\n");
        printf("\n1. Jugar contra computadora\n");
        printf("2. Jugar contra amigo\n");
        printf("3. Salir\n\n");
        printf("Ingresa el número de tu elección: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Call a function to play against the computer
                // playAgainstComputer(players);
                printf("Función para jugar contra computadora aún no implementada.\n");
                break;

            case 2:
                initializePlayers(players);
                playGame(players);
                break;

            case 3:
                playAgain = 0;
                break;

            default:
                printf("Opción inválida. Inténtalo de nuevo.\n");
        }
    }

    return 0;
}
