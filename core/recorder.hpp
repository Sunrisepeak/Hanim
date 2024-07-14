#ifndef RECORDER_HPP_HANIM
#define RECORDER_HPP_HANIM

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

namespace hanim {

class Recorder {

public:
    Recorder() : mWidth { 0 }, mHeight { 0 }, mFPS { 0 } {
        mName = "hanim-recorder";
        mLoseless = false;
    }

public: // cofig

    Recorder & set_name(std::string name) {
        mName = name;
        return *this;
    }

    Recorder & set_fps(int fps) {
        mFPS = fps;
        return *this;
    }

    Recorder & set_size(int w, int h) {
        mWidth = w;
        mHeight = h;
        return *this;
    }

    Recorder & set_lossless(bool enable) {
        mLoseless = enable;
        return *this;
    }

public:
    void save_frame_to_img(std::string name) {
        static int cnt = 0;
        if (!mPixels.empty()) {
            cv::cvtColor(mPixels, mPixels, cv::COLOR_RGBA2BGRA);
            auto imgName = name + "." + std::to_string(cnt++) + ".png";
            cv::imwrite(imgName, mPixels);
            HONLY_LOGP("save to image %s", imgName.c_str());
        }
    }

public: // control
    void start() {
        HONLY_LOGP("recorder start...");
        if (mHeight <= 0 || mWidth <= 0) {
            HONLY_LOGE("w/h <= 0 - reset to 1920x1080");
            mPixels = cv::Mat(1920, 1080, CV_8UC4);
        } else {
            mPixels = cv::Mat(mHeight, mWidth, CV_8UC4);
        }

        if (mFPS == 0) {
            HONLY_LOGE("fps == 0 - reset to 60");
            mFPS = 60;
        }

        mVWriter.open(
            (mName + (mLoseless ? ".mkv" : ".mp4")).c_str(),
            mLoseless ? cv::VideoWriter::fourcc('H', 'F', 'Y', 'U') : // Huffman / mkv
            cv::VideoWriter::fourcc('a', 'v', 'c', '1'), // 43K H.264 / mp4
            //cv::VideoWriter::fourcc('H', '2', '6', '4'), 
            //cv::VideoWriter::fourcc('X', 'V', 'I', 'D'),
            //cv::VideoWriter::fourcc('m', 'p', '4', 'v'), // 300K
            mFPS,
            cv::Size(mWidth, mHeight),
            true
        );
    }

    void write(std::vector<unsigned char> &buff) {
        if (mHeight * mWidth * 4 > buff.size()) {
            HONLY_LOGE("buff size error\n");
            return;
        }
        std::memcpy(mPixels.data, buff.data(), buff.size());
        cv::flip(mPixels, mPixels, 0);
        cv::Mat cv_pixels;
        cv::cvtColor(mPixels, cv_pixels, cv::COLOR_RGBA2BGR);
        mVWriter << cv_pixels;
    }

    void repeat_write(int frameNumber) {
        if (mVWriter.isOpened()) {
            cv::Mat cv_pixels;
            cv::cvtColor(mPixels, cv_pixels, cv::COLOR_RGBA2BGR);
            for (int i = 0; i < frameNumber; i++) {
                mVWriter << cv_pixels;
            }
        }
    }

    void stop() {
        if (mVWriter.isOpened()) {
            HONLY_LOGP("save to video-%dx%d-%d-%s...",
                mWidth, mHeight, mFPS,
                mLoseless ? "lossless" : "compressed"
            );
            mVWriter.release();
        }
    }

private:
    int mWidth, mHeight, mFPS;
    std::string mName;
    cv::Mat mPixels;
    cv::VideoWriter mVWriter;
    bool mLoseless;
};

}

#endif