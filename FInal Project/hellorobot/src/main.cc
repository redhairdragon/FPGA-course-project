#include <stdio.h>
#include <xil_printf.h>
#include "platform.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

using namespace std;

#define BLANK '0'
#define WALL '1'
#define BOX '2'
#define KEEPER '3'
#define STAR '4'
#define BOXSTAR '5'
#define KEEPERSTAR '6'

#define COL_SIZE 4
#define ROW_SIZE 4

struct Coordinate{
    int r;
    int c;
};

// Some helper funtions for checking the content of a square
bool isBlank(char s){
    return (s == BLANK);
}
bool isWall(char s){
    return (s == WALL);
}
bool isBox(char s){
    return (s == BOX);
}
bool isKeeper(char s){
    return (s == KEEPER);
}
bool isStar(char s){
    return (s == STAR);
}
bool isBoxStar(char s){
    return (s == BOXSTAR);
}
bool isKeeperStar(char s){
    return (s == KEEPERSTAR);
}

// Return a Coordinate struct indicating the position
// of the keeper
struct Coordinate getKeeperPosition(char *map[]){
    struct Coordinate keeper = {0,0};
    for(int i=0; i < ROW_SIZE; i++){
        for(int j=0; j < COL_SIZE; j++){
            if(isKeeper(map[i][j]) || isKeeperStar(map[i][j])){
                keeper.r = i;
                keeper.c = j;
                return keeper;
            }
        }
    }
    return keeper;
}

// Return a Coordinate struct indicating the position of the box
struct Coordinate getBoxPosition(char *map[]){
    struct Coordinate box = {0, 0};
    for(int i=0; i < ROW_SIZE; i++){
        for(int j=0; j < COL_SIZE; j++){
            if(isBox(map[i][j])){
                box.r = i;
                box.c = j;
                return box;
            }
        }
    }
    return box;
}

// Return a Coordinate struct indicating the position of the star
struct Coordinate getStarPosition(char *map[]){
    struct Coordinate star = {0, 0};
    for(int i=0; i < ROW_SIZE; i++){
        for(int j=0; j < COL_SIZE; j++){
            if(isStar(map[i][j]) || isKeeperStar(map[i][j]) || isBoxStar(map[i][j])){
                star.r = i;
                star.c = j;
                return star;
            }
        }
    }
    return star;
}

// Return true if and only if s is a goal state of a Sokoban game.
// (no box is on a non-goal square)

bool goal_test(char* s[]){
    for(int i=0; i < ROW_SIZE; i++){
        for(int j=0; j < COL_SIZE; j++){
            if(isBox(s[i][j])){
                return false;
            }
        }
    }
    return true;
}

// Helper function for TRY-MOVE
// The function takes in a state s, a row number r, a column numberc.
// Return the content of state s at square (r,c)
// If the square is outside the scope of the problem, return the value of a Wall(1)
char get_square(char* s[], int r, int c){
    if (r >= ROW_SIZE || r < 0 || c >= COL_SIZE || c < 0){
        return WALL;
    }
    else{
        return s[r][c];
    }
}

// Helper function for TRY-MOVE
// The function takes in a state s, a row number r, a column number c and a square content v
// Returns a new state that is obtained by setting by the square (r,c) to value v
char** set_square(char* s[], int r, int c, char v){
    char** state = (char**)malloc(ROW_SIZE * sizeof(char*));
    for (int i=0; i < ROW_SIZE; i++){
        state[i] = (char*)malloc(COL_SIZE * sizeof(char));
    }
    for (int i=0; i < ROW_SIZE; i++){
        for (int j=0; j < COL_SIZE+1; j++){
            state[i][j] = s[i][j];
        }
    }
    state[r][c] = v;
    return state;
}

