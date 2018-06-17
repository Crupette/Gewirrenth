#ifndef UTIL_H
#define UTIL_H

#include <glm/glm.hpp>

glm::vec2 rotatePoint(float x, float y, float orX, float orY, float deg){
	return glm::vec2(
			cos(deg * x) - sin(deg * y),
			sin(deg * x) + cos(deg * y)
			);
}

#endif
