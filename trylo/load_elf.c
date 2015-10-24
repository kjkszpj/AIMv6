/*	little endian to big endian */
#include "sys/types.h"
#define BS 512

int main(void)
{
	/*	sdb2, start from mbr[0x1d5] */
	void (*puts)(const char *) = (void *)0x1FF0000C;
	int (*sd_dma_spin_read)(u32 pa, u16 count, u32 offset) = (void *)0x1ff00014;
	//int (*sd_dma_spin_write)(u32 pa, u16 count, u32 offset) = (void *)0x1ff00014;
	
	/* Alignment 4? */
	volatile u8 *buffer = (void *) 0x130002;
	u32 i;

	sd_dma_spin_read((u32)buffer, 1, 0);

	/* get second partition */
	/* TODO sector = page? */
	/* TODO buffer address, fixed or ...? */
	/* TODO volatile buffer? */
	u32 elf_addr = *(u32 *)(buffer + 0x1D6);
	// so far so good

	/* TODO here assume eh followed by ph */
	buffer = (void *) 0x120000;
	sd_dma_spin_read((u32)buffer, 3, elf_addr);

	void (*kernel_entry)(void) = (void *)(*(u32 *)(buffer + 0x18)); /* offset 0x18 */
	u32 *phoff = *((u32 *)(buffer + 0x1C)); /* offset 0x1C */
	u16 phentsize = *((u16 *)(buffer + 0x2A)); /* offset 0x2A */
	u16 phnum = *((u16 *)(buffer + 0x2C)); /* offset 0x2E */
	
	/* address of current program header entry */
	phoff = (u32 *)(((u32)buffer) + ((u32)phoff));
	/* TODO alignment issue */
	for (i = 0; i < phnum; i++)
	{
		// puts(".\r\n");
		u32 phe_offset = phoff[1];
		u32 phe_vaddr = phoff[2];
		u32 phe_filesz = phoff[4];
		/* u32 phe_memsz = le2be((u32 *)(phoff + 0x5)); */
		u8 offset = phe_offset & 511;
		/* TODO load to buffer */
		u32 start_id = phe_offset >> 9;
		u32 end_id = ((phe_offset + (u32)phe_filesz - 1) >> 9) + 1;
		sd_dma_spin_read(phe_vaddr - offset, end_id - start_id, (u32)(phe_offset + buffer));
		phoff += phentsize >> 2;
	}
	
	/*now entering kernel*/
	puts("MBR!\r\n");
	kernel_entry();
	while (1);
}
