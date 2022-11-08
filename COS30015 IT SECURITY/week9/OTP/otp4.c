/*FILE OTP4.c
* one-time pad generator and server
* J Hamlyn-Harris
* 10/8/2011
* multi-user persistant data version
* XOR of user ID and salt used for seed
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100
#define ERR_USER 2
#define ERR_PSWD 1
#define ERR_OK 0
#define SALT 135790

/*functions*/
int myrand(unsigned long next, unsigned uid);
void make_pad(const int number);
int validate();
void myshell();

unsigned long user = 1L;
unsigned long next1;


int main()
{
	char c = ' ';
	
	while(c != '0')
	{
		printf("OTP Authentication System\n");
		printf("0 = exit\n1 = Generate pad\n2 = Start Server\n");
		printf("command: ");
		fflush(stdin);
		scanf("%c", &c);
		switch (c)
		{
			case '0': //exit
			break;
			case '1': make_pad(SIZE);
			c = ' ';
			break;
			case '2':
			//start server
			while (1)
			{
				validate();
			}
			break;
			
			default:
			printf("Error");
		}
	}
	
	return 0;
}

/*Make the pad, and reset the seed to the first token value
* @param number - the number of successful logins permitted
*/
void make_pad(const int number)
{
	FILE * file;
	char userfname[30] = {0};
	int i;
	printf("ONE-TIME PAD\n============\n");
	printf("Enter your 6-digit User number: ");
	while(1!=scanf("%lu", &user));
	next1 = myrand(user, user);
	printf("\n");
	printf("ONE-TIME PAD\n============\n");
	printf("Here is the pad. Write down/print these numbers and keep them safe. \n Use the numbers IN ORDER and only ONCE\n");
	printf("After each successful log-in, \ncross out the used number and move onto the next\n\n");
	
	//printf("%5i\t", (int)next1); first number (stored in file) - do not display
	for (i = 1 ; i < number ; i++)
	{
		if (i%8==0) printf("\n");
		next1 = myrand(next1, user);
		printf("%lu\t", (unsigned long)next1);
	}
	printf("\n\n");
	next1 = myrand(user, user);
	sprintf(userfname, "opt%6u.dat", (unsigned)user);
	file=fopen(userfname, "w");
	if (file==0) 
	{
		printf("Error: file not writable\n");
	}
	else 
	{
		fprintf(file, "%lu", next1);
		fclose(file);
	}
	
	return;
}


/* Get the next random number
* @param next - the previous random number
* @param uid - the user's user ID (used to encode the number)
* @returns the next random number
*/
int myrand(unsigned long next, unsigned uid)  /* RAND_MAX assumed to be 32767. */
{
	next = next * 1103515245 + 12345;
	next = (unsigned)(next / 65536) % 32768;
	next = next ^ uid ^ SALT;
	return next;
}

/*validation loop - admits the user and increments the random number if valid
* @returns 1 (true) if valid; 0 (false) if invalid; 2 if no user
*/
int validate()
{
	static unsigned invalid = 0; /*3 strikes and you're out*/
	unsigned long token;
	FILE* file;
	char userfname[30] = {0};
	
	printf("\n\n\n\nWelcome to The Server\n Type your 6-digit User number to Login\n");
	while(1!=scanf("%lu", &user));
	sprintf(userfname, "opt%6u.dat", (unsigned)user);
	file=fopen(userfname, "r");	    
	if (file==0) 
	{
		printf("Error: User not registered\n");
	}
	else 
	{
		fscanf(file, "%lu", &next1);
		fclose(file);
		next1 = myrand(next1, user);
		printf("Type the next pad number to authenticate. \n");
	
		while (1 != scanf("%lu", &token));
//	  printf("next1=%lu, input=%lu\n", next1, token);
		if (token == next1)
		{
			printf("Access granted\n\n");
			file=fopen(userfname, "w");
			if (file==0) 
			{
				printf("Error: file not writable\n");
			}
			else 
			{
				fprintf(file, "%lu", next1);
				fclose(file);
			}
			invalid = 0;
			myshell();	  

			return ERR_OK;
		}
		else
		{
			printf("Access DENIED\n\n");
			invalid++;
			if (invalid > 2) exit(1);
			return ERR_PSWD;
		}
	}
	return ERR_USER; /*no user*/
}
/*test shell for successful log-ins
*/
void myshell()
{
	char command[11] = {0};
	printf("JIMUX 0.10 Fake shell for demonstration\n\n");
	printf("Available commands: \n");
	printf("ls    exit    help\n");
	while(1)
	{
		printf("\n-> ");
		scanf("%10s", command);
		if (strncmp(command, "ls", 2) == 0) printf("No files\n");
		else if (strncmp(command, "help", 4) == 0)
		{
			printf("JIMUX 0.10 Fake shell for demonstration\n\n");
			printf("Available commands: \n");
			printf("ls    exit    help\n");
		}
		else if (strncmp(command, "exit", 4)==0) break;
	}
	return;
}

	 	    

