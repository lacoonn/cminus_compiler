#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// dfa���� ���� state���Դϴ�.
enum state { SID, SNUM, SYM, EQ, SYM2, WOW, COMMENT1, COMMENT2, COMMENT3, START, FIN, ERROR, IDNUM };
enum tokenType { ID, NUM, INT, VOID, SEMI, LSQUARE, RSQUARE, LPAREN, RPAREN, COMMA, LBRACE, RBRACE, IF, ELSE, WHILE, RETURN, ASSIGN,
	SEQUAL, BEQUAL, SMALLER, BIGGER, EQUAL, NEQUAL, PLUS, MINUS, MUL, DIV, EMPTY, EOS, ERR };

class treeNode
{
public:
	treeNode *sibling = NULL;
	treeNode *children[20] = { NULL };
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

static void printTree(treeNode *);
static void getToken();
static void match(tokenType expected);
static treeNode *program();
static treeNode *declaration_list();
static treeNode *declaration();
static treeNode *var_declaration();
static treeNode *type_specifier();
static treeNode *fun_declaration();
static treeNode *params();
static treeNode *param_list();
static treeNode *param();
static treeNode *compound_stmt();
static treeNode *local_declarations();
static treeNode *statement_list();
static treeNode *statement();
static treeNode *expression_stmt();
static treeNode *selection_stmt();
static treeNode *iteration_stmt();
static treeNode *return_stmt();
static treeNode *expression();
static treeNode *var();
static treeNode *simple_expression();
static treeNode *relop();
static treeNode *additive_expression();
static treeNode *addop();
static treeNode *term();
static treeNode *mulop();
static treeNode *factor();
static treeNode *factor_prime();
static treeNode *call();
static treeNode *args();
static treeNode *arg_list();

static ifstream fs_in;
static ofstream fs_out;
static tokenType token = EMPTY;
static string tokenData = "";
static int tokenLine = 0;
static tokenType lhd = EMPTY;
static string lhdData = "";
static int lhdLine = 0;



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

	cout << "scanning FIN" << endl;
	

	//fs_in.close();
	//fs_out.close();

	fs_in.open("scan.txt", ifstream::in);
	fs_out.open("parse.txt", ofstream::out);


	parse(fs_in, fs_out);
	

	return 0;
}

void printTree(treeNode *root)
{
	for (int i = 0; i < 20; i++) {
		if (root->children[i] != NULL)
			printTree(root->children[i]);
	}
	if (root->token != EMPTY) {
		cout << root->data << '\t';
		fs_out << root->data << '\t';
	}
	delete root;
}

void getToken()
{
	string tempString;

	token = lhd;
	tokenData = lhdData;
	tokenLine = lhdLine;

	fs_in >> lhdLine >> tempString >> lhdData;
	
	if (tempString == "SYM") {
		if (lhdData == ";")
			lhd = SEMI;
		else if (lhdData == "[")
			lhd = LSQUARE;
		else if (lhdData == "]")
			lhd = RSQUARE;
		else if (lhdData == "(")
			lhd = LPAREN;
		else if (lhdData == ")")
			lhd = RPAREN;
		else if (lhdData == ",")
			lhd = COMMA;
		else if (lhdData == "{")
			lhd = LBRACE;
		else if (lhdData == "}")
			lhd = RBRACE;
		else if (lhdData == "=")
			lhd = ASSIGN;
		else if (lhdData == "<=")
			lhd = SEQUAL;
		else if (lhdData == ">=")
			lhd = BEQUAL;
		else if (lhdData == "<")
			lhd = SMALLER;
		else if (lhdData == ">")
			lhd = BIGGER;
		else if (lhdData == "==")
			lhd = EQUAL;
		else if (lhdData == "!=")
			lhd = NEQUAL;
		else if (lhdData == "+")
			lhd = PLUS;
		else if (lhdData == "-")
			lhd = MINUS;
		else if (lhdData == "*")
			lhd = MUL;
		else if (lhdData == "/")
			lhd = DIV;
		else
			cout << "ERROR : " << tempString << " : " << lhdData << endl;
		//enum tokenType { ID, NUM, INT, VOID, SEMI, LSQUARE, RSQUARE, LPAREN, RPAREN, COMMA, LBRACE, RBRACE, IF, ELSE, WHILE, RETURN, ASSIGN,
		//SEQUAL, BEQUAL, SMALLER, BIGGER, EQUAL, NEQUAL, PLUS, MINUS, MUL, DIV, EMPTY };
	}
	else if (tempString == "RESERVED") {
		if (lhdData == "int")
			lhd = INT;
		else if (lhdData == "void")
			lhd = VOID;
		else if (lhdData == "if")
			lhd = IF;
		else if (lhdData == "else")
			lhd = ELSE;
		else if (lhdData == "while")
			lhd = WHILE;
		else if (lhdData == "return")
			lhd = RETURN;
		else
			cout << "ERROR : " << tempString << " : " << lhdData << endl;

	}
	else if (tempString == "ID") {
		lhd = ID;
	}
	else if (tempString == "NUM") {
		lhd = NUM;
	}
	else if (tempString == "EOF") {
		lhd = EOS;
	}
	else if (tempString == "ERROR") {
		lhd = ERR;
	}
	else {
		cout << "ERROR : " << tempString << " : " << lhdData << endl;
	}
	
}

