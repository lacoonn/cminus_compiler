#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
int main()
{
	// ���� �б� �غ�
	
	ifstream in("input.c");
	ofstream out("output.txt");
	string s[] = { "else", "if", "int", "return", "void", "while" };
	char k;
	stringstream stream;
	int line = 5;
	stream << line << "  hello\n";
	
	for (string temp : s) {
		cout << temp << endl;
		if (!temp.compare("return"))
			cout << "HERE!!!!" << endl;
	}
	printSTR();
	
	//cout << in.gcount();
	while (in.get(k)) {

		out << k;;
	}
	


	return 0;

}

void printSTR()
{
	cout << "I can do it" << endl;
}