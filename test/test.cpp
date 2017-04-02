#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
	// 파일 읽기 준비
	
	ifstream in("input.c");
	ofstream out("output.txt");
	string s;
	char k;
	stringstream stream;
	int line = 5;
	stream << line << "  hello\n";
	s = stream.str();
	cout << s;

	
	//cout << in.gcount();
	while (in.get(k)) {

		out << k;;
	}
	


	return 0;

}