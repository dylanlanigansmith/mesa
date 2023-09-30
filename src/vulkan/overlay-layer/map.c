
#include "map.h"
//static struct screenPos *players;
int map(struct shared **players) {
	
   
    *players = malloc(sizeof(struct shared) * 10);
    // Open the device file
	fd = open(DEVICE_FILE, O_RDWR);
	if (fd < 0) {
		printf("Failed to open %s: %s\n", DEVICE_FILE, strerror(errno));
		return -1;
	}

    // mmap the device file
	*players = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (*players== MAP_FAILED) {
		printf("mmap failed: %s\n", strerror(errno));
		close(fd);
		return -1;
	}

   

	  if(*players != NULL) {
        printf("mapped");
    }
    return 0;
}

int unmap(struct shared **players){
    // Cleanup
    munmap(*players, sizeof(struct shared) * 10);
	
	close(fd);

	return 0;
}
    