void match(tokenType expected)
{
	if (token == expected)
		getToken();
	else {
		// exception
		cout << "match error!, expected : " << expected << ", line : " << tokenLine << ", token : " << token << ", tokenData : " << tokenData << endl;
	}
}

void parse(ifstream &fs_in, ofstream &fs_out)
{
	getToken();
	getToken();

	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	t->children[count++] = program();

	if (token != EOS)
		cout << "Code ends before file" << endl;

	printTree(t);
}

static treeNode *program()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	t->children[count++] = declaration_list();
	
	cout << "program() END" << endl;

	return t;
}
// declaration-list -> declaration {{ declaration }}
static treeNode *declaration_list()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;
	
	if (token == VOID || token == INT) {
		t->children[count++] = declaration();
		
		while (token != EOS) {
			cout << "delcaration_list iteration : " << count << endl;
			treeNode *temp = declaration();
			if (temp->token != EMPTY)
				t->children[count++] = temp;
		}
	}
	else {
		// exception
		cout << "declaration_list!" << endl;
	}

	return t;
}
// declaration -> type-specifier ID var-declaration | func-declaration
static treeNode *declaration()
{
	treeNode *t = new treeNode(EMPTY, "");
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
			cout << "delcaration(inner) error!" << endl;
		}
	}
	else {
		cout << "delcaration error!" << endl;
	}

	return t;
}
// var-declaration -> ; | [ NUM ] ;
static treeNode *var_declaration()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == SEMI) {
		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}
	else if (token == LSQUARE) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LSQUARE);

		t->children[count++] = new treeNode(token, tokenData);
		match(NUM);

		t->children[count++] = new treeNode(token, tokenData);
		match(RSQUARE);

		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}
	else {
		cout << "var_declaration error! " << tokenLine << endl;
		// ��ū �Һ��ұ�
	}

	return t;
}

// type-specifier -> int | void
static treeNode *type_specifier()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == VOID) {
		t->children[count++] = new treeNode(token, tokenData);
		match(VOID);
	}
	else if (token == INT) {
		t->children[count++] = new treeNode(token, tokenData);
		match(INT);
	}
	else {
		cout << "type_specifier error!" << endl;
		// ��ū �Һ��ұ�
	}

	return t;
}
// fun-declaration -> ( params ) compound-stmt
static treeNode *fun_declaration()
{
	treeNode *t = new treeNode(EMPTY, "");
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
		cout << "fun_declaration error!" << endl;
	}

	return t;
}

/*
treeNode *params()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	// ���� ��ū�� �о �ű⿡ ���� ����(first�� ���⶧����)
	switch (lhd) {
	case RPAREN:
		t->children[count++] = new treeNode(token, tokenData);
		match(VOID);
		break;
	case ID:
		t->children[count++] = param_list();
		break;
	default:
		// exception
		cout << "params error!" << endl;
	}

	return t;
}
*/

