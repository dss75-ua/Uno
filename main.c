#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DECK_SIZE 108
#define HAND_SIZE 7
#define MAX_NAME_LENGTH 20

typedef enum {
    RED, GREEN, BLUE, YELLOW
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

void initializeDeck();
void shuffleDeck();
Card drawCard();
void printCard(Card card);
void printHand(Player player);
void initializePlayers(Player players[2]);
void switchPlayer(Player** currentPlayer, Player players[2]);
void playGame(Player players[2]);
int isValidMove(Card topCard, Card playedCard);
void drawCardFromDeck(Player* player, int numCards);

int main() {
    srand(time(NULL));
    Player players[2];
    initializePlayers(players);
    playGame(players);
    return 0;
}

void initializeDeck() {
    int i, j, index = 0;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 10; ++j) {
            deck[index].color = i;
            deck[index].value = j;
            ++index;
            if (j != 0) {
                deck[index].color = i;
                deck[index].value = j;
                ++index;
            }
        }
    }
}

void shuffleDeck() {
    int i, j;
    Card temp;
    for (i = DECK_SIZE - 1; i > 0; --i) {
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
    const char* colors[] = {"Red", "Green", "Blue", "Yellow"};
    const char* values[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    printf("%s %s", colors[card.color], values[card.value]);
}

void initializePlayers(Player players[2]) {
    // Assign generic player names
    strcpy(players[0].name, "Player 1");
    strcpy(players[1].name, "Player 2");

    initializeDeck();
    shuffleDeck();

    int i, j;
    for (i = 0; i < 2; ++i) {
        players[i].handSize = HAND_SIZE;
        for (j = 0; j < HAND_SIZE; ++j) {
            players[i].hand[j] = drawCard();
        }
    }
}

void switchPlayer(Player** currentPlayer, Player players[2]) {
    *currentPlayer = (*currentPlayer == &players[0]) ? &players[1] : &players[0];
}

int isValidMove(Card topCard, Card playedCard) {
    return (playedCard.color == topCard.color) || (playedCard.value == topCard.value);
}

void drawCardFromDeck(Player* player, int numCards) {
    int i;
    for (i = 0; i < numCards; ++i) {
        if (deckIndex < DECK_SIZE && player->handSize < HAND_SIZE) {
            // Only draw if there are cards remaining in the deck and the hand is not full
            player->hand[player->handSize++] = drawCard();
        } else if (deckIndex >= DECK_SIZE) {
            printf("Out of cards to draw!\n");
            break;
        } else {
            printf("Cannot draw more cards, hand is full.\n");
            break;
        }
    }
}

void printHand(Player player) {
    printf("%s's Hand:\n", player.name);
    for (int i = 0; i < player.handSize; ++i) {
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
    printf("                           JUEGO DE CARTAS UNO\n");
    printf("******************************************************\n");
    printf("\n1. Jugar otra vez\n");
    printf("2. Salir\n\n");
    printf("Ingresa el numero de tu eleccion: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        *playAgain = 1;
    } else if (choice == 2) {
        *playAgain = 0;
    } else {
        printf("Opcion invalida. Saliendo del juego.\n");
        *playAgain = 0;
    }
}

void playGame(Player players[2]) {
    srand(time(NULL));
    Player* currentPlayer = &players[0];
    Card topCard = drawCard(); // Draw the first card to start the game

    printf("Let's play UNO!\n");

    while (1) {
        printMenu(topCard, *currentPlayer);

        if (currentPlayer->handSize == 0) {
            printf("%s wins!\n", currentPlayer->name);
        }

        if (deckIndex >= DECK_SIZE) {
            printf("Out of cards to draw!\n");
        }

        if (currentPlayer->handSize == 0 || deckIndex >= DECK_SIZE) {
            // End of game menu
            int playAgain;
            endOfGameMenu(&playAgain);

            if (playAgain) {
                // Reset the game for a new round
                initializeDeck();
                shuffleDeck();
                initializePlayers(players);
                currentPlayer = &players[0];
                topCard = drawCard();
                printf("Let's play UNO!\n");
            } else {
                // Exit the game
                break;
            }
        }

        printf("Enter the card number to play (or 0 to draw a card): ");
        int choice;
        scanf("%d", &choice);
        getchar(); // Consume newline character

        if (choice == 0) {
            drawCardFromDeck(currentPlayer, 1);
        } else if (choice >= 1 && choice <= currentPlayer->handSize) {
            Card playedCard = currentPlayer->hand[choice - 1];
            if (isValidMove(topCard, playedCard)) {
                topCard = playedCard;
                currentPlayer->hand[choice - 1] = currentPlayer->hand[--currentPlayer->handSize]; // Remove played card from hand
            } else {
                printf("Invalid move! Try again.\n");
                continue;
            }
        } else {
            printf("Invalid choice! Try again.\n");
            continue;
        }

        if (currentPlayer->handSize > 0) {
            switchPlayer(&currentPlayer, players);
        }
    }
}
