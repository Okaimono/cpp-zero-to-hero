#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "UIPanel.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Crosshair : public UIPanel {
    public:
        Crosshair(Texture& t);
};

#endif