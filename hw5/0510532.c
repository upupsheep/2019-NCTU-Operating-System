#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <inttypes.h>
#include "os_hw5_driver.h"

int fd;

// Obtain my cr3 value (a.k.a. PML4 table physical address)
uint64_t get_cr3_value()
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_CR3;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Given a physical address, return the value
uint64_t read_physical_address(uint64_t physical_address)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_READ;
	cmd.request[1] = physical_address;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Write value to a physical address
void write_physical_address(uint64_t physical_address, uint64_t value)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_WRITE;
	cmd.request[1] = physical_address;
	cmd.request[2] = value;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
}

uint64_t get_entry_content(uint64_t target_va, uint64_t base_addr, uint64_t mask, uint64_t shift){
	//printf("base addr: %" PRIu64 "\n", base_addr);
	
	uint64_t offset = target_va & mask;
	offset = (offset >> shift) * 8;
	//printf("offset: %" PRIu64 "\n", offset);
	
	uint64_t pt_entry = base_addr | offset;
	//printf("page table entry: %" PRIu64 "\n", pt_entry);

	//uint64_t entry_content = pt_entry;
        uint64_t entry_content = read_physical_address(pt_entry);
	//printf("entry content: %" PRIu64 "\n", entry_content);

	return entry_content;
	//return *pt_entry;
}

uint64_t page_table_entry_addr(uint64_t target_va, uint64_t base_addr, uint64_t mask, uint64_t shift){
	//printf("base addr: %" PRIu64 "\n", base_addr);
	
	uint64_t offset = target_va & mask;
	offset = (offset >> shift) * 8;
	//printf("offset: %" PRIu64 "\n", offset);
	
	uint64_t pt_entry = base_addr | offset;
	//printf("page table entry: %" PRIu64 "\n", pt_entry);

	return pt_entry;
	//return *pt_entry;
}

uint64_t virtual_to_physical(char *x, int want_pt_addr){
	uint64_t cr3_val = get_cr3_value();
	//printf("cr3: %" PRIu64 "\n", cr3_val);
	
	// level 1: PML4
	//printf("PMLe:\n");
	uint64_t mask = 280925220896768;

	uint64_t PML4_content = get_entry_content((uint64_t)x, cr3_val, mask, 39);
	
	//printf("\n-------------\n");

	// level 2: PDPE
	//printf("PDPD\n");
	mask = mask >> 9;
	uint64_t PDPE_base = (PML4_content & 0xFFFFF000); // 0xFFFFF000
	uint64_t PDPE_content = get_entry_content((uint64_t)x, PDPE_base, mask, 30);

	//printf("\n-------------\n");

	// level 3: PD
	//printf("PD\n");
	mask = mask >> 9;
	uint64_t PD_base = (PDPE_content & 0xFFFFF000);
	uint64_t PD_content = get_entry_content((uint64_t)x, PD_base, mask, 21);

	//printf("\n-------------\n");
	
	// level 4: PTE
	//printf("PTE\n");
	mask = mask >> 9;
	uint64_t PTE_base = (PD_content & 0xFFFFF000);

	if (want_pt_addr == 0){
		uint64_t PTE_content = get_entry_content((uint64_t)x, PTE_base, mask, 12);
		return PTE_content;
		//printf("\n--------------\n");
	} else {
		uint64_t pt_entry_address = page_table_entry_addr((uint64_t)x, PTE_base, mask, 12);
		return pt_entry_address;
		//printf("\n--------------\n");
	}
}


int main()
{
	char *x = (char*)aligned_alloc(4096, 4096) + 0x123;
	char *y = (char*)aligned_alloc(4096, 4096) + 0x123;
	strcpy(x, "This is OS homework 5.");
	strcpy(y, "You have to modify my page table.");

	fd = open("/dev/os", O_RDONLY);
	if(fd < 0) 
	{
		printf("Cannot open device!\n");
		return 0;
	}

	printf("Before\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	/* TODO 1 */
	// ------------------------------------------------
	// Modify page table entry of y
	// Let y point to x's physical address
	//printf("x's addr: %p, y's addr: %p\n", x, y);
	//printf("\n");

	//printf("x: \n");
	uint64_t x_pa = virtual_to_physical(x, 0);
	//printf("\n===================\n");
	//printf("y: \n");
	uint64_t y_pa = virtual_to_physical(y, 0);
	//printf("\n===================\n");
	uint64_t y_pt_entry_addr = virtual_to_physical(y, 1);

	write_physical_address(y_pt_entry_addr, x_pa);
	// ------------------------------------------------

	getchar();

	printf("After modifying page table\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	getchar();

	strcpy(y, "When you modify y, x is modified actually.");
	printf("After modifying string y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);
	
	/* TODO 2 */
	// ------------------------------------------------
	// Recover page table entry of y
	// Let y point to its original address
	// You may need to store y's original address at previous step
	
	write_physical_address(y_pt_entry_addr, y_pa);

	// ------------------------------------------------

	getchar();

	printf("After recovering page table of y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	close(fd);
}
