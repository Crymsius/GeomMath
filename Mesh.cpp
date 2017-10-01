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
            face_courante = i;
        return *this;
        }
    }
    return *this;
}

int Circulateur_de_faces::operator* () const {
    return face_courante;
}

Circulateur_de_faces Mesh::faces_incidentes (const Vertex& v) const {
    return Circulateur_de_faces(*this, v);
}

/*CIRCULATEUR_DE_SOMMETS*/
Circulateur_de_sommets::Circulateur_de_sommets () :
    mesh(nullptr),
    sommet_parent(nullptr),
    sommet_courant(0),
    premier_sommet(sommet_courant)
{}

Circulateur_de_sommets::Circulateur_de_sommets (const Mesh& mesh_associe, const Vertex& v) :
    mesh(std::make_shared<Mesh>(mesh_associe)),
    sommet_parent(std::make_shared<Vertex>(v)),
    sommet_courant(mesh_associe._Faces()[v.faceIncidente].indices[0]),
    premier_sommet(sommet_courant)
{}

Circulateur_de_sommets& Circulateur_de_sommets::operator=(const Circulateur_de_sommets& rhs)
{
    if (this == &rhs)
        return *this;
    mesh = rhs.mesh;
    sommet_parent = rhs.sommet_parent;
    sommet_courant = rhs.sommet_courant;
    premier_sommet = rhs.premier_sommet;
    return *this;
}


Circulateur_de_sommets& Circulateur_de_sommets::operator++() {
//A FINIR
    //    //sommet_parent->sommet_courant en sens trigo
//    (mesh._Faces()[sommet_courant.faceIncidente].
//    sommet_courant
    return *this;
}

unsigned Circulateur_de_sommets::operator*() const {
    return sommet_courant;
}

Circulateur_de_sommets Mesh::sommets_adjacents(const Vertex& v) const {
    return Circulateur_de_sommets (*this, v);
}

//
//Circulateur_de_contours::Circulateur_de_contours (const Mesh& mesh_associe) :
//    mesh(std::make_shared<Mesh>(mesh_associe)),
//    it_contours(mesh->contours.begin())
//{}
//
//Circulateur_de_contours& Circulateur_de_contours::operator=(const Circulateur_de_contours& rhs){
//    if (this == &rhs)
//        return *this;
//    mesh = rhs.mesh;
//    it_contours= rhs.it_contours;
//    return *this;
//}
//Circulateur_de_contours& Circulateur_de_contours::operator++() {
//    if (++it_contours == mesh->_Contours().end()) {
//        auto it = mesh->_Contours().begin();
//        it_contours = it;
//    }
//    else
//        ++it_contours;
//    return *this;
//}
//Circulateur_de_contours& Circulateur_de_contours::operator--() {
//    if (--it_contours == mesh->_Contours().begin())
//        it_contours = mesh->_Contours().end();
//    else
//        --it_contours;
//    return *this;
//}

/*MESH*/
Iterateur_de_sommets Mesh::sommets_debut() {
    return Iterateur_de_sommets(vertices.begin());
}

Iterateur_de_sommets Mesh::sommets_fin() {
    return Iterateur_de_sommets(vertices.end());
}

