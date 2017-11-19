#ifndef Mesh_H
#define Mesh_H
#include <string>
#include <vector>
#include <list>
#include <queue>
#include "Face.h"
#include "Vertex.h"
#include "Shape.h"

/// Mesh
// classe représentant un maillage
//
// vector<Vertex> vertices : vecteur des sommets du maillage
// vector<Face> faces : vecteur des faces du maillage
// list<Contours> contours : liste contenant le contour du maillage = enveloppe convexe
///
// METHODES
//
// privées:
//
// • int VertexIndex(const int f, const int i) const
//      renvoie l'indice dans le mesh d'un sommet décrit par son indice dans une face.
//
// • int NextVertexIndex(const int f, const int i) const;
//      renvoie l'indice dans le mesh du sommet suivant le sommet décrit par son indice dans une face dans l'ordre trigonométrique
//
// • int PrevVertexIndex(const int f, const int i) const;
//      renvoie l'indice dans le mesh du sommet précédant le sommet décrit par son indice dans une face dans l'ordre trigonométrique
//
// • int VertexIndexOnFace(const Vertex& vertex, const Face& face) const :
//      renvoie l'indice du sommet dans dans la face considérée (pour une triangulation,
//      le nombre sera entre 0 et 2 compris).
//
// • int faceFromPair(const Vertex& v1, const Vertex& v2) const :
//      renvoie l'indice de la face qui contient l'arête décrite par les deux sommets, dans l'ordre donné.
//
// • int locatePointInFace(const Vertex v) :
//      effectue une marche depuis la face d'indice 0 pour trouver si le point est dans une face ou à l'extérieur.
//      On renvoie alors l'indice de la face ou -1 si c'est dehors
//
// • bool is_trigo (Vertex a, Vertex b, Vertex c) :
//      teste si le triangle défini par les sommets a, b et c est trigonométrique.
//
// • bool is_in_triangle (Face f, Vertex v) :
//      teste si le sommet est dans le triangle f.
//
// • void split_face (int face, unsigned p) :
//      partage la face en 3 (2 nouvelles + l'ancienne) avec le nouveau sommet p, commun aux trois.
//      modifie toutes les faces adjacentes / indices des faces / faces incidentes
//      modifie le contour si la face à split a une arête sur le contour
//
// • void split_face_delaunay_incr (int face, unsigned p) :
//      mêmes propriétés que split_face.
//      crée une liste des arêtes à flip si pas de delaunay localement
//      appelle la fonction lawsonQueue si la file n'est pas vide.
//
// • void flip (int face, unsigned oppArete) :
//      flip l'arête définie par l'indice de la face + l'indice du sommet opposé à l'arête.
//      modifie les indices des faces, les faces adjacentes, les faces incidentes des sommets
//      et le contour si nécessaire.
//
// publiques :
//
// • void ReadFromOFF(const string & path) :
//      Lit un fichier OFF et construit un Mesh avec.
//
// • void ReadFromPoints(const string & path) :
//      Lit un fichier POINTS et construit un Mesh avec.
//
// • const vector<Vertex> &_Vertices() const
// • const vector<Face> &_Faces() const
// • const list<unsigned> &_Contours() const
//      Ces trois méthodes permettent de récupérer les vecteurs de sommets, faces et contours sans les modifier.
//
// • void inserer_sommet (Vertex v) :
//      ajoute le sommet dans le vecteur de sommets du Mesh.
//      s'il se situe dans une face, split la face
//      s'il se situe à l'extérieur, parcourt du contour, ajout de faces et modifications du contour.
//
// • void inserer_sommet_delaunay_incr(Vertex v) :
//      mêmes propriétés qu'inserer_sommet.
//      Appelle split_face_delaunay_incr à la place de split_face
//      si le sommet est à l'exterieur, teste le caractère de Delauay localement pour chaque nouvelle arête crée
//      et remplie une file d'arêtes à flipper
//      appelle la fonction lawsonQueue si la file n'est pas vide.
//
// • void lawsonAll () :
//      Appelle lawsonQueue sur la liste de toutes les arêtes du maillage trouvées avec findAretesToFlip();
//
// • void lawsonQueue(queue<pair<int,unsigned>> file) :
//      Regarde la file d'arête à flip, vérifie qu'elles sont bien non de Delaunay localement,
//      flip l'arête
//      teste si les nouvelles arêtes sont à fliper ou pas (et si oui, si elles existent déjà dans la file) puis les rajoute dans la file
//      recommence tant que la file n'est pas vide.
//
// • queue<pair<int,unsigned>> findAretesToFlip() :
//      Parcourt toute les faces du Mesh et ajoute les arêtes non delaunay dans la file.
//      Pour chaque ajout: vérifie que l'arête n'existe pas déjà.
//
// • Shape crust() :
//      Renvoie la croute du Mesh (un Shape)
//      Popule un vecteur de vertex des centres de voronoï de toutes les faces du Mesh
//      Insère ces centres dans le Mesh
//      Parcourt tous les sommets du Mesh entre le premier et le dernier avant insertion des centres de voronoï.
//      Pour chaque sommet, cherche dans ses sommets voisins : si c'est un sommet avant insertion, et si l'indice est < au sommet courant,
//      ajout dans la Shape. (sinon l'arête sera ajouté lors de son passage plus tard).
//
// • Iterateur_de_faces faces_debut() :
// • Iterateur_de_faces faces_fin() :
//      génère un itérateur de face pointant au début / à la fin du vector de faces du Mesh
//
// • Iterateur_de_sommets sommets_debut() :
// • Iterateur_de_sommets sommets_fin() :
//      génère un itérateur de sommets pointant au début / à la fin du vector de sommets du Mesh