// Helper funtion for try-move
// The function takes in a state s and keeper's position r and c
// Returns the state that is the result of moving the keeper up in state s
// NULL is returned if the move is invalid
char** move_up(char** s, int r, int c, char next){
    char neighbor = get_square(s, r-1, c);
    char** state = (char**)malloc(ROW_SIZE * sizeof(char*));
    for (int i=0; i < ROW_SIZE; i++){
        state[i] = (char*)malloc(COL_SIZE * sizeof(char));
    }
    if(isWall(neighbor)){
        // up square is wall, invalid move
        return NULL;
    }
    if(isBlank(neighbor)){
        // up square is blank, it becomes keeper after move
        state = set_square(s, r, c, next);
        state = set_square(state, r-1, c, KEEPER);
        return state;
    }
    if(isStar(neighbor)){
        // up square is star, it becomes keeperstar after move
        state = set_square(s, r, c, next);
        state = set_square(state, r-1, c, KEEPERSTAR);
        return state;
    }
    if(isBox(neighbor)){
        // up square is box, we need to further check the upper square
        char upper = get_square(s, r-2, c);
        if(isWall(upper) || isBox(upper) || isBoxStar(upper)){
            // if upper square is wall or box or boxstar, invalid move
            return NULL;
        }
        if(isBlank(upper)){
            // if upper is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r-1, c, KEEPER);
            state = set_square(state, r-2, c, BOX);
            return state;
        }
        if(isStar(upper)){
            // if upper is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r-1, c, KEEPER);
            state = set_square(state, r-2, c, BOXSTAR);
            return state;
        }
    }
    if(isBoxStar(neighbor)){
        // up square is boxstar, we need to further check the upper square
        char upper = get_square(s, r-2, c);
        if(isWall(upper) || isBox(upper) || isBoxStar(upper)){
            // if up square is wall, or box, or boxstar, invalid move
            return NULL;
        }
        if(isBlank(upper)){
            // if upper square is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r-1, c, KEEPERSTAR);
            state = set_square(state, r-2, c, BOX);
            return state;
        }
        if(isStar(upper)){
            // if upper square is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r-1, c, KEEPERSTAR);
            state = set_square(state, r-2, c, BOXSTAR);
            return state;
        }
    }
    return NULL;
}

// Helper funtion for try-move
// The function takes in a state s and keeper's position r and c
// Returns the state that is the result of moving the keeper down in state s
// NULL is returned if the move is invalid
char** move_down(char** s, int r, int c, char next){
    char neighbor = get_square(s, r+1, c);
    char** state = (char**)malloc(ROW_SIZE * sizeof(char*));
    for (int i=0; i < ROW_SIZE; i++){
        state[i] = (char*)malloc(COL_SIZE * sizeof(char));
    }
    if(isWall(neighbor)){
        // down square is wall, invalid move
        return NULL;
    }
    if(isBlank(neighbor)){
        // down square is blank, it becomes keeper after move
        state = set_square(s, r, c, next);
        state = set_square(state, r+1, c, KEEPER);
        return state;
    }
    if(isStar(neighbor)){
        // down square is star, it becomes keeperstar after move
        state = set_square(s, r, c, next);
        state = set_square(state, r+1, c, KEEPERSTAR);
        return state;
    }
    if(isBox(neighbor)){
        // down square is box, we need to further check the down square
        char down = get_square(s, r+2, c);
        if(isWall(down) || isBox(down) || isBoxStar(down)){
            // if down square is wall or box or boxstar, invalid move
            return NULL;
        }
        if(isBlank(down)){
            // if down is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r+1, c, KEEPER);
            state = set_square(state, r+2, c, BOX);
            return state;
        }
        if(isStar(down)){
            // if down is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r+1, c, KEEPER);
            state = set_square(state, r+2, c, BOXSTAR);
            return state;
        }
    }
    if(isBoxStar(neighbor)){
        // up square is boxstar, we need to further check the down square
        char down = get_square(s, r+2, c);
        if(isWall(down) || isBox(down) || isBoxStar(down)){
            // if up square is wall, or box, or boxstar, invalid move
            return NULL;
        }
        if(isBlank(down)){
            // if down square is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r+1, c, KEEPERSTAR);
            state = set_square(state, r+2, c, BOX);
            return state;
        }
        if(isStar(down)){
            // if down square is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r+1, c, KEEPERSTAR);
            state = set_square(state, r+2, c, BOXSTAR);
            return state;
        }
    }
    return NULL;
}

