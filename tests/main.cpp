#include "hanim.h"

struct MyTest : public hanim::Scene {
    virtual void timeline() override {
        using namespace hanim;

        auto rect = Rectangle({1, 1, 0}, {2, 2, 0})
            .fill_color({0, 1, 0, 1})
            .opacity(0.3)
            .stroke_color({1, 1, 0, 0.5});
/*
        this->play(new Rotate(rect), 60);
        this->play(new Shift(rect));
        this->play(new Scale(rect), 90);
        this->play(new Shift(rect, {-1, -1, 0}), 60);
    */
        //this->play(new Scale(rect, 1), 60);
        //this->play(new Rotate(rect, -90));
        //this->play(new Thickness(rect, 3));
        /*
        this->play(new Colors(rect, {
            {1, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1, 1},
            {1, 1, 1, 1},
        }), 45);
        
        //this->play(new Opacity(rect, 0.1));
        //this->play(new Opacity(rect, 0.5));
        this->play(new Create(
            Rectangle({1, 0, 0}, {3, 3, 0}).fill_color({0, 1, 0, 0.2})
        ));

        play(new Create(
            Triangle({-2, -2, 0}, {-2, 1, 0}, {0, 0, 0})
                .fill_color({1, 0, 0, 0.4})
                .color({1, 1, 0, 1})
        ));
        */
        //play(new DrawBorder(rect));

        auto triangle = Triangle({-2, 1, 0}, {0, 0, 0}, {-2, -2, 0})
            .fill_color({1, 1, 0, 0.2})
            .color({1, 1, 0, 1});

        //play(new DrawBorder(rect));
        //play(new Scale(rect));
        auto line = Line({0, -1, 0}, {2, -1, 0})
            .color({0, 1, 0, 1});
/*
        play(new Create(rect));
        play(new Opacity(rect, 1));
        play(new Opacity(rect, 0));
        play(new Transform(rect, triangle));
        play(new Transform(triangle, line));
        play(new Create(line));
*/
/*
        play(new DrawBorder(rect));
        play(DrawBorder(triangle));
        play(Transform(rect, line));
        play(Transform(triangle, line));
*/

        //play(DrawBorder(rect));
        //add(line);
/*
        auto a1 = Scale(triangle);
        auto a2 = Scale(rect);
        play(Scale(rect));
        play(Scale(triangle));
        play(a2); play(a1);

        play(Transform(triangle, line));
        play(Transform(rect, line));
*/
/*
        play(DrawBorder(line));
        play(DrawBorder(triangle));
        play(DrawBorder(rect));

        play(CAnimGroup(
            DrawBorder(line),
            DrawBorder(triangle),
            DrawBorder(rect)
        ));

        CAnimGroup anims = CAnimGroup(
            Transform(rect, line),
            Transform(triangle, line)
        );
        anims.add(Opacity(line, 0));

        play(std::move(anims));
*/

/*
        auto t = DrawBorder(line);
        play(t);
        auto tt = Transform(line, triangle);
        play(tt);
        play(Transform(triangle, rect));
        play(Opacity(line, 0));
        play(Transform(rect, line));
        play(tt);
*/
/*
        play(CAnimGroup(
            DrawBorder(line),
            DrawBorder(triangle),
            DrawBorder(rect)
        ));

        CAnimGroup anims = CAnimGroup(
            Transform(rect, line),
            Transform(triangle, line)
        );

        auto t = CAnimGroup(anims, Scale(line), anims);

        play(t);
*/

        auto circle = Circle(vec3(0), 1, 64); //.fill_color({0, 1, 0, 0.2});
        //play(DrawBorder(rect));
        //play(Transform(rect, circle));
        //play(Transform(circle, line));
/*
        play(DrawBorder(circle));
        play(Shift(circle, {-1, 0, 0}));
        circle.scale(2);
        rect.scale(2);
        play(Transform(circle, rect));
        play(Transform(rect, line));
*/
/*
        auto rect2 = rect.clone();
        rect2.shift({2, 0,  0});
        play(CAnimGroup(DrawBorder(rect2)));
        auto group = CAnimGroup(
            Create(line),
            Create(rect),
            Create(circle)
        );
        play(group);
*/
/*
        auto objGroup = HObject()
            .add(rect)
            .add(line)
            .add(circle);

        //play(Scale(objGroup, 2));
        //play(Shift(objGroup), 30);
        //play(DrawBorder(objGroup));
        play(Create(rect));
        play(Shift(rect, {2, 0, 0}));
        auto rect2 = rect;
        play(Transform(rect2, objGroup));

        auto ag = CAnimGroup();

        ag.setFrameNumber(120);
        ag.add(Opacity(rect, 0).setFrameNumber(30), 60);
        ag.add(Opacity(rect2, 0), 0);

        play(ag);
*/
/*
        auto grid = Grid();
        auto objs = HObject().add(rect).add(line).add(circle);
        play(Create(objs));
        play(CameraScale(2));
        play(Rotate(objs));
*/

        auto objs = HObject().add(rect).add(line).add(circle);

        play(Create(objs));
/*
        play(HAnimGroup(
            Opacity(objs[0], 0),
            Opacity(objs[1], 0),
            Opacity(objs[2], 0)
        ));
*/
        play(Opacity(objs, 0));
    }

};

int main() {
    auto test = MyTest();
    hanim::HEngine::default_config1();
    hanim::HEngine::recorder_file_name("Hanim-Test");
    //hanim::HEngine::recorder_lossless(true);
    //hanim::HEngine::set_window(true);
    hanim::HEngine::render(test);
    hanim::HEngine::save_frame_to_img("hanim-img");
    return 0;
}