void Mesh::ReadFromOFF(const std::string & path) {
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
    
    //creation et remplissage du vector de sommets
    std::vector<Vertex> vertices(nbVertices);
    for (unsigned i = 0; i < nbVertices; ++i) {
        stream >> vertex.position.x;
        stream >> vertex.position.y;
        stream >> vertex.position.z;
        
        vertices[i] = vertex;
    }
    
    std::vector<Face> faces(nbFaces);
    //map de (sommet j, sommet j+1), face
    std::map<pair<unsigned, unsigned>, unsigned> mapFaces;
    
    for (unsigned i = 0; i < nbFaces; ++i) {
        unsigned nbIndicesPerFace;
        stream >> nbIndicesPerFace;
        Face face(nbIndicesPerFace);
        
        //on attribue à chaque sommet de chaque face la face incidente correspondante
        for (unsigned j = 0; j < face.nbIndicesPerFace; ++j) {
            stream >> face.indices[j];
            vertices[face.indices[j]].faceIncidente = i;
        }
        
        for (unsigned j = 0; j < face.nbIndicesPerFace; ++j) {
            const auto it = mapFaces.find(std::make_pair(face.indices[j], face.indices[(j+1) % face.nbIndicesPerFace]));
            //existe-t-il la paire d'indices (j,j+1)[3]
            if (it != mapFaces.end()) {
                face.facesAdjacentes[(j+2)%face.nbIndicesPerFace] = it->second;
                int l = 0;
                for (int k = 0; k < faces[it->second].nbIndicesPerFace; ++k) {
                    if (faces[it->second].indices[k] == face.indices[j]) {
                        l = k;
                    }
                }
                faces[it->second].facesAdjacentes[(l + 1) % faces[it->second].nbIndicesPerFace] = i;
            } else {
                face.facesAdjacentes[(j+2)%face.nbIndicesPerFace] = -1;
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

void Mesh::ReadFromPoints(const std::string & path) {
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
    Face premiere_face;
    
    std::string buf;
    stream >> buf;
    if (buf != "POINTS")
    {
        //todo: log file is not POINTS, throw exception
    }

    //creation et remplissage du vector des 3 premiers sommets
    std::vector<Vertex> vertices(3);
//    std::list<unsigned> contours(3);
    std::list<Contours>listContours;
    
    for (int i = 0; i < 3; ++i) {
        stream >> vertex.position.x;
        stream >> vertex.position.y;
        stream >> vertex.position.z;
        
        vertex.faceIncidente = 0;
        vertices[i] = vertex;
    }
    
    //creation et remplissage de la première face orientée
    if (is_trigo(vertices[0], vertices[1], vertices[2])) {
        premiere_face = Face({0,1,2}, 3);
        Contours a;
        Contours b;
        Contours c;
        a.aretes = std::make_pair(0,1);
        a.faces = 0;
        b.aretes = std::make_pair(1,2);
        b.faces = 0;
        c.aretes = std::make_pair(2,0);
        c.faces = 0;
        listContours.push_back(a);
        listContours.push_back(b);
        listContours.push_back(c);
        
    } else {
        premiere_face = Face({0,2,1}, 3);
        Contours a;
        Contours b;
        Contours c;
        a.aretes = std::make_pair(0,2);
        a.faces = 0;
        b.aretes = std::make_pair(2,1);
        b.faces = 0;
        c.aretes = std::make_pair(1,0);
        c.faces = 0;
        listContours.push_back(a);
        listContours.push_back(b);
        listContours.push_back(c);
    }
    for (int i = 0; i < 3; ++i) {
        premiere_face.facesAdjacentes[i] = -1;
    }
    this->faces.push_back(premiere_face);
    this->vertices = std::move(vertices);
    this->contours = listContours;
    
    //creation et remplissage du vector de sommets
    while (!stream.eof()) {
        stream >> vertex.position.x;
        stream >> vertex.position.y;
        stream >> vertex.position.z;
        
        inserer_sommet(vertex);
    }
}


const std::vector<Vertex> &Mesh::_Vertices() const {
    return vertices;
}

const std::vector<Face> &Mesh::_Faces() const {
    return faces;
}

int Mesh::VertexIndexOnFace (const Vertex& vertex, const Face& face) const {
    for (int i = 0; i < face.nbIndicesPerFace; ++i) {
        if (vertices[face.indices[i]].position == vertex.position) {
            return i;
        }
    }
    return -1;
}

void Mesh::inserer_sommet(Vertex v) {
    
    unsigned indice_sommet = (unsigned) vertices.size();
    
    //ajout du vertex dans le mesh
    vertices.push_back(v);
    
    //tester si c'est dans une face ou pas
    int i = 0;
    for (i; i < faces.size(); ++i) {
        if (is_in_triangle(faces[i], v)){
            //c'est dans la face i
            split_face(i, indice_sommet);
            return;
        }
    }
  
    //c'est en dehors
    //insertion sommet + premiere face
    if (i == faces.size()) {
        
//        std::pair<unsigned, unsigned> previous = std::make_pair(0, 0);
//        for (auto it = contours.begin(); it != contours.end(); ++it) {
//            if (is_trigo(vertices[it->aretes.first], v, vertices[it->aretes.second])) {
//                
//                int m = faces.size();
//                
//                std::vector<unsigned> sommets_m(3);
//                sommets_m[0] = it->aretes.first;
//                sommets_m[1] = indice_sommet;
//                sommets_m[2] = it->aretes.second;
//                
//                faces.emplace_back(sommets_m, 3);
//                faces[m].facesAdjacentes[0] = -1;
//                faces[m].facesAdjacentes[1] = it->faces; //indice de la face opposée au novueau sommet
//                faces[m].facesAdjacentes[2] = -1;
//                
//                if (previous == std::make_pair(sommets_m[1], sommets_m[0])) {
//                    it = contours.erase(--it);
//                    it = contours.erase(it);
//                    
//                    previous.first = 0;
//                    previous.second = 0;
//                } else {
//                    it->aretes.first = sommets_m[0];
//                    it->aretes.second = sommets_m[1];
//                    it->faces = m;
//                    
//                    previous.first = sommets_m[1];
//                    previous.second = sommets_m[2];
//                }
//                Contours newcontours;
//                newcontours.aretes = std::make_pair(sommets_m[1], sommets_m[2]);
//                newcontours.faces = m;
//                
//                it = contours.insert(++it, newcontours);
//            }
        
        //premiere face
        auto itDebut = contours.begin();
        for (auto it = contours.begin(); it != contours.end(); ++it) {
            if (is_trigo(vertices[it->aretes.first], v, vertices[it->aretes.second])) {
                
                int m = faces.size();
                vertices[indice_sommet].faceIncidente = m;
                
                std::vector<unsigned> sommets_m(3);
                sommets_m[0] = it->aretes.first;
                sommets_m[1] = indice_sommet;
                sommets_m[2] = it->aretes.second;
                
                faces.emplace_back(sommets_m, 3);
                faces[m].facesAdjacentes[0] = -1;
                faces[m].facesAdjacentes[1] = it->faces; //indice de la face opposée au novueau sommet
                faces[m].facesAdjacentes[2] = -1;
                
                faces[it->faces].facesAdjacentes[(VertexIndexOnFace(vertices[it->aretes.first], faces[it->faces])+2)%faces[it->faces].nbIndicesPerFace] = m;
                
                it->aretes.second = sommets_m[1];
                it->faces = m;
                
                Contours newcontours;
                newcontours.aretes = std::make_pair(sommets_m[1], sommets_m[2]);
                newcontours.faces = m;
                itDebut = it;
                it = contours.insert(++it, newcontours);
                break;
            }
        }
        if (itDebut == contours.begin()) {
            auto itl = std::prev(contours.end(),1);
            //trigo à la fin
            if (is_trigo(vertices[itl->aretes.first], v, vertices[itl->aretes.second])) {
                
                int m = faces.size();
                
                std::vector<unsigned> sommets_m(3);
                sommets_m[0] = itl->aretes.first;
                sommets_m[1] = indice_sommet;
                sommets_m[2] = itl->aretes.second;
                
                faces.emplace_back(sommets_m, 3);
                faces[m].facesAdjacentes[0] = contours.begin()->faces;
                faces[m].facesAdjacentes[1] = itl->faces; //indice de la face opposée au novueau sommet
                faces[m].facesAdjacentes[2] = -1;
                
                //faces adjacentes
                faces[itl->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2)% faces[itl->faces].nbIndicesPerFace] = m;
                faces[itDebut->faces].facesAdjacentes[2] = m;
                
                itl->aretes.second = sommets_m[1];
                itl->faces = m;
                
                contours.erase(contours.begin());
            }
            
            while (itl != std::next(itDebut,1)) {
                if (is_trigo(vertices[itl->aretes.first], v, vertices[itl->aretes.second])) {
                    
                    int m = faces.size();
                    
                    std::vector<unsigned> sommets_m(3);
                    sommets_m[0] = itl->aretes.first;
                    sommets_m[1] = indice_sommet;
                    sommets_m[2] = itl->aretes.second;
                    
                    faces.emplace_back(sommets_m, 3);
                    faces[m].facesAdjacentes[0] = std::next(itl,1)->faces;
                    faces[m].facesAdjacentes[1] = itl->faces; //indice de la face opposée au novueau sommet
                    faces[m].facesAdjacentes[2] = -1;
                    
                    faces[itl->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2)% faces[itl->faces].nbIndicesPerFace] = m;
                    faces[std::next(itl,1)->faces].facesAdjacentes[2] = m;
                    
                    itl->aretes.second = sommets_m[1];
                    itl->faces = m;
                    
                    contours.erase(std::next(itl,1));
                    
                } else break;
                --itl;
            }
        }
        
        auto itr = itDebut;
        while (itr != contours.end()) {
            if (is_trigo(vertices[itr->aretes.first], v, vertices[itr->aretes.second])) {
                
                int m = faces.size();
                
                std::vector<unsigned> sommets_m(3);
                sommets_m[0] = itr->aretes.first;
                sommets_m[1] = indice_sommet;
                sommets_m[2] = itr->aretes.second;
                
                faces.emplace_back(sommets_m, 3);
                faces[m].facesAdjacentes[0] = -1;
                faces[m].facesAdjacentes[1] = itr->faces; //indice de la face opposée au novueau sommet
                faces[m].facesAdjacentes[2] = std::prev(itr,1)->faces;
                
                faces[itr->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itr->aretes.first], faces[itr->faces])+2)% faces[itr->faces].nbIndicesPerFace] = m;
                faces[std::prev(itr,1)->faces].facesAdjacentes[0] = m;
                
                itr->aretes.second = sommets_m[1];
                itr->faces = m;
                
                contours.erase(std::next(itr,1));
                
            } else break;
            ++itr;
        }
        
    }
}

