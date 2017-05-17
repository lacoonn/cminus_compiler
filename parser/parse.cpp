#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// dfa에서 나온 state들입니다.
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

	// getToken 함수로 파일스트림을 넘겨주면 getToken 함수가 파일스트림에서
	// 텍스트를 읽어 토큰으로 분해 후 출력합니다.
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
		// 토큰 소비할까
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
		// 토큰 소비할까
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

	// 다음 토큰을 읽어서 거기에 따라 갈림(first가 같기때문에)
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

		// param_list의 [[ , [param] ]]
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

		// param의 ID
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

	// var로 들어가는 경우
	if (token == ID && (lhd == ASSIGN || lhd == LSQUARE)) {
		while (token == ID && (lhd == ASSIGN || lhd == LSQUARE)) {
			t->children[count++] = var();

			t->children[count++] = new treeNode(token, tokenData);
			match(ASSIGN);
		}
		t->children[count++] = simple_expression();
	}
	// var에 들어가지 않는 경우
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
					now_state = SID;
				else if (isDigit(temp))
					now_state = SNUM;
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
			case SID:
				if (isLetter(temp))
					now_state = SID;
				else if (isDigit(temp))
					now_state = IDNUM; // 문자 + 숫자는 에러지만 별도의 상태로 둡니다.
				else
					now_state = FIN;
				break;
			case SNUM:
				if (isDigit(temp))
					now_state = SNUM;
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
				if (prev_state == SID) { // ID일 경우 출력양식입니다.
					if (isReserved(temp_buf)) // ID에서 올 때 예약어인지 확인합니다.
						temp_string << line_num << '\t' << "RESERVED\t" << temp_buf << '\n';
					else
						temp_string << line_num << '\t' << "ID\t\t" << temp_buf << '\n';
				}
				else if (prev_state == NUM) // NUM일 경우 출력양식입니다.
					temp_string << line_num << '\t' << "NUM\t\t" << temp_buf << '\n';
				else if (prev_state == SYM || prev_state == EQ || prev_state == SYM2 || prev_state == COMMENT1) // symbol일 경우 출력양식입니다.
					temp_string << line_num << '\t' << "SYM\t\t" << temp_buf << '\n';
				else if (prev_state == ERROR || prev_state == IDNUM || prev_state == WOW) // 에러로 처리되는 상태일 경우 출력양식입니다.
					temp_string << line_num << '\t' << "Error\t\t" << temp_buf << '\n';
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
					//temp_string << line_num << ':' << '\t' << line_string;
					line_num++;
					//fp_out << temp_string.str();
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
	fp_out << line_num++ << '\t' << "EOF\n";
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