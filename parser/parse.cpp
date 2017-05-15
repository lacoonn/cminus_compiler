#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// dfa���� ���� state���Դϴ�.
enum state { ID, NUM, SYM, EQ, SYM2, WOW, COMMENT1, COMMENT2, COMMENT3, START, FIN, ERROR, IDNUM };
enum tokenType { ID, NUM, INT, VOID, SEMI, LSQUARE, RSQUARE, LPAREN, RPAREN, COMMA, LBRACE, RBRACE, IF, ELSE, WHILE, RETURN, ASSIGN, SEQUAL, BEQUAL, SMALLER, BIGGER, EQUAL, NEQUAL, PLUS, MINUS, MUL, DIV, EMPTY };


class treeNode
{
public:
	treeNode *sibling = NULL;
	treeNode *children[100] = { NULL };
	tokenType token;
	string data;

	treeNode(tokenType _token, string _data)
	{
		token = _token;
		data = _data;
	}
};

void makeToken(ifstream &fp_in, ofstream &fp_out);
void parse(ifstream &fs_in, ofstream &fs_out);
bool isLetter(char input);
bool isDigit(char input);
bool isWS(char input);
bool isSymbol(char input);
bool isReserved(string input);
static void getToken();
static void match();
static treeNode *program();
static treeNode *declaration_list();
static treeNode *declaration();
static treeNode *var_declaration();
static treeNode *type_specifier();
static treeNode *fun_declaration();

static tokenType token;
static string tokenData;


int main(int argc, char **argv)
{
	string input_file_name;
	cin >> input_file_name;
	ifstream fp_in(input_file_name);
	ofstream fp_out("scan.txt");

	//fp_out << "C- COMPILATION: " << argv[1] << "\n\n";

	// getToken �Լ��� ���Ͻ�Ʈ���� �Ѱ��ָ� getToken �Լ��� ���Ͻ�Ʈ������
	// �ؽ�Ʈ�� �о� ��ū���� ���� �� ����մϴ�.
	makeToken(fp_in, fp_out);

	fp_in.close();
	fp_out.close();

	ifstream fs_in("scan.txt");
	ofstream fs_out("parse.txt");

	fs_in.close();
	fs_out.close();

	parse(fs_in, fs_out);

	return 0;
}

void parse(ifstream &fs_in, ofstream &fs_out)
{

}

void getToken()
{

}

void match(tokenType expected)
{
	if (token == expected)
		getToken();
	else {
		// exception
	}
}
static treeNode *program()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	t->children[count] = declaration_list();
	
	//����� follow�� �ʿ��� ����
	//while()

	return t;
}
static treeNode *declaration_list()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;
	
	switch (token) {
	case INT:
	case VOID:
		t->children[count++] = declaration();
		while (token == INT || token == VOID) {
			t->children[count++] = declaration();
			//temp->sibling = declaration();
			//temp = temp->sibling;
		}
		break;
	default:
		// exception
	}

	return t;
}
static treeNode *declaration()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (token == VOID || token == INT) {
		t->children[count++] = type_specifier();

		t->children[count++] = new treeNode(token, tokenData);
		match(ID);
		
		if (token == SEMI || token == LSQUARE) {
			t->children[count++] = var_declaration();
		}
		else if (token == LPAREN) {
			t->children[count++] = fun_declaration();
		}
		else {
			// exception
		}
	}

	return t;
}
static treeNode *var_declaration()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	switch (token) {
	case SEMI:
		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
		break;
	case LSQUARE:
		t->children[count++] = new treeNode(token, tokenData);
		match(LSQUARE);

		t->children[count++] = new treeNode(token, tokenData);
		match(NUM);

		t->children[count++] = new treeNode(token, tokenData);
		match(RSQUARE);

		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
		break;
	default:
		// exception
	}

	return t;
}
static treeNode *type_specifier()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (token == VOID) {
		t->children[count++] = new treeNode(token, tokenData);
		match(VOID);
	}
	else {
		t->children[count++] = new treeNode(token, tokenData);
		match(INT);
	}

	return t;
}
static treeNode *fun_declaration()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (token == LPAREN) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LPAREN);

		t->children[count++] = params();

		t->children[count++] = new treeNode(token, tokenData);
		match(RPAREN);

		t->children[count++] = compound_stmt();
	}
	else {
		// exception
	}

	return t;
}

