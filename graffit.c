

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#ifndef __testing
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
	char name[MAX_STR_LEN];
	struct friend_node_struct *friends;
	struct brand_node_struct *brands;
	bool visited;
} User;

typedef struct friend_node_struct
{
	User *user;
	struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
	char brand_name[MAX_STR_LEN];
	struct brand_node_struct *next;
} BrandNode;

/** Note: We are reusing the FriendNode here as a Linked List for allUsers.
 * This is usually bad coding practice but it will allow us to reuse the
 * helper functions.
 **/
FriendNode *allUsers;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Checks if a user is inside a FriendNode LL.
 **/
bool in_friend_list(FriendNode *head, User *node)
{
	for (FriendNode *cur = head; cur != NULL; cur = cur->next)
	{
		if (strcmp(cur->user->name, node->name) == 0)
		{
			return true;
		}
	}
	return false;
}

/**
 * Checks if a brand is inside a BrandNode LL.
 **/
bool in_brand_list(BrandNode *head, char *name)
{
	for (BrandNode *cur = head; cur != NULL; cur = cur->next)
	{
		if (strcmp(cur->brand_name, name) == 0)
		{
			return true;
		}
	}
	return false;
}

/**
 * Inserts a User into a FriendNode LL in sorted position. If the user
 * already exists, nothing is done. Returns the new head of the LL.
 **/
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
	if (node == NULL)
		return head;

	if (in_friend_list(head, node))
	{
		printf("User already in list\n");
		return head;
	}
	FriendNode *fn = calloc(1, sizeof(FriendNode));
	fn->user = node;
	fn->next = NULL;

	if (head == NULL)
		return fn;

	if (strcmp(head->user->name, node->name) > 0)
	{
		fn->next = head;
		return fn;
	}

	FriendNode *cur;
	for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
		 cur = cur->next)
		;
	fn->next = cur->next;
	cur->next = fn;
	return head;
}

/**
 * Inserts a brand into a BrandNode LL in sorted position. If the brand
 * already exists, nothing is done. Returns the new head of the LL.
 **/
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
	if (node == NULL)
		return head;

	if (in_brand_list(head, node))
	{
		printf("Brand already in list\n");
		return head;
	}
	BrandNode *fn = calloc(1, sizeof(BrandNode));
	strcpy(fn->brand_name, node);
	fn->next = NULL;

	if (head == NULL)
		return fn;

	if (strcmp(head->brand_name, node) > 0)
	{
		fn->next = head;
		return fn;
	}

	BrandNode *cur;
	for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
		 cur = cur->next)
		;
	fn->next = cur->next;
	cur->next = fn;
	return head;
}

/**
 * Deletes a User from FriendNode LL. If the user doesn't exist, nothing is
 * done. Returns the new head of the LL.
 **/
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
	if (node == NULL)
		return head;

	if (!in_friend_list(head, node))
	{
		printf("User not in list\n");
		return head;
	}

	if (strcmp(head->user->name, node->name) == 0)
	{
		FriendNode *temp = head->next;
		free(head);
		return temp;
	}

	FriendNode *cur;
	for (cur = head; cur->next->user != node; cur = cur->next)
		;

	FriendNode *temp = cur->next;
	cur->next = temp->next;
	free(temp);
	return head;
}

/**
 * Deletes a brand from BrandNode LL. If the user doesn't exist, nothing is
 * done. Returns the new head of the LL.
 **/
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
	if (node == NULL)
		return head;

	if (!in_brand_list(head, node))
	{
		printf("Brand not in list\n");
		return head;
	}

	if (strcmp(head->brand_name, node) == 0)
	{
		BrandNode *temp = head->next;
		free(head);
		return temp;
	}

	BrandNode *cur;
	for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
		;

	BrandNode *temp = cur->next;
	cur->next = temp->next;
	free(temp);
	return head;
}

/**
 * Prints out the user data.
 **/
void print_user_data(User *user)
{
	printf("User name: %s\n", user->name);
	printf("Friends:\n");
	for (FriendNode *f = user->friends; f != NULL; f = f->next)
	{
		printf("   %s\n", f->user->name);
	}
	printf("Brands:\n");
	for (BrandNode *b = user->brands; b != NULL; b = b->next)
	{
		printf("   %s\n", b->brand_name);
	}
}

/**
 * Get the index into brand_names for the given brand name. If it doesn't
 * exist in the array, return -1
 **/
int get_brand_index(char *name)
{
	for (int i = 0; i < MAT_SIZE; i++)
	{
		if (strcmp(brand_names[i], name) == 0)
		{
			return i;
		}
	}
	printf("brand '%s' not found\n", name);
	return -1; // Not found
}

