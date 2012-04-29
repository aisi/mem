#include "red_black_tree.h"

rb_red_blk_tree* RBTreeCreate( void * heap)
{
  rb_red_blk_tree* newTree;
  rb_red_blk_node * temp;
 newTree = new (heap) rb_red_blk_tree;
  newTree->nil= new ((char*)heap + TREESZIE) rb_red_blk_node;
  newTree->root= new ((char*)heap + TREESZIE + MINSIZE)rb_red_blk_node;
  temp= newTree->nil;
  temp->parent=temp->left=temp->right=temp;
  temp->red=0;
  temp=newTree->root;
  temp->parent=temp->left=temp->right=newTree->nil;
  temp->red=0;
  return(newTree);
}


void LeftRotate(rb_red_blk_tree*&  tree, rb_red_blk_node* x)
{
  rb_red_blk_node* y;
  rb_red_blk_node* nil=tree->nil;
  y=x->right;
  x->right=y->left;
  if (y->left != nil) y->left->parent=x; 
  y->parent=x->parent;   
  if( x == x->parent->left) {
    x->parent->left=y;
  } else {
    x->parent->right=y;
  }
  y->left=x;
  x->parent=y;

  assert(!tree->nil->red);

}


void RightRotate(rb_red_blk_tree* & tree, rb_red_blk_node* y) 
{
  rb_red_blk_node* x;
  rb_red_blk_node* nil=tree->nil;
  x=y->left;
  y->left=x->right;
  if (nil != x->right)  x->right->parent=y; 
  x->parent=y->parent;
  if( y == y->parent->left) {
    y->parent->left=x;
  } else {
    y->parent->right=x;
  }
  x->right=y;
  y->parent=x;
  assert(!tree->nil->red);
}

void TreeInsertHelp(rb_red_blk_tree* tree, rb_red_blk_node* z)
{
  rb_red_blk_node* x;
  rb_red_blk_node* y;
  rb_red_blk_node* nil=tree->nil;
  
  z->left=z->right=nil;
  y=tree->root;
  x=tree->root->left;
  while( x != nil) {
    y=x;
	if (1==rb_red_blk_key::cmp(x->key ,z->key)) { 
      x=x->left;
    } else {
      x=x->right;
    }
  }
  z->parent=y;
  if ( (y == tree->root) ||
       ( 1==rb_red_blk_key::cmp(y->key , z->key)) ){ 
    y->left=z;
  } else {
    y->right=z;
  }
  assert(!tree->nil->red);
}

rb_red_blk_node * RBTreeInsert(rb_red_blk_tree*&  tree, void* heap,rb_red_blk_key& key)
{
  rb_red_blk_node * y;
  rb_red_blk_node * x;
  rb_red_blk_node * newNode;

  x= new(heap) rb_red_blk_node;
  x->key.SetSize(key.GetSize());
  x->key.SetPreSize(key.GetPreSize());
  x->key.SetUseState(key.GetUseState());
	if(x->HaveNext())
		x->GetNextNode()->key.SetPreSize(key.GetSize());
  TreeInsertHelp(tree,x);
  newNode=x;
  x->red=1;
  while(x->parent->red) { 
    if (x->parent == x->parent->parent->left) {
      y=x->parent->parent->right;
      if (y->red) {
	x->parent->red=0;
	y->red=0;
	x->parent->parent->red=1;
	x=x->parent->parent;
      } else {
	if (x == x->parent->right) {
	  x=x->parent;
	  LeftRotate(tree,x);
	}
	x->parent->red=0;
	x->parent->parent->red=1;
	RightRotate(tree,x->parent->parent);
      } 
    } else { 
      y=x->parent->parent->left;
      if (y->red) {
	x->parent->red=0;
	y->red=0;
	x->parent->parent->red=1;
	x=x->parent->parent;
      } else {
	if (x == x->parent->left) {
	  x=x->parent;
	  RightRotate(tree,x);
	}
	x->parent->red=0;
	x->parent->parent->red=1;
	LeftRotate(tree,x->parent->parent);
      } 
    }
  }
  tree->root->left->red=0;
  return(newNode);

  assert(!tree->nil->red);
  assert(!tree->root->red);
}

  
rb_red_blk_node* TreeSuccessor(rb_red_blk_tree* tree,rb_red_blk_node* x) { 
  rb_red_blk_node* y;
  rb_red_blk_node* nil=tree->nil;
  rb_red_blk_node* root=tree->root;

  if (nil != (y = x->right)) {
    while(y->left != nil) {
      y=y->left;
    }
    return(y);
  } else {
    y=x->parent;
    while(x == y->right) {
      x=y;
      y=y->parent;
    }
    if (y == root) return(nil);
    return(y);
  }
}


