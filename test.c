#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>

#define RESET 0
#define READ_UNFULL 1

typedef struct {
	long pcid;
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
    // printf("int:%d\n",sizeof(int));
    // printf("u long:%d\n",sizeof(unsigned long));
    // printf("u long long:%d\n",sizeof(unsigned long long));
    // printf("segfault:%d\n", sizeof(SegFault));

    const char *filename = "log.txt";
    FILE *p = fopen(filename, "ab");
    if(p==NULL){
        printf("open log.txt failed!");
        return 0;
    }
    printf("usr read\n");
    vmallocmem_in = (SegFault *)malloc(sizeof(SegFault));
    int fd = open("/dev/extern1", O_RDWR, S_IRUSR|S_IWUSR);
    if(fd != -1){
        // ioctl(fd, RESET);
        while(!read(fd, vmallocmem_in, sizeof(SegFault))){
            read_segfault();
            fprintf(p, "{%d,%lu,%llu}\r\n",vmallocmem_in->pcid, vmallocmem_in->address, vmallocmem_in->timestamp);
            // fflush(p);
        }
        printf("read end\n");
        fclose(p);  
        close(fd);    
    }else{
        printf("wrong!!");
    }
    free(vmallocmem_in);
    
    return 0;
}