#pragma once
#ifndef _MOCHEN_CV_H_
#define _MOCHEN_CV_H_

#include "../../../server/src/route/route.h"
#include "../cv/imgalg.h"

namespace mochen
{

namespace page
{


inline void cvPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
    //if (_httpServerRequest.getUrl().isFindParamter("save") == true) {
    //    // ......
    //    return;
    //}

    if (_httpServerRequest.getData().empty() == false)
    {
        route::Route* route = _httpServerRequest.getSession()->getParamter<route::Route*>("Route");                        

        json::JsonParser jp(_httpServerRequest.getData());
        json::Json json = jp.parse();

        int len = std::stoi(json["length"].asString());
        std::string loadFilename = route->getDefaultStaticFilePath() + "/img" + json["loadFilename"].asString();
        std::string saveFilename = route->getDefaultStaticFilePath() + "/temp/" + json["saveFilename"].asString() + ".bmp";

        cv::IMAGE im = cv::Image_load((char*)loadFilename.c_str());
        
        for (int i = 0; i < len; i++) {

            std::string func = json["Data"][i]["funcName"].asString();

            if (func == "box3_btn1_1")
            {
                double k = atof(json["Data"][i]["parameter"].asString().c_str());
                im = cv::Transform_shape_linear(im, im.w * (k / 100.0), im.h * (k / 100.0));
            }
            else if (func == "box3_btn1_2")
            {
                double k = atof(json["Data"][i]["parameter"].asString().c_str());


                k = (int)k % 360 + 360;  // 处理k是负数的情况
                im = cv::Transform_shape_whirl(im, k);
            }
            else if (func == "box3_btn1_3")
            {
                double k = atof(json["Data"][i]["parameter"].asString().c_str());
                if (k > 0) {
                    im = cv::Transform_shape_upturn(im, UPTURN_MODE_HORIZONTAL);
                }
                else {
                    im = cv::Transform_shape_upturn(im, UPTURN_MODE_VERTICAL);
                }
            }
            else if (func == "box3_btn2_1")
            {
                cv::Transform_color_grayscale(im, GRAY_MODE_WEIGHT);
            }
            else if (func == "box3_btn2_2")
            {
                cv::Transform_color_BW_OSTU(im);
            }
            else if (func == "box3_btn2_3") 
            {
                cv::Transform_color_opposite(im);
            }
            else if (func == "box3_btn2_4")
            {
                cv::Transform_color_Histogram_all(im);
            }
            else if (func == "box3_btn3_1")
            {
                im = cv::Wavefiltering_Gauss(im);
            }
            else if (func == "box3_btn3_2")
            {
                im = cv::Edge_detection_Sobel(im, cv::KERNELS_Edge_Sobel_X, cv::KERNELS_Edge_Sobel_Y);
            }
            else if (func == "box3_btn4_1")
            {
                im = cv::Pooling(im, 2);
            }
            else if (func == "box3_btn4_2")
            {
                im = cv::Transform_color_BW_Adaptive(im, 9);
            }
            else if (func == "box3_btn4_3")
            {
                Transform_color_BW_grayscale(im, 9);
            }
        }
        cv::Image_bmp_save((char*)saveFilename.c_str(), im);
        cv::Image_free(im);
    }

};




};

};




#endif // ! _MOCHEN_CV_H_
