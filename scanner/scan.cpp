#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// dfa���� ���� state���Դϴ�.
enum state { ID, NUM, SYM, EQ, SYM2, WOW, COMMENT1, COMMENT2, COMMENT3, START, FIN, ERROR, IDNUM };

void getToken(ifstream &fp_in, ofstream &fp_out);
bool isLetter(char input);
bool isDigit(char input);
bool isWS(char input);
bool isSymbol(char input);
bool isReserved(string input);

int main(int argc, char **argv)
{  

	if (argc != 3)
		fputs("scan.exe <input.c> <output.txt>", stderr);

	ifstream fp_in(argv[1]);
	ofstream fp_out(argv[2]);

	fp_out << "C- COMPILATION: " << argv[1] << "\n\n";

	// getToken �Լ��� ���Ͻ�Ʈ���� �Ѱ��ָ� getToken �Լ��� ���Ͻ�Ʈ������
	// �ؽ�Ʈ�� �о� ��ū���� ���� �� ����մϴ�.
	getToken(fp_in, fp_out);

	fp_in.close();
	fp_out.close();

	return 0;
}

void getToken(ifstream &fp_in, ofstream &fp_out)
{
	// ���� �� �ʱ�ȭ �κ��Դϴ�.
	// ���� �� �� �ܰ� ���� ���¸� �����ϴ� ������ �����ϰ� START�� �ʱ�ȭ�մϴ�.
	enum state now_state = START;
	enum state prev_state = START;
	char temp = 0, line_char[256];
	stringstream temp_string; // ���ϴ� ��� ����� ���� �� ����� ��Ʈ����Ʈ���Դϴ�.
	string words_buf; // ��¾�� �� ��ū�θ� �����ϴ� ��Ʈ���Դϴ�.
	string temp_buf; // ��ū�� ��¾�Ŀ� ���� words_buf�� �ű�� ������ �����ϰ� �ִ� ��Ʈ���Դϴ�.
	string line_string; // getline�� ���������� ������ �����ϰ� ���� ��Ʈ���Դϴ�.
	int line_num = 1, ptr;

	while (!fp_in.eof()) {
		fp_in.getline(line_char, 256); // �Է� ���Ͻ�Ʈ�����κ��� �� ������ �н��ϴ�.
		line_string = line_char;
		line_string += '\n'; // getline�� ����ϸ鼭 �ҽǵ� ���๮�ڸ� �ٿ��ݴϴ�. eof�ÿ� ������ ����� ���ؼ��� ���˴ϴ�.
		ptr = 0; // ���� ������ �� ��° ĳ���͸� ����ų ������ �����ϴ� �����Դϴ�.

		while (1) {
			if (now_state != FIN) {
				temp = line_string.at(ptr++); // ���� FIN ���·� ������ �ʾҴٸ� ���� ĳ���͸� �н��ϴ�.
			}
			if (now_state == FIN) {
				now_state = START; // ������ FIN ���·� �����ٸ� START ���·� �̵��մϴ�.
			}
			switch (now_state) { // switch������ state�� ��ȭ�� �����մϴ�.
			case START:
				if (isWS(temp))
					now_state = START;
				else if (isLetter(temp))
					now_state = ID;
				else if (isDigit(temp))
					now_state = NUM;
				else if (isSymbol(temp)) {
					if (temp == '<' || temp == '>' || temp == '=') // �ڿ� �ɺ��� �ϳ� �� �� �� ������ SYM ����
						now_state = SYM;
					else if (temp == '!') // ����ǥ�� ȥ�ڿ��� �������� �ڿ� ���� ���ڿ� ���� �ɺ��� �� �� �ֱ⶧���� ������ ���·� �Ӵϴ�.
						now_state = WOW;
					else if (temp == '/') // /�� ���� �ּ��� ����ϴ� ����
						now_state = COMMENT1;
					else
						now_state = SYM2; // ���� �ɺ��� ������ SYM2 ����
				}
				else
					now_state = ERROR; // c-minus���� ������� �ʴ� ���ڸ� ERROR ����
				break;
			case ID:
				if (isLetter(temp))
					now_state = ID;
				else if (isDigit(temp))
					now_state = IDNUM; // ���� + ���ڴ� �������� ������ ���·� �Ӵϴ�.
				else
					now_state = FIN;
				break;
			case NUM:
				if (isDigit(temp))
					now_state = NUM;
				else if (isLetter(temp))
					now_state = IDNUM; // ���� ����
				else
					now_state = FIN;
				break;
			case SYM:
				if (temp == '=')
					now_state = EQ; // �ڿ� = �� ���� �̰ͱ��� �����ϰ� �ƴϸ� FIN���� ���� ����մϴ�.
				else
					now_state = FIN;
				break;
			case EQ:
				now_state = FIN; // ��ǻ� = ���� �ٿ��ֱ� ���� �����̹Ƿ� ������ ������ FIN���� ���ϴ�.
				break;
			case SYM2:
				now_state = FIN; // �ڿ� ������ ������ ���� �ɺ��̹Ƿ� ���� FIN���� ���ϴ�.
				break;
			case WOW:
				if (temp == '=') // �ڿ� = �� ���� �ɺ��̱⶧���� EQ�� �̵��մϴ�.
					now_state = EQ;
				else if (isLetter(temp) || isDigit(temp) || isWS(temp) || isSymbol(temp)) // c-minus ���� �ȿ� �ִ� ĳ���Ͱ� ���� ���� �����Դϴ�.
					now_state = FIN;
				else
					now_state = ERROR; // �ڿ� ���� ���ڰ� ���� ��� ó���մϴ�.
			case COMMENT1: // �ּ��κ��� �����ڷ�� �����մϴ�.
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
			case ERROR: // ���� �����Դϴ�. �ڿ� ������ ���� ��� ó���ϰ� �ƴϸ� FIN���� ���� ��µ˴ϴ�.
				if (isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp))
					now_state = FIN;
				else
					now_state = ERROR;
				break;
			case IDNUM: // ����+���� Ȥ�� ����+���ڸ� ó���� �����Դϴ�. �̰� �ܿ� �ٸ� ���ڰ� ���� FIN���� ���� ����մϴ�.
				if (isLetter(temp) || isDigit(temp))
					now_state = IDNUM;
				else
					now_state = FIN;
				break;

			}
			// ��� ����� FIN���� �̷�����ϴ�.
			// FIN���� ����� �� ���� ���� ���ڸ� ������ �ʱ� ���� START ���·� �ٽ� ����ֽ��ϴ�.
			if (now_state == FIN) { 
				if (prev_state == COMMENT1) // �ּ� ���ۺδ� �̰� �ּ����� ���� ���� ������ �̸� �� �� ���� ������ FIN���� �������ݴϴ�.
					temp_buf += '/';
				if (prev_state == ID) { // ID�� ��� ��¾���Դϴ�.
					temp_string << '\t';
					if (isReserved(temp_buf)) // ID���� �� �� ��������� Ȯ���մϴ�.
						temp_string << line_num << ": " << "reserved word: " << temp_buf << '\n';
					else
						temp_string << line_num << ": " << "ID, name= " << temp_buf << '\n';
				}
				else if (prev_state == NUM) // NUM�� ��� ��¾���Դϴ�.
					temp_string << '\t' << line_num << ": " << "NUM, val= " << temp_buf << '\n';
				else if (prev_state == SYM || prev_state == EQ || prev_state == SYM2 || prev_state == COMMENT1) // symbol�� ��� ��¾���Դϴ�.
					temp_string << '\t' << line_num << ": " << temp_buf << '\n';
				else if (prev_state == ERROR || prev_state == IDNUM || prev_state == WOW) // ������ ó���Ǵ� ������ ��� ��¾���Դϴ�.
					temp_string << '\t' << line_num << ": " << "Error: " << temp_buf << '\n';
				words_buf.append(temp_string.str()); // ������� ��ū(��¾��)�� ���ۿ� �߰��մϴ�.
				// ����ߴ� �ӽ� ���۸� ���ϴ�.
				temp_buf = "";
				temp_string.str(""); 
			}

			if (now_state != FIN) {
				// ȭ��Ʈ�����̽�, �ּ��� ��ū ���ۿ� ���� �ʽ��ϴ�.
				if (!isWS(temp) && now_state != COMMENT1 && now_state != COMMENT2 &&
					now_state != COMMENT3 && prev_state != COMMENT3)
					temp_buf += temp;
				// ���๮�ڸ� �޴� ���� �������� �� ������ ��Ʈ�� ����մϴ�.
				if (temp == '\n') {
					// temp_string�� ��� ����� ���߰� �� line�� ����մϴ�.
					temp_string << line_num++ << ':' << '\t' << line_string;
					fp_out << temp_string.str();
					// �� ���ο��� ������ ��ū�� ����մϴ�.
					fp_out << words_buf;
					// ����� ���۸� ���ϴ�.
					temp_string.str("");
					words_buf = ""; 
					break;
				}
			}
			//FIN���� ����� ���� ���� ������ ���¸� �����մϴ�.
			prev_state = now_state;

		}	
	}
	// �ּ� ���¿��� ����Ǹ� �����޽����� ����մϴ�.
	if (now_state == COMMENT2 || now_state == COMMENT3) {
		fp_out << "Error: stop before ending";
	}
}

// �Ʒ��� c-minus ���� ���� �ִ� ĳ�������� Ȯ���ϱ� ���� �Լ����Դϴ�.

bool isLetter(char input)
{
	if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'))
		return true;
	return false;
}

bool isDigit(char input)
{
	if (input >= '0' && input <= '9')
		return true;
	return false;
}

bool isWS(char input)
{
	if (input == ' ' || input == '\n' || input == '\t')
		return true;
	return false;
}

bool isSymbol(char input) {
	if (input == '+' || input == '-' || input == '*' || input == '/' || input == '<' || input == '>' ||
		input == '=' || input == ';' || input == ',' || input == '(' || input == ')' ||
		input == '[' || input == ']' || input == '{' || input == '}')
		return true;
	return false;
}

// ���� Ȯ���ϴ� �Լ��Դϴ�.
// ����� ���ڰ� ���� �ʾ� �׳� string �迭�� ó���߽��ϴ�.
bool isReserved(string input)
{
	const string reserved[] = { "else", "if", "int", "return", "void", "while" };

	for (string temp : reserved) {
		if (!temp.compare(input)) // compare�� ������ 0�� ��ȯ
			return true;
	}
	return false;
}