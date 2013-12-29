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
	a1 = new YAFScene("scene.yaf");
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