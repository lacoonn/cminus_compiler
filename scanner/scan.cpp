#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// dfa에서 나온 state들입니다.
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

	// getToken 함수로 파일스트림을 넘겨주면 getToken 함수가 파일스트림에서
	// 텍스트를 읽어 토큰으로 분해 후 출력합니다.
	getToken(fp_in, fp_out);

	fp_in.close();
	fp_out.close();

	return 0;
}

void getToken(ifstream &fp_in, ofstream &fp_out)
{
	// 선언 및 초기화 부분입니다.
	// 현재 및 한 단계 전의 상태를 저장하는 변수를 생성하고 START로 초기화합니다.
	enum state now_state = START;
	enum state prev_state = START;
	char temp = 0, line_char[256];
	stringstream temp_string; // 원하는 출력 양식을 만들 때 사용할 스트링스트림입니다.
	string words_buf; // 출력양식 중 토큰부를 저장하는 스트링입니다.
	string temp_buf; // 토큰을 출력양식에 맞춰 words_buf로 옮기기 전까지 저장하고 있는 스트링입니다.
	string line_string; // getline을 실행했을때 라인을 저장하고 있을 스트링입니다.
	int line_num = 1, ptr;

	while (!fp_in.eof()) {
		fp_in.getline(line_char, 256); // 입력 파일스트림으로부터 한 라인을 읽습니다.
		line_string = line_char;
		line_string += '\n'; // getline을 사용하면서 소실된 개행문자를 붙여줍니다. eof시에 마지막 출력을 위해서도 사용됩니다.
		ptr = 0; // 읽은 라인의 몇 번째 캐릭터를 가리킬 것인지 저장하는 변수입니다.

		while (1) {
			if (now_state != FIN) {
				temp = line_string.at(ptr++); // 전에 FIN 상태로 끝나지 않았다면 다음 캐릭터를 읽습니다.
			}
			if (now_state == FIN) {
				now_state = START; // 이전에 FIN 상태로 끝났다면 START 상태로 이동합니다.
			}
			switch (now_state) { // switch문으로 state의 변화를 구현합니다.
			case START:
				if (isWS(temp))
					now_state = START;
				else if (isLetter(temp))
					now_state = ID;
				else if (isDigit(temp))
					now_state = NUM;
				else if (isSymbol(temp)) {
					if (temp == '<' || temp == '>' || temp == '=') // 뒤에 심볼이 하나 더 올 수 있으면 SYM 상태
						now_state = SYM;
					else if (temp == '!') // 느낌표는 혼자오면 에러지만 뒤에 오는 문자에 따라 심볼이 될 수 있기때문에 별도의 상태로 둡니다.
						now_state = WOW;
					else if (temp == '/') // /가 오면 주석을 대기하는 상태
						now_state = COMMENT1;
					else
						now_state = SYM2; // 단일 심볼로 끝나면 SYM2 상태
				}
				else
					now_state = ERROR; // c-minus에서 사용하지 않는 문자면 ERROR 상태
				break;
			case ID:
				if (isLetter(temp))
					now_state = ID;
				else if (isDigit(temp))
					now_state = IDNUM; // 문자 + 숫자는 에러지만 별도의 상태로 둡니다.
				else
					now_state = FIN;
				break;
			case NUM:
				if (isDigit(temp))
					now_state = NUM;
				else if (isLetter(temp))
					now_state = IDNUM; // 위와 동일
				else
					now_state = FIN;
				break;
			case SYM:
				if (temp == '=')
					now_state = EQ; // 뒤에 = 가 오면 이것까지 저장하고 아니면 FIN으로 가서 출력합니다.
				else
					now_state = FIN;
				break;
			case EQ:
				now_state = FIN; // 사실상 = 까지 붙여주기 위한 상태이므로 무엇이 오더라도 FIN으로 갑니다.
				break;
			case SYM2:
				now_state = FIN; // 뒤에 무엇이 오더라도 단일 심볼이므로 역시 FIN으로 갑니다.
				break;
			case WOW:
				if (temp == '=') // 뒤에 = 가 오면 심볼이기때문에 EQ로 이동합니다.
					now_state = EQ;
				else if (isLetter(temp) || isDigit(temp) || isWS(temp) || isSymbol(temp)) // c-minus 문법 안에 있는 캐릭터가 오면 단일 에러입니다.
					now_state = FIN;
				else
					now_state = ERROR; // 뒤에 에러 문자가 오면 묶어서 처리합니다.
			case COMMENT1: // 주석부분은 강의자료와 동일합니다.
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
			case ERROR: // 에러 상태입니다. 뒤에 에러가 오면 묶어서 처리하고 아니면 FIN으로 가서 출력됩니다.
				if (isLetter(temp) || isDigit(temp) || isSymbol(temp) || isWS(temp))
					now_state = FIN;
				else
					now_state = ERROR;
				break;
			case IDNUM: // 숫자+문자 혹은 문자+숫자를 처리할 상태입니다. 이것 외에 다른 문자가 오면 FIN으로 가서 출력합니다.
				if (isLetter(temp) || isDigit(temp))
					now_state = IDNUM;
				else
					now_state = FIN;
				break;

			}
			// 모든 출력은 FIN에서 이루어집니다.
			// FIN에서 출력한 후 새로 받은 문자를 버리지 않기 위해 START 상태로 다시 집어넣습니다.
			if (now_state == FIN) { 
				if (prev_state == COMMENT1) // 주석 시작부는 이게 주석으로 갈지 가지 않을지 미리 알 수 없기 때문에 FIN에서 보정해줍니다.
					temp_buf += '/';
				if (prev_state == ID) { // ID일 경우 출력양식입니다.
					temp_string << '\t';
					if (isReserved(temp_buf)) // ID에서 올 때 예약어인지 확인합니다.
						temp_string << line_num << ": " << "reserved word: " << temp_buf << '\n';
					else
						temp_string << line_num << ": " << "ID, name= " << temp_buf << '\n';
				}
				else if (prev_state == NUM) // NUM일 경우 출력양식입니다.
					temp_string << '\t' << line_num << ": " << "NUM, val= " << temp_buf << '\n';
				else if (prev_state == SYM || prev_state == EQ || prev_state == SYM2 || prev_state == COMMENT1) // symbol일 경우 출력양식입니다.
					temp_string << '\t' << line_num << ": " << temp_buf << '\n';
				else if (prev_state == ERROR || prev_state == IDNUM || prev_state == WOW) // 에러로 처리되는 상태일 경우 출력양식입니다.
					temp_string << '\t' << line_num << ": " << "Error: " << temp_buf << '\n';
				words_buf.append(temp_string.str()); // 만들어진 토큰(출력양식)을 버퍼에 추가합니다.
				// 사용했던 임시 버퍼를 비웁니다.
				temp_buf = "";
				temp_string.str(""); 
			}

			if (now_state != FIN) {
				// 화이트스페이스, 주석은 토큰 버퍼에 넣지 않습니다.
				if (!isWS(temp) && now_state != COMMENT1 && now_state != COMMENT2 &&
					now_state != COMMENT3 && prev_state != COMMENT3)
					temp_buf += temp;
				// 개행문자를 받는 것을 기준으로 한 라인의 세트를 출력합니다.
				if (temp == '\n') {
					// temp_string에 출력 양식을 맞추고 한 line을 출력합니다.
					temp_string << line_num++ << ':' << '\t' << line_string;
					fp_out << temp_string.str();
					// 한 라인에서 생성한 토큰을 출력합니다.
					fp_out << words_buf;
					// 출력한 버퍼를 비웁니다.
					temp_string.str("");
					words_buf = ""; 
					break;
				}
			}
			//FIN에서 출력할 때를 위해 이전의 상태를 저장합니다.
			prev_state = now_state;

		}	
	}
	// 주석 상태에서 종료되면 에러메시지를 출력합니다.
	if (now_state == COMMENT2 || now_state == COMMENT3) {
		fp_out << "Error: stop before ending";
	}
}

// 아래는 c-minus 문법 내에 있는 캐릭터인지 확인하기 위한 함수들입니다.

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

// 예약어를 확인하는 함수입니다.
// 예약어 숫자가 많지 않아 그냥 string 배열로 처리했습니다.
bool isReserved(string input)
{
	const string reserved[] = { "else", "if", "int", "return", "void", "while" };

	for (string temp : reserved) {
		if (!temp.compare(input)) // compare은 같으면 0을 반환
			return true;
	}
	return false;
}