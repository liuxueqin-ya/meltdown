#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// #include <unistd.h>
// #include <linux/ioctl.h>
#include <string.h>
// #include <errno.h>
#include <malloc.h>

typedef struct {
	int pcid;
	unsigned long address;
	unsigned long long timestamp;
    
}SegFault;
SegFault *vmallocmem_in;

void read_segfault(void) {
	printf("test read_segfault\n");
	printf("pcid:%d\n",vmallocmem_in->pcid);
	printf("address:%lu\n",vmallocmem_in->address);
	printf("timestamp:%llu\n", vmallocmem_in->timestamp);
}

int main(){

    const char *filename = "log.txt";
    FILE *p = fopen(filename, "ab");
    if(p==NULL){
        printf("open log.txt failed!");
        return 0;
    }
    printf("usr read\n");
    vmallocmem_in = (SegFault *)malloc(sizeof(SegFault));
    int fd = open("/dev/extern1", O_RDWR, S_IRUSR|S_IWUSR);
    // printf("%d\n", fd);
    if(fd != -1){
        while(!read(fd, vmallocmem_in, sizeof(SegFault))){  
            read_segfault();
            fprintf(p, "{%d,%lu,%llu}\r\n",vmallocmem_in->pcid, vmallocmem_in->address, vmallocmem_in->timestamp);
        }
        printf("read end\n");      
    }else{
        printf("wrong!!");
    }
    close(fd);
    free(vmallocmem_in);
    fclose(p);
    return 0;
}