//-------------------MY TREE---------------------
struct LEAF *add_tree(int new_value);
int height(struct LEAF *node);
void preOrder(struct LEAF *node);
struct LEAF *left_rotation(struct LEAF *node);
struct LEAF *right_rotation(struct LEAF *node);
struct LEAF *check_tree(struct LEAF *node, int new_value);
struct LEAF *insert_tree(struct LEAF *component, int new_value);
void search_tree(struct LEAF *component, int new_value);
//void delete_tree(struct LEAF* node);
//-----------------------------------------------
//------------------MY HASH----------------------
int my_hash(int *arr, int value);
int resize_hash(int *arr);
int *insert_hash(int *arr);
void find_hash(int *arr, int value);
int check_hash(int *arr, int value);
int *insert_my_hash(int *arr, int num);
//-----------------------------------------------
//----------------NOT_MY_HASH--------------------
int NotMyHash();
unsigned int hash(int sum);
void create(int a);
int find(int a);
void read();
//-----------------------------------------------
//----------------NOT_MY_TREE--------------------
struct Node* NotMyTree(struct Node* RBT);
struct Node* BST_search(struct Node* root, int x);
void preorder(struct Node* root);
struct Node* RB_insert(struct Node* T, int data);
void RB_insert_fixup(struct Node** T, struct Node** z);
void RightRotate(struct Node** T, struct Node** x);
void LeftRotate(struct Node** T, struct Node** x);
int	search_RBT(struct Node* root, int x);
//------------------------------------------------
//--------------------BVL-------------------------
struct ITEM *add_tree_bvl(int value);
struct ITEM *insert_tree_bvl(struct ITEM *component, int new_value);
void search_tree_bvl(struct ITEM *component, int new_value);