treeNode *params()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	// �ӽ� �������� int�� void�� �����
	tokenType tempToken = token;
	string tempData = tokenData;
	getToken();

	// ���� ��ū�� �о �ű⿡ ���� ����(first�� ���⶧����)
	switch (token) {
	case RPAREN:
		t->children[count++] = new treeNode(tempToken, tokenData);
		break;
	case ID:
		t->children[count++] = param_list(tempToken, tempData);
		break;
	default:
		// exception
	}

	return t;
}

treeNode *param_list(tokenType tempToken, string tempData)
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;
	
	// �ӽ� ��ū�� ���������� ù param�� ���⼭ ���� ó�����ش�
	if (tempToken == VOID || tempToken == INT) {
		// type_specifier
		if (token == VOID) {
			t->children[count++] = new treeNode(token, tokenData);
		}
		else {
			t->children[count++] = new treeNode(token, tokenData);
		}
		// param�� ID
		t->children[count++] = new treeNode(token, tokenData);
		match(ID);

		if (token == COMMA || token == LPAREN) { // param�� follow
			
		}
		else if (token == LSQUARE) {
			t->children[count++] = new treeNode(token, tokenData);
			match(LSQUARE);

			t->children[count++] = new treeNode(token, tokenData);
			match(RSQUARE);
		}
		else {
			// exception
		}
		
		// param_list�� ,[param]
		if (token == COMMA) {
			// param_list�� COMMA
			t->children[count++] = new treeNode(token, tokenData);
			match(COMMA);

			// param�� first�̸�
			if (token == INT || token == VOID) {
				while (token == INT || token == VOID) {
					t->children[count++] = param();
				}
			}
			// param�� follow�̸�
			else if (token == RPAREN) {
				// ����
			}
			else {
				// exception
			}
		}
	}

	return t;
}

treeNode *param()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	t->children[count++] = type_specifier();

	// param�� ID
	t->children[count++] = new treeNode(token, tokenData);
	match(ID);

	if (token == COMMA || token == LPAREN) { // param�� follow
		// param�� follow�� ������ ���⼭ ������
	}
	else if (token == LSQUARE) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LSQUARE);

		t->children[count++] = new treeNode(token, tokenData);
		match(RSQUARE);
	}
	else {
		// exception
	}

	return t;
}

treeNode *compound_stmt()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	t->children[count++] = new treeNode(token, tokenData);
	match(LBRACE);

	t->children[count++] = local_declarations();
	
	t->children[count++] = statement_list();

	t->children[count++] = new treeNode(token, tokenData);
	match(RBRACE);

	return t;
}

treeNode *local_declarations()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (token == INT || token == VOID) {
		while (token == INT || token == VOID) {
			t->children[count++] = var_declaration();
		}
	}
	else if (token == ID || token == RBRACE || token == LBRACE || token == IF || token == WHILE || token == RETURN || token == SEMI) {
		t->children[count++] = new treeNode(EMPTY, NULL);
	}
	else {
		// exception
	}

	return t;
}

treeNode *statement_list()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (token == ID || token == LBRACE || token == IF || token == WHILE || token == RETURN || token == SEMI) {
		while (token == INT || token == VOID) {
			t->children[count++] = statement();
		}
	}
	else if (token == RBRACE) {
		t->children[count++] = new treeNode(EMPTY, NULL);
	}
	else {
		// exception
	}

	return t;
}

treeNode *statement()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (token == ID || token == SEMI) {
		t->children[count++] = expression_stmt();
	}
	else if (token == LBRACE) {
		t->children[count++] = compound_stmt();
	}
	else if (token == IF) {
		t->children[count++] = selection_stmt();
	}
	else if (token == WHILE) {
		t->children[count++] = iteration_stmt();
	}
	else if (token == RETURN) {
		t->children[count++] = return_stmt();
	}
	else {
		// exception
	}

	return t;
}

treeNode *expression_stmt()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (token == ID) {
		t->children[count++] = expression();

		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}
	else if (token == SEMI) {
		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}

	return t;
}

treeNode *selection_stmt()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	t->children[count++] = new treeNode(token, tokenData);
	match(IF);

	t->children[count++] = new treeNode(token, tokenData);
	match(LPAREN);

	t->children[count++] = expression();

	t->children[count++] = new treeNode(token, tokenData);
	match(RPAREN);

	t->children[count++] = statement();

	if (token == ELSE) {
		t->children[count++] = new treeNode(token, tokenData);
		match(ELSE);

		t->children[count++] = statement();
	}

	return t;
}