bool Mesh::is_trigo (Vertex a, Vertex b, Vertex c) {
    return ((b.position.x - a.position.x)*(c.position.y - a.position.y) - (b.position.y - a.position.y)*(c.position.x - a.position.x) > 0) ;
}

bool Mesh::is_in_triangle (Face f, Vertex v) {
    Vertex a = vertices[f.indices[0]];
    Vertex b = vertices[f.indices[1]];
    Vertex c = vertices[f.indices[2]];
    return is_trigo(a, b, v) && is_trigo(b, c, v) && is_trigo(c, a, v);
}

void Mesh::split_face (int i, unsigned p) {
    // i : indice de la face à split
    // p : indice du point à placer
    
    Face face_i = this->faces[i];
    
    //sommets de i :
    unsigned a = face_i.indices[0];
    unsigned b = face_i.indices[1];
    unsigned c = face_i.indices[2];
    
    //faces
    int j = face_i.facesAdjacentes[1];
    int k = face_i.facesAdjacentes[2];
    int m = (int) faces.size();
    
    //changement face incidente des points
    vertices[p].faceIncidente = i;
    vertices[a].faceIncidente = m;
    
    std::vector<unsigned> sommets_m(3);
    std::vector<unsigned> sommets_n(3);
    
    sommets_m[0] = p;
    sommets_m[1] = c;
    sommets_m[2] = a;
    
    sommets_n[0] = p;
    sommets_n[1] = a;
    sommets_n[2] = b;
    
    faces.emplace_back(sommets_m, 3);
    faces.emplace_back(sommets_n, 3);
    
    faces[m].facesAdjacentes[0] = j;
    faces[m].facesAdjacentes[1] = m+1;
    faces[m].facesAdjacentes[2] = i;
    
    faces[m+1].facesAdjacentes[0] = k;
    faces[m+1].facesAdjacentes[1] = i;
    faces[m+1].facesAdjacentes[2] = m;
    
    faces[i].indices[0] = p;
    faces[i].facesAdjacentes[1] = m;
    faces[i].facesAdjacentes[2] = m+1;
    
    if (j != -1) {
        int aDansj = VertexIndexOnFace(vertices[a], faces[j]);
        faces[j].facesAdjacentes[(aDansj + 2)%faces[j].nbIndicesPerFace] = m;
    }
    
    if (k != -1) {
        int aDansk = VertexIndexOnFace(vertices[a], faces[k]);
        faces[k].facesAdjacentes[(aDansk + 1)%faces[k].nbIndicesPerFace] = m+1;
    }
    
    //contours
    for (auto it = contours.begin(); it != contours.end(); ++it) {
        if (it->aretes.first == faces[i].indices[1] && it->aretes.second == faces[i].indices[2])
            it->faces = i;
        else if (it->aretes.first == faces[m].indices[1] && it->aretes.second == faces[m].indices[2])
            it->faces = m;
        else if (it->aretes.first == faces[m+1].indices[1] && it->aretes.second == faces[m+1].indices[2])
            it->faces = m+1;
    }
    
}

