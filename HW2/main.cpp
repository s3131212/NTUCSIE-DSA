#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define min(x,y) (x < y?x:y)
#define ARRAY_SIZE 200100
#define ARRAY_SIZE_LOG2 20
//#define ARRAY_SIZE 13
//#define ARRAY_SIZE_LOG2 4
 
 
typedef struct Node{
    struct Node* children[26];
    struct Node* parent;
    int depth;
    // int index;
    int firstOccurrenceinET;
    int ETvisitTime;
    bool doneVisit;
} Node;
 
/* Stack */
Node* stack[3 * ARRAY_SIZE] = {0};
int stackNum = 0;
 
void stack_push(Node* value){
    stack[stackNum] = value;
    stackNum ++;
}
Node* stack_pop(){
    stackNum--;
    return stack[stackNum];
}
bool stack_empty(){
    return (stackNum == 0);
}
 
/* trie */
Node* trieMap[ARRAY_SIZE] = {0};
 
void initNode(Node* n){
    for(int i = 0; i < 26; i++){
        n->children[i] = NULL;
    }
    n->doneVisit = false;
    n->ETvisitTime = 0;
    n->firstOccurrenceinET = -1;
}
 
Node* createTrie(){
    Node* trie = (Node*)malloc(sizeof(struct Node));
    trie->parent = NULL;
    initNode(trie);
    return trie;
}
 
Node* insertToTrie(int base, char c, int index){
    if(trieMap[base] == NULL) return NULL;
    if(trieMap[base]->children[c - 'a'] == NULL){
        Node* child = (Node*)malloc(sizeof(struct Node));
        initNode(child);
        child->parent = trieMap[base];
        child->depth = trieMap[base]->depth + 1;
        //child->index = index;
        trieMap[base]->children[c - 'a'] = child;
        return child;
    }else{
        return trieMap[base]->children[c - 'a'];
    }
}
 
/* RMQ */
int dp[ARRAY_SIZE * 2][ARRAY_SIZE_LOG2 + 1];
int trieDepthinET[2 * ARRAY_SIZE] = {0};
int lengthOfET;
 
void EulerTour(Node* root) {
    stack_push(root);
    Node *temp;
    int i = 0;
    bool doneVisit;
    while (!stack_empty()) {
        temp = stack_pop();
        //if(temp->doneVisit) continue;
 
        //printf("%d, %c \n", temp->depth, temp->self);
 
        if(temp->ETvisitTime == 0)
            temp->firstOccurrenceinET = i;
        temp->ETvisitTime ++;
 
        trieDepthinET[i] = temp->depth;
        i++;
 
        doneVisit = true;
        for (int j = 0; j < 26; j++)
            if (temp->children[j] != NULL)
                if(temp->children[j]->ETvisitTime == 0 && !temp->children[j]->doneVisit) {
                    stack_push(temp->children[j]);
                    doneVisit = false;
                    break;
                }
        temp->doneVisit = doneVisit;
        if(doneVisit)
            if (temp->parent != NULL)
                stack_push(temp->parent);
    }
    lengthOfET = i;
}
void generateDPTable(){
    for(int i = 0; i < ARRAY_SIZE * 2; i++) {
        for (int j = 1; j < ARRAY_SIZE_LOG2 + 1; j++)
            dp[i][j] = -1;
        dp[i][0] = trieDepthinET[i];
    }
    for (int j = 1; 1 << j <= lengthOfET; j++)
        for (int i = 0; i + (1 << j) < lengthOfET; i++){
            //if(i >= ARRAY_SIZE * 2 || j >= ARRAY_SIZE_LOG2 + 1) printf("access dp: %d, %d \n", i, j );
            dp[i][j] = min(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
        }
}
 
int getLCA(int i1, int i2){
    Node* s1 = trieMap[i1];
    Node* s2 = trieMap[i2];
    if(s1 == s2) return s1->depth;
 
    // Swap so that s1 is always first
    if(s2->firstOccurrenceinET < s1->firstOccurrenceinET){
        Node *tmp = s1;
        s1 = s2;
        s2 = tmp;
    }
 
    int j = (int)log2(s2->firstOccurrenceinET - s1->firstOccurrenceinET + 1);
    //printf("%d %d %d %d %d\n", s1->firstOccurrenceinET, s2->firstOccurrenceinET, dp[s1->firstOccurrenceinET][j] , dp[s2->firstOccurrenceinET - (1 << j) + 1][j], j);
    return min(dp[s1->firstOccurrenceinET][j], dp[s2->firstOccurrenceinET - (1 << j) + 1][j]);
}
 
int main() {
    trieMap[0] = createTrie();
 
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        int l;
        char c;
        scanf("%d %c", &l, &c);
        trieMap[i + 1] = insertToTrie(l, c, i + 1);
    }
 
    EulerTour(trieMap[0]);
    /*for(int i = 0; i < 50; i++)
        printf("%d ", trieDepthinET[i]);
    printf("\n");*/
 
    generateDPTable();
 
    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        int i1, i2;
        scanf("%d %d", &i1, &i2);
        printf("%d", getLCA(i1, i2));
        printf("\n");
    }
 
    return 0;
}