// Helper funtion for try-move
// The function takes in a state s and keeper's position r and c
// Returns the state that is the result of moving the keeper left in state s
// NULL is returned if the move is invalid
char** move_left(char** s, int r, int c, char next){
    char neighbor = get_square(s, r, c-1);
    char** state = (char**)malloc(ROW_SIZE * sizeof(char*));
    for (int i=0; i < ROW_SIZE; i++){
        state[i] = (char*)malloc(COL_SIZE * sizeof(char));
    }
    if(isWall(neighbor)){
        // left square is wall, invalid move
        return NULL;
    }
    if(isBlank(neighbor)){
        // left square is blank, it becomes keeper after move
        state = set_square(s, r, c, next);
        state = set_square(state, r, c-1, KEEPER);
        return state;
    }
    if(isStar(neighbor)){
        // left square is star, it becomes keeperstar after move
        state = set_square(s, r, c, next);
        state = set_square(state, r, c-1, KEEPERSTAR);
        return state;
    }
    if(isBox(neighbor)){
        // left square is box, we need to further check the left square
        char left = get_square(s, r, c-2);
        if(isWall(left) || isBox(left) || isBoxStar(left)){
            // if left square is wall or box or boxstar, invalid move
            return NULL;
        }
        if(isBlank(left)){
            // if left is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c-1, KEEPER);
            state = set_square(state, r, c-2, BOX);
            return state;
        }
        if(isStar(left)){
            // if left is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c-1, KEEPER);
            state = set_square(state, r, c-2, BOXSTAR);
            return state;
        }
    }
    if(isBoxStar(neighbor)){
        // up square is boxstar, we need to further check the left square
        char left = get_square(s, r, c-2);
        if(isWall(left) || isBox(left) || isBoxStar(left)){
            // if up square is wall, or box, or boxstar, invalid move
            return NULL;
        }
        if(isBlank(left)){
            // if left square is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c-1, KEEPERSTAR);
            state = set_square(state, r, c-2, BOX);
            return state;
        }
        if(isStar(left)){
            // if left square is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c-1, KEEPERSTAR);
            state = set_square(state, r, c-2, BOXSTAR);
            return state;
        }
    }
    return NULL;
}

// Helper funtion for try-move
// The function takes in a state s and keeper's position r and c
// Returns the state that is the result of moving the keeper right in state s
// NULL is returned if the move is invalid
char** move_right(char** s, int r, int c, char next){
    char neighbor = get_square(s, r, c+1);
    char** state = (char**)malloc(ROW_SIZE * sizeof(char*));
    for (int i=0; i < ROW_SIZE; i++){
        state[i] = (char*)malloc(COL_SIZE * sizeof(char));
    }
    if(isWall(neighbor)){
        // right square is wall, invalid move
        return NULL;
    }
    if(isBlank(neighbor)){
        // right square is blank, it becomes keeper after move
        state = set_square(s, r, c, next);
        state = set_square(state, r, c+1, KEEPER);
        return state;
    }
    if(isStar(neighbor)){
        // right square is star, it becomes keeperstar after move
        state = set_square(s, r, c, next);
        state = set_square(state, r, c+1, KEEPERSTAR);
        return state;
    }
    if(isBox(neighbor)){
        // right square is box, we need to further check the right square
        char right = get_square(s, r, c+2);
        if(isWall(right) || isBox(right) || isBoxStar(right)){
            // if right square is wall or box or boxstar, invalid move
            return NULL;
        }
        if(isBlank(right)){
            // if right is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c+1, KEEPER);
            state = set_square(state, r, c+2, BOX);
            return state;
        }
        if(isStar(right)){
            // if right is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c+1, KEEPER);
            state = set_square(state, r, c+2, BOXSTAR);
            return state;
        }
    }
    if(isBoxStar(neighbor)){
        // up square is boxstar, we need to further check the right square
        char right = get_square(s, r, c+2);
        if(isWall(right) || isBox(right) || isBoxStar(right)){
            // if up square is wall, or box, or boxstar, invalid move
            return NULL;
        }
        if(isBlank(right)){
            // if right square is blank, it becomes box and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c+1, KEEPERSTAR);
            state = set_square(state, r, c+2, BOX);
            return state;
        }
        if(isStar(right)){
            // if right square is star, it becomes boxstar and box becomes keeper after move
            state = set_square(s, r, c, next);
            state = set_square(state, r, c+1, KEEPERSTAR);
            state = set_square(state, r, c+2, BOXSTAR);
            return state;
        }
    }
    return NULL;
}

// This is the top-level next_states (successor) function
char*** next_states(char** s){
    struct Coordinate keeper = getKeeperPosition(s);
    char content = get_square(s,keeper.r, keeper.c);
    char next;
    if(isKeeper(content)){
        next = BLANK;
    }else if (isKeeperStar(content)){
        next = STAR;
    }

    char*** result = (char***)malloc(4 * sizeof(char**));
    result[0] = move_up(s, keeper.r, keeper.c, next);
    result[1] = move_down(s, keeper.r, keeper.c, next);
    result[2] = move_left(s, keeper.r, keeper.c, next);
    result[3] = move_right(s, keeper.r, keeper.c, next);

    return result;
}

struct Node{
    Node* parent;
    char** state;
    int cost;
    char action;
    Node* next;
};

