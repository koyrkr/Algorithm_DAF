// written by Zongheng Wu for DS


#include <stdio.h>
#include <stdlib.h>
#define MAXTABLESIZE 1000


typedef struct _minheap{
  int size;
  int data[MAXTABLESIZE];
}MinHeap;

typedef MinHeap *pH;

typedef struct _node{
  int index;
  struct _node* next;
}Node;

typedef Node* List;
typedef Node* pNode;

int Parent(int index);
int Left(int index);
int Right(int index);
void Swap(int* pa,int* pb);
int RemoveMin(pH heap);
void DownHeap(pH heap,int index);
void AddToHeap(pH heap,int element);
void UpHeap(pH heap,int index);
int* ToplogicalSort(int HashTable[MAXTABLESIZE],int G[MAXTABLESIZE][MAXTABLESIZE],int *indegree,int N,pH heap);
int FindHash(int HashTable[MAXTABLESIZE],int cur_min,int N);
void BuildDAG(int HashTable[MAXTABLESIZE],int G[MAXTABLESIZE][MAXTABLESIZE],int *indegree,int N);

int main(){
  int N;
  scanf("%d",&N);
  int* indegree = (int*)malloc(sizeof(int) * N);
  for(int i=0;i<N;i++){     // initialize the indegree
    indegree[i] = -1;
  }
  int number;
  int HashTable[MAXTABLESIZE];
  for(int i=0;i<N;i++){
    scanf("%d", &number);
    HashTable[i] = number;
  }
  int G[MAXTABLESIZE][MAXTABLESIZE];   // initialize the DAG
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      G[i][j] = -1;
    }
  }
  BuildDAG(HashTable,G,indegree,N);
  pH heap = (pH)malloc(sizeof(MinHeap));
  heap->size = 0;
  int* top = ToplogicalSort(HashTable,G,indegree,N,heap);
  int first = 1;
  for(int i=0;i<N;i++){
    if(top[i] != -1){
      if(first){
        printf("%d",top[i]);
        first = 0;
      }
      else{
        printf(" %d",top[i]);
      }
    }
  }
  return 0;
}

int Parent(int index){
  return (index - 1) / 2;
}

int Left(int index){
  return 2 * index + 1;
}

int Right(int index){
  return 2 * index + 2;
}

void Swap(int* pa,int* pb){
  int temp = *pa;
  *pa = *pb;
  *pb = temp;
}

int RemoveMin(pH heap){
  int min = heap->data[0];
  Swap(&(heap->data[0]),&(heap->data[heap->size - 1]));
  heap->size -= 1;
  DownHeap(heap,0);
  return min;
}

void DownHeap(pH heap,int index){
  int left = Left(index);
  int right = Right(index);
  int swap_index = left;
  if(swap_index < heap->size - 1 && heap->data[left] > heap->data[right]){
    swap_index = right;
  }
  if(swap_index < heap->size && heap->data[index] > heap->data[swap_index]){
    Swap(&(heap->data[swap_index]),&(heap->data[index]));
    DownHeap(heap,swap_index);
  }
}

void AddToHeap(pH heap,int element){
  if(heap->size == 0){
    heap->data[0] = element;
    heap->size += 1;
  }
  else{
    heap->data[heap->size] = element;
    heap->size += 1;
    UpHeap(heap,heap->size - 1);
  }
}

void UpHeap(pH heap,int index){
  int parent = Parent(index);
  if(index > 0 && heap->data[parent] > heap->data[index]){
    Swap(&(heap->data[parent]),&(heap->data[index]));
    UpHeap(heap,parent);
  }
}

int* ToplogicalSort(int HashTable[MAXTABLESIZE],int G[MAXTABLESIZE][MAXTABLESIZE],int *indegree,int N,pH heap){
  int* top_sort = (int*)malloc(sizeof(int)*N);
  int count = 0;
  int cur_min;
  int min_hash;
  for(int i=0;i<N;i++){
    top_sort[i] = -1;
  }
  for(int i=0;i<N;i++){
    if(indegree[i] == 0){
      AddToHeap(heap,HashTable[i]);
      indegree[i] = -2;
    }
  }
  while(heap->size){  // as long as the Queue is not empty, do the while loop
    cur_min = RemoveMin(heap);
    top_sort[count++] = cur_min;
    min_hash = FindHash(HashTable,cur_min,N);
    for(int i =0;i < N;i++){
      if(G[min_hash][i] == 1){
        indegree[i]--;
        if(indegree[i] == 0){
          AddToHeap(heap,HashTable[i]);
          indegree[i] = -2;
        }
      }
    }
  }
  return top_sort;
}

int FindHash(int HashTable[MAXTABLESIZE],int cur_min,int N){
  int original = cur_min % N;
  while(1){
    if(HashTable[original] == cur_min){
      break;
    }
    else{
      original += 1;
      if(original >= N){
        original = 0;
      }
    }
  }
  return original;
}

void BuildDAG(int HashTable[MAXTABLESIZE],int G[MAXTABLESIZE][MAXTABLESIZE],int *indegree,int N){
  int remainder_value;
  int cur_pos;
  for(int i=0;i<N;i++){
    if(HashTable[i] != -1){
      remainder_value = HashTable[i] % N;
      if(remainder_value == i){
        indegree[i] = 0;
      }
      else{
        cur_pos = remainder_value;
        while(cur_pos != i){
          G[cur_pos][i] = 1;  // add the index i to the children of index remainder_value
          if(indegree[i] == -1){
            indegree[i] = 1;
          }
          else{
            indegree[i] += 1;
          }
          cur_pos += 1;
          if(cur_pos >= N){
            cur_pos = 0;
          }
       }
      }
    }
  }
}
