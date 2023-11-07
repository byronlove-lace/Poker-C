#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DECK_TOTAL 52
#define MX_CRD_NAME_LEN 22
#define HAND_SIZE 5
#define SUIT_COUNT 4
#define FACE_CRD_COUNT 3
#define NUM_CRD_COUNT 10

const unsigned int BUY_IN = 5;
const int P1_CASH = 150;
const int P2_CASH = 150;

void assemble(const char const *suits[], const char const *faces[], const char const *nums[], char pack[][MX_CRD_NAME_LEN]);
void shuffle(char pack[DECK_TOTAL][MX_CRD_NAME_LEN]);
void deal_crd(char pack[DECK_TOTAL][MX_CRD_NAME_LEN], unsigned int *nxt_crd, char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *hand_count);
void deal_phase(char pack[DECK_TOTAL][MX_CRD_NAME_LEN], unsigned int *nxt_crd, char p1_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *p1_hand_count, char p2_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *p2_hand_count);
void read_hands(char p_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int p_hand_count, char cpu_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int cpu_hand_count);

int main (void)
{
        srand(time(NULL));

        const char *SUITS[SUIT_COUNT] = {"Spades", "Diamonds", "Clubs", "Hearts"};
        const char *FACE_CRDS[FACE_CRD_COUNT] = {"Jack", "Queen", "King"};
        const char *NUM_CRDS[NUM_CRD_COUNT] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

        char deck[DECK_TOTAL][MX_CRD_NAME_LEN] = {0};
        unsigned int top_crd = 0;

        char player_hand[HAND_SIZE][MX_CRD_NAME_LEN] = {0};
        char cpu_hand [HAND_SIZE][MX_CRD_NAME_LEN] = {0};

        unsigned int player_crd_count = 0;
        unsigned int cpu_crd_count = 0;

        unsigned int player_hand_val = 0;
        unsigned int cpu_hand_val = 0;

        assemble(SUITS, FACE_CRDS, NUM_CRDS, deck);
        shuffle(deck);
        deal_phase(deck, &top_crd, player_hand, &player_crd_count, cpu_hand, &cpu_crd_count);
}

void mk_crd(const char *val, const char *suit, char *crd)
{
        const char of[5] = " of ";
        size_t i, j, k;
        i = j = k = 0;

        while (val[i] != '\0')
        {
                crd[i] = val[i];
                i++;
        }

        while (of[k] != '\0')
        {
                crd[i] = of[k];
                k++;
                i++;
        }

        while (suit[j] != '\0')
        {
                crd[i] = suit[j];
                i++;
                j++;
        }

        crd[i] = '\0';
}

void assemble(const char const *suits[], const char const *faces[], const char const *nums[], char pack[DECK_TOTAL][MX_CRD_NAME_LEN])
{
        void mk_crd(const char *val, const char *suit, char *crd);
        char current_card[MX_CRD_NAME_LEN];  

        unsigned int pos = 0;

        for (size_t s = 0; s < SUIT_COUNT/2; ++s)
        {
                for (size_t n = 0; n < NUM_CRD_COUNT; ++n)
                {
                        mk_crd(nums[n], suits[s], current_card);
                        strcpy(pack[pos], current_card);
                        ++pos;
                }

                for (size_t f = 0; f < FACE_CRD_COUNT; ++f)
                {
                        mk_crd(faces[f], suits[s], current_card);
                        strcpy(pack[pos], current_card);
                        ++pos;
                }
        }

        for (size_t s = 2; s < SUIT_COUNT; ++s)
        {
                for (int f = (FACE_CRD_COUNT - 1); f >= 0; --f)
                {
                        mk_crd(faces[f], suits[s], current_card);
                        strcpy(pack[pos], current_card);
                        ++pos;
                }

                for (int n = (NUM_CRD_COUNT - 1); n >= 0; --n)
                {
                        mk_crd(nums[n], suits[s], current_card);
                        strcpy(pack[pos], current_card);
                        ++pos;
                }
        }
}


void shuffle(char pack[DECK_TOTAL][MX_CRD_NAME_LEN])
{
        int r; 
        char rand_card[MX_CRD_NAME_LEN];  
        char hold_card[MX_CRD_NAME_LEN];  

       for (size_t i = 0; i < DECK_TOTAL; ++i) 
               {
                       r = rand() % DECK_TOTAL;
                       strcpy(rand_card, pack[r]);
                       strcpy(hold_card, pack[i]);
                       strcpy(pack[i], rand_card);
                       strcpy(pack[r], hold_card);
               }
}

void deal_crd(char pack[DECK_TOTAL][MX_CRD_NAME_LEN], unsigned int *nxt_crd, char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *hand_count)
{ 
        strcpy(hand[*hand_count], pack[*nxt_crd]);

        ++*nxt_crd; 
        ++*hand_count;
}

void deal_phase(char pack[DECK_TOTAL][MX_CRD_NAME_LEN], unsigned int *nxt_crd, char p1_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *p1_hand_count, char p2_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *p2_hand_count)
{
        for (size_t i = 0; i < HAND_SIZE; ++i)
        {
                deal_crd(pack, nxt_crd, p1_hand, p1_hand_count);
                deal_crd(pack, nxt_crd, p2_hand, p2_hand_count);
        }
}

void read_hands(char p_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int p_hand_count, char d_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int d_hand_count)
{
        puts("Player: \n");
        for (size_t i = 0; i < p_hand_count; ++i)
        {
                printf("%s\n", p_hand[i]);
        }

        puts("Dealer: \n");

        for (size_t i = 0; i < d_hand_count; ++i)
        {
                printf("%s\n", d_hand[i]);
        }
}
