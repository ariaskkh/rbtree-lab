// 1. 노드는 레드 혹은 블랙 중의 하나이다.
// 2. 루트 노드는 블랙이다.
// 3. 모든 리프 노드들(NIL)은 블랙이다.
// 4. 레드 노드의 자식노드 양쪽은 언제나 모두 블랙이다. (즉, 레드 노드는 연달아 나타날 수
//    없으며,블랙 노드만이 레드 노드의 부모 노드가 될 수 있다)
// 5. 어떤 노드로부터 시작되어 그에 속한 하위 리프 노드에 도달하는 모든 경로에는 리프 노드를
//    제외하면 모두 same 개수의 블랙 노드가 있다.

#include "rbtree.h"
#include <malloc.h>

void delete_case1(node_t *n);
void delete_case2(node_t *n);
void delete_case3(node_t *n);
void delete_case4(node_t *n);
void delete_case5(node_t *n);
void delete_case6(node_t *n);
void delete_one_child(rbtree* t, node_t* n);


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);
  return p;
}

node_t *newNode(key_t k){
  node_t *t = calloc(sizeof(node_t),1);
  t->left = t->right = t->parent = NULL;
  t->key = k;
  t->color = 0; // RED
  return t;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *grandparent(node_t *t){
  if ((t != NULL) && (t->parent != NULL)){
    return t->parent->parent;
  } else return NULL;
}

node_t *uncle(node_t *t){
  node_t *g = grandparent(t);
  if (g == NULL) return NULL; // no grandparent means no uncle
  if (t->parent == g->left) return g->right;
  else return g->left;
}

node_t *sibling(node_t *t){
  if (t == t->parent->left)
    return t->parent->right;
  else
    return t->parent->left;
}

void rotate_left(node_t *t){ // 좌회전, 위 아래로 level이 바뀌는 노드 중 윗 노드가 들어와야함
  node_t *cr = t->right; // 
  node_t *p = t->parent;

  if (cr->left != NULL)
    cr->left->parent = t;

  t->right = cr->left;
  t->parent = cr;
  cr->left = t;
  cr->parent = p;

  if (p != NULL){
    if (p->left == t)
    p->left = cr;
    else
    p->right = cr;
  }
}

void rotate_right(node_t *t){ // 우회전
  node_t *cl = t->left; // 
  node_t *p = t->parent;

  if (cl->right != NULL)
    cl->right->parent = t;

  t->left = cl->right;
  t->parent = cl;
  cl->right = t;
  cl->parent = p;

  if (p != NULL){
    if (p->left == t)
    p->left = cl;
    else
    p->right = cl;
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *new = newNode(key);
  node_t *curr = NULL;

  if (t->root == NULL){                      // case 1 root== NULL
    new->color = RBTREE_BLACK; // BLACK
    t->root = new;
    return t->root;
    }
  ////////////////////////////////////////////////////////////////////  
  else{ // case 2,3,4,5 를 위해 새노드 위치 리프 노드까지 탐색
    curr = t->root;
    t->root = curr;
    while (curr != NULL){ // 노드 삽입 위치 찾기
      if (curr->key < key){ // 새노드 key가 클 때
        if(curr->right != NULL)
          curr = curr->right;
        else {// curr->right == NULL 일 때
          curr->right = new;
          new->parent = curr;
        }
          break;
      }
      else { // 새노드 key가 작을 때
        if(curr->left != NULL)
          curr = curr->left;
        else {// curr->right == NULL 일 때
          curr->left = new;
          new->parent = curr;
          break;
        }
      }
    }
  while (1){
  if (curr->parent == NULL){
  return t->root;
  }
  //////////////////////////////////////////////////////////////////
  else if (new->parent->color == RBTREE_BLACK)  // case 2 부모가 Black
    return t->root; // 뭘 return 해야하지
  //////////////////////////////////////////////////////////////////
  else {                                  // case 3 부모, 삼촌이 Red
    node_t *u = uncle(new), *g; 
    if ((u != NULL) && (u->color == RBTREE_RED)) { // 부모, 삼촌이 Red
      new->parent->color = RBTREE_BLACK; // 부모 Black
      u->color = RBTREE_BLACK; // 삼촌 Black
      g = grandparent(new); // 조부모 생성
      g->color = RBTREE_RED; // 조부모 Red
      curr = g;
      continue;
    }
    // u : Black인 경우 빠져나옴
    else{                                   // case 4
      node_t *g = grandparent(new);

      if ((new == new->parent->right) && (new->parent == g->left)){
        rotate_left(new->parent); // 위 아래로 level이 바뀌는 노드 중 위. 즉 parent가 아래로 내려가고 new가 올라감
        new = new->left;
      } else if ((new == new->parent->left)&& (new->parent == g->right)){
        rotate_right(new->parent);
        new = new->right;
      }
      g = grandparent(new);                // case 5
      new->parent->color = RBTREE_BLACK;
      g->color = RBTREE_RED;
      if (new == new->parent->left)
        rotate_right(g);
      else
        rotate_left(g);
    }
    }
  }
  }
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {

  node_t *curr = t->root;
  while (1){
    if (curr->key < key){ // key가 큼
      if (curr->right != NULL) curr = curr->right;
      else {return NULL;}
    }
    else if (curr->key > key){ // key가 작음
      if(curr->left != NULL) curr = curr->left;
      else {return NULL;}
    }
    else {return curr;}
    }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* n = t->root;
    while(n->left != NULL)
      n = n->left;
  return n;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
    node_t* n = t->root;
    while(n->right != NULL)
      n = n->right;
  return n;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 삭제 부분

int rbtree_erase(rbtree *t, node_t *p) { // p가 key값을 가진 node
  // TODO: implement erase
  if (t->root == NULL || p==NULL) return 0;

  if (p->right != NULL && p->left != NULL){ // 삭제 노드 자식 2개
    node_t* temp = p->left;
    node_t* parent = NULL;
    while(temp->right!=NULL){
      parent=temp;
      temp=temp->right;
    }

    if (parent!=NULL){
      if(temp->left!=NULL){ // succesor의 부모노드
        parent->right = temp->left;
        temp->left->parent = parent; // 내가 추가
      }
    }
    else{ //    ???????????????
      if (temp->left!=NULL){
        p->left=temp->left;
      }
    }
    p->left = temp->left;
    delete_one_child(t,temp);
  }
  else{ // 삭제 노드 0개 or 1개
    delete_one_child(t,p);

  }
  return 0;
}

void replace_node(node_t* t, node_t* child){
// 앞에서 n의 부모가 NULL이 되는 경우를 delete_case에 오지 않게 미리 처리해주면 된다.
  child->parent = t->parent;
  if(t->parent->left == t)
    t->parent->left = child;
  else if (t->parent->right == t)
  t->parent->right = child;
}

void delete_one_child(rbtree* t, node_t *n) { // successor 자식 1개인 경우
    /* 선제조건: n이 최대 하나의 non-null 자식을 갖고 있음.*/
    // node_t *child = is_leaf(n->right) ? n->left: n->right;
    if(t->root == n){
      t->root=NULL;
      free(n);
      return;
      }
    if (n->left ==NULL && n->right == NULL){ // 자식이 없는 경우
      if(n->color == RBTREE_BLACK){ // Red일 경우 바로 free
        delete_case1(n);
      }
    if (n==n->parent->left){
      n->parent->left = NULL;
    }
    else if(n==n->parent->right){
      n->parent->right=NULL;
    }
    free(n);
    return;
    }

    node_t *child; // 자식이 하나인 경우
    if (n->left == NULL){
      child = n->right;
    }
    else {
      child = n->left;
    }
    replace_node(n, child);
    if (n->color == RBTREE_BLACK) {
        if (child->color == RBTREE_RED)
            child->color = RBTREE_BLACK;
    }
    free(n);
}

void delete_case1(node_t *n) // N이 root 일 때는 할 게 없음
{
    if (n->parent != NULL) 
        delete_case2(n);
}

void delete_case2(node_t *n) // S가 Red일 경우. P는 Black이고 S,P 색 바꾼 후 회전
{
    node_t *s = sibling(n);

    if (s->color == RBTREE_RED) {
        n->parent->color = RBTREE_RED;
        s->color = RBTREE_BLACK;
        if (n == n->parent->left)
            rotate_left(n->parent); // n이 개수가 적으므로 왼쪽에 있으면 좌회전
        else
            rotate_right(n->parent);
    }
    delete_case3(n);
}

void delete_case3(node_t *n) // s, sl, sr이 모두 Black -> S를 Red -> 양쪽 same bH
{
    node_t *s = sibling(n);

    if ((n->parent->color == RBTREE_BLACK) &&
        (s->color == RBTREE_BLACK) &&
        (s->left->color == RBTREE_BLACK) &&
        (s->right->color == RBTREE_BLACK)) {
        s->color = RBTREE_RED;
        delete_case1(n->parent); // 재귀 부분. p방향으로 올라가며 개수 하나씩 다 줄여줌
    } else
        delete_case4(n);
}

void delete_case4(node_t *n) // p = Red, s, sr, sl = Black인 경우 p, s color swap. 왼쪽 BH +=1, 오른쪽 BH 유지
{
    node_t *s = sibling(n);

    if ((n->parent->color == RBTREE_RED) &&
        (s->color == RBTREE_BLACK) &&
        (s->left->color == RBTREE_BLACK) &&
        (s->right->color == RBTREE_BLACK)) {
        s->color = RBTREE_RED;
        n->parent->color = RBTREE_BLACK;
    } else
        delete_case5(n);
}

void delete_case5(node_t *n) // s, sr = Black, sl = Red 경우 sl, s 색 swap. s기준 우회전
{
    node_t *s = sibling(n);

    if  (s->color == RBTREE_BLACK) {
        /* 이 문은 자명하다,
            case 2로 인해서(case 2에서 '''N'''의 형제 노드를 원래 형제 노드 '''S'''의 자식노드로 바꾸지만,
            빨강 부모노드는 빨강 자식 노드를 가질 수 없기 때문에 '''N'''의 새로운 형제노드는 빨강일 수 없다). */
        /* 다음의 문은 빨강을 '''N'''의 부모노드의 오른쪽 자식의 오른쪽 자식으로 두기 위함이다.
            혹은 '''N'''의 부모노드의 왼쪽 자식의 왼쪽 자식으로 두기 위함. case 6에 넘기기 위해 */
        if ((n == n->parent->left) &&
            (s->right->color == RBTREE_BLACK) &&
            (s->left->color == RBTREE_RED)) { /* this last test is trivial too due to cases 2-4. */
            s->color = RBTREE_RED;
            s->left->color = RBTREE_BLACK;
            rotate_right(s);
        } else if ((n == n->parent->right) &&
            (s->left->color == RBTREE_BLACK) &&
            (s->right->color == RBTREE_RED)) {/* this last test is trivial too due to cases 2-4. */
            s->color = RBTREE_RED;
            s->right->color = RBTREE_BLACK;
            rotate_left(s);
        }
    }
    delete_case6(n);
}

void delete_case6(node_t *n) // 마지막 젤 빡센거. s = Black, sr = Red. // P색 -> s색, p, sr =-> Black p기준 좌회전 
{
    node_t *s = sibling(n); // s가 바뀌어서 새로 정의 해줘야 함

    s->color = n->parent->color;
    n->parent->color = RBTREE_BLACK;

    if (n == n->parent->left) {
        s->right->color = RBTREE_BLACK;
        rotate_left(n->parent);
    } else {
        s->left->color = RBTREE_BLACK;
        rotate_right(n->parent);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

// int main(){

//   rbtree *tree = new_rbtree();
//   rbtree_insert(tree, 10);
//   rbtree_insert(tree, 20);
//   printf("%d\n", tree->root->key);
//   printf("%d\n", tree->root->right->key);


// }