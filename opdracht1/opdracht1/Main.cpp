#include <iostream>
#include "Source.cpp"

using namespace std;

int main(int argc, char* argv[]){

	//Een beeld(argument 1) inleest, naar grijswaarden converteert, en opslaat onder de naam “grey_” + [original filename].
	for (int i = 1; i < argc; i++)
		cout << "argv[" << i << "] = " << argv[i] << endl;

	if (argv[1] == NULL){
		return 666;
	}
	else{
		return 0;
	}
}