treeNode *params()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == INT) {
		t->children[count++] = new treeNode(token, tokenData);
		match(INT);

		t->children[count++] = new treeNode(token, tokenData);
		match(ID);

		if (token == LSQUARE) {
			t->children[count++] = new treeNode(token, tokenData);
			match(LSQUARE);

			t->children[count++] = new treeNode(token, tokenData);
			match(RSQUARE);
		}

		while (token == COMMA) {
			t->children[count++] = new treeNode(token, tokenData);
			match(COMMA);

			t->children[count++] = param();
		}
	}
	else if (token == VOID) {
		t->children[count++] = new treeNode(token, tokenData);
		match(VOID);

		if (token == ID) {
			t->children[count++] = new treeNode(token, tokenData);
			match(ID);

			if (token == LSQUARE) {
				t->children[count++] = new treeNode(token, tokenData);
				match(LSQUARE);

				t->children[count++] = new treeNode(token, tokenData);
				match(RSQUARE);
			}

			while (token == COMMA) {
				t->children[count++] = new treeNode(token, tokenData);
				match(COMMA);

				t->children[count++] = param();
			}
		}
	}
	else {
		cout << "params() ERROR!" << endl;
	}

	return t;
}

treeNode *param_list()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;
	
	if (token == VOID || token == INT) {
		t->children[count++] = param();

		// param_list�� [[ , [param] ]]
		while (token == COMMA) {
			t->children[count++] = new treeNode(token, tokenData);
			match(COMMA);

			t->children[count++] = param();
		}
	}
	else {
		cout << "param_list error!" << endl;
	}

	return t;
}

treeNode *param()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == VOID || token == INT) {
		t->children[count++] = type_specifier();

		// param�� ID
		t->children[count++] = new treeNode(token, tokenData);
		match(ID);

		if (token == LSQUARE) {
			t->children[count++] = new treeNode(token, tokenData);
			match(LSQUARE);

			t->children[count++] = new treeNode(token, tokenData);
			match(RSQUARE);
		}
	}
	else {
		// exception
		cout << "param error!" << endl;
	}

	return t;
}

treeNode *compound_stmt()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == LBRACE) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LBRACE);

		t->children[count++] = local_declarations();

		t->children[count++] = statement_list();

		t->children[count++] = new treeNode(token, tokenData);
		match(RBRACE);
	}
	else {
		cout << "compound_stmt error!" << endl;
	}

	return t;
}

treeNode *local_declarations()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == INT || token == VOID) {
		while (token == INT || token == VOID) {
			// type_specifier
			t->children[count++] = type_specifier();

			t->children[count++] = new treeNode(token, tokenData);
			match(ID);

			t->children[count++] = var_declaration();
		}
	}
	else if (token == ID || token == LPAREN || token == NUM || token == LBRACE || token == IF || token == WHILE || token == RETURN || token == SEMI || token == RBRACE) {
		t->children[count++] = new treeNode(EMPTY, "");
	}
	else {
		// exception
		cout << "local_declaration" << endl;
	}

	return t;
}

treeNode *statement_list()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == ID || token == LPAREN || token == NUM || token == LBRACE || token == IF || token == WHILE || token == RETURN || token == SEMI) {
		while (token == ID || token == LPAREN || token == NUM || token == LBRACE || token == IF || token == WHILE || token == RETURN || token == SEMI) {
			t->children[count++] = statement();
		}
	}
	else if (token == RBRACE) {
		t->children[count++] = new treeNode(EMPTY, "");
	}
	else {
		// exception
		cout << "statement_list error!" << endl;
	}

	return t;
}

treeNode *statement()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == ID || token == LPAREN || token == NUM || token == SEMI) {
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
		cout << "statement error!" << endl;
	}

	return t;
}

treeNode *expression_stmt()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == ID || token == NUM || token == LPAREN) {
		t->children[count++] = expression();

		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}
	else if (token == SEMI) {
		t->children[count++] = new treeNode(token, tokenData);
		match(SEMI);
	}
	else {
		cout << "expression_stmt() error! " << endl;
	}

	return t;
}

treeNode *selection_stmt()
{
	treeNode *t = new treeNode(EMPTY, "");
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
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == WHILE) {
		t->children[count++] = new treeNode(token, tokenData);
		match(WHILE);

		t->children[count++] = new treeNode(token, tokenData);
		match(LPAREN);

		t->children[count++] = expression();

		t->children[count++] = new treeNode(token, tokenData);
		match(RPAREN);

		t->children[count++] = statement();
	}
	else {
		cout << "iteration_stmt error! " << endl;
	}

	return t;
}