/**
 * Print out brand name, index and similar brands.
 **/
void print_brand_data(char *brand_name)
{
	int idx = get_brand_index(brand_name);
	if (idx < 0)
	{
		printf("Brand '%s' not in the list.\n", brand_name);
		return;
	}
	printf("Brand name: %s\n", brand_name);
	printf("Brand idx: %d\n", idx);
	printf("Similar brands:\n");
	for (int i = 0; i < MAT_SIZE; i++)
	{
		if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
		{
			printf("   %s\n", brand_names[i]);
		}
	}
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
	// Read the file
	char buff[MAX_STR_LEN];
	FILE *f = fopen(file_name, "r");
	fscanf(f, "%s", buff);
	char *line = buff;
	// Load up the brand_names matrix
	for (int i = 0; i < MAT_SIZE; i++)
	{
		if (i == MAT_SIZE - 1)
		{
			strcpy(brand_names[i], line);
			break;
		}
		int index = strchr(line, ',') - line;
		strncpy(brand_names[i], line, index);
		line = strchr(line, ',') + sizeof(char);
	}
	// Load up the brand_adjacency_matrix
	for (int x = 0; x < MAT_SIZE; x++)
	{
		fscanf(f, "%s", buff);
		for (int y = 0; y < MAT_SIZE; y++)
		{
			int value = (int)buff[y * 2];
			if (value == 48)
			{
				value = 0;
			}
			else
			{
				value = 1;
			}
			brand_adjacency_matrix[x][y] = value;
		}
	}
}
///////////////////////**********************************************************///////////////////////////
// Users
/**TODO: Complete this function
 * Creates and returns a user. Returns NULL on failure.
 **/
User *create_user(char *name)
{
	User *new = (User *)calloc(1, sizeof(User));
	if (new == NULL)
	{
		return NULL;
	}
	strcpy(new->name, name);
	new->friends = NULL;
	new->brands = NULL;
	new->visited = false; // false or true?
	// link to allUSers?
	allUsers = insert_into_friend_list(allUsers, new);
	return new;
}

/**TODO: Complete this function
 * Deletes a given user.
 * Returns 0 on success, -1 on failure.
 **/
int delete_user(User *user)
{
	if (user == NULL || allUsers == NULL)
	{
		return -1;
	}
	FriendNode *curr = allUsers;
	while (curr != NULL)
	{
		if (in_friend_list(curr->user->friends, user) == 1)
		{
			curr->user->friends = delete_from_friend_list(curr->user->friends, user);
		}
		curr = curr->next;
	}
	allUsers = delete_from_friend_list(allUsers, user);
	user = NULL;
	return 0;
}

/**TODO: Complete this function
 * Create a friendship between user and friend.
 * Returns 0 on success, -1 on failure.
 **/
int add_friend(User *user, User *friend)
{
	if (user == NULL || friend == NULL || allUsers == NULL)
	{
		return -1;
	}
	if (in_friend_list(allUsers, user) == 0 || in_friend_list(allUsers, friend) == 0)
	{
		return -1;
	}
	user->friends = insert_into_friend_list(user->friends, friend);
	friend->friends = insert_into_friend_list(friend->friends, user);
	return 0;
}

/**TODO: Complete this function
 * Removes a friendship between user and friend.
 * Returns 0 on success, -1 on faliure.
 **/
int remove_friend(User *user, User *friend)
{
	if (user == NULL || friend == NULL || allUsers == NULL)
	{
		return -1;
	}
	user->friends = delete_from_friend_list(user->friends, friend);
	friend->friends = delete_from_friend_list(friend->friends, user);
	return 0;
}

/**TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int follow_brand(User *user, char *brand_name)
{
	int ret = 0;
	if (user == NULL || brand_name == NULL)
	{
		ret = -1;
	}
	if (get_brand_index(brand_name) < 0)
	{
		ret = -1;
	}
	else if (get_brand_index(brand_name) >= 0)
	{
		user->brands = insert_into_brand_list(user->brands, brand_name);
		ret = 0;
	}
	return ret;
}

/**TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int unfollow_brand(User *user, char *brand_name)
{
	if (user == NULL || brand_name == NULL)
	{
		return -1;
	}
	if (get_brand_index(brand_name) >= 0)
	{
		user->brands = delete_from_brand_list(user->brands, brand_name);
		return 0;
	}
	return -1;
}

/**TODO: Complete this function
 * Return the number of mutual friends between two users.
 **/
