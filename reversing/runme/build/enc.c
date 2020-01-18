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

	EVP_CIPHER_CTX  en;
	char    *key,*data;
	int     datasize;
	int     c_len,f_len=0;
	unsigned char   *ciphertext;
//	int p_stts;

	if(argc == 1)
	{
		fprintf(stderr, "Usage: %s file\n", argv[0]);
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
   				printf("\nmd5 digestlength2 : %d\n", MD5_DIGEST_LENGTH);
   				printf("src0 = %2x\n", src[0]);
   				printf("src1 = %2x\n", src[1]);
   				printf("src2 = %2x\n", src[2]);
   				printf("src3 = %2x\n", src[3]);

				/* enc */

				EVP_CIPHER_CTX_init(&en);
				EVP_EncryptInit_ex(&en,EVP_aes_128_ecb(),NULL,(unsigned char *)src,NULL);
				c_len=datasize+EVP_MAX_BLOCK_LENGTH;
				ciphertext=calloc(c_len,sizeof(char));
				EVP_EncryptUpdate(&en,(unsigned char *)ciphertext,&c_len,(unsigned char *)fdata,datasize);
				EVP_EncryptFinal_ex(&en,(unsigned char *)(ciphertext+c_len),&f_len);
				fd = open("encdata", O_CREAT|O_RDWR);
				write(fd, ciphertext, c_len+f_len);
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
			exit(0);
		}
//		wait(&p_stts);
		wait();
	}
}

void hash_md5_val(char *data, int datalen)
{
    MD5_CTX c;
    unsigned char md[MD5_DIGEST_LENGTH];
    char mdString[33];
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
         sprintf(&mdString[i * 2], "%02x", (unsigned int)md[i]);
    }
}

