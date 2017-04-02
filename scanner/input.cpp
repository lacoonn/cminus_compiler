#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define BIGBUF 1024
#define SMALLBUF 128

typedef enum state { ERROR, START, FIN, ID, NUM, SYM, COMMENT1, COMMENT2, COMMENT3 };

void get_Token(ifstream &fp_in, ofstream &fp_out);
void error_handling(char *message);
int isLetter(char input);
int isDigit(char input);
int isWS(char input);
int isSymbol(char input);

int line_num = 0;

using namespace std;

int main(int argc, char **argv)
{
	char buf[BUFSIZ];

	if (argc != 3)
		error_handling("%s : <input.txt> <output.txt>");
	/*
	if ((fp_in = fopen(argv[1], "r")) == NULL)
		error_handling("input file open error");
	if ((fp_out = fopen(argv[2], "w")) == NULL)
		error_handling("input file open error");
	*/
	//ifstream fp_in(argv[1]);
	//ofstream fp_out(argv[2]);
	ifstream fp_in("input.c");
	ofstream fp_out("output.txt");

	get_Token(fp_in, fp_out);

}

void get_Token(ifstream &fp_in, ofstream &fp_out)
{
	enum state now_state = START; // 사실 이전의 상태임(ptr - 1)
	enum state prev_state = START;
	char temp;
	stringstream temp_string; // temporary string for many object
	string line_buf; //save one line
	string words_buf; //save words token
	string temp_buf; // save small tokens
	int ptr = 0, line_ptr = 0, words_ptr = 0;

	while (fp_in.get(temp)) {
		switch (now_state) {
		case START:
			if (isWS(temp)) {
				now_state = START;
				if (temp == '\n') {
					line_buf += '\n';
					fwrite(line_buf.data(), 1, line_buf.size(), fp_out);
					fwrite(words_buf.data(), 1, words_buf.size(), fp_out);
					line_buf = "";
					words_buf = ""; // line, words 버퍼 비우기
					line_num++;
				}
			}
			else if (isLetter(temp))
				now_state = ID;
			else if (isDigit(temp))
				now_state = NUM;
			else if (isSymbol(temp)) {
				if (temp == '<' || temp == '>' || temp == '=' || temp == '!') // 뒤에 심볼이 하나 더 올 수 있으면 SYM 상태
					now_state = SYM;
				else if (temp == '/') // /가 오면 주석을 대기하는 상태
					now_state = COMMENT1;
				else
					now_state = FIN;
			}
			else
				now_state = ERROR;
			break;
		case ID:
			if (isLetter(temp))
				now_state = ID;
			else if (isDigit(temp) || isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = ERROR;
			break;
		case NUM:
			if (isDigit(temp))
				now_state = NUM;
			else if (isLetter(temp) || isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = ERROR;
			break;
		case SYM:
			if (temp == '=')
				now_state = FIN;
			else if (isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = ERROR;
			break;
		case COMMENT1:
			if (isSymbol(temp)) {
				if (temp == '*')
					now_state = COMMENT2;
				else
					now_state = FIN;
			}
			else if (isLetter(temp) || isDigit(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = ERROR;
			break;
		case COMMENT2:
			if (temp == '*')
				now_state = COMMENT3;
			else
				now_state = COMMENT2;
			break;
		case COMMENT3:
			if (temp == '*')
				now_state = COMMENT3;
			else if (temp == '/')
				now_state = START;
			else
				now_state = COMMENT2;
			break;
		}
		if (now_state == FIN) {

			temp_string.str(); // stream을 비운다
			if (prev_state == ID)
				temp_string << '\t' << line_num << ': ' << "ID, name= ";
			if (prev_state == NUM)
				temp_string << '\t' << line_num << ': ' << "NUM, val= ";
			if (prev_state == SYM || prev_state == START)
				temp_string << '\t' << line_num << ': ';
			words_buf.append(temp_string.str());
			words_buf.append(temp_buf);
			words_buf += '\n';
			temp_buf = ""; //temp_buf를 비운다

			now_state = START;

			if (temp == '\n') { // 개행문자가 들어올경우
				line_buf += '\n';
				fwrite(line_buf.data(), 1, line_buf.size(), fp_out);
				fwrite(words_buf.data(), 1, words_buf.size(), fp_out);
				line_buf = "";
				words_buf = ""; // line, words 버퍼 비우기
				line_num++;
			}
		}
		if (now_state == ERROR) {
		}

		prev_state = now_state;

		//isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp)
		//출력하고 각 버퍼를 비울 시점!!!

		if (temp != '\n')
			line_buf += temp;
		if (!isWS(temp))
			temp_buf += temp;
		ptr++;
	}



}

int isLetter(char input)
{
	if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'))
		return 1;
	return 0;
}

int isDigit(char input)
{
	if (input >= '0' && input <= '9')
		return 1;
	return 0;
}

int isWS(char input)
{
	if (input == ' ' || input == '\n' || input == '\t')
		return 1;
	return 0;
}

int isSymbol(char input) {
	if (input == '+' || input == '-' || input == '*' || input == ';' || input == '<' || input == '>' || input == '=' || input == '!' ||
		input == ',' || input == '(' || input == ')' || input == '[' || input == ']' || input == '/')
		return 1;
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}