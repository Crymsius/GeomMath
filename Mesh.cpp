#include "Mesh.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <memory>
using std::pair;

/*ITERATEUR_DE_FACES*/

Iterateur_de_faces::Iterateur_de_faces (std::vector<Face>::iterator pos) : current_it (pos) {}

Iterateur_de_faces& Iterateur_de_faces::operator= (const Iterateur_de_faces& rhs)
{
    current_it = rhs.current_it;
    return *this;
}

Iterateur_de_faces& Iterateur_de_faces::operator++ () {
    ++current_it;
    return *this;
}

Face& Iterateur_de_faces::operator* () const {
        return *current_it;
}

Iterateur_de_faces Mesh::faces_debut () {
    return Iterateur_de_faces(faces.begin());
}

Iterateur_de_faces Mesh::faces_fin () {
    return Iterateur_de_faces(faces.end());
}

/*ITERATEUR_DE_SOMMETS*/

Iterateur_de_sommets::Iterateur_de_sommets (std::vector<Vertex>::iterator pos) : current_it(pos) {}

Iterateur_de_sommets& Iterateur_de_sommets::operator= (const Iterateur_de_sommets& rhs)
{
    current_it = rhs.current_it;
    return *this;
}

Iterateur_de_sommets& Iterateur_de_sommets::operator++ () {
    ++current_it;
    return *this;
}

Vertex& Iterateur_de_sommets::operator* () const {
    return *current_it;
}

/*CIRCULATEUR_DE_FACES*/

Circulateur_de_faces Circulateur_de_faces::end;

Circulateur_de_faces::Circulateur_de_faces () :
    mesh(nullptr),
    sommet_parent(nullptr),
    face_courante(0),
    premiere_face(face_courante)
{}

Circulateur_de_faces::Circulateur_de_faces (const Mesh& mesh_associe, const Vertex& v) :
    mesh(std::make_shared<Mesh>(mesh_associe)),
    sommet_parent(std::make_shared<Vertex>(v)),
    face_courante(v.faceIncidente),
    premiere_face(face_courante)
{}

const Circulateur_de_faces& Circulateur_de_faces::End () {
    return end;
}

Circulateur_de_faces& Circulateur_de_faces::operator= (const Circulateur_de_faces& rhs)
{
    if (this == &rhs)
        return *this;
    mesh = rhs.mesh;
    sommet_parent = rhs.sommet_parent;
    face_courante = rhs.face_courante;
    premiere_face = rhs.premiere_face;
    return *this;
}

Circulateur_de_faces& Circulateur_de_faces::operator++ () {
    //face incidente de parent + 1

    // vertex index on the current face
    auto& current_face = mesh->faces[face_courante];
    
    const auto& vertex_index = mesh->VertexIndexOnFace(*sommet_parent, current_face);
    // for each adjacent faces
    for (auto& i : current_face.facesAdjacentes) {
        auto& adjacent_face = mesh->faces[i];
        const auto v_index = mesh->VertexIndexOnFace(*sommet_parent, adjacent_face);
        // if the vertex isn't part of the face, we continue on the next adjacent face
        if (v_index == -1)
            continue;
        // if the left edge of the current face is the right edge of the adjacent face
        if (current_face.indices[(vertex_index + 2) % 3] == adjacent_face.indices[(v_index + 1) % 3]) {
            if (premiere_face == i) {
                *this = Circulateur_de_faces::end;
            } else {
                face_courante = i;
            }
            return *this;
        }
    }
    *this = Circulateur_de_faces::end;
    return *this;
}

unsigned Circulateur_de_faces::operator* () const {
    return face_courante;
}

Circulateur_de_faces Mesh::faces_incidentes (const Vertex& v) const {
    return Circulateur_de_faces(*this, v);
}

/*CIRCULATEUR_DE_SOMMETS*/
//Circulateur_de_sommets::Circulateur_de_sommets(std::vector<Vertex>::iterator pos, Vertex vparent, unsigned vcourant, Mesh& mesh) : current_it(pos), sommet_parent(vparent), sommet_courant(vcourant), mesh(mesh) {}
//
//Circulateur_de_sommets& Circulateur_de_sommets::operator=(const Circulateur_de_sommets& rhs)
//{
//    current_it = rhs.current_it;
//    sommet_parent = rhs.sommet_parent;
//    sommet_courant = rhs.sommet_courant;
//    mesh = rhs.mesh;
//    return *this;
//}
//
//Circulateur_de_sommets Circulateur_de_sommets::operator++() {
//    //sommet_parent->sommet_courant en sens trigo
//    const std::vector<Face> faces = mesh._Faces();
//    
//    auto it = std::find(faces.begin(),faces.end(), sommet_parent);
//    auto i = it - faces[face_courante].indices.begin();
//
//    return *this;
//}
//
//Vertex& Circulateur_de_sommets::operator*() const {
//    return *current_it;
//}
//
//Circulateur_de_sommets Mesh::sommets_adjacents(Vertex& v) {
//    auto it = find(vertices.begin(), vertices.end(), v);
//    return Circulateur_de_sommets(it, v, (unsigned) (it - vertices.begin()), *this);
//}

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
    std::map<pair<unsigned, unsigned>, unsigned> mapFaces;
    
    for (unsigned i = 0; i < nbFaces; ++i) {
        unsigned nbIndicesPerFace;
        stream >> nbIndicesPerFace;
        Face face(nbIndicesPerFace);
        
        for (unsigned j = 0; j < face.nbIndicesPerFace; ++j) {
            stream >> face.indices[j];
            vertices[face.indices[j]].faceIncidente = i;
        }
        
        for (unsigned j =0; j<face.nbIndicesPerFace; ++j) {
            const auto it = mapFaces.find(std::make_pair(face.indices[j], face.indices[(j+1) % face.nbIndicesPerFace]));
        
            if (it != mapFaces.end()) {
                face.facesAdjacentes.push_back(it->second);
                faces[it->second].facesAdjacentes.push_back(i);
            }
        }
        
        for (unsigned j = 0; j < face.nbIndicesPerFace; ++j) {
             mapFaces.insert(std::make_pair(
                 std::make_pair(face.indices[(j+1) % face.nbIndicesPerFace], face.indices[j]), i));
        }
        
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

int Mesh::VertexIndexOnFace (const Vertex& vertex, const Face& face) const {
    for (unsigned i = 0; i < face.nbIndicesPerFace; ++i) {
        if (vertices[face.indices[i]].position == vertex.position) {
            return i;
        }
    }
    return -1;
}
