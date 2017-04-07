#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
using namespace std;

void arraytest(int **arr) {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d ", arr[i][j]);
		}
	}
}
int main()
{
	// 파일 읽기 준비
	/*
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
	

	*/
	int a[3][3] = { 1,2,3,4,5,6,7,8,9 };

	arraytest(a);

	return 0;

}

void printSTR()
{
	cout << "I can do it" << endl;
}