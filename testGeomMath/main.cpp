//
//  main.cpp
//  testGeomMath
//
//  Created by Crymsius on 08/09/2017.
//  Copyright © 2017 Crymsius. All rights reserved.
//

#include "Mesh.h"
#include <iostream>

int main(void) {
    Mesh mesh;
    Iterateur_de_sommets its = mesh.sommets_debut();
    Circulateur_de_faces cf;
    
//    Circulateur_de_faces cf(mesh, mesh._Vertices()[0]);
//    mesh.ReadFromOFF("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/test.off");
      mesh.ReadFromOFF("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/cube.off");
//    mesh.ReadFromOFF("/Users/crymsius/Desktop/Boulot/Master/ID3D/MGA/TP/DisplayMesh/DisplayMesh/queen.off");

//    for (its = mesh.sommets_debut(); its != mesh.sommets_fin(); ++its) {
//        std::cout << (*its).position.x << " "<< (*its).position. y << " " << (*its).position.z << std::endl ;
//    }
    for (its = mesh.sommets_debut(); its != mesh.sommets_fin(); ++its) {
        Circulateur_de_faces cfdebut = mesh.faces_incidentes(*its) ;
        int cmpt=0 ;
        for (cf = cfdebut, ++cf; cf != cfdebut; ++cf)
            cmpt++ ;
        std ::cout<< "valence du sommet" << cmpt << std ::endl ;
    }
    
    return 0;
}