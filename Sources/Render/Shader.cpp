
#include<Render/Shader.h>

unordered_map<int, Shader*> Shader::loadedShaders;
Shader* Shader::errorShader;