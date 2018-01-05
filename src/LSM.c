#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "LSM.h"

typedef LSMNode node; /* Because refactoring is effort */
typedef LSMINode nodei; /* "" */
typedef LSM lsm; /* "" */

/* wrappers created to maintain my standard naming conventions */

void LSMInit(LSM **plsm) {
  *plsm = init_new_lsm();
  commonPersist(*plsm, sizeof(LSM));
}

void LSMInsert(LSM **plsm, Generic data, int64_t index) {
  if (index) {} // make compiler keep quiet
  const keyType key = (const keyType) GenericHash(data);
  const valType val = (const valType) data;
  put(&key, &val, *plsm);
}

Generic LSMGetElement(LSM **plsm, int64_t key) {
  LSMNode *ret = get((const keyType) key, *plsm);
  Generic retData = ret != NULL ? ret->val : NULL;
  // because the author of this code decided to malloc in a get func
  commonFree(&ret);
  return retData;
}

void LSMDestroy(LSM **plsm) {
  destruct_lsm(*plsm);
  *plsm = NULL;
}

void LSMPrint(FILE *out, LSM **plsm) {
  fprintf(out, "LSM Tree:\n");
  fprintf(out, "Component 0:\n");
  print_buffer_data(out, *plsm);
  fprintf(out, "Component 1:\n");
  print_disk_data(out, *plsm);
}

uintptr_t LSMGetMemSize(LSM **plsm) {
  LSM *tree = *plsm;
  return sizeof(LSM) + tree->block_size*tree->node_size;
}

/* end wrappers */

void check_file_ret(FILE* f, int r){
  if (TRUE) return; // Contextless function that prints unnecessary garbage
  /*Check return value of file pointer.
Args:
f (FILE*): file pointer.
r (int): file return value.
*/
  if(r == 0){
    if(ferror(f)){
      perror("ferror\n");
    }
    else if(feof(f)){
      perror("EOF found\n");
    }
  }
}

lsm* init_new_lsm(){
  /*Initializes new lsm tree onject.
buffer_size (size_t): size of in-memory buffer (provided by .h file / makefile).
sorted (bool): if true data on disk will be sorted. ("")
Args:
None
Returns:
populated lsm object.
*/
  lsm* tree;
  commonMalloc(&tree, sizeof(lsm), LSM_TYPE);
  if(!tree){
    perror("init_lsm: block is null \n");
    return NULL;
  }
  tree->block_size = LSM_BLOCK_SIZE;
  tree->k = 2;
  tree->next_empty = 0;
  tree->node_size = sizeof(node);
  commonMalloc(&tree->block, tree->block_size*tree->node_size, LSMNode_TYPE);
  if(!tree->block){
    perror("init_lsm: block is null \n");
    return NULL;
  }
  tree->disk1 = LSM_DISK_PATH;
  FILE *fp = fopen(tree->disk1, "r");
  if (!fp) { // if the file doesn't exist, create it
    fp = fopen(tree->disk1, "w");
  }
  fclose(fp);


  tree->sorted = LSM_SORTED;
  return tree;
}


void destruct_lsm(lsm* tree){
  /* Destruct LSM object.
Args:
tree (lsm*): pointer to LSM object to destroy.
*/
  // clear disk data
  FILE *fp = fopen(tree->disk1, "w");
  fclose(fp);
  // clear memory
  commonFree(&tree->block);
  commonFree(&tree);
}

void merge(node *whole, node *left,int left_size,node *right,int right_size){
  /* Performs the merge step in merge sort.
Args:
whole (node*): pointer to the node representing the merged array of nodes.
left (node*): pointer to the left split of nodes to be merged.
left_size (int): int representing the the size of the left split.
right (node*): pointer to the right split of nodes to be merged.
rightt_size (int): int representing the the size of the right split.
*/
  int l, r, i;
  l = 0; r = 0; i = 0;

  while(l < left_size && r < right_size) {
    if(left[l].key  < right[r].key){
      whole[i++] = left[l++];
    } else{
      whole[i++] = right[r++];
    }
  }
  while(l < left_size){
    whole[i++] = left[l++];
  }
  while(r < right_size){
    whole[i++] = right[r++];
  }
}


