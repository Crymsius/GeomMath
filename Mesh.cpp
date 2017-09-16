#include "Mesh.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
using std::pair;

/*ITERATEUR_DE_FACES*/

Iterateur_de_faces::Iterateur_de_faces(std::vector<Face>::iterator pos) : current_it(pos) {}

Iterateur_de_faces& Iterateur_de_faces::operator=(const Iterateur_de_faces& rhs)
{
    current_it = rhs.current_it;
    return *this;
}

Iterateur_de_faces Iterateur_de_faces::operator++() {
    ++current_it;
    return *this;
}

Face& Iterateur_de_faces::operator*() const {
        return *current_it;
}

Iterateur_de_faces Mesh::faces_debut() {
    return Iterateur_de_faces(faces.begin());
}

Iterateur_de_faces Mesh::faces_fin() {
    return Iterateur_de_faces(faces.end());
}

/*ITERATEUR_DE_SOMMETS*/

Iterateur_de_sommets::Iterateur_de_sommets(std::vector<Vertex>::iterator pos) : current_it(pos) {}

Iterateur_de_sommets& Iterateur_de_sommets::operator=(const Iterateur_de_sommets& rhs)
{
    current_it = rhs.current_it;
    return *this;
}

Iterateur_de_sommets Iterateur_de_sommets::operator++() {
    ++current_it;
    return *this;
}

Vertex& Iterateur_de_sommets::operator*() const {
    return *current_it;
}

/*CIRCULATEUR_DE_FACES*/
Circulateur_de_faces::Circulateur_de_faces(std::vector<Face>::iterator pos, Vertex v, unsigned f, Mesh& mesh_associe) : current_it(pos), sommet_parent(v), face_courante(f), mesh(mesh_associe) {}

Circulateur_de_faces& Circulateur_de_faces::operator=(const Circulateur_de_faces& rhs)
{
    current_it = rhs.current_it;
    sommet_parent = rhs.sommet_parent;
    face_courante = rhs.face_courante;
    return *this;
}

Circulateur_de_faces Circulateur_de_faces::operator++() {
    //face incidente de parent + 1
    auto it = std::find(mesh.faces[face_courante].indices.begin(),mesh.faces[face_courante].indices.end(), sommet_parent);
    unsigned i = it - mesh.faces[face_courante].indices.begin();
    if ( (it - mesh.faces[face_courante].indices.begin()) != mesh.faces[face_courante].nbIndicesPerFace-2) {
        return mesh.faces[face_courante].indices[it - mesh.faces[face_courante].indices.begin()]
    } else {
        
    }
    return *this;
}

Face& Circulateur_de_faces::operator*() const {
    return *current_it;
}

Circulateur_de_faces Mesh::faces_incidentes(Vertex& v) {
    return Circulateur_de_faces(faces.begin() + v.faceIncidente, v, v.faceIncidente, *this);
}

/*CIRCULATEUR_DE_SOMMETS*/
Circulateur_de_sommets::Circulateur_de_sommets(std::vector<Vertex>::iterator pos, Vertex vparent, unsigned vcourant) : current_it(pos), sommet_parent(vparent), sommet_courant(vcourant) {}

Circulateur_de_sommets& Circulateur_de_sommets::operator=(const Circulateur_de_sommets& rhs)
{
    current_it = rhs.current_it;
    sommet_parent = rhs.sommet_parent;
    sommet_courant = rhs.sommet_courant;
    return *this;
}

Circulateur_de_sommets Circulateur_de_sommets::operator++() {
    //sommet_parent->sommet_courant en sens trigo
    
    return *this;
}

Vertex& Circulateur_de_sommets::operator*() const {
    return *current_it;
}

Circulateur_de_sommets Mesh::sommets_adjacents(Vertex& v) {
    auto it = find(vertices.begin(), vertices.end(), v);
    return Circulateur_de_sommets(it, v, (unsigned) (it - vertices.begin()));
}

/*MESH*/
Iterateur_de_sommets Mesh::sommets_debut() {
    return Iterateur_de_sommets(vertices.begin());
}

Iterateur_de_sommets Mesh::sommets_fin() {
    return Iterateur_de_sommets(vertices.end());
}

void Mesh::ReadFromOFF(std::string path) {
    std::ifstream input_stream;
    input_stream.exceptions(input_stream.badbit | input_stream.failbit);
    
    std::stringstream stream;
    try {
        input_stream.open(path);
        stream << input_stream.rdbuf();
        input_stream.close();
    }
    catch (std::ifstream::failure& error) {
        //todo : log error
        input_stream.close();
    }
    
    Vertex vertex;
    Face face (3); // todo : handle other face format
    unsigned nbVertices;
    unsigned nbFaces;
    unsigned def;
    
    std::map<pair<int, int>, unsigned> mapFaces;
    
    std::string buf;
    stream >> buf;
    if (buf != "OFF")
    {
        //todo: log file is not OFF, throw exception
    }
    
    stream >> nbVertices;
    stream >> nbFaces;
    stream >> def;
    
    std::vector<Vertex> vertices(nbVertices);
    for (unsigned i = 0; i < nbVertices; ++i)
    {
        stream >> vertex.position.x;
        stream >> vertex.position.y;
        stream >> vertex.position.z;
        
        vertices[i] = vertex;
    }
    
    std::vector<Face> faces(nbFaces);
    
    for (unsigned i = 0; i < nbFaces; ++i)
    {
        stream >> face.nbIndicesPerFace;
        face.indices.reserve(face.nbIndicesPerFace);
        for (unsigned j = 0; j < face.nbIndicesPerFace; ++j)
        {
            stream >> face.indices[j];
            vertices[face.indices[j]].faceIncidente = i;
        }
        auto it = mapFaces.find(std::make_pair(face.indices[0], face.indices[1]));
        
        if (it != mapFaces.end()) {
            face.facesAdjacentes.push_back(it->second);
            faces[it->second].facesAdjacentes.push_back(i);
            mapFaces.erase(it);
        } 
        
        it = mapFaces.find(std::make_pair(face.indices[1], face.indices[2]));
        
        if (it != mapFaces.end()) {
            face.facesAdjacentes.push_back(it->second);
            faces[it->second].facesAdjacentes.push_back(i);
            mapFaces.erase(it);
        }
        
        it = mapFaces.find(std::make_pair(face.indices[2], face.indices[0]));
        
        if (it != mapFaces.end()) {
            face.facesAdjacentes.push_back(it->second);
            faces[it->second].facesAdjacentes.push_back(i);
            mapFaces.erase(it);
        }
        
        mapFaces.insert(std::make_pair(std::make_pair(face.indices[1], face.indices[0]), i));
        mapFaces.insert(std::make_pair(std::make_pair(face.indices[2], face.indices[1]), i));
        mapFaces.insert(std::make_pair(std::make_pair(face.indices[0], face.indices[2]), i));
        faces[i] = std::move(face);
    }
    
    this->vertices = std::move(vertices);
    this->faces = std::move(faces);
    
}

const std::vector<Vertex> &Mesh::_Vertices() const {
    return vertices;
}

const std::vector<Face> &Mesh::_Faces() const {
    return faces;
}

//Box3 getBox3(void) {
//    Box3 box;
////    Vector3 a (min(x), min(y), min(z));
////    Vector3 a (max(x), max(y), max(z));
////    box.a = a;
////    box.b = b;
//    return box;
//}