// Priority queue
Node* newNode(char** state, int cost, char action){
    Node* temp = (Node *)malloc(sizeof(Node));
    temp->parent = NULL;
    temp->state = state;
    temp->cost = cost;
    temp->action = action;
    temp->next = NULL;

    return temp;
}

void print_state(char* state[]){
	for(int i=0; i < ROW_SIZE; i++){
		xil_printf(state[i]);
		xil_printf("\n");
	}
}

void print_node(Node* node){
	print_state(node->state);
	xil_printf("cost: %d\n", node->cost);
	xil_printf("action: %c\n", node->action);
}
// return the node at head
Node* peek(Node** head){
    return *head;
}

// Remoces the element with the highest priority from the list
void pop(Node** head){
    (*head) = (*head)->next;
    // free(temp);
}

// function to check if list is empty
bool isEmpty(Node** head){
    return (*head) == NULL;
}

// function to push according to priority
void push(Node** head, Node* temp){
    // special case: the head of list has larger cost than
    // new node. So insert new node before head node and change
    // head node
    Node* start = (*head);

    if(isEmpty(head)){
        (*head) = temp;
        return;
    }
    if((*head)->cost > temp->cost){
        temp->next = *head;
        (*head) = temp;
    }else{
        // traverse the list and find a position to insert new node
        while(start->next != NULL && start->next->cost < temp->cost){
            start = start->next;
        }

        temp->next = start->next;
        start->next = temp;
    }
}

// set of char**
bool isSame(char* s1[], char*s2[]){
    if(!s1 || !s2){
        return false;
    }
    for(int i=0; i < ROW_SIZE; i++){
        for(int j=0; j < COL_SIZE; j++){
            if(s1[i][j] != s2[i][j]){
                return false;
            }
        }
    }
    return true;
}

bool isMember(Node** head, Node* temp){
    Node* start = (*head);

    if(isEmpty(head)){
        return false;
    }

    if(isSame((*head)->state, temp->state)){
        return true;
    }
    // traverse the list and find a position to insert new node
    while(start->next != NULL && !isSame(start->next->state, temp->state)){
        start = start->next;
    }

    if(start->next != NULL){
        return true;
    }

    return false;
}

void insert(Node** head, Node* temp){
    if(isMember(head, temp)){
        return;
    }

    Node* start = (*head);

    while(start->next != NULL){
        start = start->next;
    }

    start->next = temp;
}

int heuristic(char** state){
    Coordinate box = getBoxPosition(state);
    Coordinate star = getStarPosition(state);

    return abs(box.r-star.r) + abs(box.c-star.c);
}

Node* a_star(char** init){
    Node* frontier = newNode(init, heuristic(init), NULL);

    if(goal_test(frontier->state)){
        return frontier;
    }

    Node* explored = newNode(NULL, NULL, NULL);

    while(!isEmpty(&frontier)){
        Node* node = peek(&frontier);// choose the shallowest node in frontier
        pop(&frontier);

        if(goal_test(node->state)){
            return node;
        }

        insert(&explored, node);
        // next states
        char*** children = next_states(node->state);

        if(!children){
            continue;
        }

        for(int i=0; i < 4; i++){
            if(!children[i]){
                continue;
            }

            int h = heuristic(children[i]);
            char** child_state = children[i];
            int child_cost = node->cost - heuristic(node->state) + 1 + heuristic(children[i]);
            char child_action = NULL;
            switch(i){
                case 0: child_action = 'u';break;
                case 1: child_action = 'd';break;
                case 2: child_action = 'l';break;
                case 3: child_action = 'r';break;
            }
            Node* child = newNode(child_state, child_cost, child_action);
            child->parent = node;
            child->next = NULL;

            if(!isMember(&explored, child)){
            	push(&frontier, child);
            }

        }
    }
}

void string_reverse(char* str){
	char* end = str + strlen(str) - 1;
#define XOR_SWAP(a,b) do\
{\
	a ^= b;\
	b ^= a;\
	a ^= b;\
}while(0)

	while(str < end){
		XOR_SWAP(*str, *end);
		str++;
		end--;
	}
}
char* actions(char** state){
	char* actions = (char*) malloc(8);
	Node* result = a_star(state);
	actions[0] = result->action;
	int i = 1;
	while(result->parent != NULL){
		result = result->parent;
		actions[i]=result->action;
		i++;
	}
	string_reverse(actions);
	return actions;
}
int main(){

	init_platform();

    char *map1[] = {"4020",
                   	"1100",
                   	"3000",
                   	"0000"};

    char* act = actions(map1);
    xil_printf(act);

    cleanup_platform();
    return 0;
}


