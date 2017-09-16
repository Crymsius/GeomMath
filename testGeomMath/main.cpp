//
//  main.cpp
//  testGeomMath
//
//  Created by Crymsius on 08/09/2017.
//  Copyright © 2017 Crymsius. All rights reserved.
//

#include "Mesh.h"
int main(void) {
    Mesh mesh;
    //mesh.ReadFromOFF("/Users/crymsius/Documents/Xcode/GeomMath/testGeomMath/test.off");
    mesh.ReadFromOFF("/Users/crymsius/Desktop/Boulot/Master/ID3D/MGA/TP/DisplayMesh/DisplayMesh/queen.off");
    
    return 0;
}