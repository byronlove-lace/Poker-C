#include <stdio.h>
#include<stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>

#define DECK_TOTAL 52
#define MX_CRD_NAME_LEN 22
#define MX_VAL_NAME_LEN 6 
#define MX_SUIT_NAME_LEN 9 
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
unsigned int eval_crds_in_hand(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int crd_vals[HAND_SIZE]);
void sort_hand(unsigned int crd_vals[HAND_SIZE]);
float assess_hand(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int crd_vals[HAND_SIZE]);

int main (void)
{
        srand(time(NULL));

        const char *SUITS[SUIT_COUNT] = {"Spades", "Diamonds", "Clubs", "Hearts"};
        const char *FACE_CRDS[FACE_CRD_COUNT] = {"Jack", "Queen", "King"};
        const char *NUM_CRDS[NUM_CRD_COUNT] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

        char deck[DECK_TOTAL][MX_CRD_NAME_LEN] = {0};
        unsigned int top_crd = 0;

        char player_hand[HAND_SIZE][MX_CRD_NAME_LEN] = {0};
        unsigned int player_crd_vals[HAND_SIZE] = {0};
        unsigned int player_crd_count = 0;
        float player_hand_value = 0;

        char cpu_hand [HAND_SIZE][MX_CRD_NAME_LEN] = {0};
        unsigned int cpu_crd_vals [HAND_SIZE] = {0};
        unsigned int cpu_crd_count = 0;
        float cpu_hand_value = 0;



        assemble(SUITS, FACE_CRDS, NUM_CRDS, deck);
        shuffle(deck);
        deal_phase(deck, &top_crd, player_hand, &player_crd_count, cpu_hand, &cpu_crd_count);
        read_hand(player_hand, player_crd_count);

        eval_crds_in_hand(player_hand, player_crd_vals);
        sort_hand(player_crd_vals);

        eval_crds_in_hand(cpu_hand, cpu_crd_vals);
        sort_hand(cpu_crd_vals);

        player_hand_value = assess_hand(player_hand, player_crd_vals);
        cpu_hand_value = assess_hand(cpu_hand, cpu_crd_vals);
        printf("PLAYER: %.2f\n", player_hand_value);
        printf("CPU: %.2f\n", cpu_hand_value);
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
        char current_crd[MX_CRD_NAME_LEN];  

        unsigned int pos = 0;

        for (size_t s = 0; s < SUIT_COUNT/2; ++s)
        {
                for (size_t n = 0; n < NUM_CRD_COUNT; ++n)
                {
                        mk_crd(nums[n], suits[s], current_crd);
                        strcpy(pack[pos], current_crd);
                        ++pos;
                }

                for (size_t f = 0; f < FACE_CRD_COUNT; ++f)
                {
                        mk_crd(faces[f], suits[s], current_crd);
                        strcpy(pack[pos], current_crd);
                        ++pos;
                }
        }

        for (size_t s = 2; s < SUIT_COUNT; ++s)
        {
                for (int f = (FACE_CRD_COUNT - 1); f >= 0; --f)
                {
                        mk_crd(faces[f], suits[s], current_crd);
                        strcpy(pack[pos], current_crd);
                        ++pos;
                }

                for (int n = (NUM_CRD_COUNT - 1); n >= 0; --n)
                {
                        mk_crd(nums[n], suits[s], current_crd);
                        strcpy(pack[pos], current_crd);
                        ++pos;
                }
        }
}


