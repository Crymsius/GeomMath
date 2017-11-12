#include "Mesh.h"
#include "Cercle.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <memory>
#include <queue>
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
    // vertex index on the current face
    auto& current_vertex = mesh->vertices[sommet_courant];
    
    const auto& current_face = mesh->faceFromPair(*sommet_parent, current_vertex);
    const auto& vertex_index = mesh->VertexIndexOnFace(current_vertex, mesh->faces[current_face]);
    
    sommet_courant = (vertex_index + 1) % mesh->faces[current_face].nbIndicesPerFace;
    return *this;
}

unsigned Circulateur_de_sommets::operator*() const {
    return sommet_courant;
}

Circulateur_de_sommets Mesh::sommets_adjacents(const Vertex& v) const {
    return Circulateur_de_sommets (*this, v);
}

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
        std::cout << "Caught an ios_base::failure.\n"
        << "Explanatory string: " << error.what() << '\n'
        << "Error code: " << error.code() << '\n';
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
//        inserer_sommet_delaunay_incr(vertex);
    }
}


const std::vector<Vertex> &Mesh::_Vertices() const {
    return vertices;
}

const std::vector<Face> &Mesh::_Faces() const {
    return faces;
}

int Mesh::VertexIndex(const int f, const int i) const {
    return faces[f].indices[i];
}

int Mesh::NextVertexIndex(const int f, const int i) const {
    return faces[f].indices[(i+1) % 3];
}

int Mesh::PrevVertexIndex(const int f, const int i) const {
    return faces[f].indices[(i-1) % 3];
}

int Mesh::VertexIndexOnFace (const Vertex& vertex, const Face& face) const {
    for (int i = 0; i < face.nbIndicesPerFace; ++i) {
        if (vertices[face.indices[i]].position == vertex.position) {
            return i;
        }
    }
    return -1;
}

int Mesh::faceFromPair(const Vertex& v1, const Vertex& v2) const {
    for (int i = 0; i < faces.size(); ++i) {
        for (int j = 0; j < faces[i].nbIndicesPerFace ; ++i) {
            if (faces[i].indices[j] == VertexIndexOnFace(v1, faces[i]) &&
                faces[i].indices[(j+1) % faces[i].nbIndicesPerFace] == VertexIndexOnFace(v2, faces[i]))
                return i;
        }
    }
    return -1;
}

int Mesh::locatePointInFace(const Vertex v) {
    int f = 0;
    //wip
    while (!is_in_triangle(faces[f], v) || f == -1) {
        for (int i = 0; i < faces[f].nbIndicesPerFace; ++i) {
            if (!is_trigo(vertices[faces[f].indices[i]],
                          vertices[faces[f].indices[(i+1) % faces[f].nbIndicesPerFace]],
                           v)) {
                f = faces[f].facesAdjacentes[(i+2) % faces[f].nbIndicesPerFace];
                break;
            }
        }
    }
    return f;
}

void Mesh::inserer_sommet(Vertex v) {
    
//    auto itcout = contours.begin();
//    int indexContour = 0;
//    std::cout << "------ Debut ----\n" << std::endl;
//    while (itcout != contours.end()){
//        std::cout << "indexContour : " << indexContour << std::endl;
//        std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
//        indexContour++;
//        itcout++;
//    }

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
  
//    itcout = contours.begin();
//    indexContour = 0;
//    std::cout << "------ En dehors ----\n" << std::endl;
//    while (itcout != contours.end()){
//        std::cout << "indexContour : " << indexContour << std::endl;
//        std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
//        indexContour++;
//        itcout++;
//    }

    //c'est en dehors
    //insertion sommet + premiere face
    if (i == faces.size()) {
        //premiere face
        auto itDebut = contours.begin();
        auto itFin = contours.end();
        auto itr = contours.begin();
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
                itr = it;
                break;
            }
        }
    
