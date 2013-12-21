/* 
* G0_Base: projeto inicial de CGra
* 
*/

#include <iostream>
#include <exception>
#include <string>

#include "CGFapplication.h"
#include "YAFScene.h"
#include "YAFinterface.h"

using std::cout;
using std::string;
using std::exception;


int main(int argc, char* argv[]) 
{

	YAFScene *a1;
	if (argc > 1)
		a1 = new YAFScene(argv[1]);
	else
	{
		cout << "Nome do ficheiro: " << endl;
		string s;
		getline(cin,s);

		char *a=new char[s.size()+1];
		a[s.size()]=0;
		memcpy(a,s.c_str(),s.size());

		a1 = new YAFScene(a);
	}

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		app.setScene(a1);
		app.setInterface(new YAFinterface());

		app.run();
	}
	catch(GLexception& ex) {
		cout << "Error: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		cout << "Unexpected error: " << ex.what();
		return -1;
	}

	return 0;
}