treeNode *return_stmt()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == RETURN) {
		t->children[count++] = new treeNode(token, tokenData);
		match(RETURN);

		if (token == SEMI) {
			t->children[count++] = new treeNode(token, tokenData);
			match(SEMI);
		}
		else if (token == ID || token == LPAREN || token == NUM) {
			t->children[count++] = expression();

			t->children[count++] = new treeNode(token, tokenData);
			match(SEMI);
		}
		else {
			cout << "return_stmt(inner) error!" << endl;
		}
	}
	else {
		// exception
		cout << "return_stmt error!" << endl;
	}

	return t;
}
/*
// expression -> {{ var = }} simple-expression
treeNode *expression()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	// var�� ���� ���
	if (token == ID && (lhd == ASSIGN || lhd == LSQUARE)) {
		while (token == ID && (lhd == ASSIGN || lhd == LSQUARE)) {
			t->children[count++] = var();

			t->children[count++] = new treeNode(token, tokenData);
			match(ASSIGN);
		}
		t->children[count++] = simple_expression();
	}
	// var�� ���� �ʴ� ���
	else if (token == LPAREN || token == ID || token == NUM) {
		t->children[count++] = simple_expression();
	}
	else {
		cout << "expression error!" << endl;
	}

	return t;
}
*/

// expression -> simple-expression [[ var = expression ]]
treeNode *expression()
{
	treeNode *t = new treeNode(EMPTY, "expression");
	int count = 0;

	t->children[count++] = simple_expression();

	if (token == ID) {
		t->children[count++] = var();

		t->children[count++] = new treeNode(token, tokenData);
		match(EQUAL);

		t->children[count++] = expression();
	}
	else {
		cout << "expression() error!" << endl;
	}

	return t;
}

treeNode *var()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == ID) {
		t->children[count++] = new treeNode(token, tokenData);
		match(ID);

		if (token == LSQUARE) {
			t->children[count++] = new treeNode(token, tokenData);
			match(LSQUARE);

			t->children[count++] = expression();

			t->children[count++] = new treeNode(token, tokenData);
			match(RSQUARE);
		}
	}
	else {
		// exception
		cout << "var error! " << token << tokenData << endl;
	}

	return t;
}

treeNode *simple_expression()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == LPAREN || token == ID || token == NUM) {
		t->children[count++] = additive_expression();

		if (token == BEQUAL || token == BIGGER || token == SMALLER || token == SEQUAL || token == EQUAL || token == NEQUAL) {
			t->children[count++] = relop();

			t->children[count++] = additive_expression();
		}
	}
	else
	{
		cout << "simple expression error!" << endl;
	}

	return t;
}

treeNode *relop()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;
	
	if (token == SEQUAL) {
		t->children[count++] = new treeNode(token, tokenData);
		match(SEQUAL);
	}
	else if (token == BIGGER) {
		t->children[count++] = new treeNode(token, tokenData);
		match(BIGGER);
	}
	else if (token == SMALLER) {
		t->children[count++] = new treeNode(token, tokenData);
		match(SMALLER);
	}
	else if (token == BEQUAL) {
		t->children[count++] = new treeNode(token, tokenData);
		match(BEQUAL);
	}
	else if (token == EQUAL) {
		t->children[count++] = new treeNode(token, tokenData);
		match(EQUAL);
	}
	else if (token == NEQUAL) {
		t->children[count++] = new treeNode(token, tokenData);
		match(NEQUAL);
	}
	else {
		cout << "relop error!" << endl;
	}

	return t;
}

treeNode *additive_expression()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == LPAREN || token == ID || token == NUM) {
		t->children[count++] = term();
		
		while (token == PLUS || token == MINUS) {
			t->children[count++] = addop();

			t->children[count++] = term();
		}
	}
	else {
		cout << "additive_expression error!" << endl;
	}

	return t;
}

treeNode *addop()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == PLUS) {
		t->children[count++] = new treeNode(token, tokenData);
		match(PLUS);
	}
	else if (token == MINUS) {
		t->children[count++] = new treeNode(token, tokenData);
		match(MINUS);
	}
	else {
		cout << "addop error!" << endl;
	}

	return t;
}