//        itcout = contours.begin();
//        indexContour = 0;
//        std::cout << "------ Premiere Droite ----\n" << std::endl;
//        while (itcout != contours.end()){
//            std::cout << "indexContour : " << indexContour << std::endl;
//            std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
//            indexContour++;
//            itcout++;
//        }
        
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
                faces[itl->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2) % 3] = m;
                faces[itDebut->faces].facesAdjacentes[2] = m;
                
                itl->aretes.second = sommets_m[1];
                itl->faces = m;
//                std::cout << "\n\nbegin supprimé : " << contours.begin()->aretes.first << " " << contours.begin()->aretes.second << "\n" << std::endl;
                contours.erase(contours.begin());
                
//                itcout = contours.begin();
//                indexContour = 0;
//                std::cout << "------Première Gauche ----\n" << std::endl;
//                std::cout << "itl : " << itl->aretes.first << " " << itl->aretes.second << "\n"<< std::endl;
//                while (itcout != contours.end()){
//                    std::cout << "indexContour : " << indexContour << std::endl;
//                    std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
//                    indexContour++;
//                    itcout++;
//                }

                while (itl != std::next(itDebut,1)) {
                    itl--;
//                    std::cout << "------Pre Itl ----\n" << std::endl;
//                    std::cout << "itl : " << itl->aretes.first << " " << itl->aretes.second << "\n"<< std::endl;
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
                        
                        faces[itl->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2) % 3] = m;
                        faces[std::next(itl,1)->faces].facesAdjacentes[2] = m;
                        
                        itl->aretes.second = sommets_m[1];
                        itl->faces = m;
                        
//                        std::cout << "\n\nitl supprimé : " << std::next(itl,1)->aretes.first << " " << std::next(itl,1)->aretes.second << "\n" << std::endl;
                        
                        contours.erase(std::next(itl,1));
                        
                    } else break;
                    --itl;
                }
            }
        }
        
//        itcout = contours.begin();
//        indexContour = 0;
//        std::cout << "------Pre-itr ----\n" << std::endl;
//        std::cout << "itr : " << itr->aretes.first << " " << itr->aretes.second << "\n"<< std::endl;
//        while (itcout != contours.end()){
//            std::cout << "indexContour : " << indexContour << std::endl;
//            std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n"<< std::endl;
//            indexContour++;
//            itcout++;
//        }
//        std::cout << "----prev-end----\n"<< std::endl;
//        std::cout << std::prev(contours.end(),1)->aretes.first << " " << std::prev(contours.end(),1)->aretes.second << "\n"<< std::endl;
        
        if (itr != std::prev(contours.end(),1)) {
            itr = std::next(itr, 1);
            while (itr != contours.end()) {
                if (is_trigo(vertices[itr->aretes.first], v, vertices[itr->aretes.second])) {

                    int m = faces.size();

                    std::vector<unsigned> sommets_m(3);
                    sommets_m[0] = itr->aretes.first;
                    sommets_m[1] = indice_sommet;
                    sommets_m[2] = itr->aretes.second;

                    faces.emplace_back(sommets_m, 3);
                    faces[m].facesAdjacentes[0] = -1;
                    faces[m].facesAdjacentes[1] = itr->faces; //indice de la face opposée au nouveau sommet
                    faces[m].facesAdjacentes[2] = std::prev(itr,1)->faces;

                    faces[itr->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itr->aretes.first], faces[itr->faces])+2) % 3] = m;
                    faces[std::prev(itr,1)->faces].facesAdjacentes[0] = m;
                    
                    itr->aretes.first = sommets_m[1];
                    itr->faces = m;
                    
                    contours.erase(std::prev(itr,1));
                    
                } else break;
                ++itr;
            }
//            std::cout << "------Post-itr ----\n"<< std::endl;
//            auto itcout = contours.begin();
//            int indexContour = 0;
//            while (itcout != contours.end()){
//                std::cout << "indexContour : " << indexContour << std::endl;
//                std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
//                indexContour++;
//                itcout++;
//            }
        }
//        else {
//            std::cout<<"-----------dépassement-------------"<< std::endl;
//        }
    }
