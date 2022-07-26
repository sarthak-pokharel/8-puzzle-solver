
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Puzzle State
int dim = 3;
int *table;
int zero_pos;
int solved = 0;
int shuffle_iterations = 2;

// Tools
int sgn(int x){return x>0?1:(x<0?-1:0);}
int geti(int k){ return (k-1)/dim+1;}
int getj(int k){ return (k-1)%dim+1; }
int getij(int k, int *i, int *j){  *i = geti(k);  *j = getj(k); }
int getk(int i, int j) { return dim*(i-1) + j; }
int getelem_tableref(int index, int *table){ return table[index-1]; }
int getelem(int index){ return getelem_tableref(index,table); }
int setelem_tableref(int index,int val, int *table){ return table[index-1] = val; }
int setelem(int index, int val){ setelem_tableref(index, val, table); }
int setelemij(int i, int j, int val){ setelem(getk(i,j), val); }
void swapmem(int *x, int *y){int tmp; tmp = *x; *x = *y; *y = tmp;}
void swapel_tableref(int k1, int k2, int*table){
	int tmp;
	tmp = getelem_tableref(k1, table);
	setelem_tableref(k1, getelem_tableref(k2, table), table);
	setelem_tableref(k2, tmp, table);
}
void swapel(int k1, int k2){ swapel_tableref(k1, k2, table); }

int randomrange(int min, int max){ return rand() % (max + 1 - min) + min; }

int calc_inversion_forel(int elv, int ind){
	int k, max = dim*dim, el;
	int inv = 0;
	for(k=ind; k<max; k++){
		el = getelem(k);
		if(el<elv) inv += 1;
	}
	printf("Inversions for %d is %d\n", elv, inv);
	return inv;
}
int calc_inversions(){
	int k, max = dim*dim, inv_el;
	int total_inversions = 0;
	for(k=1; k<max; k++){
		total_inversions += calc_inversion_forel(getelem(k), k+1);
	}
	return total_inversions;
}

// Initialize table
void fill_table_random(){
	int i;
	int r1, r2;
	int max = dim*dim;

	for(i=1; i <= max; i++){
		table[i-1] = (i)%(max);
	}
	for(i = 0; i<shuffle_iterations; i++){
		r1 = randomrange(0,max-2);
		r2 = randomrange(0,max-2);
		if(r1==r2){
			shuffle_iterations+=1;
			continue;
		}
		swapmem(table+r1, table+r2);
	}
	zero_pos = dim*dim;
}

void print_game(int *gamematrix){
	int i,j,k,max;
	int elem;

	max = dim*dim;
	printf("\n\n");
	for(k=1; k<=max; k++){
		getij(k, &i, &j);
		elem = getelem_tableref(k,gamematrix);
		if(elem == 0) {
			printf("      ");
			if(j == dim) printf("\n");
			continue;
		}
		printf("  %2d  ", elem );
		if(j == dim) printf("\n");
	}
	printf("\n");
}

void calc_del_position(int i, int j, int *dx, int *dy){
	// printf("zpos(%d)\n", zero_pos);
	*dx = geti(zero_pos) - i;
	*dy = getj(zero_pos) - j;
}

int move_exists(int dx, int dy, int zero_pos, int *table){
	// printf("zp(%d %d) (k=%d)\n", geti(zero_pos), getj(zero_pos), zero_pos);
	if((dx != 0 && dy != 0) || (dx == 0 && dy == 0)) return 0;
	if(((geti(zero_pos)-dy) > dim) || ((getj(zero_pos)-dx)>dim)) return 0;
	if(((geti(zero_pos)-dy) <= 0) || ((getj(zero_pos)-dx)<=0)) return 0;
	return 1;
}
int change_hole_pos_ref(int dx, int dy, int *zpos, int *table){
	int zero_pos, domove;
	dx = -dx;dy = -dy;
	zero_pos = *zpos;
	//Verify if movement is possible or not
	domove = move_exists(dx,dy, zero_pos, table);
	// printf("mvex (%d %d) ",dx,dy);
	if(!domove) return -1;
	int swpelk, m;
	//Shift y:
	for(m=0; m<abs(dx); m+= 1){
		swpelk = getk(geti(zero_pos), getj(zero_pos)-sgn(dx));
		swapel_tableref(swpelk, zero_pos, table);
	}
	//Shift x:
	for(m=0; m<abs(dy); m+= 1){
		swpelk = getk(geti(zero_pos)-(sgn(dy)), getj(zero_pos));
		swapel_tableref(swpelk, zero_pos, table);
	}
	*zpos = swpelk;
}
void change_hole_pos(int dx, int dy) { change_hole_pos_ref(dx,dy, &zero_pos, table); }
void click_elem(int i, int j){
	int elemk, del_p[2];

	elemk = getk(i, j);
	calc_del_position(i,j, del_p, del_p+1);
	
	printf("dp(%d %d)\n", del_p[0], del_p[1]);

	change_hole_pos(-del_p[0], -del_p[1]);
}
void click_elem_k(int k){
	click_elem(geti(k),getj(k));
}
int verify_win(){
	int k, max;

	max = dim*dim;
	for(k=1; k<=max; k++){
		if(getelem(k) != (k%max)){
			return 0;
		}
	}
	return 1;
}
int *create_table_copy(){
	static int *ctable;
	int k = 0;
	ctable = malloc(dim*dim*sizeof(int));
	for(k=0; k<dim*dim; k++){
		ctable[k] = table[k];
	}
	return ctable;
}

