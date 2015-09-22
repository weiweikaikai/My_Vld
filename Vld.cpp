#include "Vld.h"
#include<stdlib.h>
#include <stdio.h>
#define capacity 256
typedef struct MemNode
{
	int size;
	char *file;
	int  line;
	struct MemNode *next;
}MemNode;

 MemNode* hash_table[capacity];

size_t GetHash(void* p)
{
	return (size_t)p%capacity;//不用int防止指针中的地址过大
}

void* operator new(size_t sz,char*filename,int line)
{
	int hash_total = sz+sizeof(MemNode);
	MemNode*s=(MemNode*)malloc(hash_total);

	s->size=sz;
	s->file=filename;
	s->line=line;
	s->next=NULL;
	void *result = s+1;
    size_t index=GetHash(result);

	s->next = hash_table[index];
	hash_table[index]=s;

	return result;
}
void* operator new[](size_t sz,char*filename,int line)
 {
 	void *p=operator new( sz,filename, line);
 	return p;
}

void operator delete(void *ptr)
{
	 int index= GetHash(ptr);
	 MemNode *p= (MemNode*)hash_table[index];
	       if(p== NULL)
		    abort();
          if((p+1) == ptr)//是第一个节点
		  {
			  hash_table[index]=NULL;
              free(p);
			  p=NULL;
			 return;
		  }
		  MemNode*q=p->next;
		  while(q!= NULL && q+1 != ptr)
		  {
			  q=q->next;
			  p=p->next;
		  }
		  if(q==NULL)//没有找到地址
		  {
			  abort();
		  }
          p->next= q->next;
		  delete q;
		  q=NULL;
}
 void operator delete[](void *ptr)
 {
 	operator delete(ptr);
 }
void check_vld()
{
    for(int i=0;i<capacity;++i)
	{
		MemNode*p=hash_table[i];
		while(p != NULL)
		{		
   printf("Warning leaks At %p: %d Bytes\n",p+1,p->size);
   printf("file: %s, line: %d\n",p->file,p->line);
   p=p->next;
		}
	}
      printf("No memory leaks detected.\n");
}

class VLD
{
public:
	VLD()
	{

	}
	~VLD()
	{
		check_vld();
	}
};

VLD  my_vld;