//    else {
//        std::cout<<"-----------i!= face.size-------------" << std::endl;
//    }
}

void Mesh::inserer_sommet_delaunay_incr(Vertex v) {
    
    //    auto itcout = contours.begin();
    //    int indexContour = 0;
    //    std::cout << "------ Debut ----\n" << std::endl;
    //    while (itcout != contours.end()){
    //        std::cout << "indexContour : " << indexContour << std::endl;
    //        std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
    //        indexContour++;
    //        itcout++;
    //    }
    
    unsigned indice_sommet = (unsigned) vertices.size();
    
    //ajout du vertex dans le mesh
    vertices.push_back(v);
    
    //tester si c'est dans une face ou pas
    int i = 0;
    for (i; i < faces.size(); ++i) {
        if (is_in_triangle(faces[i], v)){
            //c'est dans la face i
            split_face_delaunay_incr(i, indice_sommet);
            return;
        }
    }
    
    //    itcout = contours.begin();
    //    indexContour = 0;
    //    std::cout << "------ En dehors ----\n" << std::endl;
    //    while (itcout != contours.end()){
    //        std::cout << "indexContour : " << indexContour << std::endl;
    //        std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
    //        indexContour++;
    //        itcout++;
    //    }
    
    //c'est en dehors
    //insertion sommet + premiere face
    std::queue<std::pair<int,unsigned>> file;
    
    if (i == faces.size()) {
        //premiere face
        auto itDebut = contours.begin();
        auto itFin = contours.end();
        auto itr = contours.begin();
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
                
                int indexVertOppose = (VertexIndexOnFace(vertices[it->aretes.first], faces[it->faces])+2) % 3;
                unsigned vertOppose = this->faces[it->faces].indices[indexVertOppose];
                Cercle cercle(this->faces[m], *this);
                if (cercle.isInCercle(this->vertices[vertOppose].position)) {
                    file.push(std::make_pair(m, indice_sommet));
                }
                
                faces[it->faces].facesAdjacentes[indexVertOppose] = m;
  
                it->aretes.second = sommets_m[1];
                it->faces = m;
                
                
                Contours newcontours;
                newcontours.aretes = std::make_pair(sommets_m[1], sommets_m[2]);
                newcontours.faces = m;
                itDebut = it;
                it = contours.insert(++it, newcontours);
                itr = it;
                break;
            }
        }
        
        //        itcout = contours.begin();
        //        indexContour = 0;
        //        std::cout << "------ Premiere Droite ----\n" << std::endl;
        //        while (itcout != contours.end()){
        //            std::cout << "indexContour : " << indexContour << std::endl;
        //            std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
        //            indexContour++;
        //            itcout++;
        //        }
        
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
                
                int indexVertOppose = (VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2) % 3;
                unsigned vertOppose = this->faces[itl->faces].indices[indexVertOppose];
                Cercle cercle(this->faces[m], *this);
                if (cercle.isInCercle(this->vertices[vertOppose].position)) {
                    file.push(std::make_pair(m, indice_sommet));
                }
                
                //faces adjacentes
                faces[itl->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2) % 3] = m;
                faces[itDebut->faces].facesAdjacentes[2] = m;
                
                itl->aretes.second = sommets_m[1];
                itl->faces = m;
                //                std::cout << "\n\nbegin supprimé : " << contours.begin()->aretes.first << " " << contours.begin()->aretes.second << "\n" << std::endl;
                contours.erase(contours.begin());
                
                //                itcout = contours.begin();
                //                indexContour = 0;
                //                std::cout << "------Première Gauche ----\n" << std::endl;
                //                std::cout << "itl : " << itl->aretes.first << " " << itl->aretes.second << "\n"<< std::endl;
                //                while (itcout != contours.end()){
                //                    std::cout << "indexContour : " << indexContour << std::endl;
                //                    std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
                //                    indexContour++;
                //                    itcout++;
                //                }
                
                while (itl != std::next(itDebut,1)) {
                    itl--;
                    //                    std::cout << "------Pre Itl ----\n" << std::endl;
                    //                    std::cout << "itl : " << itl->aretes.first << " " << itl->aretes.second << "\n"<< std::endl;
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
                        
                        faces[itl->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2) % 3] = m;
                        faces[std::next(itl,1)->faces].facesAdjacentes[2] = m;
                        
                        int indexVertOppose = (VertexIndexOnFace(vertices[itl->aretes.first], faces[itl->faces])+2) % 3;
                        unsigned vertOppose = this->faces[itl->faces].indices[indexVertOppose];
                        Cercle cercle(this->faces[m], *this);
                        if (cercle.isInCercle(this->vertices[vertOppose].position)) {
                            file.push(std::make_pair(m, indice_sommet));
                        }
                        
                        itl->aretes.second = sommets_m[1];
                        itl->faces = m;
                        
                        //                        std::cout << "\n\nitl supprimé : " << std::next(itl,1)->aretes.first << " " << std::next(itl,1)->aretes.second << "\n" << std::endl;
                        
                        contours.erase(std::next(itl,1));
                        
                    } else break;
                    --itl;
                }
            }
        }
        
        //        itcout = contours.begin();
        //        indexContour = 0;
        //        std::cout << "------Pre-itr ----\n" << std::endl;
        //        std::cout << "itr : " << itr->aretes.first << " " << itr->aretes.second << "\n"<< std::endl;
        //        while (itcout != contours.end()){
        //            std::cout << "indexContour : " << indexContour << std::endl;
        //            std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n"<< std::endl;
        //            indexContour++;
        //            itcout++;
        //        }
        //        std::cout << "----prev-end----\n"<< std::endl;
        //        std::cout << std::prev(contours.end(),1)->aretes.first << " " << std::prev(contours.end(),1)->aretes.second << "\n"<< std::endl;
        
        if (itr != std::prev(contours.end(),1)) {
            itr = std::next(itr, 1);
            while (itr != contours.end()) {
                if (is_trigo(vertices[itr->aretes.first], v, vertices[itr->aretes.second])) {
                    
                    int m = faces.size();
                    
                    std::vector<unsigned> sommets_m(3);
                    sommets_m[0] = itr->aretes.first;
                    sommets_m[1] = indice_sommet;
                    sommets_m[2] = itr->aretes.second;
                    
                    faces.emplace_back(sommets_m, 3);
                    faces[m].facesAdjacentes[0] = -1;
                    faces[m].facesAdjacentes[1] = itr->faces; //indice de la face opposée au nouveau sommet
                    faces[m].facesAdjacentes[2] = std::prev(itr,1)->faces;
                    
                    faces[itr->faces].facesAdjacentes[(VertexIndexOnFace(vertices[itr->aretes.first], faces[itr->faces])+2) % 3] = m;
                    faces[std::prev(itr,1)->faces].facesAdjacentes[0] = m;
                    
                    int indexVertOppose = (VertexIndexOnFace(vertices[itr->aretes.first], faces[itr->faces])+2) % 3;
                    unsigned vertOppose = this->faces[itr->faces].indices[indexVertOppose];
                    Cercle cercle(this->faces[m], *this);
                    if (cercle.isInCercle(this->vertices[vertOppose].position)) {
                        file.push(std::make_pair(m, indice_sommet));
                    }
                    
                    itr->aretes.first = sommets_m[1];
                    itr->faces = m;
                    
                    contours.erase(std::prev(itr,1));
                    
                } else break;
                ++itr;
            }
//            std::cout << "------Post-itr ----\n"<< std::endl;
//            auto itcout = contours.begin();
//            int indexContour = 0;
//            while (itcout != contours.end()){
//                std::cout << "indexContour : " << indexContour << std::endl;
//                std::cout << "itcout : " << itcout->aretes.first << " " << itcout->aretes.second << "\n" << std::endl;
//                indexContour++;
//                itcout++;
//            }
        }
        
        lawsonQueue(file);
    }
}

