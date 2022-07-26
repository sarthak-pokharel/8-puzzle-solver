
void init_puzzle();

typedef struct {
	int *state; //Table
	int hamming_val; //Hamming value for current node 
	int *frozen_moves; // List of frozen moves
	int frozen_moves_len; //Length of valid frozen moves
	void *parentNode; //Origin node
	void *childrenNodes; // List of children Nodes
	int childrenNodesLen; //Length of list of children nodes
	int isRootNode; //Is it root seeding node? 0/1
	int zpos;
} Node;



