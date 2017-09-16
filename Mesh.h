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
    Iterateur_de_faces operator++(); //pre-increment
    Face& operator*() const; //dereference
    
private:
    std::vector<Face>::iterator current_it; // the state
};

class Iterateur_de_sommets {
public:
    Iterateur_de_sommets (std::vector<Vertex>::iterator pos);    // ctor
    Iterateur_de_sommets (const Iterateur_de_sommets &); // copy ctor
    
    Iterateur_de_sommets& operator=(const Iterateur_de_sommets& other);  // assignment
    Iterateur_de_sommets operator++(); //pre-increment
    Vertex& operator*() const; //dereference
    
private:
    std::vector<Vertex>::iterator current_it; // the state
};

class Circulateur_de_faces {
public:
    Circulateur_de_faces (std::vector<Face>::iterator pos, Vertex v, unsigned f, Mesh& mesh);    // ctor
    Circulateur_de_faces (const Circulateur_de_faces &); // copy ctor
    
    Circulateur_de_faces& operator=(const Circulateur_de_faces& other);  // assignment
    Circulateur_de_faces operator++(); //pre-increment
    Face& operator*() const; //dereference
    
private:
    std::vector<Face>::iterator current_it; // the state
    Vertex sommet_parent; //sommet autour duquel on tourne
    unsigned face_courante; //face courante
    Mesh& mesh;
    
};

class Circulateur_de_sommets {
public:
    Circulateur_de_sommets (std::vector<Vertex>::iterator pos, Vertex vparent, unsigned vcourant, Mesh& mesh_associe);    // ctor
    Circulateur_de_sommets (const Circulateur_de_sommets &); // copy ctor
    
    Circulateur_de_sommets& operator=(const 	Circulateur_de_sommets& other);  // assignment
    Circulateur_de_sommets operator++(); //pre-increment
    Vertex& operator*() const; //dereference
    
private:
    std::vector<Vertex>::iterator current_it; // the state
    Vertex sommet_parent; //face autour delaquelle on tourne
    unsigned sommet_courant; //face courante
    Mesh& mesh;
};

class Mesh {
public:
    void ReadFromOFF(std::string path);
    const std::vector<Vertex> &_Vertices() const;
    const std::vector<Face> &_Faces() const;
    //Box3 getBox3(void);
    
    std::string const& operator()() const {
        return _compute_fancy_name();    // Does some internal work
    }

    
    Iterateur_de_faces faces_debut();
    Iterateur_de_faces faces_fin();
    
    Iterateur_de_sommets sommets_debut();
    Iterateur_de_sommets sommets_fin();
    
    Circulateur_de_faces faces_incidentes(Vertex& v);
    Circulateur_de_sommets sommets_adjacents(Vertex & v);
    
    std::vector<Vertex> vertices; /// Sommets du maillage
    std::vector<Face> faces; /// Faces du maillage
private:
};
#endif