int get_mutual_friends(User *a, User *b)
{
	if (a == NULL || b == NULL)
	{
		return 0;
	}
	int count = 0;
	while (a->friends != NULL && b->friends != NULL)
	{
		if (strcmp(a->friends->user->name, b->friends->user->name) == 0)
		{
			count++;
			/* printf("mutuals: %s", a->friends->user->name);
			printf("  ==  ");
			printf("mutuals: %s\n", b->friends->user->name); */
		}
		a->friends = a->friends->next;
		b->friends = b->friends->next;
	}
	return count;
}

void make_all_false(FriendNode *p)
{
	while (p != NULL)
	{
		printf("user->name:%s\n", p->user->name);
		p->user->visited = false;
		p = p->next;
	}
	return;
}

///////////////// HELPER LL ///////////////////////
typedef struct my_ll_node
{
	User *u;
	struct my_ll_node *next;
} ll_node;

typedef struct my_ll
{
	ll_node *head;
	ll_node *tail;
} ll;

void initialize_linked_list(ll *my_ll)
{
	if (my_ll == NULL)
	{
		return;
	}
	my_ll->head = NULL;
	my_ll->tail = NULL;
	return;
}

bool is_linked_list_empty(ll *my_ll)
{
	return my_ll->head == NULL;
}

void insert_tail(ll *my_ll, User *user)
{
	if (user == NULL)
	{
		return;
	}
	ll_node *new_node = (ll_node *)calloc(1, sizeof(ll_node));
	if (new_node == NULL)
	{
		return;
	}
	new_node->u = user;
	new_node->next = NULL;

	if (my_ll->head == NULL)
	{
		my_ll->head = new_node;
		my_ll->tail = new_node;
	}
	else
	{
		my_ll->tail->next = new_node;
		my_ll->tail = new_node;
	}
	return;
}

FriendNode *remove_head(ll *my_ll)
{
	if (my_ll == NULL)
	{
		return NULL;
	}
	FriendNode *ret = NULL;

	ll_node *first = my_ll->head;
	ret = first->u->friends;
	my_ll->head = first->next;
	if (my_ll->head == NULL)
	{
		my_ll->tail = NULL;
		free(first);
	}
	return ret;
}

/////////////////QUEUE CODE///////////////////////

typedef struct Queue
{
	int count;
	ll my_ll;
} Queue;

void initialize_queue(Queue *my_queue)
{
	initialize_linked_list(&(my_queue->my_ll));
	my_queue->count = 0;
}

void enqueue(Queue *my_queue, User *head)
{
	if (head == NULL || my_queue == NULL)
	{
		return;
	}
	// insert to tail to the queue
	insert_tail(&(my_queue->my_ll), head);
}

FriendNode *dequeue(Queue *my_queue)
{
	if (my_queue == NULL)
	{
		return NULL;
	}
	// remove and return head of the queue
	my_queue->count++;
	return remove_head(&(my_queue->my_ll));
}

bool is_queue_empty(Queue *my_queue)
{
	// return true iff the stack is empty
	return is_linked_list_empty(&(my_queue->my_ll));
}

/////////////////////////////////////////////////////////////////

/**TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another.
 *
 * For example, if X & Y are friends, then we expect to recieve 1 when calling
 * this on (X,Y). Continuing on, if Y & Z are friends, then we expect to
 * recieve 2 when calling this on (X,Z).
 *
 * Returns a non-negative integer representing the degrees of connection
 * between two users, -1 on failure.
 **/

int get_degrees_of_connection(User *a, User *b)
{
	if (a == NULL || b == NULL)
	{
		return -1;
	}
	make_all_false(allUsers);
	Queue Q;
	Queue *q = &Q;
	initialize_queue(q);
	User *head = a;
	// printf("NQ: %s\n",head->name);
	enqueue(q, a);
	a->visited = true;
	if (strcmp(head->name, b->name) == 0)
	{
		return q->count;
	}
	while (!is_queue_empty(q))
	{
		FriendNode *curr = NULL;
		curr = dequeue(q);
		// printf("DQ return: %s\n",head->friends->user->name);
		while (curr != NULL)
		{
			// printf("count = %d\n",q->count);
			if (curr->user->visited == false)
			{
				curr->user->visited = 1;
				// printf("NQ: %s\n",curr->user->name);
				enqueue(q, curr->user);
			}
			if (strcmp(curr->user->name, b->name) == 0)
			{
				return q->count;
			}
			// printf("curr->user->name = %s\n",curr->user->name);
			// printf("curr->next->user->name = %s\n",curr->next->user->name);//prob w curr->next
			curr = curr->next;
		}
		// printf("hello\n");
	}
	return -1;
}