void shuffle(char pack[DECK_TOTAL][MX_CRD_NAME_LEN])
{
        int r; 
        char rand_crd[MX_CRD_NAME_LEN];  
        char hold_crd[MX_CRD_NAME_LEN];  

       for (size_t i = 0; i < DECK_TOTAL; ++i) 
               {
                       r = rand() % DECK_TOTAL;
                       strcpy(rand_crd, pack[r]);
                       strcpy(hold_crd, pack[i]);
                       strcpy(pack[i], rand_crd);
                       strcpy(pack[r], hold_crd);
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

unsigned int eval_crds_in_hand(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int crd_vals[HAND_SIZE])
{
        for (size_t i = 0; i < HAND_SIZE; ++i)
        {
               crd_vals[i] = eval_crd(hand[i]);
        }
}

void sort_hand(unsigned int crd_vals[HAND_SIZE])
{
        int n = HAND_SIZE - 2;

        while (n > -1)
        {
                unsigned int key = crd_vals[n];
                unsigned int j = n + 1;

                while (j < HAND_SIZE && crd_vals[j] < key)
                {
                        crd_vals[j - 1] = crd_vals[j];
                        ++j;
                }
                crd_vals[j - 1] = key;
                --n;
        }
}


float assess_hand(char hand[HAND_SIZE][MX_CRD_NAME_LEN], unsigned int crd_vals[HAND_SIZE])
{
        void dupe_check(unsigned int crd_vals[HAND_SIZE], unsigned int *dupe_1, unsigned int *dupe_1_count, unsigned int *dupe_2, unsigned int *dupe_2_count);
        bool straight_check(unsigned int crd_vals[HAND_SIZE]);
        bool flush_check(char hand[HAND_SIZE][MX_CRD_NAME_LEN]);

        float hand_value = 0;

        unsigned int first_duplicate = crd_vals[0];
        unsigned int first_duplicate_count = 1;

        unsigned int second_duplicate = 0;
        unsigned int second_duplicate_count = 0;

        char flush_suit[MX_SUIT_NAME_LEN] = {0};

        bool pair = false;
        bool two_pair = false;
        bool three_of_a_kind = false;
        bool straight = false;
        bool flush = false;
        bool full_house = false;
        bool four_of_a_kind = false;
        bool straight_flush = false;
        bool royal_flush = false;

        float high_crd = crd_vals[HAND_SIZE - 1];

        dupe_check(crd_vals, &first_duplicate, &first_duplicate_count, &second_duplicate, &second_duplicate_count);

        if (first_duplicate_count == 2 || second_duplicate_count == 2)
        {
                pair = true;
        }

        if (first_duplicate_count == 2 && second_duplicate_count == 2)
        {
                two_pair = true;
        }

        if (first_duplicate_count == 3 || second_duplicate_count == 3)
        {
                three_of_a_kind = true;
        }

        straight = straight_check(crd_vals);
        flush = flush_check(hand);
        
        if (three_of_a_kind == true && pair == true)
        {
                full_house = true;
        }

        if (first_duplicate_count == 4 || second_duplicate_count == 4)
        {
                four_of_a_kind = true;
        }

        if (straight == true && flush == true)
        {
                straight_flush = true;
        }

        if (straight_flush == true)
        {
                hand_value += 8;
        }

        else if (four_of_a_kind == true)
        {
                hand_value += 7;
        }

        else if (full_house == true)
        {
                hand_value += 6;
        }

        else if (flush == true)
        {
                hand_value += 5;
        }

        else if (straight == true)
        {
                hand_value += 4;
        }

        else if (three_of_a_kind == true)
        {
                hand_value += 3;
        }

        else if (two_pair == true)
        {
                hand_value += 2;
        }

        else if (pair == true)
        {
                hand_value += 1;
        }

        hand_value += (high_crd / 100);

        return hand_value;
}

void dupe_check(unsigned int crd_vals[HAND_SIZE], unsigned int *dupe_1, unsigned int *dupe_1_count, unsigned int *dupe_2, unsigned int *dupe_2_count)
{

        for (size_t i = 1; i < HAND_SIZE; ++i)
        {
                if (*dupe_1_count > 1)
                {
                        if (*dupe_2_count > 0)
                        {
                                if (*dupe_2 == crd_vals[i])
                                {
                                        ++*dupe_2_count;
                                }
                        }

                        else
                        {
                                if (*dupe_1 == crd_vals[i])
                                {
                                        ++*dupe_1_count;
                                }

                                else
                                {
                                        *dupe_2 = crd_vals[i];
                                        ++*dupe_2_count;
                                }
                        }
                }

                if (*dupe_1_count < 2)
                {
                        if (*dupe_1 == crd_vals[i])
                        {
                                ++*dupe_1_count;
                        }

                        else
                        {
                                *dupe_1 = crd_vals[i];
                        }
                }

        }
}

bool straight_check(unsigned int crd_vals[HAND_SIZE]) 
{
        unsigned int previous_crd = crd_vals[0];
        unsigned int straight_count = 1;

        for (size_t i = 1; i < HAND_SIZE; ++i)
        {
                if (crd_vals[i] == ++previous_crd)
                {
                        ++straight_count;
                        previous_crd = crd_vals[i];
                }
        }

        if (straight_count == 5)
        {
                return true;
        }
        else
        {
                return false;
        }
}

bool flush_check(char hand[HAND_SIZE][MX_CRD_NAME_LEN])
{
        void get_suit(char crd[MX_CRD_NAME_LEN], char suit[MX_SUIT_NAME_LEN]);

        unsigned int flush_count = 0;

        char first_suit[MX_SUIT_NAME_LEN] = {0};
        char second_suit[MX_SUIT_NAME_LEN] = {0};
        char third_suit[MX_SUIT_NAME_LEN] = {0};
        char fourth_suit[MX_SUIT_NAME_LEN] = {0};
        char fifth_suit[MX_SUIT_NAME_LEN] = {0};

        char *hand_suit[HAND_SIZE] = {first_suit, second_suit, third_suit, fourth_suit, fifth_suit};

        for (size_t i = 0; i < HAND_SIZE; ++i)
        {
                get_suit(hand[i], hand_suit[i]);

                if (hand_suit[i] == hand_suit[0])
                {
                        ++flush_count; 
                }
        }

        if (flush_count == 5)
        {
                return true;
        }

        else 
        {
                return false;
        }
}

void get_suit(char crd[MX_CRD_NAME_LEN], char suit[MX_SUIT_NAME_LEN])
{
        size_t inp_letter_pos = 0;
        size_t outp_letter_pos = 0;

        while (crd[inp_letter_pos] != 'f')
        {
                ++inp_letter_pos;
        }

        inp_letter_pos += 2;

        while (crd[inp_letter_pos] != '\0')
        {
                suit[outp_letter_pos] = crd[inp_letter_pos];
                ++inp_letter_pos;
                ++outp_letter_pos;
        }
}

