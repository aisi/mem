#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "MemAllocator.h"
#define  MINSIZE 24
#define ALIGESIZE 8
#define TREESZIE 8
#define USEDSIZE 56
#define  USED 1
#define FREE 0
#define  RED 1
#define  BLACK 0

#pragma pack(ALIGESIZE)
class rb_red_blk_key
{
public:
	rb_red_blk_key(){}
	size_t pre;
	size_t size;
	static int cmp(rb_red_blk_key& key1,rb_red_blk_key& key2)
	{
		if((key1.size&0xfffffffe)!=(key2.size&0xfffffffe))
		{
			if((key1.size&0xfffffffe)>(key2.size&0xfffffffe))
			{
				return 1;
			}else
				return -1;
		}else if((key1.size&0xfffffffe)==(key2.size&0xfffffffe))
		{
			if((char*)(&key1)>(char*)(&key2))
			{
				return 1;
			}else if((char*)(&key1)<(char*)(&key2))
				return -1;
			else
				return 0;
		}else
		{
			return 0;
		}
	}
	bool operator > (rb_red_blk_key& key)
	{
		return  (this->size&0xfffffffe)==(key.size&0xfffffffe)?(char*)this>(char*)&key: 
			(this->size&0xfffffffe)>(key.size&0xfffffffe);
	}
	inline int GetUseState()
	{
		return this->size&0x00000001;
	}
	inline void SetUseState(int state)
	{
		if(state==USED)
			this->size|=0x00000001;
		else
			this->size&=0xfffffffe;
	}
	inline void SetSize(size_t size)
	{
		this->size=size|GetUseState();
	}
	inline size_t GetSize()
	{
		return size&0xfffffffe;
	}
	inline void SetPreSize(size_t size )
	{
		pre = size;
	}
	inline size_t GetPreSize()
	{
		return pre;
	}
};

class rb_red_blk_node 
{
public:
	rb_red_blk_node(){/*left = right = parent = NULL;red = RED;*/}
	rb_red_blk_key  key;
	int red; 
	rb_red_blk_node* left;
	rb_red_blk_node* right;
	rb_red_blk_node* parent;
	inline void* Node2Mem()
	{
		return (void*)((char*)this+(ALIGESIZE));
	}
	static  inline rb_red_blk_node* Mem2Node(void * ptr)
	{
		return (rb_red_blk_node*)((char*)ptr-(ALIGESIZE));
	}
	inline bool HaveNext()
	{
		return (((char*)this-(char*)heap+key.GetSize())<HEAPSIZE);
	}
	inline bool HavePre()
	{
		return (((char*)this-(char*)heap-key.GetPreSize())>=(USEDSIZE));
	}
	inline rb_red_blk_node* GetPreNode()
	{
		rb_red_blk_node * node = (rb_red_blk_node*)((char*)this-this->key.GetPreSize());
		return node;
	}
	inline rb_red_blk_node* GetNextNode()
	{
		rb_red_blk_node * node = (rb_red_blk_node*)((char*)this+(this->key.GetSize()));
		return node;
	}
} ;

class  rb_red_blk_tree
{
public:
	rb_red_blk_tree(){/*root = nil = NULL;*/}
	rb_red_blk_node* root;             
	rb_red_blk_node* nil;              
} ;
#pragma  pack()