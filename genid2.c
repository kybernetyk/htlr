#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <inttypes.h>
size_t get_randline (char *filename, char *destptr, size_t maxlen)
{
	int fd_in = open (filename,O_RDONLY);
	
	if (fd_in == -1)
	{
		fprintf(stderr,"could not open file!\n");
		return 0;
	}
	
	struct stat sb;
	if (fstat (fd_in, &sb) == -1) 
	{
		fprintf(stderr,"could not get status of file!\n");
		return 0;
	}
	
	if (!S_ISREG (sb.st_mode))
	{
		fprintf(stderr,"file not a file!\n");
		return 0;
	}
	
	size_t file_size = sb.st_size;
	void *map = mmap(0,file_size,PROT_READ,MAP_SHARED,fd_in,0x00);
	
	if (map == MAP_FAILED)
	{
		fprintf(stderr,"map has failed with error: %s\n",strerror(errno));
		return 0;
	}
	
	unsigned char *cmap = (unsigned char*)map;
	
	size_t rand_index = 0;
	
 	if (sizeof(size_t) == 8) //64 bit rand
 	{	 
		rand_index =rand();
		rand_index <<= 32;
		unsigned int rnd = rand();
		rand_index ^= (rnd);
		rand_index = rand_index % file_size;
	}
	else
	{
		rand_index = rand() % file_size;
	}
	
	//	printf("file size: %p\n",file_size);
	//	printf("rand index: %p\n",rand_index);
	
	if (rand_index >= file_size)
		rand_index = file_size-2; //-2 to ommit the last forced \n
	
	unsigned char c = 0;
	unsigned char *start = 0;
	size_t start_index = rand_index;
	/* find the beginning of current line */
	do
	{
		c = cmap[start_index];
		if (c == '\n')
		{
			start_index++;
			break;
		}
		
		if (start_index <= 0)
		{
			break;
		}
		start_index --;
		
	} while (1); 
	
	start = cmap+start_index;
	//	printf("start: %p\n",start_index);	
	
	
	/* get the length of current line */
	size_t length = 0;
	do
	{
		c = start[length];
		if (c == '\n')
			break;
		
		length ++;
		
		if (start_index+length >= file_size)
		{
			length --;
			break;
		}
	} while (1);
	
	//	printf("length: %p\n",length);
	
	/* copy line to tempbuffer as we may not write the mmaped memory */
	/*char *tmp_buffer = (char *)malloc(length+1);
	memset(tmp_buffer,0x00,length+1);
	memcpy(tmp_buffer,start,length);
	*/
	
	if (length > maxlen)
	{
		length = maxlen;
	}

	memcpy(destptr,start,length);
	
	munmap(map,file_size);
	close(fd_in);
	
	return length;
}

int main (int argc, char **argv)
{
	srand(time(0));
	
	if (argc != 2)
	{
		fprintf(stderr,"usage: %s <int seed>\n",argv[0]);
		return 1;
	}
	
	unsigned int seed = atoi(argv[1]);
	srand(seed);
	
	char fname[256];
	memset(fname,0x00,256);
	size_t fnbytes = get_randline("fnames",fname,255);

	char lname[256];
	memset(lname,0x00,256);
	size_t lnbytes = get_randline("lnames",lname,255);

	if (fnbytes == 0 || lnbytes == 0)
	{
		fprintf(stderr,"could not generate id!\n");
		return 1;
	}
	
	printf("%s-%s",fname,lname);
	
	return 0;
}
