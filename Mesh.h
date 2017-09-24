#ifndef Mesh_H
#define Mesh_H
#include <string>
#include <vector>
#include "Face.h"
#include "Vertex.h"

class Mesh;

///itère sur l’ensemble des faces d’une triangulation
class Iterateur_de_faces {
public:
    Iterateur_de_faces (std::vector<Face>::iterator pos);    // ctor
    Iterateur_de_faces (const Iterateur_de_faces &); // copy ctor
    
    Iterateur_de_faces& operator=(const Iterateur_de_faces& other);  // assignment
    Iterateur_de_faces& operator++(); //pre-increment
    Face& operator*() const; //dereference
    
    friend bool operator== (const Iterateur_de_faces& lhs, const Iterateur_de_faces& rhs) {
        return lhs.current_it == rhs.current_it;
    }
    friend bool operator!= (const Iterateur_de_faces& lhs, const Iterateur_de_faces& rhs) {
        return !( lhs == rhs );
    }
    
private:
    std::vector<Face>::iterator current_it; // the state
};

class Iterateur_de_sommets {
public:
    Iterateur_de_sommets (std::vector<Vertex>::iterator pos);    // ctor
    Iterateur_de_sommets (const Iterateur_de_sommets &); // copy ctor
    
    Iterateur_de_sommets& operator=(const Iterateur_de_sommets& other);  // assignment
    Iterateur_de_sommets& operator++(); //pre-increment
    Vertex& operator*() const; //dereference
    
    friend bool operator== (const Iterateur_de_sommets& lhs, const Iterateur_de_sommets& rhs) {
        return lhs.current_it == rhs.current_it;
    }
    friend bool operator!= (const Iterateur_de_sommets& lhs, const Iterateur_de_sommets& rhs) {
        return !( lhs == rhs );
    }
    
private:
    std::vector<Vertex>::iterator current_it; // the state
};

class Circulateur_de_faces {
private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Vertex> sommet_parent; //sommet autour duquel on tourne
    unsigned face_courante; //face courante
    unsigned premiere_face; //premiere face
    
    static Circulateur_de_faces end;
    

public:
    Circulateur_de_faces ();    
    Circulateur_de_faces (const Mesh& mesh, const Vertex& v);    // constructor
    Circulateur_de_faces (const Circulateur_de_faces &); // copy constructor

    static const Circulateur_de_faces& End();
    
    Circulateur_de_faces& operator=(const Circulateur_de_faces& other);  // assignment
    Circulateur_de_faces& operator++(); //pre-increment
    unsigned operator*() const; //dereference
    
    friend bool operator== (const Circulateur_de_faces& lhs, const Circulateur_de_faces& rhs)
    {
        return lhs.mesh == rhs.mesh
        && lhs.sommet_parent == rhs.sommet_parent
        && lhs.face_courante == rhs.face_courante
        && lhs.premiere_face == rhs.premiere_face;
    }
    
    friend bool operator!= (const Circulateur_de_faces& lhs, const Circulateur_de_faces& rhs) {
        return !( lhs == rhs );
    }
};

//class Circulateur_de_sommets {
//public:
//    Circulateur_de_sommets (std::vector<Vertex>::iterator pos, Vertex vparent, unsigned vcourant, Mesh& mesh_associe);    // ctor
//    Circulateur_de_sommets (const Circulateur_de_sommets &); // copy ctor
//    
//    Circulateur_de_sommets& operator=(const 	Circulateur_de_sommets& other);  // assignment
//    Circulateur_de_sommets operator++(); //pre-increment
//    Vertex& operator*() const; //dereference
//    
//private:
//    std::vector<Vertex>::iterator current_it; // the state
//    Vertex sommet_parent; //face autour delaquelle on tourne
//    unsigned sommet_courant; //face courante
//    Mesh& mesh;
//};

class Mesh {
public:
    void ReadFromOFF(std::string path);
    const std::vector<Vertex> &_Vertices() const;
    const std::vector<Face> &_Faces() const;
    
    Iterateur_de_faces faces_debut();
    Iterateur_de_faces faces_fin();
    
    Iterateur_de_sommets sommets_debut();
    Iterateur_de_sommets sommets_fin();
    
    Circulateur_de_faces faces_incidentes(const Vertex& v) const;
//    Circulateur_de_sommets sommets_adjacents(Vertex & v);
    
private:
    friend class Circulateur_de_faces;
    std::vector<Vertex> vertices; /// Sommets du maillage
    std::vector<Face> faces; /// Faces du maillage

    int VertexIndexOnFace(const Vertex& vertex, const Face& face) const;
};
#endif