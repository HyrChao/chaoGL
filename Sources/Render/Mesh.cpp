//
//  Mesh.cpp
//  chaoGL
//
//  Created by Chao on 2019/3/26.
//

#include "Render/Mesh.h"

using namespace std;

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // pass the pointer of vettex struct as buffer data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);
    
    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texcoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    glBindVertexArray(0);
}

void Mesh::Draw(Shader* shader)
{    
    // Texture
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active texture
        // get texture number（N in TypeN）
        std::string number;
        TextureType type = textures[i].type;
        std::string name =TextureTypeToString(type);
        // transform string to lower
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if(type == TextureType::Diffuse)
            number = std::to_string(diffuseNr++);
        else if(type == TextureType::Specular)
            number = std::to_string(specularNr++);
        // texture naming foramt in shader : Type_num
        shader->setFloat(("material." + name + "_" + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
//        printf("glActiveTexture %u\n", i);
    }
    glActiveTexture(GL_TEXTURE0);
    
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