std::queue<std::pair<int,unsigned>> Mesh::findAretesToFlip () {
    //file des paires <face, sommet opposé à l'autre face>
    std::queue<std::pair<int,unsigned>> file, fileCopy;
    for (auto i = 0; i < this->faces.size(); ++i) {
        for (auto j = 0; j < this->faces[i].nbIndicesPerFace; ++j) {
            //face adjacente j
            int facej = this->faces[i].facesAdjacentes[j];
            
            //pas sur le contours
            if (facej!= -1) {
                //premier sommet de l'arête
                unsigned a = (j+1) % this->faces[i].nbIndicesPerFace;
                //sommet à tester
                unsigned p = this->faces[facej].indices[(VertexIndexOnFace(this->vertices[a], this->faces[facej]) + 1) % 3];
                Cercle cercle(this->faces[i], *this);
                if (cercle.isInCercle(this->vertices[p].position)) {
                    //test si l'arête existe déjà
                    fileCopy = file;
                    while (!fileCopy.empty()) {
                        if (fileCopy.front().first == facej && fileCopy.front().second == p)
                            break;
                        fileCopy.pop();
                    }
                    if (fileCopy.empty())
                        file.push(std::make_pair(i, this->faces[i].indices[j]));
                }
            }
        }
    }
    return file;
}

