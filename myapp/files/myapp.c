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
	
	fd=open("/dev/read_mem_device", O_RDWR);
	
	target = strtoul(argv[1], 0, 0);
	

	
	printf("I am the malicious process.\n");
	printf("sched_getcpu: %d\n",sched_getcpu());
	printf("getpid: %d\n",getpid());

	printf("I want to steal the string in the physical address range 0x%lx -> 0x%lx!\n",target, target+48);	
	
	printf("The string I want to steal is as follows:\n");
	printf("\n");	
	
	for(i=0;i<10;i++){
		ioctl(fd,0,(target+i*8));
	
		read(fd,&mem_data,8);
		printf("0x%lx: %016lx\n",(target+i*8),mem_data);
	}
		
	return (0);
	
}	