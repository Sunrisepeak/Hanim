
// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/XRecorder
//

#ifndef __OPENGL_RECORDER_HPP__XRECORDER
#define __OPENGL_RECORDER_HPP__XRECORDER

#include <string>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#include <opencv2/opencv.hpp>

namespace xrecorder {

template <uint32_t W, uint32_t H, uint8_t FPS = 60>
class OpenGLRecorder {
public:
    OpenGLRecorder(std::string fileName = "xrecorder", bool autoDetectFormat = false) : __mFlip { true }, __mViewport { 0 } {
        __mFileName = fileName == "" ? "xrecorder" : fileName;

        int fourcc = cv::VideoWriter::fourcc('X', '2', '6', '4'); // x264 is a opensource impl for H.264

        auto videoFileName = autoDetectFormat ? __mFileName : __mFileName + ".mkv";

        __mVideoWriter.open(videoFileName, fourcc, FPS, cv::Size(W, H), true);

        if (!__mVideoWriter.isOpened()) {
            std::cerr << "Error opening video file for output - " << videoFileName << std::endl;
            assert(false);
            return;
        }

        // Note: Mat(int rows/h, int cols/w, int type)
        __mFrame = cv::Mat(H, W, CV_8UC3); // H264 not support CV_8UC4

        if (W * H < 3840 * 2160) // default support 4K corp/scale
            __mBuff.resize(3840 * 2160 * 4, 0);
        else
            __mBuff.resize(W * H * 4, 0);
    }

    OpenGLRecorder(const OpenGLRecorder &) = delete;
    OpenGLRecorder & operator=(const OpenGLRecorder &) = delete;

    ~OpenGLRecorder() {
        // save file and release res
        __mVideoWriter.release();
    }

public: // basic info op
    void setFlipStatus(bool flip) {
        __mFlip = flip;
    }

    bool getFlipStatus() const {
        return __mFlip;
    }

    int getW() const {
        return W;
    }

    int getH() const {
        return H;
    }

    int getFPS() const {
        return FPS;
    }

    std::string getFileName() const {
        return __mFileName;
    }

public: // data process op

    void captureFrameData() {
        __captureGLBuff();
        __processBuffer();
    }

    void show() const {
        cv::namedWindow(__mFileName, cv::WINDOW_NORMAL);
        cv::imshow(__mFileName, __mFrame);
    }

    void saveToImg() const {
        static int cnt = 0;
        cv::imwrite(__mFileName + "." + std::to_string(cnt++) + ".jpg", __mFrame);
    }

    void saveToVideo() {
        __mVideoWriter.write(__mFrame);
    }

private:
    std::string __mFileName;
    bool __mFlip;
    GLint __mViewport[4];
    std::vector<GLubyte> __mBuff; // fixed DataBuff
    cv::Mat __mFrame;
    cv::VideoWriter __mVideoWriter;

    void __captureGLBuff() {

        glGetIntegerv(GL_VIEWPORT, __mViewport);

        assert(__mBuff.size() >= __mViewport[2] * __mViewport[3] * 4);

        glReadPixels(
            __mViewport[0], __mViewport[1], // x, y
            __mViewport[2], __mViewport[3], // w, h
            GL_RGBA, GL_UNSIGNED_BYTE, // Note: dont use GL_RGB
            __mBuff.data()
        );

    }

    // Note:
    //      Mat(int rows/h, int cols/w, int type)
    //      cv::Rect(x, y, width, height)
    void __processBuffer() {

        auto frame = cv::Mat(__mViewport[3], __mViewport[2], CV_8UC4, __mBuff.data());

        std::memcpy(frame.data, __mBuff.data(), __mViewport[2] * __mViewport[3] * 4);

        cv::cvtColor(frame, frame, cv::COLOR_RGBA2BGR);

        if (__mFlip) // Note: by lib-user to control
            cv::flip(frame, frame, 0);

        __mFrame.setTo(cv::Scalar(0));

        // adapt resolution
        if (frame.cols > W) {
            cv::resize(
                frame, frame, // src, dst
                cv::Size(W, W * 1.f / frame.cols * frame.rows),
                cv::INTER_LINEAR
            );
        }

        if (frame.rows > H) {
            cv::resize(
                frame, frame, // src, dst
                cv::Size(H * 1.f / frame.cols * frame.rows, H),
                cv::INTER_LINEAR
            );
        }

        // align process
        cv::Rect roi((W - frame.cols) * 1.f / 2, (H - frame.rows) * 1.f / 2, frame.cols, frame.rows);
        frame.copyTo(__mFrame(roi));
    }
};

} // xrecorder namespace end

#endif