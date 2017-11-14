//
//  main.cpp
//  testGeomMath
//
//  Created by Crymsius on 08/09/2017.
//  Copyright © 2017 Crymsius. All rights reserved.
//

#include "Mesh.h"
#include <iostream>
#include "Logger.h"

int main(void) {
    Mesh mesh;
//    Iterateur_de_sommets its = mesh.sommets_debut();
//    Circulateur_de_faces cf;
    mesh.ReadFromPoints("test.points");
//    mesh.ReadFromPoints("test_crust/line4.points");
//    Vertex v = mesh._Vertices()[0];
    
//    Circulateur_de_faces cf = mesh.faces_incidentes(v);
//    mesh.ReadFromOFF("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/test.off");
//      mesh.ReadFromOFF("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/cube.off");
//    mesh.ReadFromOFF("/Users/crymsius/Desktop/Boulot/Master/ID3D/MGA/TP/DisplayMesh/DisplayMesh/queen.off");

//    ++cf;
//    while(*cf != cf.premiere_face) {
//        std::cout << *cf << std::endl;
//        ++cf;
//    }
    
//    for (its = mesh.sommets_debut(); its != mesh.sommets_fin(); ++its) {
//        std::cout << (*its).position.x << " "<< (*its).position. y << " " << (*its).position.z << std::endl ;
//    }
//    for (its = mesh.sommets_debut(); its != mesh.sommets_fin(); ++its) {
//        Circulateur_de_faces cfdebut = mesh.faces_incidentes(*its) ;
//        int cmpt=0 ;
//        for (cf = cfdebut, ++cf; cf != cfdebut; ++cf)
//            cmpt++ ;
//        std ::cout<< "valence du sommet " << (*its).position.x << " "<< (*its).position. y << " " << (*its).position.z << " : " << cmpt << std ::endl ;
//    }
//    Vertex v;
//    v.position = Point(0.25f, 0.5f, 0.0f);
//    
//    mesh.inserer_sommet(v);
    
//    const Vertex v = mesh._Vertices()[mesh._Vertices().size() -1];
//#ifdef DEBUG
//    
//    auto t1 = std::chrono::high_resolution_clock::now();
//#endif
//    auto circulator = mesh.faces_incidentes(v);
//#ifdef DEBUG
//    
//    auto t2 = std::chrono::high_resolution_clock::now();
//    
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//    olog(Info) << "Trouver l'indice du sommet sur la face incidente prend " << duration;
//#endif

    
    mesh.lawsonAll();
    
    Vertex v;
    v.position.x = - 0.365625;
    v.position.y = 0.909375;
    v.position.z = 0.;
    
    std::cout <<"avant insertion" << std::endl;
    
    mesh.inserer_sommet_delaunay_incr(v);
    
//    Shape shape;
//    shape = mesh.crust();

    std::cout <<"fini" << std::endl;
    return 0;
}