#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
	//error handling for improper usage
	if(argc != 2)
	{
		printf("Usage: ./shm [Segment identifier]");
		return 0;
	}

	int seg_id = atoi(argv[1]); //conver id to int
	struct shmid_ds shmbuffer; // initialize buffer
	shmctl(seg_id, IPC_STAT, &shmbuffer); //load corresponding segment into buffer
	char mode[20];
	
	int size = shmbuffer.shm_segsz; //get size
	key_t key = shmbuffer.shm_perm.__key; //get key
	unsigned short mode_num = shmbuffer.shm_perm.mode; //get mode
	unsigned short uid = shmbuffer.shm_perm.uid; //get owner uid
	short attach = shmbuffer.shm_nattch; //get number of attaches

	//decipher mode permissions
	if(mode_num & 00400)
		strcpy(mode, "Read by owner");
	if(mode_num & 00200)
		strcpy(mode, "Write by owner");
	if(mode_num & 00040)
		strcpy(mode, "Read by group");
	if(mode_num & 00020)
		strcpy(mode, "Write by group");
	if(mode_num & 00004)
		strcpy(mode, "Read by world");
	if(mode_num & 00002)
		strcpy(mode, "Write by world");

	printf("Segment ID: %d\n", seg_id);
	printf("Key: %d\n", key);
	printf("Mode: %s\n", mode);
	printf("Owner UID: %d\n", uid);
	printf("Size: %d\n", size);
	printf("Number of attaches: %d\n", attach);
}
