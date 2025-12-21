#pragma once

#include <algorithm>
#include <array>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "internal/generic/Constants.h"
#include "internal/generic/Index.h"
#include "internal/generic/Limits.h"
#include "internal/generic/Numerics.h"
#include "internal/generic/Scalar.h"
#include "internal/generic/StackAllocator.h"

#include "internal/geometry/BoundingObject.h"
#include "internal/geometry/Contour2D.h"
#include "internal/geometry/Contour3D.h"
#include "internal/geometry/Edge.h"
#include "internal/geometry/Edges.h"
#include "internal/geometry/Face.h"
#include "internal/geometry/Faces.h"
#include "internal/geometry/FaceVisitor.h"
#include "internal/geometry/FacesVisitor.h"
#include "internal/geometry/Normal.h"
#include "internal/geometry/Normals.h"
#include "internal/geometry/Point.h"
#include "internal/geometry/Points.h"
#include "internal/geometry/Shape.h"
#include "internal/geometry/ShapeFactory.h"
#include "internal/geometry/Transformation.h"
#include "internal/geometry/Vertex.h"
#include "internal/geometry/Vertices.h"

#include "internal/utilities/svg.h"