void Mesh::lawsonQueue(std::queue<std::pair<int,unsigned>> file) {
    
    std::queue<std::pair<int,unsigned>> fileCopy;
    
    //flip des arêtes dans la liste
    while (!file.empty()) {
        //face i
        int i = file.front().first;
        //sommet opposé à l'autre face
        unsigned a = file.front().second;
        int aDansi = VertexIndexOnFace(this->vertices[a], this->faces[i]);
        
        //face j (opposée à i par l'arête flippée)
        int j = this->faces[i].facesAdjacentes[aDansi];
        
        //parcourt de la liste et echange de la face avec sa face opposée si c'est celle à flip
        file.push(std::make_pair(file.front().first, file.front().second));
        file.pop();
        for (int l = 1; l < file.size(); l++) {
            if (file.front().first == i) {
                std::cout << "\nface i: "<<i<< "dans la queue\n"<< std::endl;
                int newface = this->faces[i].facesAdjacentes[VertexIndexOnFace(this->vertices[file.front().second], this->faces[i])];
                unsigned indexOppose;
                for (unsigned k = 0; k < 3; ++k) {
                    if (this->faces[newface].facesAdjacentes[k] == i) {
                        indexOppose = k;
                        break;
                    }
                }
                unsigned newvertex = this->faces[newface].indices[indexOppose];
                file.front().first = newface;
                file.front().second = newvertex;
            } else if (file.front().first == j) {
                int newface = this->faces[j].facesAdjacentes[VertexIndexOnFace(this->vertices[file.front().second], this->faces[j])];
                unsigned indexOppose;
                for (unsigned k = 0; k < 3; ++k) {
                    if (this->faces[newface].facesAdjacentes[k] == j) {
                        indexOppose = k;
                        break;
                    }
                }
                unsigned newvertex = this->faces[newface].indices[indexOppose];
                file.front().first = newface;
                file.front().second = newvertex;
            }
            file.push(std::make_pair(file.front().first, file.front().second));
            file.pop();
        }
        
        
        flip (i, a);
        file.pop();
        
        //sommet opposé
        unsigned c;
        
        //cercle de la face i
        Cercle cercleI(this->faces[i], *this);
        //face opposée à a
        int m = this->faces[i].facesAdjacentes[aDansi];
        //sommet de l'arête
        unsigned b = this->NextVertexIndex(i, aDansi);
        if (m != -1) {
            int bDansm = VertexIndexOnFace(this->vertices[b], this->faces[m]);
            c = this->NextVertexIndex(m, bDansm);
            if (cercleI.isInCercle(this->vertices[c].position)) {
                //test si l'arête existe déjà
                fileCopy = file;
                while (!fileCopy.empty()) {
                    if (fileCopy.front().first == m && fileCopy.front().second == c)
                        break;
                    fileCopy.pop();
                }
                if (fileCopy.empty())
                    file.push(std::make_pair(i, a));
            }
        }
        // face opposée à a+2
        m = this->faces[i].facesAdjacentes[(aDansi+2) % 3];
        if (m != -1) {
            int bDansm = VertexIndexOnFace(this->vertices[b], this->faces[m]);
            c = this->faces[m].indices[(bDansm+2) % 3];
            if (cercleI.isInCercle(this->vertices[c].position)) {
                //test si l'arête existe déjà
                fileCopy = file;
                while (!fileCopy.empty()) {
                    if (fileCopy.front().first == m && fileCopy.front().second == c)
                        break;
                    fileCopy.pop();
                }
                if (fileCopy.empty())
                    file.push(std::make_pair(i, this->faces[i].indices[(aDansi+2) % 3]));
            }
        }
        
        int aDansj = VertexIndexOnFace(this->vertices[a], this->faces[j]);
        
        //cercle de la face j
        Cercle cercleJ(this->faces[j], *this);
        //face opposée à a+1
        int n = this->faces[j].facesAdjacentes[(aDansj+1) % 3];
        //sommet de l'arête
        unsigned d = this->faces[j].indices[(aDansj+2) % 3];
        if (n != -1) {
            int dDansn = VertexIndexOnFace(this->vertices[d], this->faces[n]);
            c = this->faces[n].indices[(dDansn+1) % 3];
            if (cercleJ.isInCercle(this->vertices[c].position)) {
                //test si l'arête existe déjà
                fileCopy = file;
                while (!fileCopy.empty()) {
                    if (fileCopy.front().first == n && fileCopy.front().second == c)
                        break;
                    fileCopy.pop();
                }
                if (fileCopy.empty())
                    file.push(std::make_pair(j, this->faces[j].indices[(aDansj+1) % 3]));
            }
        }
        // face opposée à a
        n = this->faces[j].facesAdjacentes[aDansj];
        if (n != -1) {
            int dDansn = VertexIndexOnFace(this->vertices[d], this->faces[n]);
            c = this->faces[n].indices[(dDansn+2) % 3];
            if (cercleJ.isInCercle(this->vertices[c].position)) {
                //test si l'arête existe déjà
                fileCopy = file;
                while (!fileCopy.empty()) {
                    if (fileCopy.front().first == n && fileCopy.front().second == c)
                        break;
                    fileCopy.pop();
                }
                if (fileCopy.empty())
                    file.push(std::make_pair(j, a));
            }
        }
    }
    return;
}

