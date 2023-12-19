#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HAND_SIZE 7
#define MAX_NAME_LENGTH 20
#define MAX_CARDS 20

typedef enum {
    ROJO, VERDE, AZUL, AMARILLO
} Color;

typedef struct {
    Color color;
    int value;
} Card;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Card hand[MAX_CARDS];
    int handSize;
} Player;

typedef struct {
    Card cards[MAX_CARDS];
    int top;
} CardPile;

Card getRandomCard() {
    Card randomCard;
    randomCard.color = rand() % 4;
    randomCard.value = rand() % 10;
    return randomCard;
}

int customStringCompare(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }

    if (*str1 == '\0' && *str2 == '\0') {
        return 0;
    } else {
        return 1;
    }
}

void copyString(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0' && i < MAX_NAME_LENGTH - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

Card drawInitialCard() {
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
    copyString(players[0].name, "Jugador 1");
    copyString(players[1].name, "Jugador 2");

    int i, j;
    for (i = 0; i < 2; i++) {
        players[i].handSize = HAND_SIZE;
        for (j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = getRandomCard();
        }
    }
}

void switchPlayer(int* currentPlayer, int numPlayers) {
    *currentPlayer = (*currentPlayer + 1) % numPlayers;
}

int isValidMove(Card topCard, Card playedCard) {
    return (playedCard.color == topCard.color) || (playedCard.value == topCard.value);
}

Card drawCardFromDeck(Player* player) {
    if ((*player).handSize >= MAX_CARDS) {
        printf("¡La mano está llena! No puedes robar más cartas.\n");
        return getRandomCard();
    }

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

    (*player).hand[(*player).handSize++] = newCard;

    return newCard;
}

void printCard(Card card) {
    const char *colors[] = {"\033[31mRojo\033[0m", "\033[32mVerde\033[0m", "\033[34mAzul\033[0m", "\033[33mAmarillo\033[0m"};
    const char *values[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    printf("%s %s", colors[card.color], values[card.value]);
}

void printHand(Player player) {
    printf("Mano de %s:\n\n", player.name);
    int i;
    for (i = 0; i < player.handSize; i++) {
        printf("[%d] ", i + 1);
        printCard(player.hand[i]);
        printf("\n");
    }
}

void printPile(CardPile pile) {
    printf("Cartas en la pila:\n");
    int i;
    for (i = 0; i <= pile.top; i++) {
        printf(" - ");
        printCard(pile.cards[i]);
        printf("\n");
    }
}

void printMenu(Player currentPlayer, CardPile *cardPile, Card initialCard, Card lastPlayedCard) {
    printf("\n******************************************************\n");
    printf("                   JUEGO DE CARTAS UNO               \n");
    printf("******************************************************\n");

    printf("Carta  en la pila: ");
    printCard(lastPlayedCard);
    printf("\n\n");

    if (currentPlayer.handSize > 0) {
        printHand(currentPlayer);
    } else {
        printf("Mano de %s:\n", currentPlayer.name);
        printf("(Vacia)\n");
    }

    printf("\n******************************************************\n");
    printf("\n");
}

void endOfGameMenu(int *playAgain) {
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
    int currentPlayer = 0;

    CardPile cardPile;
    cardPile.top = -1;

    Card initialCard = drawInitialCard();
    cardPile.cards[++cardPile.top] = initialCard;

    Card lastPlayedCard = initialCard;

    while (1) {
        printMenu(players[currentPlayer], &cardPile, initialCard, lastPlayedCard);

        printf("Ingresa el número de la carta para jugar (o 0 para robar una carta): ");
        int choice;
        scanf("%d", &choice);
        getchar();

        if (choice == 0) {
            Card drawnCard = drawCardFromDeck(&players[currentPlayer]);
        } else if (choice >= 1 && choice <= players[currentPlayer].handSize) {
            Card playedCard = players[currentPlayer].hand[choice - 1];
            printf("%s juega la carta: ", players[currentPlayer].name);
            printCard(playedCard);
            printf("\n");

            cardPile.cards[++cardPile.top] = playedCard;
            lastPlayedCard = playedCard;

            players[currentPlayer].handSize--;
            int i;
            for (i = choice - 1; i < players[currentPlayer].handSize; i++) {
                players[currentPlayer].hand[i] = players[currentPlayer].hand[i + 1];
            }

            if (players[currentPlayer].handSize == 0) {
                printf("¡%s gana!\n", players[currentPlayer].name);
                break;
            }
        } else {
            printf("¡Elección inválida! Inténtalo de nuevo.\n");
            continue;
        }

        switchPlayer(&currentPlayer, 2);
    }
}

void playAgainstComputer(Player players[2]) {
    copyString(players[0].name, "Jugador 1");
    copyString(players[1].name, "Computadora");

    int i;
    for (i = 0; i < 2; i++) {
        players[i].handSize = HAND_SIZE;
        for (int j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = getRandomCard();
        }
    }

    int currentPlayer = 0;
    CardPile cardPile;
    cardPile.top = -1;
    Card initialCard = drawInitialCard();
    cardPile.cards[++cardPile.top] = initialCard;
    Card lastPlayedCard = initialCard;

    while (1) {
        if (customStringCompare(players[currentPlayer].name, "Computadora") == 0) {
            int played = 0;
            for (i = 0; i < players[currentPlayer].handSize; i++) {
                if (isValidMove(lastPlayedCard, players[currentPlayer].hand[i])) {
                    printf("Computadora juega la carta: ");
                    printCard(players[currentPlayer].hand[i]);
                    printf("\n");

                    cardPile.cards[++cardPile.top] = players[currentPlayer].hand[i];
                    lastPlayedCard = players[currentPlayer].hand[i];

                    players[currentPlayer].handSize--;
                    int j;
                    for (j = i; j < players[currentPlayer].handSize; j++) {
                        players[currentPlayer].hand[j] = players[currentPlayer].hand[j + 1];
                    }

                    played = 1;
                    break;
                }
            }

            if (!played) {
                printf("Computadora roba una carta.\n");
                drawCardFromDeck(&players[currentPlayer]);
            }
        } else {
            printMenu(players[currentPlayer], &cardPile, initialCard, lastPlayedCard);

            printf("Ingresa el número de la carta para jugar (o 0 para robar una carta): ");
            int choice;
            scanf("%d", &choice);
            getchar();

            if (choice == 0) {
                Card drawnCard = drawCardFromDeck(&players[currentPlayer]);
            } else if (choice >= 1 && choice <= players[currentPlayer].handSize) {
                Card playedCard = players[currentPlayer].hand[choice - 1];
                printf("%s juega la carta: ", players[currentPlayer].name);
                printCard(playedCard);
                printf("\n");

                cardPile.cards[++cardPile.top] = playedCard;
                lastPlayedCard = playedCard;

                players[currentPlayer].handSize--;
                int i;
                for (i = choice - 1; i < players[currentPlayer].handSize; i++) {
                    players[currentPlayer].hand[i] = players[currentPlayer].hand[i + 1];
                }
            } else {
                printf("¡Elección inválida! Inténtalo de nuevo.\n");
                continue;
            }
        }

        if (players[currentPlayer].handSize == 0) {
            if (customStringCompare(players[currentPlayer].name, "Computadora") == 0) {
                printf("\n¡La computadora gana!\n");
            } else {
                printf("¡%s gana!\n", players[currentPlayer].name);
            }
            break;
        }

        switchPlayer(&currentPlayer, 2);
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
                playAgainstComputer(players);
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
