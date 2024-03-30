#include<stdio.h>
#include<stdlib.h>
#include<queue>
#include<stack>
#define Rows 4 
#define Cols 4
#define EMPTY 0 
#define Maxlength 1000

using namespace std;
const char *action[] = {"First State",
						"Pour color tubes 1 to tubes 2",
						"Pour color tubes 1 to tubes 3",
						"Pour color tubes 1 to tubes 4",
						"Pour color tubes 2 to tubes 1",
						"Pour color tubes 2 to tubes 3",
						"Pour color tubes 2 to tubes 4",
						"Pour color tubes 3 to tubes 1",
						"Pour color tubes 3 to tubes 2",
						"Pour color tubes 3 to tubes 4",
						"Pour color tubes 4 to tubes 1",
						"Pour color tubes 4 to tubes 2",
						"Pour color tubes 4 to tubes 3"
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
		for(j=0;j<=Cols-1;j++)
			printf("|%d| ",state.M[i][j]);
		printf("\n");
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
	if (state.M[0][x]!=0 and state.M[3][y]==0){
		for (i = Rows-1; i >=0; i--) {
    	    if (state.M[i][x] != 0) {
        	    for (j = 0; j <Rows; j++) {
            	    if (state.M[j][y] == 0) {
                	    result->M[j][y] = state.M[i][x];
                    	result->M[i][x] = 0;
                    	return 1;
                	}
            	}
        	}
    	}
	}
    else return 0;	
}


typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;


int find_State(State state, queue<Node *> openQueue){
	while(!openQueue.empty()){
		if(compare_States(openQueue.front()->state,state))
			return 1;
		openQueue.pop();	
	}
	return 0;
}

Node* BFS_Algorithm(State state, State goal){
	queue<Node *> Open_BFS;
	queue<Node *> Close_BFS;
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent=NULL;
	root->no_function=0;
 	Open_BFS.push(root);
	while(!Open_BFS.empty()){
		Node* node = Open_BFS.front();
		Open_BFS.pop();
		Close_BFS.push(node);
		if(goalcheck(node->state,goal))
			return node;
		int cnt = 0;
		for(int i = 0; i < 4; ++i){
			for (int j = 0; j < 4; ++j){
				if (i != j){
					++cnt;
					State newstate;
					makeNull_State(&newstate);
					if(Pour_color(node->state,&newstate, i, j)){
						if(find_State(newstate,Close_BFS) || find_State(newstate,Open_BFS))
							continue;
						Node* newNode = (Node*)malloc(sizeof(Node));
						newNode->state=newstate;
						newNode->Parent = node;
						newNode->no_function= cnt;
						Open_BFS.push(newNode);	
					}
				}
			}
		}	
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	queue<Node *> queuePrint;
	stack<Node *> Swapstack;
	while(node->Parent!=NULL){
		queuePrint.push(node);
		node=node->Parent;
	}
	queuePrint.push(node);
	int no_action = 0;
	while(!queuePrint.empty()){
		Swapstack.push(queuePrint.front());
		queuePrint.pop();
	}
	while(!Swapstack.empty()){
		printf("\nAction %d: %s\n",no_action,action[Swapstack.top()->no_function]);
		print_State(Swapstack.top()->state);
		Swapstack.pop();
		no_action++;
	}
}
int main(){
	State startState;
	startState.M[0][0]=1;
	startState.M[0][1]=2;
	startState.M[0][2]=2;
	startState.M[0][3]=3;
	startState.M[1][0]=EMPTY;
	startState.M[1][1]=2;
	startState.M[1][2]=1;
	startState.M[1][3]=3;
	startState.M[2][0]=EMPTY;
	startState.M[2][1]=1;
	startState.M[2][2]=3;
	startState.M[2][3]=3;
	startState.M[3][0]=EMPTY;
	startState.M[3][1]=EMPTY;
	startState.M[3][2]=1;
	startState.M[3][3]=2;
	State goal;
	goal.M[0][0]=1;
	goal.M[0][1]=2;
	goal.M[0][2]=EMPTY;
	goal.M[0][3]=3;
	goal.M[1][0]=1;
	goal.M[1][1]=2;
	goal.M[1][2]=EMPTY;
	goal.M[1][3]=3;
	goal.M[2][0]=1;
	goal.M[2][1]=2;
	goal.M[2][2]=EMPTY;
	goal.M[2][3]=3;
	goal.M[3][0]=1;
	goal.M[3][1]=2;
	goal.M[3][2]=EMPTY;
	goal.M[3][3]=3;
	
	printf("Start state:\n");
	print_State(startState);
	printf("Goal state:\n");
	print_State(goal);
	Node* p=BFS_Algorithm(startState,goal);
	print_WaysToGetGoal(p);
	return 0;
}

