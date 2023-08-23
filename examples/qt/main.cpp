// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#include <QApplication>
#include <QTimer>

#include <Hanim.hpp>
#include <HanimAnimate.hpp>

#define HOBJECT_QT
#include <HanimObject.hpp>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.resize(300, 300);

    auto button = hanim::object::qt::Button("move", &window);

    auto anim = hanim::move(100, 0, 100, 200);
    anim.setEasingCurve(hanim::EasingCurve::ECType::OUT_ELASTIC);
    anim.setPlayType(hanim::HAnimate::PlayType::Repeat);
    anim.setFrameNums(120);

    /*
        QTimer timer;
        timer.setInterval(1000 / 60);
        QObject::connect(&timer, &QTimer::timeout, [&]() {
            hanim::HEngine::PlayFrame(anim, button);
        });
        anim.start();
        timer.start();
    */

    hanim::HEngine::Play(anim, button);

    window.show();

    return app.exec();
}
