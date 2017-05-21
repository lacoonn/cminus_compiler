#include "parse.h"

// 전역 변수 선언
static ifstream fp_in;
static ofstream fp_out;
static ifstream fs_in;
static ofstream fs_out;
static InputToken TokenAry[MAX_ARY];
static InputToken currentToken;
static int TokenIndex = 0;
static int TokenIndexMax = 0;



int main(int argc, char **argv)
{
	string input_file_name;
	cin >> input_file_name;


	// getToken 함수로 파일스트림을 넘겨주면 getToken 함수가 파일스트림에서
	// 텍스트를 읽어 토큰으로 분해 후 출력합니다.
	fp_in.open(input_file_name, ifstream::in);
	fp_out.open("scan.txt", ofstream::out);
	
	makeToken(fp_in, fp_out);

	fp_in.close();
	fp_out.close();

	cout << "scanning FIN" << endl;


	// 파서 부분
	fs_in.open("scan.txt", ifstream::in);
	fs_out.open("parse.txt", ofstream::out);

	parse();
	
	fs_in.close();
	fs_out.close();

	return 0;
}

void parse()
{
	TreeNode *root;

	makeTokenAry();
	getToken();

	if (currentToken.tokentype == INT || currentToken.tokentype == VOID)
		root = program();

	if (currentToken.tokentype != EOS)
		syntaxError("Code ends before file\n");

	cout << endl;
	printTree(root, 0);
}

void printTree(TreeNode *root, int tab)
{
	if (root == NULL || root->visit == true)
		return;

	root->visit = true;
	for (int j = 0; j < tab; j++)
	{
		cout << "  ";
		fs_out << "  ";
	}
	InputToken temp = root->token;
	if (temp.tokentype == ID) {
		cout << "ID: ";
		fs_out << "ID: ";
	}
	else if (temp.tokentype == NUM) {
		cout << "NUM: " ;
		fs_out << "NUM: ";
	}
	else if (temp.tokentype == ASSIGN) {
		cout << "ASSIGN: ";
		fs_out << "ASSIGN: ";
	}
	else if (temp.tokentype == PLUS || temp.tokentype == MINUS || temp.tokentype == MUL || temp.tokentype == DIV) {
		cout << "OP: ";
		fs_out << "OP: ";
	}
	else if (temp.tokentype == BIGGER || temp.tokentype == SMALLER || temp.tokentype == BEQUAL || temp.tokentype == SEQUAL || temp.tokentype == EQUAL || temp.tokentype == NEQUAL) {
		cout << "COMPARE: ";
		fs_out << "COMPARE: ";
	}
	cout << temp.TokenData << endl;
	fs_out << temp.TokenData << endl;
	for (int i = 0; i<CHILD_MAX; i++)
	{
		printTree(root->child[i], tab + 1);
	}

	TreeNode *tmp = root->sibling;
	while (tmp != NULL)
	{
		printTree(tmp, tab);
		tmp = tmp->sibling;
	}
}

