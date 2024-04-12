#include<stdio.h>
#include<stdlib.h>
#include<queue>
#include<stack>
#include<math.h>
#include<vector>
#include<algorithm>
#define Rows 4 
#define Cols 5
#define EMPTY 0 
#define Maxlength 1000

using namespace std;
const char *action[] = {"First State",
                        "Pour color tubes 1 to tubes 2",
                        "Pour color tubes 1 to tubes 3",
                        "Pour color tubes 1 to tubes 4",
                        "Pour color tubes 1 to tubes 5",
                        "Pour color tubes 2 to tubes 1",
                        "Pour color tubes 2 to tubes 3",
                        "Pour color tubes 2 to tubes 4",
                        "Pour color tubes 2 to tubes 5",
                        "Pour color tubes 3 to tubes 1",
                        "Pour color tubes 3 to tubes 2",
                        "Pour color tubes 3 to tubes 4",
                        "Pour color tubes 3 to tubes 5",
                        "Pour color tubes 4 to tubes 1",
                        "Pour color tubes 4 to tubes 2",
                        "Pour color tubes 4 to tubes 3",
                        "Pour color tubes 4 to tubes 5",
                        "Pour color tubes 5 to tubes 1",
                        "Pour color tubes 5 to tubes 2",
                        "Pour color tubes 5 to tubes 3",
                        "Pour color tubes 5 to tubes 4",
                       };

typedef struct {
    int M[Rows][Cols];
} State;

void makeNull_State(State* state) {
    int i,j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            state->M[i][j]=0;
}

void print_State(State state) {
	int i,j;
	for(i=Rows-1; i>=0; i--) {
		for(j=0; j<=Cols-1; j++)
			printf("|%d| ",state.M[i][j]);
		printf("\n");
	}
	for(i=0; i<Cols; i++) {
		printf("%c%c%c ",212, 205,190);
	}
	printf("\n");
}

