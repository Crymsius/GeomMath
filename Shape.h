//
//  Shape.h
//  GeomMath
//
//  Created by Crymsius on 13/11/2017.
//  Copyright © 2017 Crymsius. All rights reserved.
//

#ifndef Shape_h
#define Shape_h

#include <vector>
#include "Vertex.h"

struct Shape {
    std::vector<Vertex> vertices; /// Sommets du maillage
    std::vector<std::pair<unsigned, unsigned>> aretes;
};


#endif /* Shape_hp*/
