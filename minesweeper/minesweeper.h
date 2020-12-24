enum MoveResult { INVALID_MOVE=-3, REDUNDANT_MOVE=-2, BLOWN_UP=-1, SOLVED_BOARD=1, VALID_MOVE=0 };

void load_board(const char *filename, char board[9][9]);
void display_board(const char board[9][9]);
void initialise_board(char board[9][9]);

#define BD_SIZE 9

//q1
bool is_complete(char mines[][BD_SIZE], char revealed[][BD_SIZE]);

//q2
int count_mines(const char* position, char mines[][BD_SIZE]);

//q3
MoveResult make_move(const char* position, char mines[][BD_SIZE], char revealed[][BD_SIZE]);

//q4
bool find_safe_move(char revealed[][BD_SIZE], char* move);
bool helper(int a, int b, char revealed[][BD_SIZE], char* move);
