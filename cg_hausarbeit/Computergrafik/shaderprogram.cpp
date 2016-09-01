#include "shaderprogram.h"
#include <fstream>
#include <vector>

using namespace std;

ShaderProgram::ShaderProgram()
{
}


ShaderProgram::~ShaderProgram()
{
}

bool ShaderProgram::load(const char* VertexShader, const char* FragmentShader) {

	return this->loadVertexShader(VertexShader) && this->loadFragmentShader(FragmentShader);
}

bool ShaderProgram::loadVertexShader(const char* VertexShader) {
	
	string shaderStr = readFile(VertexShader);
	if (shaderStr == "") {
		return false;
	}

	const char * shaderSrc = shaderStr.c_str();

	this->m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->m_VertexShader, 1, &shaderSrc, NULL);
	return true;
}

bool ShaderProgram::loadFragmentShader(const char* FragmentShader) {

	string shaderStr = readFile(FragmentShader);
	if (shaderStr == "") {
		return false;
	}

	const char * shaderSrc = shaderStr.c_str();

	this->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->m_FragmentShader, 1, &shaderSrc, NULL);
	return true;
}

bool ShaderProgram::compile(string* CompileErrors) {

	GLint result = GL_FALSE;
	int loglenght;

	bool res = true;
		
	compileShader(this->m_VertexShader, CompileErrors);

	compileShader(this->m_FragmentShader, CompileErrors);

	this->m_ShaderProgram = glCreateProgram();
	glAttachShader(this->m_ShaderProgram, this->m_VertexShader);
	glAttachShader(this->m_ShaderProgram, this->m_FragmentShader);
	glLinkProgram(this->m_ShaderProgram);

	glGetProgramiv(this->m_ShaderProgram, GL_LINK_STATUS, &result);
	glGetProgramiv(this->m_ShaderProgram, GL_INFO_LOG_LENGTH, &loglenght);
	if (loglenght > 1) {
		vector<char> progErr(loglenght);
		glGetShaderInfoLog(this->m_ShaderProgram, loglenght, NULL, &progErr[0]);
		
		string err(progErr.begin(), progErr.end());

		CompileErrors->append(err + "\n");

		res = false;
	}

	this->deleteShader();

	return res;
}

GLint ShaderProgram::getParameterID(const char* ParameterName) const {

	return glGetUniformLocation(this->m_ShaderProgram, ParameterName); 
}

void ShaderProgram::setParameter(GLint ID, float Param) {

	glUniform1f(ID, Param);
}

void ShaderProgram::setParameter(GLint ID, int Param) {

	glUniform1i(ID, Param);
}

void ShaderProgram::setParameter(GLint ID, const Vector& Param) {

	glUniform3f(ID, Param.X, Param.Y, Param.Z);
}

void ShaderProgram::setParameter(GLint ID, const Color& Param) {

	glUniform3f(ID, Param.R, Param.G, Param.B);
}

void ShaderProgram::setParameter(GLint ID, const Matrix& Param) {

	glUniformMatrix4fv(ID, 1, false, Param.m);
}

void ShaderProgram::activate() const {

	glUseProgram(this->m_ShaderProgram);
}

void ShaderProgram::deactivate() const {

	glUseProgram(0);
}

string ShaderProgram::readFile(const char *filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);

	if (!fileStream.is_open()) {
		cerr << "Could not read file " << filePath << ". File does not exist." << endl;
		return "";
	}

	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

bool ShaderProgram::compileShader(const GLint& shader, string* CompileErrors) {

	GLint result = GL_FALSE;
	int loglenght;

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglenght);
	
	if (loglenght > 1) {
		vector<char> shaderErr(loglenght);
		glGetShaderInfoLog(shader, loglenght, NULL, &shaderErr[0]);
		
		string err(shaderErr.begin(), shaderErr.end());

		CompileErrors->append(err + "\n");
		
		return false;
	}
	
	return true;
}

GLint ShaderProgram::getFragmentShader() const {

	return this->m_FragmentShader;
}


GLint ShaderProgram::getVertexShader() const {
	
	return this->m_VertexShader;
}

void ShaderProgram::deleteShader() {
	
	if (this->m_VertexShader > 0) {

		glDeleteShader(this->m_VertexShader);
	}
	if (this->m_FragmentShader > 0) {

		glDeleteShader(this->m_FragmentShader);
	}
}