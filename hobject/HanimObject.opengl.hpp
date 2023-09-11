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

        glBegin(GL_QUADS);

        glVertex2f(_mX, _mY);
        glVertex2f(_mX + _mW, _mY);
        glVertex2f(_mX + _mW, _mY + _mH);
        glVertex2f(_mX, _mY + _mH);

        glEnd();

        glFlush();
    }
};

template <unsigned int W, unsigned int H>
class ImageView : public hanim::HObject {
public:
    ImageView() : __mBuff { 0 } { }
    ~ImageView() = default;

protected:
    void _frameHAnimate(int type, const FAFrame &frame) override {
        /*
        static int cnt = 0;
        printf("_frameHAnimate: %d, %d - %d, %d, %d\n", cnt++, type, frame.dataPtr->size(), frame.width, frame.height);
        */

        if (type == hanim::FrameAnim::RGBA) {
            for (int i = 0; i < frame.dataPtr->size() && i / 4 < __mBuff.size(); i += 4) {
                __mBuff[i / 4].rgba.r = (*(frame.dataPtr))[i + 0];
                __mBuff[i / 4].rgba.g = (*(frame.dataPtr))[i + 1];
                __mBuff[i / 4].rgba.b = (*(frame.dataPtr))[i + 2];
                __mBuff[i / 4].rgba.a = (*(frame.dataPtr))[i + 3];
            }
        }

    }

    void _render() override {
        // pixels
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                glBegin(GL_POINTS);
                    glColor4f(
                        __mBuff[y * W + x].rgba.r / 255.f,
                        __mBuff[y * W + x].rgba.g / 255.f,
                        __mBuff[y * W + x].rgba.b / 255.f,
                        __mBuff[y * W + x].rgba.a / 255.f
                    );
                    glVertex2i(x, y);
                glEnd();
            }
        }
    }
private:
    std::array<hanim::FrameAnim::Pixel, W * H> __mBuff;
};

} // opengl
} // object
} // hanim
#endif