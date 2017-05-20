#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define MAX_ARY 1000
#define CHILD_MAX 20

// dfa에서 나온 state들입니다.
enum state { SID, SNUM, SYM, EQ, SYM2, WOW, COMMENT1, COMMENT2, COMMENT3, START, FIN, ERROR, IDNUM };
enum TokenType {
	ID, NUM, INT, VOID, SEMI, LSQUARE, RSQUARE, LPAREN, RPAREN, COMMA, LBRACE, RBRACE, IF, ELSE, WHILE, RETURN, ASSIGN,
	SEQUAL, BEQUAL, SMALLER, BIGGER, EQUAL, NEQUAL, PLUS, MINUS, MUL, DIV, EMPTY, EOS, ERR
};

class TreeNode
{
public:
	TreeNode *children[CHILD_MAX] = { NULL };
	TokenType token;
	string data;
	bool isTab = false;

	TreeNode(TokenType _token, string _data)
	{
		token = _token;
		data = _data;
		isTab = false;
	}
	TreeNode(TokenType _token, string _data, bool _isTab)
	{
		token = _token;
		data = _data;
		isTab = _isTab;
	}
};

typedef struct InputToken {
	int LineNumber;
	TokenType Token;
	string TokenData;
} InputToken;

//스캐너 관련 함수 선언
void makeToken(ifstream &fp_in, ofstream &fp_out);
void parse(ifstream &fs_in, ofstream &fs_out);
bool isLetter(char input);
bool isDigit(char input);
bool isWS(char input);
bool isSymbol(char input);
bool isReserved(string input);

// 파서 관련 함수 선언
static void deleteTree(TreeNode *);
static void printTree(TreeNode *, int);
static void makeTokenAry();
static void getToken();
static void match(TokenType expected);
static TreeNode *program();
static TreeNode *declaration_list();
static TreeNode *declaration();
static TreeNode *var_declaration();
static TreeNode *type_specifier();
static TreeNode *fun_declaration();
static TreeNode *params();
static TreeNode *param_list();
static TreeNode *param();
static TreeNode *compound_stmt();
static TreeNode *local_declarations();
static TreeNode *statement_list();
static TreeNode *statement();
static TreeNode *expression_stmt();
static TreeNode *selection_stmt();
static TreeNode *iteration_stmt();
static TreeNode *return_stmt();
static TreeNode *expression();
static TreeNode *var();
static TreeNode *simple_expression();
static TreeNode *relop();
static TreeNode *additive_expression();
static TreeNode *addop();
static TreeNode *term();
static TreeNode *mulop();
static TreeNode *factor();
static TreeNode *factor_prime();
static TreeNode *call();
static TreeNode *args();
static TreeNode *arg_list();