void merge_sort(node *block, int n){
  /* Given an unsorted array of nodes, sorts by key.
Args:
block (node*): pointer to a node representing the block to sort.
n (int): int describing the size of the block to be sorted.
*/

  //Key assumption: block will only be sorted when full
  assert(block != NULL);
  if(n < 2){
    return;
  }
  int mid;
  mid = n/2;
  node *left;
  node *right;

  /* create and populate left and right subarrays */
  commonMalloc(&left, mid*sizeof(node), LSMNode_TYPE);
  commonMalloc(&right, (n-mid)*sizeof(node), LSMNode_TYPE);

  memcpy(left, block, sizeof(node) * mid);
  memcpy(right, &block[mid], sizeof(node) * (n - mid));

  /* sort and merge the two arrays */
  merge_sort(left,mid);  // sort left subarray
  merge_sort(right,n-mid);  // sort right subarray
  merge(block,left,mid,right,n-mid);
  commonFree(&left);
  commonFree(&right);
}


nodei* search_buffer(const keyType* key, lsm* tree){
  /* Searches in-memory buffer for a given key.
Args:
key (keyType*): key to search for.
tree (lsm*): pointer to an lsm tree.
*/
  for (size_t i = 0; i < tree->next_empty; i++){
    if (tree->block[i].key == *key){
      nodei* ni = NULL;
      commonMalloc(&ni, sizeof(nodei), LSMINode_TYPE);
      commonMalloc(&ni->node, sizeof(node), LSMNode_TYPE);
      ni->node->key = tree->block[i].key;
      ni->node->val = tree->block[i].val;
      ni->index = i;
      return ni;
    }
  }
  return NULL;
}

nodei* search_disk(const keyType* key, lsm* tree){
  /* Searches disk for a given key.
Args:
key (keyType*): key to search for.
tree (lsm*): pointer to an lsm tree.
*/
  int r;
  FILE* f = fopen(tree->disk1, "r");
  if(f == NULL){
    perror("search_disk: open 1: \n");
    return NULL;
  }
  node *file_data;
  size_t num_elements;
  r = fread(&num_elements, sizeof(size_t), 1, f);
  check_file_ret(f, r);
  commonMalloc(&file_data, sizeof(node)*num_elements, LSMNode_TYPE);
  r = fread(file_data, sizeof(node), num_elements, f);
  check_file_ret(f, r);
  for(size_t i = 0; i < num_elements; i++){
    if (file_data[i].key == *key){
      nodei* ni = NULL;
      commonMalloc(&ni, sizeof(nodei), LSMINode_TYPE);
      commonMalloc(&ni->node, sizeof(node), LSMNode_TYPE);
      ni->node->key = file_data[i].key;
      ni->node->val = file_data[i].val;
      ni->index = i;
      if(fclose(f)){
        perror("search_disk: fclose: ");
      }
      commonFree(&file_data);
      return ni;
    }
  }
  commonFree(&file_data);
  if(fclose(f)){
    perror("search_disk: fclose: ");
  }
  return NULL;
}

node* get(const keyType key, lsm* tree){
  /* Retrieves node associated with a given key.
Args:
key (keyType*): key to search for.
tree (lsm*): pointer to an lsm tree.
*/
  // search the buffer for this item
  nodei* ni = search_buffer(&key, tree);
  if(ni != NULL){
    node* ret_buffer = ni->node;
    commonFree(&ni);
    return ret_buffer;
  } else{
    // search through the file on disk for this item
    ni = search_disk(&key, tree);
    if(ni != NULL){
      node* ret_disk = ni->node;
      commonFree(&ni);
      return ret_disk;
    }
  }
  // If it does not find the given key, it will return NULL
  return NULL;
}

