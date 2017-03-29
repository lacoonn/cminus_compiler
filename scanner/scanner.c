#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIGBUF 1024
#define SMALLBUF 128

typedef enum state {ERROR, START, FIN, ID, NUM, SYM, COMMENT1, COMMENT2, COMMENT3};

void get_token(FILE *fp_out, char *buf);
void error_handling(char *message);
int isLetter(char input);
int isDigit(char input);
int isWS(char input);

int main(int argc, int **argv)
{
	FILE *fp_in, *fp_out;

	if (argc != 3)
		error_handling("%s : <input.txt> <output.txt>");


}

void get_token(FILE *fp_out, char *buf)
{
	enum state now_state = START;
	char temp;
	char line_buf[SMALLBUF]; //save one line
	int ptr = 0, line_ptr = 0, word_ptr = 0;


	while (now_state != FIN)
	{
		temp = buf[ptr];
		switch (now_state)
		{
		case START:
			if (isWS(buf[ptr]))
				now_state = START;
			else if (isLetter(buf[ptr]))
				now_state = ID;
			else if (isDigit(buf[ptr]))
				now_state = NUM;
			else if (temp == '<' || temp == '>' || temp == '=' || temp == '!')
				now_state = SYM;
			else if (temp == '/')
				now_state = COMMENT1;
			else
				now_state = FIN;
			break;
			


		}
	}
	if (now_state == FIN)
		fputs(fp_out, line_buf);


}

int isLetter(char input)
{

}

int isDigit(char input)
{

}

int isWS(char input)
{

}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}