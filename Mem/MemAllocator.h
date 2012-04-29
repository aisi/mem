#pragma  once
#define HEAPSIZE (1024*100)
extern   void *   heap;
int			mm_init(void);
void*	mm_malloc(size_t size);
void		mm_free(void* ptr);
void     mm_destroy();
void     mm_check();