int write_to_disk(lsm* tree){
  /* Write all data (in disk & in memory) to disk.
Args:
tree (lsm*): pointer to an lsm tree.
*/
  node *complete_data = NULL;
  node *file_data = NULL;
  size_t num_elements = 0;
  int r;
  //sort the buffer
  if(tree->sorted){
    merge_sort(tree->block, tree->next_empty);
    // size based off alllocation in init func
    commonPersist(tree->block, tree->block_size * tree->node_size);
  }
  struct stat s;
  int file_exists = stat(tree->disk1, &s);
  FILE *fr = NULL;
  if(file_exists == 0){
    // the file already exists
    fr  = fopen(tree->disk1, "r");
    // read number of elements
    r = fread(&num_elements, sizeof(size_t), 1, fr);
    check_file_ret(fr, r);
  }
  // if there are are less than 0 elements, the file may as well not exist
  if (num_elements > 0) {
    // allocate memory for nodes on disk
    commonMalloc(&file_data, sizeof(node)*num_elements, LSMNode_TYPE);
    assert(file_data);
    // read nodes on disk into memory
    r = fread(file_data, sizeof(node), num_elements, fr);
    check_file_ret(fr, r);
    // merge the sorted buffer and the sorted disk contents
    commonMalloc(&complete_data, sizeof(node)*(num_elements+tree->next_empty),
          LSMNode_TYPE);
    merge(complete_data, file_data, num_elements, tree->block,tree->next_empty);
    num_elements += tree->block_size;
    commonFree(&file_data);
  }
  if (file_exists == 0) {
    if(fclose(fr)){
      perror("put: close 2: \n");
    }
  }
  FILE* fw  = fopen(tree->disk1, "w");
  if(complete_data == NULL){
    complete_data = tree->block;
  }
  if(num_elements <= 0){
    num_elements = tree->next_empty;
  }
  // seek to the start of the file & write # of elements
  if(fseek(fw, 0, SEEK_SET)){
    perror("put: fseek 4: \n");
  }
  if(!fwrite(&num_elements, sizeof(size_t), 1, fw)){
    perror("put: fwrite 4: \n");
  }
  // seek to the first space after the number of elements
  if(fseek(fw, sizeof(size_t), SEEK_SET)){
    perror("put: fseek 5: \n");
  }
  if(!fwrite(complete_data,  sizeof(node), num_elements, fw)){
    perror("put: fwrite 5: \n");
  }
  // reset next_empty to 0
  tree->next_empty = 0;
  if(fclose(fw)){
    perror("put: close 2: \n");
  }
  if (complete_data != tree->block) {
    commonFree(&complete_data);
  }
  commonPersist(tree, sizeof(lsm));
  return 0;
}


int put(const keyType* key, const valType* val, lsm* tree){
  /* Places a given key and value into the lsm tree as a node.
Args:
key (const keyType*): key to place in node.
val (const valType*): value to place in node.
*/
  int r = 0;
  if(tree->next_empty == tree->block_size){
    // buffer is full and must be written to disk
    r = write_to_disk(tree);
  }
  node n;
  n.key = *key;
  n.val = *val;
  tree->block[tree->next_empty] = n;
  commonPersist(&tree->block[tree->next_empty], sizeof(node));
  tree->next_empty += 1;
  commonPersist(tree, sizeof(lsm));
  return r;
}


