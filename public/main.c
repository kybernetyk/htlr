#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         1024
#define MESSAGE_BANNER      "Oh Hai! Gief Updaet?\n"
#define MESSAGE_OK			"KTHXBAI!\n"
#define MESSAGE_ERROR		"FAIL!\n"
#define QUEUE_SIZE          5

#define HEADFILE			"head.inc"
#define DUMPFILE			"chat.dump"
#define FILEOUT 			"kragjin_chat.html"

#define PORT	0x9000

int append_to_log (char *string_to_append)
{
	printf("appending: %s\n", string_to_append);
	FILE *f_out = fopen(FILEOUT, "w+");
	if (!f_out)
	{	
		printf("could not open the gchat file!\n");
		return -1;
	}
	
	
	FILE *f_head = fopen(HEADFILE, "r");
	if (!f_head)
	{
		printf("Could not open head file!\n");
		return -3;
	}
	
	FILE *f_dump = fopen(DUMPFILE, "r");
	if (!f_dump)
	{	
		f_dump = fopen(DUMPFILE, "w+");
		if (!f_dump)
		{
			printf("DUMPFILE critical error!\n");
			return -4;
		}
	}
	
	fseek(f_dump,0x00,SEEK_END);
	unsigned int dumpsize = ftell(f_dump);
	fseek(f_dump,0x00,SEEK_SET);
	
	printf("the dumpfile size is %i\n",dumpsize);
	
	//let's read in the dumpfile and position the new entry on top of it
	char *tmp_dump = malloc(dumpsize+1);
	memset(tmp_dump,0x00,dumpsize+1);
	fread(tmp_dump,dumpsize,1,f_dump);
	fclose(f_dump);
	
	f_dump = fopen(DUMPFILE, "w");
	if (!f_dump)
	{
		printf("could not open dumpfile for writing!\n");
		return -5;
	}
	
	fseek(f_dump,0x00,SEEK_SET);
	fwrite(string_to_append, strlen(string_to_append), 1, f_dump);
	fwrite(tmp_dump, strlen(tmp_dump), 1, f_dump);
	fclose(f_dump);
	
	//now we have the dumpfile updated. let's append it to the head
	fseek(f_head,0x00,SEEK_END);
	unsigned int headsize = ftell(f_head);
	fseek(f_head,0x00,SEEK_SET);
	char *tmp_head = malloc(headsize+1);
	memset(tmp_head, 0x00, headsize+1);
	fread(tmp_head,headsize,1,f_head);
	fclose(f_head);
	
	
	fwrite(tmp_head,strlen(tmp_head),1,f_out);
	fwrite(string_to_append, strlen(string_to_append), 1, f_out);
	fwrite(tmp_dump,strlen(tmp_dump),1,f_out);
	
	fclose (f_out);
	
	free(tmp_head);
	free(tmp_dump);
	return 0;
}

int main(int argc, char* argv[])
{
    int hSocket,hServerSocket;  /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize=sizeof(struct sockaddr_in);
    char pBuffer[BUFFER_SIZE];
    int nHostPort = PORT;
	
    printf("\nStarting server");
    printf("\nMaking socket");
    /* make a socket */
    hServerSocket=socket(AF_INET,SOCK_STREAM,0);
	
    if(hServerSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }
	
    /* fill address struct */
    Address.sin_addr.s_addr=INADDR_ANY;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;
	
    printf("\nBinding to port %d",nHostPort);
	
    /* bind to a port */
    if(bind(hServerSocket,(struct sockaddr*)&Address,sizeof(Address))  == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
	/*  get port number */
    getsockname( hServerSocket, (struct sockaddr *) &Address,(socklen_t *)&nAddressSize);
    printf("opened socket as fd (%d) on port (%d) for stream i/o\n",hServerSocket, ntohs(Address.sin_port) );
	
	printf("Server\n\
		   sin_family        = %d\n\
		   sin_addr.s_addr   = %d\n\
		   sin_port          = %d\n"
		   , Address.sin_family
		   , Address.sin_addr.s_addr
		   , ntohs(Address.sin_port)
		   );
	
	
    printf("\nMaking a listen queue of %d elements",QUEUE_SIZE);
    /* establish listen queue */
    if(listen(hServerSocket,QUEUE_SIZE) == SOCKET_ERROR)
    {
        printf("\nCould not listen\n");
        return 0;
    }
	
    for(;;)
    {
		memset(pBuffer, 0x00, BUFFER_SIZE);
		
        printf("\nWaiting for a connection\n");
        /* get the connected socket */
        hSocket = accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);
		
		printf("\nGot a connection");
        strcpy(pBuffer,MESSAGE_BANNER);
		
        write(hSocket,pBuffer,strlen(pBuffer)+1);
        
		memset(pBuffer, 0x00, BUFFER_SIZE);
        read(hSocket,pBuffer,BUFFER_SIZE);
		
        printf("got message: %s\n", pBuffer);
		
		int err = append_to_log(pBuffer);
		
		memset(pBuffer, 0x00, BUFFER_SIZE);
		if (err == 0)
			strcpy(pBuffer,MESSAGE_OK);
		else
			strcpy(pBuffer,MESSAGE_ERROR);
		
		write(hSocket,pBuffer,strlen(pBuffer)+1);
		
		memset(pBuffer, 0x00, BUFFER_SIZE);
		printf("\nClosing the socket");
		
        
		/* close socket */
        if(close(hSocket) == SOCKET_ERROR)
        {
			printf("\nCould not close socket\n");
			return 0;
        }
    }
}