treeNode *iteration_stmt()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	t->children[count++] = new treeNode(token, tokenData);
	match(WHILE);

	t->children[count++] = new treeNode(token, tokenData);
	match(LPAREN);

	t->children[count++] = expression();

	t->children[count++] = new treeNode(token, tokenData);
	match(RPAREN);

	t->children[count++] = statement();

	return t;
}

treeNode *return_stmt()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	t->children[count++] = new treeNode(token, tokenData);
	match(RETURN);

	if (token == SEMI) {
		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}
	else if (token == ID) {
		t->children[count++] = expression();
		
		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}
	else {
		// exception
	}

	return t;
}

treeNode *expression()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;
	tokenType tempToken;
	string tempData;

	tempToken = token;
	tempData = tokenData;
	getToken();

	// var�� ���� ���
	while (tempToken == ID && (token == ASSIGN || token == LSQUARE)) {
		t->children[count++] = var(tempToken, tempData);

		t->children[count++] = new treeNode(token, tokenData);
		match(ASSIGN);

		tempToken = token;
		tempData = tokenData;
		getToken();
	}
	if(tempToken == LPAREN || tempToken == ID || tempToken == NUM)
		t->children[count++] = simple_expression(tempToken, tempData);

	return t;
}
// LPAREN, ID, NUM
treeNode *var(tokenType tempToken, string tempData)
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	t->children[count++] = new treeNode(tempToken, tempData);
	//match(ID); �ϳ� ��ܽ�⶧���� match�ϸ� �ȵ�

	if (token == LSQUARE) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LSQUARE);

		t->children[count++] = expression();

		t->children[count++] = new treeNode(token, tokenData);
		match(RSQUARE);
	}
	else {
		// exception
		cout << "var error!" << endl;
	}

	return t;
}

treeNode *simple_expression(tokenType tempToken, string tempData)
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (tempToken == LPAREN || tempToken == ID || tempToken == NUM)
		t->children[count++] = additive_expression(tempToken, tempData);

	if (token == BEQUAL || token == BIGGER || token == SMALLER || token == SEQUAL || token == EQUAL || token == NEQUAL) {
		t->children[count++] = relop();

		t->children[count++] = additive_expression(token, tokenData);
	}
	else
	{
		cout << "simple expression error!" << endl;
	}

	return t;
}

treeNode *relop()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;
	//token == SMALLER || token == SEQUAL || token == EQUAL || token == NEQUAL)
	if (token == BEQUAL) {

	}
	else if (token == BIGGER) {

	}
	else if (token == SMALLER) {

	}

	return t;
}

treeNode *additive_expression(tokenType tempToken, string tempData)
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (tempToken == LPAREN || tempToken == ID || tempToken == NUM)
		t->children[count++] = term(tempToken, tempData);

	return t;
}

treeNode *addop()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	return t;
}

treeNode *term(tokenType tempToken, string tempData)
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	if (tempToken == LPAREN || tempToken == ID || tempToken == NUM)
		t->children[count++] = factor(tempToken, tempData);

	return t;
}

treeNode *mulop()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	return t;
}

treeNode *factor(tokenType tempToken, string tempData)
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	return t;
}

treeNode *call(tokenType tempToken, string tempData)
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	return t;
}

treeNode *args()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	return t;
}

treeNode *arg_list()
{
	treeNode *t = new treeNode(EMPTY, NULL);
	int count = 0;

	return t;
}

/*
treeNode *t = new treeNode(EMPTY, NULL);
int count = 0;

return t;
*/


void makeToken(ifstream &fp_in, ofstream &fp_out)
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
						temp_string << line_num << ": " << "reserved: " << temp_buf << '\n';
					else
						temp_string << line_num << ": " << "ID: " << temp_buf << '\n';
				}
				else if (prev_state == NUM) // NUM�� ��� ��¾���Դϴ�.
					temp_string << '\t' << line_num << ": " << "NUM: " << temp_buf << '\n';
				else if (prev_state == SYM || prev_state == EQ || prev_state == SYM2 || prev_state == COMMENT1) // symbol�� ��� ��¾���Դϴ�.
					temp_string << '\t' << line_num << ": " << "SYM: " << temp_buf << '\n';
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
					//temp_string << line_num << ':' << '\t' << line_string;
					line_num++;
					//fp_out << temp_string.str();
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
	fp_out << line_num++ << ':' << '\t' << "EOF\n";
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