int compare_States(State s1, State s2) {
    int i,j;
    for (i = 0; i < Rows; i++) {
        for (j = 0; j < Cols; j++) {
            if (s1.M[i][j] != s2.M[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

int goalcheck(State state, State GOAL) {
    return compare_States(state, GOAL);
}

int Pour_color(State state, State *result, int x, int y) {
    *result = state;
    int i, j;
    if (state.M[0][x] != 0 && state.M[3][y] == 0) {
        for (i = Rows - 1; i >= 0; i--) {
            if (state.M[i][x] != 0) {
                for (j = 0; j < Rows; j++) {
                    if (state.M[j][y] == 0){
                        if (j == 0 || (j > 0 && state.M[i][x] == state.M[j-1][y])){
                            result->M[j][y] = state.M[i][x];
                            result->M[i][x] = 0;
                            return 1;
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}


int heuristic(State current, State goal) {
    // Heuristic function using Manhattan distance
    int h = 0;
    for(int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            if(current.M[i][j] != goal.M[i][j]) {
                for(int k = 0; k < Rows; ++k) {
                    for (int l = 0; l < Cols; ++l) {
                        // Ki?m tra gi?i h?n c?a m?ng
                        if (k < Rows && l < Cols && current.M[i][j] == goal.M[k][l]) {
                            h += abs(i - k) + abs(j - l);
                            break;
                        }
                    }
                }
            }
        }
    }
    return h;
}

typedef struct Node{
    State s;
    struct Node* p;
    int no_function;
    int heuristic;
}Node;

Node *findState(State s, vector<Node*> v, vector<Node*>::iterator *position) {
    vector<Node*>::iterator it = v.begin();
    if(v.size() == 0) return NULL;
    while(it != v.end()) {
        if(compare_States((*it)->s, s)) {
            *position = it;
            return *it;
        }
        it++;
    }
    return NULL;
}

bool compareHeuristic(Node *a, Node *b){
    return a->heuristic > b->heuristic;
}

Node *bestFirstSearch(State s, State goal) {
    vector<Node*> open;
    vector<Node*> closed;
    Node *root = (Node*)malloc(sizeof(Node));
    root->s = s;
    root->p = NULL;
    root->no_function = 0;
    root->heuristic = heuristic(root->s, goal);
    open.push_back(root);
    while(!open.empty()) {
        Node *node = open.back();
        open.pop_back();
        closed.push_back(node);
        if(goalcheck(node->s, goal)) {
//            printf("Goal\n");
            return node;
        }
        int cnt = 0;
        for(int i = 0; i < Rows; ++i) {
            for (int j = 0; j < Cols; ++j) {
                if (i != j) {
                	++cnt;
                    State newState;
                    newState = node->s;
                    if(Pour_color(node->s, &newState, i,j)) {
                        Node* newNode = (Node*)malloc(sizeof(Node));
                        newNode->s = newState;
                        newNode->p = node;
                        newNode->no_function = cnt;
                        newNode->heuristic = heuristic(newState, goal);
                        vector<Node*>::iterator posOpen, posClosed;
                        Node *nodeFoundOpen = findState(newState, open, &posOpen);
                        Node *nodeFoundClosed = findState(newState, closed, &posClosed);
                        if(nodeFoundOpen == NULL && nodeFoundClosed == NULL)
                            open.push_back(newNode);
                        else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic) {
                            open.erase(posOpen);
                            open.push_back(newNode);
                        }
                        else if(nodeFoundClosed != NULL && nodeFoundClosed->heuristic > newNode->heuristic) {
                            closed.erase(posClosed);
                            open.push_back(newNode);
                        }
                    }
                }
            }
        }
        sort(open.begin(), open.end(), compareHeuristic);
    }
    return NULL;
}


void printWays(Node *node){
    vector<Node*> vectorPrint;
    vectorPrint.clear();
    while(node->p != NULL){
        vectorPrint.push_back(node);
        node = node->p;
    }
    vectorPrint.push_back(node);
    int no_act = 0, i;
    for(i = vectorPrint.size() - 1; i>=0; i--){
        printf("\nAction %d: %s\n", no_act, action[vectorPrint.at(i)->no_function]);
        print_State(vectorPrint.at(i)->s);
        no_act++;
    }
}

int main(){
    State startState;
//	tubes1

	startState.M[3][0]=1;
	startState.M[2][0]=2;
	startState.M[1][0]=3;
	startState.M[0][0]=1;

//	Tubes2

	startState.M[3][1]=3;
	startState.M[2][1]=1;
	startState.M[1][1]=2;
	startState.M[0][1]=2;

//	Tubes3
	startState.M[3][2]=3;
	startState.M[2][2]=1;
	startState.M[1][2]=2;
	startState.M[0][2]=3;
//	Tubes4

	startState.M[3][3]=0;
	startState.M[2][3]=0;
	startState.M[1][3]=0;
	startState.M[0][3]=0;

//	Tubes5
	startState.M[3][4]=0;
	startState.M[2][4]=0;
	startState.M[1][4]=0;
	startState.M[0][4]=0;

	State goal;
//	Tubes1
	goal.M[3][0]=0;
	goal.M[2][0]=0;
	goal.M[1][0]=0;
	goal.M[0][0]=0;

//	Tubes2
	goal.M[3][1]=0;
	goal.M[2][1]=0;
	goal.M[1][1]=0;
	goal.M[0][1]=0;

//	Tubes3
	goal.M[3][2]=3;
	goal.M[2][2]=3;
	goal.M[1][2]=3;
	goal.M[0][2]=3;

//	Tubes4

	goal.M[3][3]=2;
	goal.M[2][3]=2;
	goal.M[1][3]=2;
	goal.M[0][3]=2;

//	Tubes5
	goal.M[3][4]=1;
	goal.M[2][4]=1;
	goal.M[1][4]=1;
	goal.M[0][4]=1;
    
    printf("Start state:\n");
    print_State(startState);
    printf("Goal state:\n");
    print_State(goal);
    Node* p = bestFirstSearch(startState, goal);
    printWays(p);
    return 0;
}

