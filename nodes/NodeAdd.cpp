#include "NodeAdd.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/imgproc.hpp>


void NodeAdd::render()
{
    renderBegin("Add");

    renderInputInt("Input 1", 0, value1);
    renderInputInt("Input 2", 1, value2);

    renderOutputImage("Image", 0, image);




    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeAdd::compute(const NodeList& nodes)
{
    auto n1 = nodes.findNodeWithPin(inputPins[0].connection);
    auto n2 = nodes.findNodeWithPin(inputPins[1].connection);

    if ((n1 && !n1->computed) || (n2 && !n2->computed))
        return;
    computed = true;

    auto p1 = n1 ? n1->getOutputPin(inputPins[0].connection) : nullptr;
    auto p2 = n2 ? n2->getOutputPin(inputPins[1].connection) : nullptr;

    auto t1 = p1 ? p1->type : PinType::Int;
    auto t2 = p2 ? p2->type : PinType::Int;

    if (t1 == PinType::Image3)
    {
        auto img1 = n1->getPinImage3(inputPins[0].connection);
        if (!img1.data)
        {
            computeError = "Image not loaded";
            return;
        }
        if (img1.cols != image.mat.cols || img1.cols != image.mat.rows)
            image.mat = cv::Mat(img1.cols, img1.rows, img1.type());


        if (t2 == PinType::Image3)
        {
            auto img2 = n2->getPinImage3(inputPins[1].connection);
            if (!img2.data)
            {
                computeError = "Image not loaded";
                return;
            }
            if (img1.cols != img2.cols || img1.rows != img2.rows)
            {
                computeError = "Image sizes don't match";
                return;
            }
            cv::add(img1, img2, image.mat);
        }
        else if (t2 == PinType::Image1)
        {
            auto img2 = n2->getPinImage1(inputPins[1].connection);
            if (!img2.data)
            {
                computeError = "Image not loaded";
                return;
            }
            if (img1.cols != img2.cols || img1.rows != img2.rows)
            {
                computeError = "Image sizes don't match";
                return;
            }
            cv::Mat combined;
            cv::cvtColor(img2, combined, cv::COLOR_GRAY2RGB);
            cv::add(img1, combined, image.mat);
        }
        else if (t2 == PinType::Int)
        {
            int v = (n2 ? n2->getPinInt(inputPins[1].connection) : value2);
            image.mat = img1 + cv::Scalar(v, v, v);
        }
        else
            std::cout << "Unknown pintype: " << (int)t2 << std::endl;
    }

    image.refresh();

}

cv::Mat NodeAdd::getPinImage3(int pinId)
{
    return image.mat; //TODO: check pinId, though that's pointless here
}

cv::Mat NodeAdd::getPinImage1(int pinId)
{
    cv::Mat planes[3];
    cv::split(image.mat, planes);
    if (pinId == outputPins[1].id) //red
        return planes[0];
    if (pinId == outputPins[2].id) //green
        return planes[1];
    if (pinId == outputPins[3].id) //blue
        return planes[2];
}

void to_json(json& j, const NodeAdd& node)
{
}