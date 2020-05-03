/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - opencv_video_capture.cpp                                *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2017 Bobby Smiles                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "opencv2/core/version.hpp"

#if CV_MAJOR_VERSION >= 3
/* this is for opencv >= 3.0 (new style headers + videoio/highgui split) */
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#elif CV_MAJOR_VERSION >= 2
/* otherwise go the safe way and let opencv include all its headers */
#include <opencv2/opencv.hpp>
#else
#error "Unsupported version of OpenCV"
#endif

#include <string>

struct opencv_video_capture
{
    unsigned int width;
    unsigned int height;

    std::string device;
    cv::VideoCapture cap;
};


extern "C"
{

#include "backends/api/video_capture_backend.h"

#define M64P_CORE_PROTOTYPES 1
#include "api/callbacks.h"
#include "api/m64p_config.h"
#include "api/m64p_types.h"
#include "main/util.h"

extern "C" const struct video_capture_backend_interface g_iopencv_video_capture_backend;



static m64p_error opencv_init_full(struct opencv_video_capture** cv, const char* dev)
{
    try {
        std::string device;
        if (dev != NULL) {
            device = dev;
        }

        *cv = new opencv_video_capture();

        (*cv)->width = 0;
        (*cv)->height = 0;
        (*cv)->device = std::move(device);

        return M64ERR_SUCCESS;
    }
    /* C++ exception must not cross C-API boundaries */
    catch(...) { return M64ERR_INTERNAL; }
}

static m64p_error opencv_init(void** vcap, const char* section)
{
    try {
        /* default parameters */
        const char* device = "0";

        if (section && strlen(section) > 0) {
            m64p_handle config = NULL;

            ConfigOpenSection(section, &config);

            /* set default parameters */
            ConfigSetDefaultString(config, "device", device, "Device to use for capture or \"0\" for default.");

            /* get parameters */
            device = ConfigGetParamString(config, "device");
        }

        return opencv_init_full(reinterpret_cast<struct opencv_video_capture**>(vcap), device);
    }
    /* C++ exception must not cross C-API boundaries */
    catch(...) { return M64ERR_INTERNAL; }
}

static void opencv_release(void* vcap)
{
    try {
        struct opencv_video_capture* cv = static_cast<struct opencv_video_capture*>(vcap);
        if (cv == NULL) {
            return;
        }

        delete cv;
    }
    /* C++ exception must not cross C-API boundaries */
    catch(...) { return; }
}

static m64p_error opencv_open(void* vcap, unsigned int width, unsigned int height)
{
    try {
        int dev_num;
        struct opencv_video_capture* cv = static_cast<struct opencv_video_capture*>(vcap);

        /* open device (we support both device number or path */
        if (string_to_int(cv->device.c_str(), &dev_num)) {
            cv->cap.open(dev_num);
        }
        else {
            cv->cap.open(cv->device);
        }

        if (!cv->cap.isOpened()) {
            DebugMessage(M64MSG_ERROR, "Failed to open video device %s", cv->device.c_str());
            return M64ERR_SYSTEM_FAIL;
        }

        /* TODO: adapt capture resolution to the desired resolution */

        DebugMessage(M64MSG_INFO, "Video successfully opened: %s", cv->device.c_str());

        cv->width = width;
        cv->height = height;

        return M64ERR_SUCCESS;
    }
    /* C++ exception must not cross C-API boundaries */
    catch(...) { return M64ERR_INTERNAL; }
}

static void opencv_close(void* vcap)
{
    try {
        struct opencv_video_capture* cv = static_cast<struct opencv_video_capture*>(vcap);

        cv->cap.release();

        DebugMessage(M64MSG_INFO, "Video closed");
    }
    /* C++ exception must not cross C-API boundaries */
    catch(...) { return; }
}

static m64p_error opencv_grab_image(void* vcap, void* data)
{
    try {
        struct opencv_video_capture* cv = static_cast<struct opencv_video_capture*>(vcap);

        /* read next frame */
        cv::Mat frame;
        if (!cv->cap.read(frame)) {
            DebugMessage(M64MSG_ERROR, "Failed to grab frame !");
            return M64ERR_SYSTEM_FAIL;
        }

        /* resize image to desired resolution */
        cv::Mat output = cv::Mat(cv->height, cv->width, CV_8UC3, data);
        cv::resize(frame, output, output.size(), 0, 0, cv::INTER_AREA);

        return M64ERR_SUCCESS;
    }
    /* C++ exception must not cross C-API boundaries */
    catch(...) { return M64ERR_INTERNAL; }
}


#if 0
void cv_imshow(const char* name, unsigned int width, unsigned int height, int channels, void* data)
{
    try {
        int type = (channels == 1) ? CV_8UC1 : CV_8UC3;

        cv::Mat frame = cv::Mat(height, width, type, data);
        cv::imshow(name, frame);
        cv::waitKey(1);
    }
    /* C++ exception must not cross C-API boundaries */
    catch(...) { return; }
}
#endif


extern "C" const struct video_capture_backend_interface g_iopencv_video_capture_backend =
{
    "opencv",
    opencv_init,
    opencv_release,
    opencv_open,
    opencv_close,
    opencv_grab_image
};

}