rb_red_blk_node* TreePredecessor(rb_red_blk_tree* tree, rb_red_blk_node* x) {
  rb_red_blk_node* y;
  rb_red_blk_node* nil=tree->nil;
  rb_red_blk_node* root=tree->root;

  if (nil != (y = x->left)) { 
    while(y->right != nil) {
      y=y->right;
    }
    return(y);
  } else {
    y=x->parent;
    while(x == y->left) { 
      if (y == root) return(nil); 
      x=y;
      y=y->parent;
    }
    return(y);
  }
}

rb_red_blk_node* RBExactQuery(rb_red_blk_tree* tree, rb_red_blk_key& q) {
  rb_red_blk_node* x=tree->root->left;
  rb_red_blk_node* nil=tree->nil;
  int compVal;
  if (x == nil) return(0);
  compVal=rb_red_blk_key::cmp(x->key,q);
  while(0 != compVal) {
    if (1 == compVal) {
      x=x->left;
    } else {
      x=x->right;
    }
    if ( x == nil) return(0);
	compVal=rb_red_blk_key::cmp(x->key,q);
  }
  return(x);
}


void RBDeleteFixUp(rb_red_blk_tree*&  tree, rb_red_blk_node* x) {
  rb_red_blk_node* root=tree->root->left;
  rb_red_blk_node* w;

  while( (!x->red) && (root != x)) {
    if (x == x->parent->left) {
      w=x->parent->right;
      if (w->red) {
	w->red=0;
	x->parent->red=1;
	LeftRotate(tree,x->parent);
	w=x->parent->right;
      }
      if ( (!w->right->red) && (!w->left->red) ) { 
	w->red=1;
	x=x->parent;
      } else {
	if (!w->right->red) {
	  w->left->red=0;
	  w->red=1;
	  RightRotate(tree,w);
	  w=x->parent->right;
	}
	w->red=x->parent->red;
	x->parent->red=0;
	w->right->red=0;
	LeftRotate(tree,x->parent);
	x=root; 
      }
    } else {
      w=x->parent->left;
      if (w->red) {
	w->red=0;
	x->parent->red=1;
	RightRotate(tree,x->parent);
	w=x->parent->left;
      }
      if ( (!w->right->red) && (!w->left->red) ) { 
	w->red=1;
	x=x->parent;
      } else {
	if (!w->left->red) {
	  w->right->red=0;
	  w->red=1;
	  LeftRotate(tree,w);
	  w=x->parent->left;
	}
	w->red=x->parent->red;
	x->parent->red=0;
	w->left->red=0;
	RightRotate(tree,x->parent);
	x=root;
      }
    }
  }
  x->red=0;

  assert(!tree->nil->red);

}

void RBDelete(rb_red_blk_tree* & tree, rb_red_blk_node* z){
  rb_red_blk_node* y;
  rb_red_blk_node* x;
  rb_red_blk_node* nil=tree->nil;
  rb_red_blk_node* root=tree->root;

  y= ((z->left == nil) || (z->right == nil)) ? z : TreeSuccessor(tree,z);
  x= (y->left == nil) ? y->right : y->left;
  if (root == (x->parent = y->parent)) {
    root->left=x;
  } else {
    if (y == y->parent->left) {
      y->parent->left=x;
    } else {
      y->parent->right=x;
    }
  }
  if (y != z) {

    assert(y!=tree->nil);
    if (!(y->red)) RBDeleteFixUp(tree,x);
    y->left=z->left;
    y->right=z->right;
    y->parent=z->parent;
    y->red=z->red;
    z->left->parent=z->right->parent=y;
    if (z == z->parent->left) {
      z->parent->left=y; 
    } else {
      z->parent->right=y;
    }

  } else {
    if (!(y->red)) RBDeleteFixUp(tree,x);
  }
  
  assert(!tree->nil->red);
}

      
rb_red_blk_node* RBGetFit(size_t size,rb_red_blk_node *root,rb_red_blk_tree *tree)
{
	rb_red_blk_node* fit = tree->nil;
	rb_red_blk_node* x = root;
	if(x==tree->nil)return tree->nil;
	if(x->key.GetSize()<=size)
		fit = RBGetFit(size,x->right,tree);
	else
	{
		fit = x;
		rb_red_blk_node*  tmp = RBGetFit(size,x->left,tree);
		if(tmp!=tree->nil)
			fit =tmp;
	}
	return fit;
}
  
  



