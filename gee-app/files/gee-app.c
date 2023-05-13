//mmap.c
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



size_t libkdump_virt_to_phys(size_t virtual_address) {
  static int pagemap = -1;
  if (pagemap == -1) {
    pagemap = open("/proc/self/pagemap", O_RDONLY);
    if (pagemap < 0) {
      errno = EPERM;
      return 0;
    }
  }
  uint64_t value;
  int got = pread(pagemap, &value, 8, (virtual_address / 0x1000) * 8);
  if (got != 8) {
    errno = EPERM;
    return 0;
  }
  uint64_t page_frame_number = value & ((1ULL << 54) - 1);
  if (page_frame_number == 0) {
    errno = EPERM;
    return 0;
  }
  return page_frame_number * 0x1000 + virtual_address % 0x1000;
}


int main()
{
	
	
   /*映射一片内存*/
   void* p = mmap(0,//让系统指定映射内存的首地址(虚拟地址)
        4,//映射4bytes的空间，其实会映射一页内存
        PROT_READ|PROT_WRITE,//权限，有读写权限
        MAP_PRIVATE|MAP_ANONYMOUS,/*MAP_ANONYMOUS:匿名映射，当映射到内存时，只能使用匿名映射。MAP_SHARED和MAP_PRIVATE必选其一。*/ 
        0,//文件描述符，用于映射到文件，内存映射中会忽略
        0);//文件偏移量，用于文件，内存映射也会忽略
	if(p==MAP_FAILED){
		perror("映射失败");
		exit(-1);
	}
	

	strcpy((char *)p,            "################");
	strcpy((char *)((long)p+16), "    I am key!   ");
	strcpy((char *)((long)p+32), "################");

	
	size_t p_phy = libkdump_virt_to_phys((long)p);		

	printf("I am the CA process.\n");
	printf("sched_getcpu: %d\n",sched_getcpu());
	printf("getpid: %d\n",getpid());
	printf("Virtual address of key page: 0x%lx\n",(long)p);
	printf("Physical address of key page: 0x%lx\n",(long)p_phy);

	printf("My secret key string is as follows:\n");
	printf("\n");
	printf("%.*s\n", 16, (char *)p);
	printf("%.*s\n", 16, (char *)((long)p+16));
	printf("%.*s\n", 16, (char *)((long)p+32));	

	
	while(1);
	munmap(p, 4);
}