int delete(const keyType* key, lsm* tree){
  /* Deletes the node cotnaining the given key.
Args:
key (const keyType*): key associated with node to delete.
tree (lsm*): lsm object to delete it from.
*/
  int r = 0;
  nodei *ni = NULL;
  // if the node is in the buffer
  ni = search_buffer(key, tree);
  if(ni != NULL){
    tree->next_empty -= 1;
    memmove(&tree->block[ni->index], &tree->block[ni->index+1], tree->block_size-ni->index);
    commonPersist(&tree->block[ni->index], tree->block_size-ni->index);
    commonPersist(tree, sizeof(lsm));
  } else {
    // if the node is on disk
    ni = search_disk(key, tree);
    assert(ni);
    FILE* fr  = fopen(tree->disk1, "r");
    if(fr == NULL){
      perror("delete: open: \n");
    }
    size_t num_elements = 0;
    node* file_data;
    // read number of elements
    r = fread(&num_elements, sizeof(size_t), 1, fr);
    check_file_ret(fr, r);
    // allocate memory for nodes on disk
    commonMalloc(&file_data, sizeof(node)*num_elements, LSMNode_TYPE);
    assert(file_data);
    // read nodes on disk into memory
    r = fread(file_data, sizeof(node), num_elements, fr);
    if(r == 0){
      if(ferror(fr)){
        perror("put fread 2: ferror\n");
      }
      else if(feof(fr)){
        perror("put fread 2: EOF found\n");
      }
    }
    if(fclose(fr)){
      perror("put: close 2: \n");
    }
    num_elements = num_elements - 1;
    memmove(&file_data[ni->index], &file_data[ni->index+1], num_elements-ni->index);
    // write the new file data to disk
    FILE* fw  = fopen(tree->disk1, "w");
    if(fseek(fw, 0, SEEK_SET)){
      perror("delete seek:  \n");
    }
    if(!fwrite(&num_elements, sizeof(size_t), 1, fw)){
      perror("delete fwrite: \n");
    }
    if(!fwrite(file_data,  sizeof(node), num_elements, fw)){
      perror("delete fwrite: \n");
    }
    if(fclose(fw)){
      perror("put: close 2: \n");
    }
  }
  return 0;
}


int update(const keyType* key, const valType* val, lsm* tree){
  /* Searches buffer &  disk for key, updates associated value.
Args:
key (const keyType*): pointer to key to search for.
val (const valType*): pointer to val to update.
tree (lsm*): pointer to lsm object to make update in.
*/
  nodei* ni = search_buffer(key, tree);
  //printf("index is: ni %d \n", ni->index);
  //printf("upating key: %d, val: %d \n", (ni->node->key, ni->node->val));
  if(ni != NULL){
    node n;
    n.key = *key;
    n.val = *val;
    tree->block[ni->index] = n;
    commonPersist(&tree->block[ni->index], sizeof(node));
    //printf("to key: %d  val: %d \n", (*key, *val));
  } else {
    delete(key, tree);
    put(key, val, tree);
  }

  return 0;
}

void print_buffer_data(FILE *out, lsm* tree){
  /* Prints all data in buffer.
Args:
tree (lsm*): pointer to lsm associated with data.
*/
  for(size_t i = 0; i < tree->next_empty; i++){
    fprintf(out, "%lu:",tree->block[i].key);
    GenericPrint(out, tree->block[i].val);
    fprintf(out, " ");
  }
  fprintf(out, "\n");
}

void print_disk_data(FILE *out, lsm* tree){
  /* Prints all data on disk.
Args:
tree (lsm*): pointer to lsm associated with data.
*/
  FILE* f = fopen(tree->disk1, "r");
  node *file_data;
  size_t num_elements = 0;
  int r;
  r = fread(&num_elements, sizeof(size_t), 1, f);
  if(r == 0){
    if(ferror(f)){
      perror("ferror\n");
    }
    else if(feof(f)){
      perror("EOF found\n");
    }
  }
  commonMalloc(&file_data, sizeof(node)*num_elements, LSMNode_TYPE);
  if(file_data == NULL){
    perror("print_disk_data: unsuccessful allocation \n");
  }
  r = fread(file_data, sizeof(node), num_elements, f);
  if(r == 0){
    if(ferror(f)){
      perror("ferror\n");
    }
    else if(feof(f)){
      perror("EOF found\n");
    }
  }
  for(size_t i = 0; i < num_elements; i++){
    fprintf(out, "%lu:",file_data[i].key);
    GenericPrint(out, file_data[i].val);
    fprintf(out, " ");
  }
  fprintf(out, "\n");
}
