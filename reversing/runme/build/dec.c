#define		FORKMAX 65536
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<openssl/md5.h>
#include	<openssl/evp.h>
#include        <openssl/aes.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>

void hash_md5_val();

main(int argc, char *argv[])
{
	int fr,i;
	int fc = 0;
	short cnt;
   	unsigned char src[16];

	int fd;
	char *fdata;
	int rc;
	int datalen;

	EVP_CIPHER_CTX  de;
	char    *key,*data;
	int     datasize;
	int     p_len,f_len=0;
	unsigned char   *plaintext;

//	int	p_stts;

	if(argc == 1)
	{
		fprintf(stderr, "Usage: %s encfile\n", argv[0]);
		exit(9);
	}
	fd = open(argv[1], O_RDONLY);
	fdata = malloc(1024);
	datasize = read(fd, fdata, 1024);
	close(fd);


//  init cnt
	cnt = cnt - cnt;


	for(i = 0; i<16;i++)
	{
		src[i] = 0;
	}
	for(;;)
	{
		fr = fork();
		cnt ++;
		if(fr != 0)
		{
     			hash_md5_val(src, 16);
			if(cnt < 0)
			{
				/* dec */

				EVP_CIPHER_CTX_init(&de);
				EVP_DecryptInit_ex(&de,EVP_aes_128_ecb(),NULL,(unsigned char *)src,NULL);
				p_len=datasize+EVP_MAX_BLOCK_LENGTH;
				plaintext=calloc(p_len,sizeof(char));
				EVP_DecryptUpdate(&de,(unsigned char *)plaintext,&p_len,(unsigned char *)fdata,datasize);
				EVP_DecryptFinal_ex(&de,(unsigned char *)(plaintext+p_len),&f_len);
				fd = open("flag.txt", O_CREAT|O_RDWR, 0644);
				write(fd, plaintext, p_len+f_len);
				close(fd);
				/* enc end */

				exit(0);
			}
		}
		else
		{
			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
    			asm volatile ("nop");
		}
//		wait(&p_stts);
		wait();
	}
}

void hash_md5_val(char *data, int datalen)
{
    MD5_CTX c;
    unsigned char md[MD5_DIGEST_LENGTH];
    int r, i;
    
    r = MD5_Init(&c);
    if(r != 1) {
        perror("init");
        exit(1);
    }
    
    r = MD5_Update(&c, data, datalen);
    if(r != 1) {
        perror("update");
        exit(1);
    }
    
    r = MD5_Final(md, &c);
    if(r != 1) {
        perror("final");
        exit(1);
    }
 
    for(i = 0; i < 16; i++)
    {
         data[i] = md[i];
    }
}

