#include "red_black_node.h"
#include <assert.h>


rb_red_blk_tree* RBTreeCreate(void *  );
rb_red_blk_node * RBTreeInsert(rb_red_blk_tree*& , void* ,rb_red_blk_key&);
void RBDelete(rb_red_blk_tree*& , rb_red_blk_node* );
rb_red_blk_node* RBExactQuery(rb_red_blk_tree*, rb_red_blk_key&);
rb_red_blk_node* RBGetFit(size_t ,rb_red_blk_node *,rb_red_blk_tree *);
