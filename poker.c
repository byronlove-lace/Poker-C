#include <stdio.h>
#include<stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DECK_TOTAL 52
#define MX_CRD_NAME_LEN 22
#define MX_VAL_NAME_LEN 5 
#define HAND_SIZE 5
#define SUIT_COUNT 4
#define FACE_CRD_COUNT 3
#define NUM_CRD_COUNT 10

const unsigned int BUY_IN = 5;
const int P1_CASH = 150;
const int P2_CASH = 150;

void assemble(const char const *suits[], const char const *faces[], const char const *nums[], char pack[][MX_CRD_NAME_LEN]);
void shuffle(char pack[DECK_TOTAL][MX_CRD_NAME_LEN]);
void deal_crd(char pack[DECK_TOTAL][MX_CRD_NAME_LEN], unsigned int *nxt_crd, char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *hand_count); void deal_phase(char pack[DECK_TOTAL][MX_CRD_NAME_LEN], unsigned int *nxt_crd, char p1_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *p1_hand_count, char p2_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int *p2_hand_count);
void read_hand(char p_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int p_hand_count); 
unsigned int eval_crd(char crd[MX_CRD_NAME_LEN]);
unsigned int eval_hand(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int hand_vals[HAND_SIZE]);

int main (void)
{
        srand(time(NULL));

        const char *SUITS[SUIT_COUNT] = {"Spades", "Diamonds", "Clubs", "Hearts"};
        const char *FACE_CRDS[FACE_CRD_COUNT] = {"Jack", "Queen", "King"};
        const char *NUM_CRDS[NUM_CRD_COUNT] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

        char deck[DECK_TOTAL][MX_CRD_NAME_LEN] = {0};
        unsigned int top_crd = 0;

        char player_hand[HAND_SIZE][MX_CRD_NAME_LEN] = {0};
        unsigned int player_hand_vals[HAND_SIZE] = {0};
        char cpu_hand [HAND_SIZE][MX_CRD_NAME_LEN] = {0};
        unsigned int cpu_hand_vals [HAND_SIZE] = {0};

        unsigned int player_crd_count = 0;
        unsigned int cpu_crd_count = 0;

        unsigned int player_hand_val = 0;
        unsigned int cpu_hand_val = 0;

        assemble(SUITS, FACE_CRDS, NUM_CRDS, deck);
        shuffle(deck);
        deal_phase(deck, &top_crd, player_hand, &player_crd_count, cpu_hand, &cpu_crd_count);
        read_hand(player_hand, player_crd_count);
        eval_hand(player_hand, player_hand_vals);
        for (size_t i = 0; i < HAND_SIZE; ++i)
        {
                printf("%u\n", player_hand_vals[i]);
        }
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

void read_hand(char p_hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int p_hand_count) 
{
        puts("Player: \n");
        for (size_t i = 0; i < p_hand_count; ++i)
        {
                printf("%s\n", p_hand[i]);
        }
}

unsigned int eval_crd(char crd[MX_CRD_NAME_LEN]) 
{
        unsigned int letter_pos = 0;
        char val_name[MX_VAL_NAME_LEN] = {0};

        while (crd[letter_pos] != ' ')
        {
                val_name[letter_pos] = crd[letter_pos];
                ++letter_pos;
        }

        letter_pos = 0;


        if (strcmp(val_name, "Two") == 0)
        {
                return 2;
        }

        if (strcmp(val_name, "Three") == 0)
        {
                return 3;
        }

        if (strcmp(val_name, "Four") == 0)
        {
                return 4;
        }

        if (strcmp(val_name, "Five") == 0)
        {
                return 5;
        }

        if (strcmp(val_name, "Six") == 0)
        {
                return 6;
        }

        if (strcmp(val_name, "Seven") == 0)
        {
                return 7;
        }

        if (strcmp(val_name, "Eight") == 0)
        {
                return 8;
        }

        if (strcmp(val_name, "Nine") == 0)
        {
                return 9;
        }

        if (strcmp(val_name, "Ten") == 0)
        {
                return 10;
        }

        if (strcmp(val_name, "Jack") == 0)
        {
                return 11;
        }

        if (strcmp(val_name, "Queen") == 0)
        {
                return 12;
        }

        if (strcmp(val_name, "King") == 0)
        {
                return 13;
        }

        if (strcmp(val_name, "Ace") == 0)
        {
                return 14;
        }

        // rem memeset(val_name) here
}

unsigned int eval_hand(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int hand_vals[HAND_SIZE])
{
        for (size_t i = 0; i < HAND_SIZE; ++i)
        {
               hand_vals[i] = eval_crd(hand[i]);
        }
}

void order_hand(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int hand_count)
{
       ; 
}

void pair_check(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int hand_count)
{
        bool has_pair = false;

        for (size_t i = 0; i < HAND_SIZE; ++i)
        {
                for (size_t j = 0; j < MX_VAL_NAME_LEN; ++j)
                {
                        while (j != '\0')
                        {
                                ;
                        }
                }
        }
}