void makeTokenAry()
{
	// TokenAry = new InputToken[MAX_ARY];
	int count = 0;

	while (!fs_in.eof()) {
		bool ErrorFlag = false;
		int tempNum;
		TokenType tempToken;
		string tempString, tempData;

		fs_in >> tempNum >> tempString >> tempData;

		//if (fs_in.eof())
		//	break;

		if (tempString == "SYM") {
			if (tempData == ";")
				tempToken = SEMI;
			else if (tempData == "[")
				tempToken = LSQUARE;
			else if (tempData == "]")
				tempToken = RSQUARE;
			else if (tempData == "(")
				tempToken = LPAREN;
			else if (tempData == ")")
				tempToken = RPAREN;
			else if (tempData == ",")
				tempToken = COMMA;
			else if (tempData == "{")
				tempToken = LBRACE;
			else if (tempData == "}")
				tempToken = RBRACE;
			else if (tempData == "=")
				tempToken = ASSIGN;
			else if (tempData == "<=")
				tempToken = SEQUAL;
			else if (tempData == ">=")
				tempToken = BEQUAL;
			else if (tempData == "<")
				tempToken = SMALLER;
			else if (tempData == ">")
				tempToken = BIGGER;
			else if (tempData == "==")
				tempToken = EQUAL;
			else if (tempData == "!=")
				tempToken = NEQUAL;
			else if (tempData == "+")
				tempToken = PLUS;
			else if (tempData == "-")
				tempToken = MINUS;
			else if (tempData == "*")
				tempToken = MUL;
			else if (tempData == "/")
				tempToken = DIV;
			else
				cout << "SYM ERROR : " << tempString << " : " << tempData << endl;
		}
		else if (tempString == "RESERVED") {
			if (tempData == "int")
				tempToken = INT;
			else if (tempData == "void")
				tempToken = VOID;
			else if (tempData == "if")
				tempToken = IF;
			else if (tempData == "else")
				tempToken = ELSE;
			else if (tempData == "while")
				tempToken = WHILE;
			else if (tempData == "return")
				tempToken = RETURN;
			else
				cout << "RESERVED ERROR : " << tempString << " : " << tempData << endl;

		}
		else if (tempString == "ID") {
			tempToken = ID;
		}
		else if (tempString == "NUM") {
			tempToken = NUM;
		}
		else if (tempString == "EOF") {
			tempToken = EOS;
		}
		else if (tempString == "ERROR") {
			tempToken = ERR;
			ErrorFlag = true;
		}
		else {
			cout << "TOTAL ERROR >> Token: " << tempString << ", Data: " << tempData << endl;
			ErrorFlag = true;
		}
		if (!ErrorFlag) {
			TokenAry[count].lineno = tempNum;
			TokenAry[count].tokentype = tempToken;
			TokenAry[count].TokenData = tempData;
			count++;
		}
	}
	TokenIndexMax = count;
	cout << "Number of Tokens: " << TokenIndexMax << endl;
}

void getToken()
{
	currentToken = TokenAry[TokenIndex];

	TokenIndex++;
}

void printToken(InputToken temp)
{
	cout << temp.lineno << ": " << temp.TokenData << endl;;
}

void syntaxError(char* message)
{
	cout << "\n>>> " << "Syntax error at line " << currentToken.lineno << ": " << message;
}

TreeNode *match(TokenType expected)
{
	TreeNode* result = NULL;
	if (currentToken.tokentype == expected)
	{
		cout << "match()->";
		result = new TreeNode(currentToken);
		getToken();
	}
	else
	{
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		cout << "expected : " << expected << endl;
	}
	return result;
}

/* EBNF */
// program -> declaration-list
static TreeNode *program()
{
	cout << "program()->";
	TreeNode* t = declaration_list();
	return t;
}

