#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "MemAllocator.h"
#include "red_black_tree.h"
void * heap = NULL;
#define   max(a,   b)     (((a)   >   (b))   ?   (a)   :   (b))
void  getRoot(rb_red_blk_tree *& tree)
{
	tree = (rb_red_blk_tree*)heap;
}

size_t alignUsrSize(size_t size)
{
	return max((size+ALIGESIZE)&(0xfffffff8),MINSIZE);
}
int mm_init()
{
	heap = malloc(HEAPSIZE);
	if(!heap)
		return -1;
	memset(heap,0,HEAPSIZE);
	rb_red_blk_tree * tree = RBTreeCreate(heap);
	rb_red_blk_node * node = new((char*)heap+USEDSIZE) rb_red_blk_node;
	node->key.SetSize(HEAPSIZE-USEDSIZE);
	node->key.SetPreSize(0);
	node->key.SetUseState(FREE);
	node->red = BLACK;
	node->parent = tree->root;
	node->left = tree->nil;
	node->right = tree->nil;
	tree->root->left = node;
	return 0;
}
void*	mm_malloc(size_t size)
{
	rb_red_blk_node* fit ;
	rb_red_blk_tree* tree;
	fit = NULL;
	tree = NULL;
	getRoot(tree);
	size = alignUsrSize(size);
	fit = RBGetFit (size,tree->root->left,tree);
	if(fit==tree->nil)
		return NULL;
//	fit = RBExactQuery(tree,fit->key);
	RBDelete(tree,fit);
	size_t remainSize =fit->key.GetSize()- size;
	if(remainSize>=MINSIZE)
	{
		rb_red_blk_key  key;
		key.SetSize(remainSize);
		key.SetPreSize(size);
		key.SetUseState(FREE);
		RBTreeInsert(tree,(char*)fit+size,key);
		fit->key.SetSize(size);
	}
	fit->key.SetUseState(USED);
	assert((unsigned long )fit->Node2Mem()%ALIGESIZE==0);
#ifdef _DEBUG
	mm_check();
#endif
	return fit->Node2Mem();
}
void		mm_free(void* ptr)
{
	rb_red_blk_node* wantFree = rb_red_blk_node::Mem2Node(ptr);
	rb_red_blk_node* pre  =NULL;
	rb_red_blk_tree* tree = NULL;
	rb_red_blk_node* next = NULL;
	getRoot(tree);
	int allState = 0;
	if(wantFree->HavePre())
	{
		pre= wantFree->GetPreNode();
		if(!pre->key.GetUseState())
		{
			allState|=2;
		}
	}
	if(wantFree->HaveNext())
	{
		next = wantFree->GetNextNode();
		if(!next->key.GetUseState())
		{
			allState|=1;
		}	
	}

	switch(allState)
	{
	case 0:
		{
			wantFree->key.SetUseState(FREE);
			if(wantFree->HaveNext())
			{
				wantFree->GetNextNode()->key.SetPreSize(wantFree->key.GetSize());
			}
			RBTreeInsert(tree,wantFree,wantFree->key);
		}
		break;
	case 1:
		{
			size_t total  = next->key.GetSize()+wantFree->key.GetSize();
			if(next->HaveNext())
			{
				next->GetNextNode()->key.SetPreSize(total);
			}
			wantFree->key.SetSize(total);
			wantFree->key.SetUseState(FREE);
			RBDelete(tree,next);
			RBTreeInsert(tree,wantFree,wantFree->key);
		}
		break;
	case 2:
		{
			size_t total  = pre->key.GetSize()+wantFree->key.GetSize();
			if(wantFree->HaveNext())
			{
				wantFree->GetNextNode()->key.SetPreSize(total);
			}

			RBDelete(tree,pre);
			pre->key.SetSize(total);
			RBTreeInsert(tree,pre,pre->key);
		}
		break;
	case 3:
		{
			size_t total  = pre->key.GetSize()+wantFree->key.GetSize()+next->key.GetSize();
			if(next->HaveNext())
			{
				next->GetNextNode()->key.SetPreSize(total);
			}
			RBDelete(tree,pre);
			RBDelete(tree,next);
			pre->key.SetSize(total);
			RBTreeInsert(tree,pre,pre->key);
		}
		break;
	}	   
#ifdef _DEBUG
	mm_check();
#endif
}
void		mm_destroy()
{
	free(heap);
}
void     mm_check()
{
#ifdef _DEBUG
	int state = -1;
	rb_red_blk_node* first = (rb_red_blk_node*)((char*)heap+USEDSIZE);
	int i =0;
	while(first->HaveNext())
	{
		rb_red_blk_node* next = first->GetNextNode();
		if((char*)first!=((char*)next-next->key.GetPreSize()))
			assert(0);
		/*printf("size %d    presize %d   mem %d ",first->key.GetSize(),first->key.GetPreSize(),first);*/
		if (first->key.GetUseState()==FREE)
		{
		//	printf("free \n");
			if (state==FREE)
			{
				assert(0);
			}
			state=FREE;
		}else  if(first->key.GetUseState()==USED)
		{
	//		printf("used \n");
			state = USED;
		}
		first = next;
		i++;
	}
#endif // _DEBUG
}