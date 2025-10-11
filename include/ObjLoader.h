#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

struct Mesh {
    GLuint vao, vbo, ebo;
    int count;
};

Mesh* loadOBJ(const char* path) {
    std::vector<float> temp_pos;
    std::vector<float> temp_norm;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::map<std::string, unsigned int> vertexMap;
    
    std::ifstream f(path);
    std::string line;
    
    while (std::getline(f, line)) {
        std::istringstream s(line);
        std::string t;
        s >> t;
        
        if (t == "v") {
            float x, y, z;
            s >> x >> y >> z;
            temp_pos.push_back(x);
            temp_pos.push_back(z);
        } 
        else if (t == "vn") {
            float x, y, z;
            s >> x >> y >> z;
            temp_norm.push_back(x);
            temp_norm.push_back(y);
            temp_norm.push_back(z);
        }
        else if (t == "f") {
            std::string v1, v2, v3;
            s >> v1 >> v2 >> v3;
            
            for (auto& v : {v1, v2, v3}) {
                if (vertexMap.find(v) == vertexMap.end()) {
                    // Parse v/vt/vn ou v//vn
                    std::istringstream vs(v);
                    std::string idx;
                    int pos_idx = -1, norm_idx = -1;
                    int part = 0;
                    
                    while (std::getline(vs, idx, '/')) {
                        if (!idx.empty()) {
                            if (part == 0) pos_idx = std::stoi(idx) - 1;
                            else if (part == 2) norm_idx = std::stoi(idx) - 1;
                        }
                        part++;
                    }
                    
                    // Ajouter position
                    vertices.push_back(temp_pos[pos_idx * 3]);
                    vertices.push_back(temp_pos[pos_idx * 3 + 1]);
                    vertices.push_back(temp_pos[pos_idx * 3 + 2]);
                    
                    // Ajouter normale
                    if (norm_idx >= 0) {
                        vertices.push_back(temp_norm[norm_idx * 3]);
                        vertices.push_back(temp_norm[norm_idx * 3 + 1]);
                        vertices.push_back(temp_norm[norm_idx * 3 + 2]);
                    } else {
                        vertices.push_back(0.0f);
                        vertices.push_back(1.0f);
                        vertices.push_back(0.0f);
                    }
                    
                    vertexMap[v] = (vertices.size() / 6) - 1;
                }
                indices.push_back(vertexMap[v]);
            }
        }
    }
    
    Mesh* m = new Mesh();
    glGenVertexArrays(1, &m->vao);
    glGenBuffers(1, &m->vbo);
    glGenBuffers(1, &m->ebo);
    
    glBindVertexArray(m->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    m->count = indices.size();
    
    return m;
}

void draw(Mesh* m) {
    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->count, GL_UNSIGNED_INT, 0);
}