class Mesh;

/// Iterateur_de_faces
// classe d'itérateur parcourant l'ensemble des faces d’une triangulation
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

/// Iterateur_de_sommets
// classe d'itérateur parcourant l'ensemble des sommets d’une triangulation
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

/// Circulateur_de_faces
// classe de circulateur permettant de tourner sur les faces autour d'un sommet, dans l'ordre trigonométrique,
// en partant de la face incidente du sommet.
class Circulateur_de_faces {
private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Vertex> sommet_parent; //sommet autour duquel on tourne

public:
    int face_courante; //face courante
    int premiere_face; //premiere face
    Circulateur_de_faces ();
    Circulateur_de_faces (const Mesh& mesh, const Vertex& v);    // constructor
    Circulateur_de_faces (const Circulateur_de_faces &); // copy constructor
    
    Circulateur_de_faces& operator=(const Circulateur_de_faces& other);  // assignment
    Circulateur_de_faces& operator++(); //pre-increment
    Circulateur_de_faces& operator--(); //pre-decrement
    int operator*() const; //dereference
    
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

/// Circulateur_de_sommets
// classe de circulateur permettant de tourner, sommet par sommet autour d'un sommet, dans l'ordre trigonométrique
class Circulateur_de_sommets {
private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Vertex> sommet_parent; //sommet autour duquel on tourne
    unsigned sommet_courant; //sommet courant
    unsigned face_courante; //sommet courant

public:
    unsigned premier_sommet; //premier sommet
    Circulateur_de_sommets ();
    Circulateur_de_sommets (const Mesh& mesh_associe, const Vertex& v); // constructor
    Circulateur_de_sommets (const Circulateur_de_sommets &); // copy constructor
    
    Circulateur_de_sommets& operator=(const 	Circulateur_de_sommets& other);  // assignment
    Circulateur_de_sommets& operator++(); //pre-increment
    unsigned operator*() const; //dereference
    
    friend bool operator== (const Circulateur_de_sommets& lhs, const Circulateur_de_sommets& rhs)
    {
        return lhs.mesh == rhs.mesh
        && lhs.sommet_parent == rhs.sommet_parent
        && lhs.sommet_courant == rhs.sommet_courant
        && lhs.premier_sommet == rhs.premier_sommet;
    }
    
    friend bool operator!= (const Circulateur_de_sommets& lhs, const Circulateur_de_sommets& rhs) {
        return !( lhs == rhs );
    }
};

struct Contours {
    std::pair<unsigned, unsigned> aretes;
    unsigned faces;
};

class Mesh {
public:
    void ReadFromOFF(const std::string & path);
    void ReadFromPoints(const std::string & path);
    
    const std::vector<Vertex> &_Vertices() const;
    const std::vector<Face> &_Faces() const;
    const std::list<unsigned> &_Contours() const;

    void inserer_sommet(Vertex v);
    void inserer_sommet_delaunay_incr(Vertex v);
    
    void lawsonAll ();
    void lawsonQueue(std::queue<std::pair<int,unsigned>> file);
    std::queue<std::pair<int,unsigned>> findAretesToFlip();

    Shape crust();
    
    Iterateur_de_faces faces_debut();
    Iterateur_de_faces faces_fin();
    
    Iterateur_de_sommets sommets_debut();
    Iterateur_de_sommets sommets_fin();
    
    Circulateur_de_faces faces_incidentes(const Vertex& v) const;
    Circulateur_de_sommets sommets_adjacents(const Vertex& v) const;
    
private:
    std::vector<Vertex> vertices; /// Sommets du maillage
    std::vector<Face> faces; /// Faces du maillage
    std::list<Contours> contours; ///  contour
    
    int VertexIndex(const int f, const int i) const;
    int NextVertexIndex(const int f, const int i) const;
    int PrevVertexIndex(const int f, const int i) const;
    
    int VertexIndexOnFace(const Vertex& vertex, const Face& face) const;
    int faceFromPair(const Vertex& v1, const Vertex& v2) const;
    int locatePointInFace(const Vertex v);
    
    bool is_trigo (Vertex a, Vertex b, Vertex c);
    bool is_in_triangle (Face f, Vertex v);
    
    void split_face (int face, unsigned p);
    void split_face_delaunay_incr (int face, unsigned p);
    void flip (int face, unsigned oppVertex);
    
    friend class Circulateur_de_faces;
    friend class Circulateur_de_sommets;
};
#endif