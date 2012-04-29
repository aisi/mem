#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <vector>
#include <windows.h>
#include  <iostream>
#include "MemAllocator.h"
using namespace std;
int ranged_rand(int range_min,int range_max,int exp)
{
	int i ;
	double u = (double)rand()/(RAND_MAX+1);
	for (i = 1;i<exp;i++)
	{
		u=u*u;
	}
	u=u*(range_max-range_min)+range_min;
	return (int)u;
}
int process()
{
	srand(time(0));
	vector<void*> pointers;
	int end=FALSE,op=0,allocated=0,no=0;
	size_t size;
	void *p = NULL; 
	while (!end)
	{
		op = ranged_rand(0,100,1);
		if(op<60)
		{
			
			size=ranged_rand(4,256,3);
			p=mm_malloc(size);
			if(p==NULL)
				end=TRUE;
			else
			{
				allocated++;
				pointers.push_back(p);
			}
		}
		else if(allocated>0)
		{
			no=ranged_rand(0,allocated,1);
			vector<void*>::iterator itr = pointers.begin();
			itr+=no;
			mm_free(*itr);
			pointers.erase(itr);
			allocated--;
		}
	}
	return allocated;
}

int main()
{
	mm_init();
	int size = 0;
	long time1 = clock();
	size = process();
	long time2 = clock();
	mm_destroy();
	cout<<"总共申请了"<<size<<"块"<<endl;
	cout<<"耗时"<<time2-time1<<"毫秒"<<endl;
	system("pause");
	return 0;
}