void Mesh::lawsonAll () {
    lawsonQueue(findAretesToFlip());
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
        faces[j].facesAdjacentes[(aDansj + 2) % 3] = m;
    }
    
    if (k != -1) {
        int aDansk = VertexIndexOnFace(vertices[a], faces[k]);
        faces[k].facesAdjacentes[(aDansk + 1) % 3] = m+1;
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

void Mesh::split_face_delaunay_incr (int i, unsigned p) {
    
    std::queue<std::pair<int,unsigned>> file;
    //face / vertex
    
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
    int l = face_i.facesAdjacentes[0];
    int m = (int) faces.size();
    int n = m+1;
    
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
    faces[m].facesAdjacentes[1] = n;
    faces[m].facesAdjacentes[2] = i;
    
    faces[m+1].facesAdjacentes[0] = k;
    faces[m+1].facesAdjacentes[1] = i;
    faces[m+1].facesAdjacentes[2] = m;
    
    faces[i].indices[0] = p;
    faces[i].facesAdjacentes[1] = m;
    faces[i].facesAdjacentes[2] = n;
    
    if (j != -1) {
        int aDansj = VertexIndexOnFace(vertices[a], faces[j]);
        faces[j].facesAdjacentes[(aDansj + 2) % 3] = m;
        
        std::cout <<"face j :"<< j << std::endl;
        
        Cercle cercleM(this->faces[m], *this);
        if (cercleM.isInCercle(this->vertices[this->faces[j].indices[(aDansj + 2) % 3]].position)) {
            file.push(std::make_pair(m, p));
            std::cout <<"face m" << std::endl;
        }
    }
    
    if (k != -1) {
        int aDansk = VertexIndexOnFace(vertices[a], faces[k]);
        faces[k].facesAdjacentes[(aDansk + 1) % 3] = m+1;
        
        std::cout <<"face k : "<< k << std::endl;
        
        Cercle cercleN(this->faces[n], *this);
        if (cercleN.isInCercle(this->vertices[this->faces[k].indices[(aDansk + 1) % 3]].position)) {
            file.push(std::make_pair(n, p));
            std::cout <<"face n" << std::endl;
        }
    }
    
    if (l != -1) {
        int bDansl = VertexIndexOnFace(vertices[b], faces[l]);
        std::cout <<"face l : "<< l << std::endl;
        
        Cercle cercleI(this->faces[i], *this);
        if (cercleI.isInCercle(this->vertices[this->faces[l].indices[(bDansl + 1) % 3]].position)) {
            file.push(std::make_pair(i, p));
            std::cout <<"face i" << std::endl;
        }
    }
    
    //contours
    for (auto it = contours.begin(); it != contours.end(); ++it) {
        if (it->aretes.first == faces[i].indices[1] && it->aretes.second == faces[i].indices[2])
            it->faces = i;
        else if (it->aretes.first == faces[m].indices[1] && it->aretes.second == faces[m].indices[2])
            it->faces = m;
        else if (it->aretes.first == faces[n].indices[1] && it->aretes.second == faces[n].indices[2])
            it->faces = n;
    }
    
    if(file.size()) {
        std::cout <<"lawsonQ" << std::endl;
        lawsonQueue(file);
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
    int bDansj = VertexIndexOnFace(vertices[faces[i].indices[(aDansi+1) % 3]], faces[j]);
    
    // faces externes
    int m = faces[j].facesAdjacentes[(bDansj+2) % 3];
    int n = faces[i].facesAdjacentes[(aDansi+1) % 3];

    // sommets
    unsigned b = faces[j].indices[bDansj];
    unsigned c = faces[j].indices[(bDansj+1) % 3];
    unsigned d = faces[j].indices[(bDansj+2) % 3];
    
    // modifications faces adjacentes externes
    if (m != -1) {
        int bDansm = VertexIndexOnFace(vertices[faces[j].indices[(bDansj)]], faces[m]);
        faces[m].facesAdjacentes[(bDansm+1) % 3] = i;
    } else {
        auto it = contours.begin();
        while (it != contours.end()) {
            if (it->faces == j && (
               (it->aretes.first == b && it->aretes.second == c) ||
               (it->aretes.first == c && it->aretes.second == b))) {
                it->faces = i;
                break;
            }
            it++;
        }
    }
    if (n!= -1) {
        int aDansn = VertexIndexOnFace(vertices[faces[i].indices[(aDansi)]], faces[n]);
        faces[n].facesAdjacentes[(aDansn+2) % 3] = j;
    } else {
        auto it = contours.begin();
        while (it != contours.end()) {
            if (it->faces == i && (
               (it->aretes.first == a && it->aretes.second == d) ||
               (it->aretes.first == d && it->aretes.second == a))) {
                it->faces = j;
                break;
            }
            it++;
        }
    }
    
    // modifications sommets
    faces[i].indices[(aDansi+2) % 3] = faces[j].indices[(bDansj+1) % 3];
    faces[j].indices[bDansj] = a;
   
    // modifications faces incidentes
    vertices[b].faceIncidente = i;
    vertices[d].faceIncidente = j;
    
    // modifications faces adjacentes
    faces[i].facesAdjacentes[(aDansi+1) % 3] = j;
    faces[j].facesAdjacentes[(bDansj+2) % 3] = i;
    
    faces[i].facesAdjacentes[aDansi] = m;
    faces[j].facesAdjacentes[(bDansj+1) % 3] = n;
    
}
