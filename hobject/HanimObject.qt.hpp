// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __HANIM_OBJECT_QT_HPP__HANIM
#define __HANIM_OBJECT_QT_HPP__HANIM

#include <Hanim.hpp>

#include <QPushButton>

namespace hanim {
namespace object {
namespace qt {

class Button : public QPushButton, public hanim::HObject {
public:
    explicit Button(const QString& text, QWidget* parent = nullptr) :
        QPushButton(text, parent) { }
protected:
    virtual void _interpolationHAnimate(int type, const hanim::IAFrame &frame) override {
        auto rect = geometry();
        auto p = palette();
        switch (type) {
            case hanim::InterpolationAnim::SCALE:
                rect.setWidth(80 * frame.data[0]);
                rect.setHeight(80 * frame.data[0]);
                setGeometry(rect);
                break;
            case hanim::InterpolationAnim::SCALE_2:
                setFixedSize({frame.data[0], frame.data[1]});
                break;
            case hanim::InterpolationAnim::MOVE:
            case hanim::InterpolationAnim::PATH:
                this->move(frame.data[0], frame.data[1]);
                break;
            case hanim::InterpolationAnim::GRADIENT:
                p.setColor(
                    QPalette::Button,
                    {
                        frame.data[0],
                        frame.data[1],
                        frame.data[2]
                    }
                );
                setPalette(p);
                break;
            case hanim::InterpolationAnim::ALPHA:
                // TODO:
            default:
                break;
        }
    }
};

} // qt
} // object
} // hanim

#endif