// declaration-list -> declaration {{ declaration }}
static TreeNode *declaration_list()
{
	cout << "declaration_list()->";
	TreeNode *t = declaration();
	TreeNode *p = t;
	while ((currentToken.tokentype != EOS))
	{
		TreeNode *q;
		q = declaration();
		if (q != NULL)
		{
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

// declaration -> type-specifier ID declaration-prime
static TreeNode *declaration()
{
	cout << "declaration()->";
	TreeNode *t = NULL;

	t = type_specifier();
	if (t != NULL)
	{
		TreeNode *left = match(ID);
		t->child[0] = left;

		TreeNode *right = declaration_prime();
		if (right != NULL)
			t->child[1] = right;
	}
	return t;
}

// declaration-prime -> var-declaration | fun-declaration
TreeNode* declaration_prime(void)
{
	cout << "declaration_prime()->";
	TreeNode *t = NULL;
	switch (currentToken.tokentype)
	{
	case SEMI:
		t = var_declaration();
		break;
	case LSQUARE:
		t = var_declaration();
		break;
	case LPAREN:
		t = fun_declaration();
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}

// var-declaration -> ; | [ NUM ] ;
static TreeNode *var_declaration()
{
	cout << "var_declaration()->";
	TreeNode *result = NULL;
	switch (currentToken.tokentype)
	{
	case SEMI:
		match(SEMI);
		break;
	case LSQUARE:
		match(LSQUARE);
		result = match(NUM);
		match(RSQUARE);
		match(SEMI);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return result;
}

// type-specifier -> int | void
static TreeNode *type_specifier()
{
	cout << "type_specifier()->";
	TreeNode *p = NULL;
	switch (currentToken.tokentype)
	{
	case INT:
		p = match(INT);
		break;
	case VOID:
		p = match(VOID);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return p;
}

// fun-declaration -> ( params ) compound-stmt
static TreeNode *fun_declaration()
{
	cout << "fun_declaration()->";
	TreeNode *p = NULL;
	p = match(LPAREN);
	if (p != NULL)
	{
		TreeNode *q = NULL;
		q = params();
		if (q != NULL)
		{
			p->child[0] = q;
			q = NULL;
		}
		q = match(RPAREN);
		if (q != NULL)
		{
			p->child[1] = q;
			q = NULL;
		}
		q = compound_stmt();
		if (q != NULL)
		{
			p->child[2] = q;
			q = NULL;
		}
	}
	return p;
}
// params -> int ID [ [ ] ] { , param } | params-void
TreeNode *params(void)
{
	cout << "params()->";
	TreeNode *p = NULL;
	TreeNode *t = NULL;
	TreeNode *q = NULL;
	switch (currentToken.tokentype)
	{
	case VOID:
		p = params_prime();
		break;
	case INT:
		p = match(INT);
		q = match(ID);
		if (p != NULL)
			p->child[0] = q;
		if (currentToken.tokentype == LSQUARE)
		{
			TreeNode *r = match(LSQUARE);
			TreeNode *s = match(RSQUARE);
			p->child[1] = r;
			p->child[2] = s;
		}

		t = p;
		while (currentToken.tokentype == COMMA)
		{
			match(COMMA);
			q = param();
			if (q != NULL)
			{
				if (p == NULL) p = t = q;
				else
				{
					t->sibling = q;
					t = q;
				}
			}
		}
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return p;
}

// params-void -> void [ ID [ [ ] ] { , param } ]
TreeNode *params_prime(void)
{
	cout << "params_prime()->";
	TreeNode *t = match(VOID);
	if (currentToken.tokentype == ID)
	{
		TreeNode *p = match(ID);
		if (t != NULL)
			t->child[0] = p;
		if (currentToken.tokentype == LSQUARE)
		{
			TreeNode *q = match(LSQUARE);
			TreeNode *r = match(RSQUARE);
			if (t != NULL)
			{
				t->child[1] = q;
				t->child[2] = r;
			}
		}

		p = t;
		while (currentToken.tokentype == COMMA)
		{
			match(COMMA);
			TreeNode *q = param();
			if (q != NULL)
			{
				if (t == NULL) t = p = q;
				else
				{
					p->sibling = q;
					p = q;
				}
			}
		}
	}
	return t;
}

// param -> type-specifier ID [ [ ] ]
TreeNode *param(void)
{
	cout << "param()->";
	TreeNode *t = type_specifier();
	TreeNode *q = match(ID);
	if (t != NULL)
		t->child[0] = q;

	if (currentToken.tokentype == LSQUARE)
	{
		TreeNode *r = match(LSQUARE);
		TreeNode *s = match(RSQUARE);
		if (t != NULL)
		{
			t->child[1] = r;
			t->child[2] = s;
		}
	}
	return t;
}

// compound-stmt -> { local-declarations statement-list }
TreeNode *compound_stmt(void)
{
	cout << "compound_stmt()->";
	TreeNode *t = match(LBRACE);
	TreeNode *p = local_declarations();
	if (t != NULL)
		t->child[0] = p;
	p = statement_list();
	if (t != NULL)
		t->child[1] = p;
	p = match(RBRACE);
	if (t != NULL)
		t->child[2] = p;

	return t;
}

// local-declarations -> empty { type-specifier ID var-declaration }
TreeNode *local_declarations(void)
{
	cout << "local_declaration()->";
	TreeNode *t = NULL;
	if (currentToken.tokentype == INT || currentToken.tokentype == VOID)
	{
		t = type_specifier();
		TreeNode *p = match(ID);
		TreeNode *q = var_declaration();
		if (t != NULL)
		{
			t->child[0] = p;
			t->child[1] = q;
		}
	}

	TreeNode *p = t;
	while (currentToken.tokentype == INT || currentToken.tokentype == VOID)
	{
		TreeNode *q = type_specifier();
		TreeNode *r = match(ID);
		TreeNode *s = var_declaration();
		if (q != NULL)
		{
			q->child[0] = r;
			q->child[1] = s;
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

// statement-list -> empty { statement } 
TreeNode *statement_list(void)
{
	cout << "statement_list()->";
	TreeNode *t = NULL;
	TokenType type = currentToken.tokentype;
	if (type == SEMI || type == LBRACE || type == IF || type == WHILE || type == RETURN || type == LPAREN || type == ID || type == NUM)
	{
		t = statement();
	}

	TreeNode *p = t;
	type = currentToken.tokentype;
	while (type == SEMI || type == LBRACE || type == IF || type == WHILE || type == RETURN || type == LPAREN || type == ID || type == NUM)
	{
		TreeNode *q = statement();
		if (q != NULL)
		{
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
		type = currentToken.tokentype;
	}
	return t;
}

// statement -> expression-stmt | compund-stmt | selection-stmt | iteration-stmt | return-stmt
TreeNode *statement(void)
{
	cout << "statement()->";
	TreeNode *t = NULL;
	switch (currentToken.tokentype)
	{
		/* expression-stmt first*/
	case SEMI:
		t = expression_stmt();
		break;
	case LPAREN:
		t = expression_stmt();
		break;
	case ID:
		t = expression_stmt();
		break;
	case NUM:
		t = expression_stmt();
		break;
		/* compound-stmt first */
	case LBRACE:
		t = compound_stmt();
		break;
		/* selection-stmt first */
	case IF:
		t = selection_stmt();
		break;
		/* iteration-stmt first */
	case WHILE:
		t = iteration_stmt();
		break;
		/* return-stmt first */
	case RETURN:
		t = return_stmt();
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}

// expression-stmt -> expression ; | ;
TreeNode *expression_stmt(void)
{
	cout << "expression_stmt()->";
	TreeNode *t = NULL;
	TokenType type = currentToken.tokentype;
	switch (type)
	{
	case LPAREN:
		t = expression();
		match(SEMI);
		break;
	case ID:
		t = expression();
		match(SEMI);
		break;
	case NUM:
		t = expression();
		match(SEMI);
		break;
	case SEMI:
		match(SEMI);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}

// selection-stmt -> if ( expression ) statement [ else statement ] 
TreeNode *selection_stmt(void)
{
	cout << "selection_stmt()->";
	TreeNode *t = NULL;

	t = match(IF);
	match(LPAREN);
	TreeNode *q = expression();
	if (t != NULL)
		t->child[0] = q;
	match(RPAREN);

	q = statement();
	if (t != NULL)
		t->child[1] = q;

	if (currentToken.tokentype == ELSE)
	{
		match(ELSE);
		q = statement();
		if (t != NULL)
			t->child[2] = q;
	}
	return t;
}

// iteration-stmt -> while ( expression ) statement
TreeNode *iteration_stmt(void)
{
	cout << "iteration_stmt()->";
	TreeNode *t = NULL;
	t = match(WHILE);
	match(LPAREN);
	TreeNode *q = expression();
	if (t != NULL)
		t->child[0] = q;
	match(RPAREN);

	q = statement();
	if (t != NULL)
		t->child[1] = q;

	return t;
}

// return-stmt -> return return-stmt-prime
TreeNode *return_stmt(void)
{
	cout << "return_stmt()->";
	TreeNode *t = NULL;
	t = match(RETURN);

	TreeNode *p = return_stmt_prime();
	if (t != NULL)
		t->child[0] = p;

	return t;
}

// return-stmt-prime -> ; | expression ; 
TreeNode *return_stmt_prime(void)
{
	cout << "return_stmt_prime()->";
	TreeNode *t = NULL;
	TokenType type = type = currentToken.tokentype;
	switch (type)
	{
	case LPAREN:
		t = expression();
		match(SEMI);
		break;
	case ID:
		t = expression();
		match(SEMI);
		break;
	case NUM:
		t = expression();
		match(SEMI);
		break;
	case SEMI:
		match(SEMI);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}

// expression -> simple-expression [ var = expression ] 
TreeNode *expression(void)
{
	cout << "expression()->";
	TreeNode *t = NULL;
	t = simple_expression();
	if (currentToken.tokentype == ID)
	{
		TreeNode *p = var();
		TreeNode *q = match(EQUAL);
		TreeNode *r = expression();
		if (q != NULL)
		{
			q->child[0] = p;
			q->child[1] = r;
		}
		if (t != NULL)
		{
			t->sibling = q;
		}
	}
	return t;
}

// var -> ID [ [ expression ] ] 
TreeNode *var(void)
{
	cout << "var()->";
	TreeNode *t = match(ID);
	TreeNode *q = NULL;

	q = match(LSQUARE);
	if (t != NULL)
	{
		t->child[0] = q;
	}
	q = expression();
	if (t != NULL)
	{
		t->child[1] = q;
	}
	q = match(RSQUARE);
	if (t != NULL)
	{
		t->child[2] = q;
	}

	return t;
}

// simple-expression -> additive-expression [ relop additive-expression ] 
TreeNode *simple_expression(void)
{
	cout << "simple_expression()->";
	TreeNode *t = additive_expression();

	TokenType type = currentToken.tokentype;
	if (type == SEQUAL || type == BEQUAL || type == BIGGER || type == SMALLER || type == EQUAL || type == NEQUAL || type == ASSIGN)
	{
		TreeNode *p = relop();
		TreeNode *q = additive_expression();
		if (p != NULL)
		{
			p->child[0] = t;
			p->child[1] = q;
		}
		t = p;
	}
	return t;
}

// relop -> <= | < | > | >= | == | !=
TreeNode *relop(void)
{
	cout << "relop()->";
	TreeNode *t = NULL;
	TokenType type = currentToken.tokentype;
	switch (type)
	{
	case SEQUAL:
		t = match(SEQUAL);
		break;
	case BEQUAL:
		t = match(BEQUAL);
		break;
	case SMALLER:
		t = match(SMALLER);
		break;
	case BIGGER:
		t = match(BIGGER);
		break;
	case EQUAL:
		t = match(EQUAL);
		break;
	case NEQUAL:
		t = match(NEQUAL);
		break;
	case ASSIGN:
		t = match(ASSIGN);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}


// additive-expression -> term { addop term } 
TreeNode *additive_expression(void)
{
	cout << "additive_expression()->";
	TreeNode *t = term();
	TreeNode *p = NULL;
	if (currentToken.tokentype == PLUS || currentToken.tokentype == MINUS)
	{
		TreeNode *p = addop();
		TreeNode *q = term();
		if (p != NULL)
		{
			p->child[0] = t;
			p->child[1] = q;
			t = p;
		}
	}

	while (currentToken.tokentype == PLUS || currentToken.tokentype == MINUS)
	{
		TreeNode *q = addop();
		TreeNode *r = term();
		if (q != NULL)
		{
			q->child[0] = p->child[1];
			q->child[1] = r;
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

// addop -> + | -
TreeNode *addop(void)
{
	cout << "addop()->";
	TreeNode *t = NULL;
	switch (currentToken.tokentype)
	{
	case PLUS:
		t = match(PLUS);
		break;
	case MINUS:
		t = match(MINUS);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}

// term -> factor { mulop factor } 
TreeNode *term(void)
{
	cout << "term()->";
	TreeNode *t = factor();
	TreeNode *p = NULL;
	if (currentToken.tokentype == MUL || currentToken.tokentype == DIV)
	{
		p = mulop();
		TreeNode *q = factor();
		if (p != NULL)
		{
			p->child[0] = t;
			p->child[1] = q;
			t = p;
		}
	}

	while (currentToken.tokentype == MUL || currentToken.tokentype == DIV)
	{
		TreeNode *q = mulop();
		TreeNode *r = factor();
		if (q != NULL)
		{
			q->child[0] = p->child[1];
			q->child[1] = r;
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

// mulop -> * | /
TreeNode *mulop(void)
{
	cout << "mulop()->";
	TreeNode *t = NULL;
	switch (currentToken.tokentype)
	{
	case MUL:
		t = match(MUL);
		break;
	case DIV:
		t = match(DIV);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}

// factor -> ( expression ) | ID id-factor | NUM
TreeNode *factor(void)
{
	cout << "factor()->";
	TreeNode *t = NULL;
	TreeNode *p = NULL;
	TokenType type = currentToken.tokentype;
	switch (type)
	{
	case LPAREN:
		match(LPAREN);
		t = expression();
		match(RPAREN);
		break;
	case ID:
		t = match(ID);
		p = factor_prime();
		if (t != NULL)
			t->child[0] = p;
		break;
	case NUM:
		t = match(NUM);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(currentToken);
		getToken();
		break;
	}
	return t;
}

// id-factor -> [ [ expression ] ] | ( args )
TreeNode *factor_prime(void)
{
	cout << "factor_prime()->";
	TreeNode *t = NULL;
	TokenType type = currentToken.tokentype;
	switch (type)
	{
	case LPAREN:
		match(LPAREN);
		t = args();
		match(RPAREN);
		break;
	case LSQUARE:
		match(LSQUARE);
		t = expression();
		match(RSQUARE);
		break;
	}
	return t;
}

// args -> arg-list | empty
TreeNode *args(void)
{
	cout << "args()->";
	TreeNode *t = NULL;
	TokenType type = currentToken.tokentype;
	if (type == LPAREN || type == ID || type == NUM)
		t = arg_list();
	return t;
}

// arg-list -> expression { , expression }
TreeNode *arg_list(void)
{
	cout << "arg_list()->";
	TreeNode *t = expression();
	TreeNode *p = t;
	while (currentToken.tokentype == COMMA)
	{
		TreeNode *q = NULL;
		match(COMMA);
		q = expression();
		if (q != NULL)
		{
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

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
				// 뒤에 = 가 오면 심볼이기때문에 EQ로 이동합니다.
				if (temp == '=') {
					//cout << "WOW" << endl;
					now_state = EQ;
				}
				else if (isLetter(temp) || isDigit(temp) || isWS(temp) || isSymbol(temp)) { // c-minus 문법 안에 있는 캐릭터가 오면 단일 에러입니다.
					//cout << "WOW2" << endl;
					now_state = FIN;
				}
				else
					now_state = ERROR; // 뒤에 에러 문자가 오면 묶어서 처리합니다.
				break;
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
		//fp_out << "Error: stop before ending";
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
		input == '[' || input == ']' || input == '{' || input == '}' || input == '!')
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