// Brands5
/**TODO: Complete this function
 * Marks two brands as similar.
 **/
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
	int a = get_brand_index(brandNameA);
	int b = get_brand_index(brandNameB);
	if (a == -1 || b == -1)
	{
		return;
	}
	brand_adjacency_matrix[a][b] = 1;
	brand_adjacency_matrix[b][a] = 1;
	return;
}

/**TODO: Complete this function
 * Marks two brands as not similar.
 **/
void remove_similar_brands(char *brandNameA, char *brandNameB)
{
	int a = get_brand_index(brandNameA);
	int b = get_brand_index(brandNameB);
	if (a == -1 || b == -1)
	{
		return;
	}
	brand_adjacency_matrix[a][b] = 0;
	brand_adjacency_matrix[b][a] = 0;
	return;
}

// Harder ones
/**TODO: Complete this function
 * Returns a suggested friend for the given user, returns NULL on failure.
 * See the handout for how we define a suggested friend.
 **/

typedef struct mutual_users
{
	FriendNode *mutual;
	struct mutual_users *next;
} mutuals;
typedef struct non_mutual_users
{
	FriendNode *not_mutual;
	struct non_mutual_users *next;
} not_mutuals;
// completely diff names
// sam vs sammy kinda examples
// prioritize uppercase //z>a>A>Z
// some words similar(starting)
User *get_suggested_friend(User *user)
{
	/*	//User* get_suggested_friend(User* user){
		if (user == NULL){
			return NULL;
		}
		User* pointer = NULL;
		int counter = 0;
		FriendNode* p = NULL;
		p = allUsers;
		while (p != NULL){
			if(strcmp(p->user->name, user->name) == 0){
				// do nothing
			}else if (in_friend_list(p->user->friends, user) == false){
				if (user->brands == NULL){
					if (p->user->brands == NULL){
						if ( pointer == NULL){
							pointer = p->user;
						}else if (strcmp(p->user->name, pointer->name) > 0){
							pointer = p->user;
						}
					}
				}else{
					int temp = 0;
					BrandNode* head = NULL;
					head = p->user->brands;
					while (head != NULL){
						if (in_brand_list(user->brands, head->brand_name)){
								temp ++;
						}
						head = head -> next;
					}
					if ( temp > counter){
						counter = temp;
						pointer = p->user;
					}else if ( temp == counter){
						if (pointer == NULL){
							pointer = p->user;
						}else if (strcmp(p->user->name, pointer->name) > 0){
							pointer = p->user;
						}
					}
				}
			}
			p = p-> next;
		}
		return pointer;
	}
	*/
	if (user == NULL || allUsers == NULL)
	{
		return NULL;
	}
	mutuals M;
	not_mutuals M2;
	mutuals *m = &M;
	not_mutuals *m1 = &M2;
	m->next = NULL;
	m1->next = NULL;
	m->mutual = NULL;
	m1->not_mutual = NULL;
	int count1 = 0;
	User *ret = NULL;
	while (allUsers != NULL)
	{
		bool is_friend = in_friend_list(allUsers->user->friends, user);
		if (strcmp(user->name, allUsers->user->name) != 0 && is_friend == 0)
		{
			while (allUsers->user->brands != NULL)
			{
				while (user->brands != NULL)
				{
					if (strcmp(user->brands->brand_name, allUsers->user->brands->brand_name) == 0 || (user->brands == NULL && allUsers->user->brands == NULL))
					{
						m->mutual = insert_into_friend_list(m->mutual, allUsers->user);
						count1++;
						printf("ret->name: %s\n", allUsers->user->name);
					}
					else
					{
						m1->not_mutual = insert_into_friend_list(m1->not_mutual, allUsers->user);
						printf("2.ret->name: %s\n", allUsers->user->name);
					}
					user->brands = user->brands->next;
				}
				allUsers->user->brands = allUsers->user->brands->next;
			}
		}
		allUsers = allUsers->next;
	}
	if (count1 > 0)
	{
		while (m->mutual->next != NULL)
		{
			m->mutual = m->mutual->next;
		}
		ret = m->mutual->user;
	}
	else
	{
		while (m1->not_mutual->next != NULL)
		{
			m1->not_mutual = m1->not_mutual->next;
		}
		ret = m1->not_mutual->user;
	}

	return ret;
}
/* printf("ret->name: %s\n",ret->name);
printf("m->mutual->user->name: %s\n",m->mutual->user->name);
printf("count=: %d\n",count);
if(count==1){
	return ret;
}
if(count>1){
	while(m->mutual->next->user!=NULL){
		printf("m->mutual->user->name: %s\n",m->mutual->user->name);
		m->mutual->user=m->mutual->next->user;
	}
	ret=m->mutual->user;

}
if(count==0){
	while(m->not_mutual->next->user!=NULL){
		printf("m->not_mutual->user->name: %s\n",m->not_mutual->user->name);
		m->not_mutual->user=m->not_mutual->next->user;
	}
	ret=m->not_mutual->user;
}
return ret;
}	*/

