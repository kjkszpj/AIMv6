//
// Created by Peijie You on 15/11/16.
//

#include <kern/mm/kmemory_manage.h>

//	local function
u32 up_bound(u32 size);
u32 lowbit(u32 x);


//	key function implemented
void *kmalloc(u32 size)
{
	u32* p = slb_alloc(up_bound(size));

	if (p == NULL) return NULL;
	*p = size;
	return (void*)((u32)p + sizeof(u32));
}

int kmfree(void *p)
{
	p = (void*)((u32)p - sizeof(u32));
	u32 size = *(u32*)p;
	return slb_free(p, up_bound(size));
}


//	TODO, should be used only in this file
inline u32 lowbit(u32 x) {return x & (-x);}

u32 up_bound(u32 size)
{
//	upbound to power of two
//	add meta data
	size += sizeof(u32);
	while (size != lowbit(size)) size += lowbit(size);
	return size;
}
