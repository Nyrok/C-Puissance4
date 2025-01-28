#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define PLAYERS 2
#define SIZE 16

char	CHIP[PLAYERS] = "XO";

char	board[SIZE][SIZE];

void	INIT_SCREEN(void)
{
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
}

void	DONE_SCREEN(void)
{
	endwin();
	exit(0);
}

void	init_board(void)
{
	unsigned int	i = 0;
	unsigned int	j = 0;

	while (i < SIZE)
	{
		j = 0;
		while (j < SIZE)
		{
			board[i][j] = ' ';
			j++;	
		}
		i++;
	}
}

void	draw_board(void)
{
	move(0, 0);
	unsigned int	i = 0;
	unsigned int	j = 0;
	char		c;

	while (i < SIZE)
	{
		j = 0;
		while (j < SIZE)
		{
			printw("|%c", board[i][j]);
			j++;
		}
		printw("|");
		printw("\n");
		i++;
	}
	i = 0;
	while (i < SIZE)
	{
		printw("+-");
		if (i + 1 == SIZE)
			printw("+");
		i++;
	}
	printw("\n");
	i = 0;
	c = 'A';
	while (i < SIZE)
	{
		printw(" %c", c++);
		i++;
	}
	printw("\n");
	refresh();
}

int	get_col(void)
{
	int c;
	
	while ((c = getch()))
	{
		if (c == KEY_BACKSPACE)
			break;
		if (c >= 'A' && c <= 'A' + SIZE - 1)
			return (c);
	}
	return (-1);
}

int	add_coin(int col, int player)
{
	unsigned int	i = 0;

	while (i < SIZE)
	{
		if (board[i][col - 'A'] == ' ')
			break;
		i++;
	}
	if(i == SIZE)
		return (player);
	i = 0;
	while (i < SIZE)
	{
		if (i == SIZE - 1 || board[i + 1][col - 'A'] != ' ')
		{
			board[i][col - 'A'] = CHIP[player];
			break;
		}
		i++;
	}
	move(0, 0);
	printw("good");
	refresh();
	if(player + 1 == PLAYERS)
		return (0);
	return (player + 1);
}

int	game_over(void)
{
	unsigned int i = 0;
	unsigned int j = 0;


	// Check ligne
	while (i < SIZE)
	{
		j = 0;
		while (j < SIZE)
		{
			if (board[i][j] != ' ' && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3])
				return board[i][j];
			j++;
		}
		i++;
	}

	// Check colonne
	i = 0;
	while (i < SIZE)
	{
		j = 0;
		while (j < SIZE)
		{
			if (board[j][i] != ' ' && board[j][i] == board[j + 1][i] && board[j][i] == board[j + 2][i] && board[j][i] == board[j + 3][i])
				return board[j][i];
			j++;
		}
		i++;
	}

	// Check diagonale 
	i = 0;
	while (i < SIZE - 4)
	{
		j = 0;
		while (j < SIZE - 4)
		{
			if (board[i][j] != ' ' && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3])
				return board[i][j];
			j++;
		}
		i++;
	}


	// Check diagonale inverse 
	i = 0;
	while (i < SIZE - 1)
	{
		j = SIZE - 1;
		while (j >= 3)
		{
			if (board[i][j] != ' ' && board[i][j] == board[i - 1][j - 1] && board[i][j] == board[i - 2][j - 2] && board[i][j] == board[i - 3][j - 3])
				return board[i][j];
			j--;
		}
		i++;
	}

	// Le jeu continue
	i = 0;
	while (i < SIZE)
	{
		j = 0;
		while (j < SIZE)
		{
			if (board[i][j] == ' ')
				return (0);
			j++;
		}
		i++;
	}

	// Match nul
	return (1);
}

void	play(void)
{
	int	winner;
	int	player = 0;
	int	col;

	move(SIZE + 2, 0);
	while ((winner = game_over()) == 0)
	{
		printw("player %c? ", CHIP[player]);
		refresh();
		col = get_col();
		if (col == -1) break;
		player = add_coin(col, player);
		draw_board();
	}
	move(SIZE + 2, 0);
	if (winner > 1)
		printw("player %c Won!", winner);
	else if (winner == 1)
		printw("game has drawn!");
	else
		printw("game has been aborted");
	refresh();
}

int	main(void)
{
	INIT_SCREEN();
	init_board();
	draw_board();
	play();
	getch();
	DONE_SCREEN();
	return (0);
}