/* *TODO: Complete this function
 * Friends n suggested friends for the given user.
 * See the handout for how we define a suggested friend.
 * Returns how many friends were successfully followed.
 **/
int add_suggested_friends(User *user, int n)
{
	if (user == NULL || n < 0)
	{
		return 0;
	}
	int count = 0, added = 0, i = 0;
	User *ret = get_suggested_friend(user);
	printf("ret->name: %s", ret->name);
	while (i < n)
	{
		User *ret = get_suggested_friend(user);
		added = add_friend(user, ret);
		if (added == 0)
		{
			count++;
		}
		i++;
	}
	return count;
}

typedef struct mutual_brands
{
	BrandNode *mutual_b;
	BrandNode *not_mutual_b;
	struct mutual_brands *next;
} b_mutuals;

BrandNode *get_suggested_brand(User *user)
{
	/*  if(allUsers->user->brands == NULL){
		return NULL;
	} */
	b_mutuals MB;
	b_mutuals *m = &MB;
	m->next = NULL;
	m->mutual_b = NULL;
	m->not_mutual_b = NULL;
	BrandNode *ret = NULL;
	int count = 0;
	int i = get_brand_index(user->brands->brand_name);
	while (user->brands != NULL)
	{
		// bool is_tru = in_brand_list(brand_names[]user->brands->brand_name);
		for (int j = 0; j < MAT_SIZE; j++)
		{
			if (strcmp(user->brands->brand_name, brand_names[j]) != 0)
			{
				if (brand_adjacency_matrix[i][j] == 1 && brand_adjacency_matrix[j][i] == 1)
				{
					m->mutual_b = insert_into_brand_list(m->mutual_b, brand_names[j]);
					while (m->mutual_b->next != NULL)
					{
						m->mutual_b = m->mutual_b->next;
					}
					count++;
				}

				else
				{
					m->not_mutual_b = insert_into_brand_list(m->not_mutual_b, brand_names[j]);
					while (m->not_mutual_b->next != NULL)
					{
						m->not_mutual_b = m->not_mutual_b->next;
					}
					count--;
				}
			}
		}
		if (count < 0)
		{
			ret = m->not_mutual_b;
		}
		ret = m->mutual_b;
		user->brands = user->brands->next;
	}
	return ret;
}

int follow_suggested_brands(User *user, int n)
{
	if (user == NULL || user->brands == NULL || n < 0)
	{
		return 0;
	}
	int count = 0;

	BrandNode *ret = get_suggested_brand(user);
	for (int i = 0; i < n && ret != NULL; i++)
	{
		BrandNode *ret = get_suggested_brand(user);
		int followed = follow_brand(user, ret->brand_name);
		if (followed == 0)
		{
			count++;
		}
		else
		{
			i = n + 5;
		}
	}
	return count;
}
/* if(count>1){
	mutuals* temp=m;
		ret=temp->mutual->user;
		while(temp!=NULL && count!=1){
			for(int i=0;i<MAX_STR_LEN;i++){
				if(temp->mutual->user->name[i]>ret->name[i]){
					ret=temp->mutual->user;
					i=MAX_STR_LEN+5;
				}
				else if(temp->mutual->user->name[i]<ret->name[i]){
					delete_from_friend_list(temp->mutual,temp->mutual->user);

					i=MAX_STR_LEN+5;
					count--;
				}
				else if(temp->mutual->user->name[i]==ret->name[i]){
					if(temp->mutual->user->name[i+1]=='\0' && ret->name[i]!='\0'){
						delete_from_friend_list(temp->mutual,ret);
						i=MAX_STR_LEN+5;
						ret=temp->mutual->user;
						count--;
					}
				}
				else if(strcmp(temp->mutual->user->name,ret->name)==0){
					i=MAX_STR_LEN+5;
				}
			}
			if(count==1){
				return ret;
			}
			temp=temp->next;
		}
	} */