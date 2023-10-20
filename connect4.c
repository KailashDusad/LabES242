#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "math.h"

/* Define constants for player colors and board dimensions. */
enum
{
    EMPTY = 0,
    RED = 1,
    BLUE = 2,
};

typedef char board_t[4][5];
typedef char player_t;

/* Define a structure to represent a move. */
typedef struct
{
    int col;
    int score;
} move_t;

bool has_won(board_t board, player_t player)
{
    for (int i = 0; i < 4; i++)
    {
        // int row_win = 1;
        for (int j = 0; j < 2; j++)
        {
            if (board[i][j] == player && board[i][j+1] == player && board[i][j+2] == player && board[i][j+3] == player)
            {
                return true;
            }
        }
    
    }

    for (int j = 0; j < 4; j++)
    {
        // int col_win = 1;
        for (int i = 0; i < 1; i++)
        {
            if (board[i][j] == player && board[i+1][j] == player && board[i+2][j] == player && board[i+3][j] == player)
            {
                return true;
            }
        }
        
    }
    // for(int i = 0; i<4;i++){
    //     for(int j =0; j<5; j++){
    //         if()
    //     }
    // }
    int d2_win = 1;
    for (int i = 0; i < 4; i++)
    {
        if (board[i][i] != player)
        {
            d2_win = 0;
            break;
        }
    }
    if (d2_win)
        return true;

    int d3_win = 1;
    for (int i = 0; i < 4; i++)
    {
        if (board[i][3 - i] != player)
        {
            d3_win = 0;
            break;
        }
    }
    if (d3_win)
        return true;

    return false;
}

bool is_full(board_t board)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == EMPTY)
                return false;
    return true;
}

player_t other_player(player_t player)
{
    if (player == RED)
    {
        return BLUE;
    }
    else if (player == BLUE)
    {
        return RED;
    }
    else
    {
        assert(0);
    }
}

move_t best_moves[33554432];

void init_game(board_t board)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
            board[i][j] = EMPTY;

    int j = 0;
    while (j < 33554432)
    {
        move_t m = {-2, -1};
        best_moves[j] = m;
        j++;
    }
}

move_t best_move(board_t board, player_t player)
{
    int fi = 0, si = 0;
    for (int i = 0; i < 5; i++)
    {
        int f = 0, bm = 0;
        for (int j = 3; j >= 0; j--)
        {
            if (board[j][i] != EMPTY)
            {
                if (!f)
                {
                    if (board[j][i] == player)
                        fi += (1 << i);
                    f = (int)board[j][i];
                }
                if (f == board[j][i])
                    bm = (bm << 1) + 1;
                else
                    bm <<= 1;
            }
        }
        si += bm * (1 << (4 * i));
    }
    si += fi * (1 << 20);

    if (best_moves[si].col != -2)
    {
        return best_moves[si];
    }

    move_t m, bm = {-1, -21}, draw = {-1, 0}, won = {-1, 21}, lost = {-1, -21}, mm;

    if (has_won(board, player))
    {
        best_moves[si] = won;
        return won;
    }

    if (has_won(board, other_player(player)))
    {
        best_moves[si] = lost;
        return lost;
    }

    if (is_full(board))
    {
        best_moves[si] = draw;
        return draw;
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 3; j >= 0; j--)
        {
            if (board[j][i] == EMPTY)
            {
                board[j][i] = player;
                m = best_move(board, other_player(player));
                board[j][i] = EMPTY;
                mm.col = i;

                if (m.score > 0)
                    mm.score = -m.score + 1;
                else if (m.score < 0)
                    mm.score = -m.score - 1;
                else
                    mm.score = 0;

                if (mm.score > bm.score)
                    bm = mm;
            }
        }
    }
    best_moves[si] = bm;
    return bm;
}

void play_move(board_t board, move_t move, player_t player)
{
    for (int i = 3; i >= 0; i--)
    {
        if (board[i][move.col] == EMPTY)
        {
            board[i][move.col] = player;
            return;
        }
    }
}

void print_board(board_t board)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            switch (board[i][j])
            {
            case RED:
                printf("R ");
                break;
            case BLUE:
                printf("B ");
                break;
            case EMPTY:
                printf(". ");
                break;
            default:
                assert(0);
            }
        }
        printf("\n");
    }
}

int main()
{
    /* Game play logic. */
    /* The user can select the red or blue player. */
    char c;
    printf("Enter your color (R/B): ");
    scanf(" %c", &c);
    board_t board;
    player_t comp, user, curr = RED;
    init_game(board);
    if (c == 'R')
    {
        comp = BLUE;
        user = RED;
    }
    else if (c == 'B')
    {
        comp = RED;
        user = BLUE;
    }
    else
    {
        assert(0);
    }
    while (!is_full(board) && !has_won(board, user) && !has_won(board, comp))
    {
        if (curr == comp)
        {
            play_move(board, best_move(board, comp), comp);
        }
        else
        {
            print_board(board);
            int choice;
            printf("Enter your move: ");
            scanf("%d", &choice);
           
            move_t m = {choice, 0};
            play_move(board, m, user);
        }
        if (has_won(board, user))
        {
            printf("You Won!\n");
            print_board(board);
        }
        else if (has_won(board, comp))
        {
            printf("You Lost.\n");
            print_board(board);
        }
        else if (is_full(board))
        {
            printf("It's a Draw.\n");
            print_board(board);
        }
        curr = other_player(curr);
    }
    return 0;
}