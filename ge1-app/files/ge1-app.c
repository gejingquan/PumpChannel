#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <sched.h>


int main(int argc, const char **argv) {
	int  fd,i;
	long mem_data,target;
	
	fd=open("/dev/ExChannel_device", O_RDWR);
	
	target = strtoul(argv[1], 0, 0);
	
	
	ioctl(fd,0,target);
	
	read(fd,&mem_data,8);
	
	printf("0x%lx: %016lx\n",target,mem_data);
	
		
	return (0);
	
}	