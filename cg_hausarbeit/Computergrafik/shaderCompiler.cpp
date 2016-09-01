#include "shaderCompier.h"

#include "shaderprogram.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>



using namespace std;

#define STRINGSIZE 512

void ShaderCompiler::compile() {

	vector<string> cmds;
	size_t found;
	string err;
	ShaderProgram sp;

	char in[STRINGSIZE];
	cout << "CompileShader ";


	cin.getline(in, STRINGSIZE);
	
	stringstream ss(in);
	
	while(!ss.eof()) {
		string tmp;
		ss >> tmp;

		if (tmp == "-f") {
			
			string path;
			ss >> path;
			sp.loadFragmentShader(path.c_str());
			sp.compileShader(sp.getFragmentShader(), &err);
		}
		if (tmp == "-v") {
			
			string path;
			ss >> path;
			sp.loadVertexShader(path.c_str());
			sp.compileShader(sp.getVertexShader(), &err);			
		}
	}
	sp.deleteShader();

	cout << err << endl;
}