treeNode *term()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == LPAREN || token == ID || token == NUM) {
		t->children[count++] = factor();

		while (token == MUL || token == DIV) {
			t->children[count++] = mulop();
			
			t->children[count++] = factor();
		}
	}
	else {
		cout << "term error!" << endl;
	}

	return t;
}

treeNode *mulop()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == MUL) {
		t->children[count++] = new treeNode(token, tokenData);
		match(MUL);
	}
	else if (token == DIV) {
		t->children[count++] = new treeNode(token, tokenData);
		match(DIV);
	}
	else {
		cout << "mulop error!" << endl;
	}

	return t;
}

treeNode *factor()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == LPAREN) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LPAREN);

		t->children[count++] = expression();

		t->children[count++] = new treeNode(token, tokenData);
		match(RPAREN);
	}
	else if (token == NUM) {
		t->children[count++] = new treeNode(token, tokenData);
		match(NUM);
	}
	else if (token == ID) {
		t->children[count++] = new treeNode(token, tokenData);
		match(ID);

		t->children[count++] = factor_prime();
	}
	else {
		cout << "factor error!" << endl;
	}

	return t;
}

treeNode *factor_prime() {
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == LSQUARE) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LSQUARE);

		t->children[count++] = expression();

		t->children[count++] = new treeNode(token, tokenData);
		match(RSQUARE);
	}
	else if (token == LPAREN) {
		t->children[count++] = call();
	}
	else {
		cout << "factor_prime() ERROR!" << endl;
	}

	return t;
}

treeNode *call()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == LPAREN) {
		t->children[count++] = new treeNode(token, tokenData);
		match(LPAREN);

		t->children[count++] = args();

		t->children[count++] = new treeNode(token, tokenData);
		match(RPAREN);
	}
	else {
		cout << "call error!" << endl;
	}

	return t;
}

treeNode *args()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == ID || token == NUM || token == LPAREN) {
		t->children[count++] = arg_list();
	}
//	else if (token == RPAREN) {
//		t->children[count++] = new treeNode(EMPTY, "");
//	}
	else {
		cout << "args() error!" << endl;
	}

	return t;
}

treeNode *arg_list()
{
	treeNode *t = new treeNode(EMPTY, "");
	int count = 0;

	if (token == ID || token == NUM || token == LPAREN) {
		t->children[count++] = expression();
		
		while (token == COMMA) {
			t->children[count++] = new treeNode(token, tokenData);
			match(COMMA);

			t->children[count++] = expression();
		}
	}
	else {
		cout << "arg_list error!" << endl;
	}

	return t;
}

/*
treeNode *t = new treeNode(EMPTY, "");
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
					now_state = SID;
				else if (isDigit(temp))
					now_state = SNUM;
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
			case SID:
				if (isLetter(temp))
					now_state = SID;
				else if (isDigit(temp))
					now_state = IDNUM; // ���� + ���ڴ� �������� ������ ���·� �Ӵϴ�.
				else
					now_state = FIN;
				break;
			case SNUM:
				if (isDigit(temp))
					now_state = SNUM;
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
				if (prev_state == SID) { // ID�� ��� ��¾���Դϴ�.
					if (isReserved(temp_buf)) // ID���� �� �� ��������� Ȯ���մϴ�.
						temp_string << line_num << '\t' << "RESERVED\t" << temp_buf << '\n';
					else
						temp_string << line_num << '\t' << "ID\t\t" << temp_buf << '\n';
				}
				else if (prev_state == NUM) // NUM�� ��� ��¾���Դϴ�.
					temp_string << line_num << '\t' << "NUM\t\t" << temp_buf << '\n';
				else if (prev_state == SYM || prev_state == EQ || prev_state == SYM2 || prev_state == COMMENT1) // symbol�� ��� ��¾���Դϴ�.
					temp_string << line_num << '\t' << "SYM\t\t" << temp_buf << '\n';
				else if (prev_state == ERROR || prev_state == IDNUM || prev_state == WOW) // ������ ó���Ǵ� ������ ��� ��¾���Դϴ�.
					temp_string << line_num << '\t' << "Error\t\t" << temp_buf << '\n';
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
	fp_out << line_num++ << '\t' << "EOF\n";
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