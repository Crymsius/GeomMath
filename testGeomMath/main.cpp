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
    Circulateur_de_faces cf;
    
//    Circulateur_de_faces cf(mesh, mesh._Vertices()[0]);
//    mesh.ReadFromOFF("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/test.off");
    mesh.ReadFromPoints("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/test.points");
//      mesh.ReadFromOFF("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/cube.off");
//    mesh.ReadFromOFF("/Users/crymsius/Desktop/Boulot/Master/ID3D/MGA/TP/DisplayMesh/DisplayMesh/queen.off");

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
    
    return 0;
}