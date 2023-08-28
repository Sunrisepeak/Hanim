// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __HANIM_OBJECT_OPENGL_HPP__HANIM
#define __HANIM_OBJECT_OPENGL_HPP__HANIM

#include <Hanim.hpp>

#include <GL/gl.h>

namespace hanim {
namespace object {
namespace opengl {

class Button : public hanim::HObjectTemplate {
public:
    Button() : HObjectTemplate() {
        _mX = _mY = 0;
        _mW = _mH = 85;
        _mR = 153;
        _mG = _mB = 255;
        _mA = 255;
    }

protected: // interface impl
    void _render() override {

        glColor4f(_mR / 255, _mG / 255, _mB / 255, _mA / 255);
        //glColor3ui((int)_mR, (int)_mG, (int)_mB);

        glBegin(GL_QUADS);

        glVertex2f(_mX, _mY);
        glVertex2f(_mX + _mW, _mY);
        glVertex2f(_mX + _mW, _mY + _mH);
        glVertex2f(_mX, _mY + _mH);

        glEnd();

        glFlush();
    }
};

} // opengl
} // object
} // hanim
#endif