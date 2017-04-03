#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define BIGBUF 1024
#define SMALLBUF 128

typedef enum state { ID, NUM, SYM, EQ, SYM2, COMMENT1, COMMENT2, COMMENT3, START, FIN, ERROR, IDNUM };

void get_Token(ifstream &fp_in, ofstream &fp_out);
void error_handling(char *message);
int isLetter(char input);
int isDigit(char input);
int isWS(char input);
int isSymbol(char input);
int isReserved(string input);

int main(int argc, char **argv)
{  
	char buf[BUFSIZ];

	//if (argc != 3)
		//error_handling("%s : <input.txt> <output.txt>");
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

	fp_out << "C- COMPILATION: " << "input.c" << "\n\n";

	get_Token(fp_in, fp_out);

	//fp_in.close();
	//fp_out.close();

	return 0;
}

void get_Token(ifstream &fp_in, ofstream &fp_out)
{
	enum state now_state = START;
	enum state prev_state = START;
	char temp;
	stringstream temp_string; // temporary string for many object
	string line_buf; //save one line
	string words_buf; //save words token
	string temp_buf; // save small tokens
	int line_num = 1;

	temp_string << line_num << ':' << '\t';
	line_buf = temp_string.str();
	temp_string.str("");
	fp_in.get(temp);
	while (!fp_in.eof()) {
		switch (now_state) {
		case START:
			if (isWS(temp)) {
				now_state = START;
				/*
				// ���๮�ڸ� �޴� ���� �������� �� ��Ʈ�� ����մϴ�.
				if (temp == '\n') {
					line_buf += '\n';
					fp_out << line_buf;
					fp_out << words_buf;
					temp_string << ++line_num << ':' << '\t';
					line_buf = temp_string.str();
					temp_string.str("");
					words_buf = ""; // line, words ���� ����
				}
				*/
			}
			else if (isLetter(temp))
				now_state = ID;
			else if (isDigit(temp))
				now_state = NUM;
			else if (isSymbol(temp)) {
				if (temp == '<' || temp == '>' || temp == '=' || temp == '!') // �ڿ� �ɺ��� �ϳ� �� �� �� ������ SYM ����
					now_state = SYM;
				else if (temp == '/') // /�� ���� �ּ��� ����ϴ� ����
					now_state = COMMENT1;
				else
					now_state = SYM2;
			}
			else
				now_state = ERROR;
			break;
		case ID:
			if (isLetter(temp))
				now_state = ID;
			else if (isDigit(temp))
				now_state = IDNUM;
			else if (isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = FIN;
			break;
		case NUM:
			if (isDigit(temp))
				now_state = NUM;
			else if (isLetter(temp))
				now_state = IDNUM;
			else if (isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = FIN;
			break;
		case SYM:
			if (temp == '=')
				now_state = EQ;
			else if (isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = FIN;
			break;
		case EQ:
			if (isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = FIN;
			break;
		case SYM2:
			if (isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = FIN;
			break;
		case COMMENT1:
			if (temp == '*')
				now_state = COMMENT2;
			else
				now_state = FIN;
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
		case ERROR:
			if (isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp))
				now_state = FIN;
			else
				now_state = ERROR;
			break;
		case IDNUM:
			if (isLetter(temp) || isDigit(temp))
				now_state = IDNUM;
			else
				now_state = FIN;
			break;

		}
		if (now_state == FIN) {
			if (prev_state == COMMENT1)
				temp_buf += '/';
			if (prev_state == ID)
				temp_string << '\t' << line_num << ": " << "ID, name= " << temp_buf << '\n';
			else if (prev_state == NUM)
				temp_string << '\t' << line_num << ": " << "NUM, val= " << temp_buf << '\n';
			else if (prev_state == SYM || prev_state == EQ || prev_state == SYM2)
				temp_string << '\t' << line_num << ": " << temp_buf << '\n';
			else if (prev_state == ERROR || prev_state == IDNUM)
				temp_string << '\t' << line_num << ": " << "Error: " << temp_buf << '\n';
			words_buf.append(temp_string.str());
			temp_buf = ""; //temp_buf�� ����
			temp_string.str(""); // stream�� ����
		}
		
		if (now_state != FIN) {
			if (temp != '\n')
				line_buf += temp;
			if (!isWS(temp) && now_state != COMMENT1 && now_state != COMMENT2 &&
				now_state != COMMENT3 && prev_state != COMMENT3)
				temp_buf += temp;
			// ���๮�ڸ� �޴� ���� �������� �� ��Ʈ�� ����մϴ�.
			if (temp == '\n') {
				line_buf += '\n';
				fp_out << line_buf;
				fp_out << words_buf;
				temp_string << ++line_num << ':' << '\t';
				line_buf = temp_string.str();
				temp_string.str("");
				words_buf = ""; // line, words ���� ����
			}

		}
		//FIN���� ����� ���� ���� ���� ���¸� ����
		prev_state = now_state;
		// FIN�� ��� START ���°� �ǰ� ���� ���ڸ� �ѹ� �� �д´�.
		if (now_state != FIN)
			fp_in.get(temp);
		if (now_state == FIN) {
			now_state = START;
		}
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
	if (input == '+' || input == '-' || input == '*' || input == '/' || input == '<' || input == '>' ||
		input == '=' || input == '!' || input == ';' || input == ',' || input == '(' || input == ')' ||
		input == '[' || input == ']' || input == '{' || input == '}')
		return 1;
	return 0;
}

int isReserved(string input)
{
	const string reserved[] = { "else", "if", "int", "return", "void", "while" };

	for (string temp : reserved) {
		if (!temp.compare(input)) // compare�� ������ 0�� ��ȯ
			return 1;
	}
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}