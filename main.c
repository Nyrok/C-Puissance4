// ldd: -lncurses
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_WIDTH 7
#define DEFAULT_HEIGHT 6
#define DEFAULT_PLAYERS 2
#define MIN_WIDTH 4
#define MAX_WIDTH 26
#define MIN_HEIGHT 4
#define MAX_HEIGHT 16
#define MIN_PLAYERS 2
#define MAX_PLAYERS 8
#define ENV_WIDTH "P4WIDTH="
#define ENV_HEIGHT "P4HEIGHT="
#define ENV_PLAYERS "P4PLAYERS="

char	CHIP[MAX_PLAYERS] = "XOIKMSVZ";

int		WIDTH, HEIGHT, PLAYERS;
char	board[MAX_HEIGHT][MAX_WIDTH];

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
	int	i = 0;
	int	j = 0;

	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
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
	int		i = 0;
	int		j = 0;
	char	c;

	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			printw("|%c", board[i][j]);
			j++;
		}
		printw("|");
		printw("\n");
		i++;
	}
	i = 0;
	while (i < WIDTH)
	{
		printw("+-");
		if (i + 1 == WIDTH)
			printw("+");
		i++;
	}
	printw("\n");
	i = 0;
	c = 'A';
	while (i < WIDTH)
	{
		printw(" %c", c++);
		i++;
	}
	printw("\n");
	refresh();
}

int		get_col(void)
{
	int	c;

	while ((c = getch()))
	{
		if (c == KEY_BACKSPACE)
			break;
		if (c >= 'A' && c <= 'A' + WIDTH - 1)
			return (c);
		else if (c >= 'a' && c <= 'a' + WIDTH - 1)
			return (c - 32);
	}
	return (-1);
}

int		add_coin(int col, int player)
{
	int	i = 0;

	while (i < HEIGHT)
	{
		if (board[i][col - 'A'] == ' ')
			break;
		i++;
	}
	if (i == HEIGHT)
		return (player);
	i = 0;
	while (i < HEIGHT)
	{
		if (i == HEIGHT - 1 || board[i + 1][col - 'A'] != ' ')
		{
			board[i][col - 'A'] = CHIP[player];
			break;
		}
		i++;
	}
	if (player + 1 == PLAYERS)
		return (0);
	return (player + 1);
}

int		game_over(void)
{
	int	i = 0;
	int	j = 0;

	// Check ligne
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			if (board[i][j] != ' ' && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3])
				return (board[i][j]);
			j++;
		}
		i++;
	}

	// Check colonne
	i = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			if (board[j][i] != ' ' && board[j][i] == board[j + 1][i] && board[j][i] == board[j + 2][i] && board[j][i] == board[j + 3][i])
				return (board[j][i]);
			j++;
		}
		i++;
	}

	// Check diagonale 
	i = 0;
	while (i < HEIGHT - 3)
	{
		j = 0;
		while (j < WIDTH - 3)
		{
			if (board[i][j] != ' ' && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3])
				return (board[i][j]);
			j++;
		}
		i++;
	}

	// Check diagonale inverse 
	i = 0;
	while (i < HEIGHT - 3)
	{
		j = WIDTH - 1;
		while (j >= 3)
		{
			if (board[i][j] != ' ' && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3])
				return (board[i][j]);
			j--;
		}
		i++;
	}

	// Le jeu continue
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
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
	int winner;
	int player = 0;
	int col;

	move(HEIGHT + 2, 0);
	while ((winner = game_over()) == 0)
	{
		printw("player %c? ", CHIP[player]);
		refresh();
		col = get_col();
		if (col == -1) break;
		player = add_coin(col, player);
		draw_board();
	}
	move(HEIGHT + 2, 0);
	if (winner > 1)
		printw("player %c Won!", winner);
	else if (winner == 1)
		printw("game has drawn!");
	else
		printw("game has been aborted");
	refresh();
}

int		main(int argc, char **argv, char **envp)
{
	char *env_width = NULL;
	char *env_height = NULL;
	char *env_players = NULL;

	for (int i = 0; envp[i] != NULL; i++)
	{
		if (strncmp(envp[i], ENV_WIDTH, sizeof(ENV_WIDTH)) == 0)
			env_width = envp[i] + sizeof(ENV_WIDTH);
		if (strncmp(envp[i], ENV_HEIGHT, sizeof(ENV_HEIGHT)) == 0)
			env_height = envp[i] + sizeof(ENV_HEIGHT);
		if (strncmp(envp[i], ENV_PLAYERS, sizeof(ENV_PLAYERS)) == 0)
			env_players = envp[i] + sizeof(ENV_PLAYERS);
	}

	if (argc > 1)
	{
		WIDTH = atoi(argv[1]);
		if (WIDTH < MIN_WIDTH || WIDTH > MAX_WIDTH)
			WIDTH = DEFAULT_WIDTH;
	}
	else
	{
		WIDTH = (env_width) ? atoi(env_width) : DEFAULT_WIDTH;
		if (WIDTH < MIN_WIDTH || WIDTH > MAX_WIDTH)
			WIDTH = DEFAULT_WIDTH;
	}

	if (argc > 2)
	{
		HEIGHT = atoi(argv[2]);
		if (HEIGHT < MIN_HEIGHT || HEIGHT > MAX_HEIGHT)
			HEIGHT = DEFAULT_HEIGHT;
	}
	else
	{
		HEIGHT = (env_height) ? atoi(env_height) : DEFAULT_HEIGHT;
		if (HEIGHT < MIN_HEIGHT || HEIGHT > MAX_HEIGHT)
			HEIGHT = DEFAULT_HEIGHT;
	}

	if (argc > 3)
	{
		PLAYERS = atoi(argv[3]);
		if (PLAYERS < MIN_PLAYERS || PLAYERS > MAX_PLAYERS)
			PLAYERS = DEFAULT_PLAYERS;
	}
	else
	{
		PLAYERS = (env_players) ? atoi(env_players) : DEFAULT_PLAYERS;
		if (PLAYERS < MIN_PLAYERS || PLAYERS > MAX_PLAYERS)
			PLAYERS = DEFAULT_PLAYERS;
	}

	CHIP[PLAYERS] = "01234567"[PLAYERS-1];

	INIT_SCREEN();
	init_board();
	draw_board();
	play();
	getch();
	DONE_SCREEN();
	return (0);
}