void Mesh::flip (int i, unsigned a) {
    // i : indice d'une des faces
    // a : indice du sommet opposé à l'autre face
    int aDansi = VertexIndexOnFace(vertices[a], faces[i]);
    
    // j
    int j = faces[i].facesAdjacentes[aDansi];
    if (j == -1) {
        return;
    }
    int bDansj = VertexIndexOnFace(vertices[faces[i].indices[(aDansi+1)%faces[i].nbIndicesPerFace]], faces[j]);
    
    // faces externes
    int m = faces[j].facesAdjacentes[(bDansj+2)%faces[j].nbIndicesPerFace];
    int n = faces[i].facesAdjacentes[(aDansi+1)%faces[i].nbIndicesPerFace];

    // sommets
    unsigned b = faces[j].indices[bDansj];
    unsigned d = faces[j].indices[(bDansj+2)%faces[j].nbIndicesPerFace];
    
    // modifications faces adjacentes externes
    if (m != -1) {
        int bDansm = VertexIndexOnFace(vertices[faces[j].indices[(bDansj)]], faces[m]);
        faces[m].facesAdjacentes[(bDansm+1)%faces[m].nbIndicesPerFace] = i;
    }
    if (n!= -1) {
        int aDansn = VertexIndexOnFace(vertices[faces[i].indices[(aDansi)]], faces[n]);
        faces[n].facesAdjacentes[(aDansn+2)%faces[n].nbIndicesPerFace] = j;
    }
    
    // modifications sommets
    faces[i].indices[(aDansi+2)%faces[i].nbIndicesPerFace] = faces[j].indices[(bDansj+1)%faces[j].nbIndicesPerFace];
    faces[j].indices[bDansj] = a;
   
    // modifications faces incidentes
    vertices[b].faceIncidente = i;
    vertices[d].faceIncidente = j;
    
    // modifications faces adjacentes
    faces[i].facesAdjacentes[(aDansi+1)%faces[i].nbIndicesPerFace] = j;
    faces[j].facesAdjacentes[(bDansj+2)%faces[j].nbIndicesPerFace] = i;
    
    faces[i].facesAdjacentes[aDansi] = m;
    faces[j].facesAdjacentes[(bDansj+1)%faces[j].nbIndicesPerFace] = n;
    
}
