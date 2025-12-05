#pragma once

#include "internal/generic/Index.h"

class Edge
{
public:
    Index face;

    Index startVertex;
    Index endVertex;

    Index nextEdge;
    Index prevEdge;
    Index mirrorEdge;
};

