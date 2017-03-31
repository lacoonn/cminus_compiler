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
	char line_buf[SMALLBUF], //save one line
		words_buf[SMALLBUF]; //save words token
	int ptr = 0, line_ptr = 0, words_ptr = 0;


	while (now_state != FIN || now_state != ERROR)
	{
		temp = buf[ptr];
		switch (now_state)
		{
		case START:
			if (isWS(buf[ptr]))
			{
				now_state = START;
				if (temp == '\n')
				{
					fwrite(line_buf, 1, strlen(line_buf), fp_out);
					fwrite(words_buf, 1, strlen(words_buf), fp_out);
				}
			}
			else if (isLetter(buf[ptr]))
				now_state = ID;
			else if (isDigit(buf[ptr]))
				now_state = NUM;
			else if (temp == '<' || temp == '>' || temp == '=' || temp == '!')
				now_state = SYM;
			else if (temp == '/')
				now_state = COMMENT1;
			else
				now_state = ERROR;
			break;
		case ID:
			if (isLetter(temp))
				now_state = ID;

			break;



		}
	}
	if (now_state == FIN)
		fputs(fp_out, line_buf);


}

int isLetter(char input)
{
	if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'))
		return 1;
	else
		return 0;
}

int isDigit(char input)
{
	if (input >= '0' && input <= '9')
		return 1;
	else
		return 0;
}

int isWS(char input)
{
	if (input == ' ' || input == '\n' || input == '\t')
		return 1;
	else
		return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}