int calc_manhattan_and_hamming(int *table){
	int k,el, total_manhatten = 0, mh, total_hamming = 0;
	for(k=1; k<=dim*dim; k++){
		el = getelem_tableref(k, table);
		mh = abs(geti(el)-geti(k))+abs(getj(el)-getj(k));
		if(el == 0) mh = 0;
		if(mh != 0 ) total_hamming+=1;
		total_manhatten += mh;
		// printf("m(%d, = %d) \n", el,mh);
	}
	// printf("total hamming = %d \n", total_hamming);
	// printf("om");
	// print_game(table);
	// total_manhatten = 0;
	// total_hamming = 0;
	return total_manhatten+total_hamming;
}


int possible_hole_moves[4][2] = 
		{  {-1,0}, {1,0},
		   {0,1}, {0,-1} };

int test_zero_pos = 9;

int get_manhattan_and_hamming_for_move(int *move, int *table){
	int val, didmove;
	didmove = change_hole_pos_ref(move[0], move[1], &test_zero_pos, table);
	if(didmove == -1) return didmove;
	// printf("Move of m(%d %d)\n", move[0], move[1]);
	val = calc_manhattan_and_hamming(table);
	// print_game(table);
	// printf("New test zero_pos = %d\n",test_zero_pos);
	change_hole_pos_ref(-move[0], -move[1],&test_zero_pos, table);
	// printf("reversing back\n");
	// print_game(table);
	return val;
}
int total_itr = 0;


int frozenmove[2] = {0,0};

void solve_bruteforce(){
	total_itr+=1;
	// if(total_itr>=5) return;
	int *cptable, i;
	int mhval, bestmoveindex = 0, leastmhval=9999;

	// printf("Solving now\n");
	cptable = create_table_copy();
	test_zero_pos = zero_pos;
	print_game(cptable);
	for(i=0;i<4;i++){
		if(possible_hole_moves[i][0] == frozenmove[0] && possible_hole_moves[i][1] == frozenmove[1]){
			// printf("move (%d %d) is frozen\n", possible_hole_moves[i][0],possible_hole_moves[i][1]);
			continue;
		}
		mhval = get_manhattan_and_hamming_for_move(possible_hole_moves[i], cptable);
		if(mhval == -1) {
			// printf("mhval(%d %d) == -1\n", possible_hole_moves[i][0],possible_hole_moves[i][1]);
			continue;
		};
		if(mhval<leastmhval){
			leastmhval = mhval;
			bestmoveindex = i;
		}
		// printf("MHval for move[%d] is %d \n",i, mhval);
	}
	// printf("hence best move is %d\n", bestmoveindex);
	change_hole_pos(possible_hole_moves[bestmoveindex][0],possible_hole_moves[bestmoveindex][1]);
	frozenmove[0] = -possible_hole_moves[bestmoveindex][0];
	frozenmove[1] = -possible_hole_moves[bestmoveindex][1];
	if(verify_win()){
		printf("Solved");
		return;
	}
	solve_bruteforce();
}

void init_puzzle(){
	table = malloc(dim*dim*sizeof(int));
	fill_table_random();
	print_game(table);
	solve_bruteforce();
	// change_hole_pos(-1,0);
	printf("\nLatest solution is with %d iters:\n", total_itr);
	print_game(table);
	// printf("total M+H = %d\n", calc_manhattan_and_hamming(table));
}

