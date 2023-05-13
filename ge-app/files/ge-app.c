#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, const char **argv) {
	int  fd,i;
	long mem_phy_addr,cmd;
	
	fd=open("/dev/write_mem_device", O_RDWR);
	
	mem_phy_addr = strtoul(argv[1], 0, 0);
	cmd = strtoul(argv[2], 0, 0);          

	ioctl(fd,cmd,mem_phy_addr);

	printf("cmd=%x,mem_phy_addr=%016lx\n",cmd,mem_phy_addr);

	
	return (0);
	
}
