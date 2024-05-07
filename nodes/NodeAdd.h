#pragma once

#include "Node.h"
#include "../Image.h"

class NodeAdd : public Node
{
public:
    Image3 image;

    uint8_t value1;
    uint8_t value2;

    NodeAdd(int id, int& currentPinId) : Node(NodeType::Add, id, 2, 4, currentPinId)
    {
        inputPins[0].type = PinType::Any;
        inputPins[1].type = PinType::Any;
        setImage3OutputPin(0, image);
    }
    NodeAdd(json j) : Node(j) {
        setImage3OutputPin(0, image, true);
    }
    void render() override;
    void compute(const NodeList& nodes);
    cv::Mat getPinImage1(int pinId) override;
};

void to_json(json& j, const NodeAdd& node);