const int SCENE_HEIGHT=24, SCENE_WIDTH=72;

enum Status {VALID_GOAL_STATE=2, VALID_NONGOAL_STATE=1,ERROR_INVALID_CANNIBAL_COUNT=-1, ERROR_INVALID_MISSIONARY_COUNT=-2, ERROR_INVALID_MOVE=-3, ERROR_MISSIONARIES_EATEN=-4, ERROR_INPUT_STREAM_ERROR=-5, ERROR_BONUS_NO_SOLUTION=-6};

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns);

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene();

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene);

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene);

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line);

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename);

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code);

// Q1
char** make_river_scene(const char* left, const char* boat);


//Q2
int perform_crossing(char* left, const char* target);

void remove_B(char* str);

void add_B(char* str);

int count_char(const char* str, char ch);

// Q3
int play_game();


// Bonus challenge
int find_solution(const char* left, char* answer);

bool solver(char* left, char* answer, const char* last_target);

int perform_crossing2(char* left, const char* target);
