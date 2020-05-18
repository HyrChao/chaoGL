
#include<Render/Shader.h>
#include<Assets/CommonAssets.h>
std::unordered_map<int, Shader*> Shader::loadedShaders;
std::unordered_map<int, Shader*> Shader::activeShaders;
Shader* Shader::errorShader;


Shader::Shader(const string& vsPath, const string& fsPath, bool isProp) :
	vsPath(vsPath), fsPath(fsPath), isProp(isProp)
{
	noError = CreateShaderProgram();
	Shader::loadedShaders[ID] = this;
}

Shader::~Shader()
{
	if (this != nullptr)
		if (!Shader::loadedShaders.empty())
			Shader::loadedShaders.erase(this->ID);
	if (this != nullptr)
		if (!Shader::activeShaders.empty() )
			Shader::activeShaders.erase(this->ID);
	glDeleteProgram(this->ID);
}


bool Shader::CreateShaderProgram()
{
	string vertexPath = FileSystem::getPath(vsPath);
	string fragmentPath = FileSystem::getPath(fsPath);

	// 1. get shader source file from a gaven path
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// make sure throw a exception while open failed：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open file from path
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// pass buffer to file stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file after read
		vShaderFile.close();
		fShaderFile.close();
		// convert stream to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << vertexPath << fragmentPath << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 22. compile shader
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// check if compile success
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << vertexPath << std::endl;
	};

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// check if compile success
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << fragmentPath << std::endl;
	}

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// check if link success
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// release resource
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (!success)
	{
		std::cout << "ERROR::SHADER::PROGRAM::CREATE_SHADER_PROGRAM_FAILD\n" << infoLog << std::endl;
		glDeleteProgram(this->ID);
		this->ID = CommonAssets::instance->defaltErrorShader->ID;
		return false;
	}

	std::cout << "Load shader" << vsPath << " " << fsPath << " succuss!" << std::endl;
	return true;
}
// use shader program

void Shader::use()
{
	glUseProgram(ID);
	Shader::activeShaders[ID] = this;
}

bool Shader::Reload()
{
	if (noError)
	{
		if (this != nullptr)
			if (!Shader::loadedShaders.empty())
				Shader::loadedShaders.erase(this->ID);
		if (this != nullptr)
			if (!Shader::activeShaders.empty())
				Shader::activeShaders.erase(this->ID);
		glDeleteProgram(this->ID);
	}
	// Recreate shader program
	noError = CreateShaderProgram();
	if(noError)
		Shader::activeShaders[ID] = this;
	return noError;
}

// set uniform values

void Shader::setBool(const std::string & name, bool value)
{
	GLCall(glUniform1i(GetCachedUniformLocation(name), (int)value));
}

void Shader::setInt(const std::string & name, int value)
{
	GLCall(glUniform1i(GetCachedUniformLocation(name), value));
}

void Shader::setFloat(const std::string & name, float value)
{
	GLCall(glUniform1f(GetCachedUniformLocation(name), value));
}

void Shader::setVec2f(const std::string & name, glm::vec2& vec2)
{
	glUniform2f(GetCachedUniformLocation(name), vec2.x, vec2.y);
}

void Shader::setVec3f(const std::string & name, float x, float y, float z)
{
	GLCall(glUniform3f(GetCachedUniformLocation(name), x, y, z));
}

void Shader::setVec3f(const std::string & name, glm::vec3& vec3)
{
	GLCall(glUniform3f(GetCachedUniformLocation(name), vec3.x, vec3.y, vec3.z));
}

void Shader::setVec4f(const std::string & name, float x, float y, float z, float w)
{
	GLCall(glUniform4f(GetCachedUniformLocation(name), x, y, z, w));
}

void Shader::setVec4f(const std::string & name, glm::vec4& vec4)
{
	GLCall(glUniform4f(GetCachedUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w));
}

void Shader::setMat4f(const std::string & name, glm::mat4& mat)
{
	GLCall(glUniformMatrix4fv(GetCachedUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::SetParam(const std::string & name, bool value)
{
	setBool(name, value);
}

void Shader::SetParam(const std::string & name, int value)
{
	setInt(name, value);
}

void Shader::SetParam(const std::string& name, unsigned int value)
{
	setInt(name, (int)value);
}

void Shader::SetParam(const std::string & name, float value)
{
	setFloat(name, value);
}

void Shader::SetParam(const std::string& name, glm::vec2& vec2)
{
	setVec2f(name, vec2);
}

void Shader::SetParam(const std::string & name, float x, float y, float z)
{
	setVec3f(name, x, y, z);
}

void Shader::SetParam(const std::string & name, glm::vec3& vec3)
{
	setVec3f(name, vec3);
}

void Shader::SetParam(const std::string & name, float x, float y, float z, float w)
{
	setVec4f(name, x, y, z, w);
}

void Shader::SetParam(const std::string & name, glm::vec4& vec4)
{
	setVec4f(name, vec4);
}

void Shader::SetParam(const std::string & name, glm::mat4& mat)
{
	setMat4f(name, mat);;
}

void Shader::BindTexture(const char* paramname, unsigned int id, unsigned int type)
{
	use();

	for (auto i = slot.begin(); i != slot.end(); i++)
	{
		auto s = *i;
		if (paramname == s.first)
		{
			glActiveTexture(GL_TEXTURE0 + s.second);
			glBindTexture(type, id);
			SetParam(s.first, s.second);
			return;
		}
	}

	slot[paramname] = occupiedSlot;
	if (++occupiedSlot > TextureSlot16)
	{
		slot.clear();
		occupiedSlot = TextureSlot1;
		slot[paramname] = TextureSlot1;
	}
	glActiveTexture(GL_TEXTURE0 + slot[paramname]);
	glBindTexture(type, id);
	SetParam(paramname, slot[paramname]);

}


void Shader::ChangeErrorShader(Shader * errShader)
{
	Shader::errorShader = errShader;
}

int Shader::GetCachedUniformLocation(const string & name)
{
	//if (cachedUniformLocation.find(name) != cachedUniformLocation.end())
	//{
	//	return cachedUniformLocation[name];
	//}
	int location = glGetUniformLocation(ID, name.c_str());
	//cachedUniformLocation[name] = location;

	// Log warning if can't find uniform name
	//if(location == -1)
	//	std::cout << "WARNING::SHADER::PROGRAM::UNIFORM ERROR\n" << "Can't find parameter "<< name.c_str() <<" in shader" << vsPath.c_str()  << " "<< fsPath.c_str() << std::endl;

	return location;
}
