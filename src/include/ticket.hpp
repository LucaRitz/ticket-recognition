#pragma once

#include "dllmacro.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace cti {
    class Text;
    class TicketImage;
    class BoundingBox;

    class EXPORT Ticket {
    public:
        Ticket(string name, const TicketImage& image, vector<const Text*>& texts)
                : Ticket(std::move(name), image, texts, *(new vector<const BoundingBox*>())) {}
        Ticket(string name, const TicketImage& image, vector<const Text*>& texts, vector<const BoundingBox*>& matchingAreas)
                : _name(std::move(name)), _image(image), _texts(texts), _matchingAreas(matchingAreas) {}

        Ticket(const Ticket& other) = delete;
        Ticket(Ticket&& other) = delete;
        Ticket& operator=(const Ticket& other) = delete;
        Ticket& operator=(Ticket&& other) = delete;

        const string name() {
            return _name;
        }

        const TicketImage& image() const {
            return _image;
        }

        const vector<const Text*>& texts() const {
            return _texts;
        }

        const vector<const BoundingBox*>& matchingAreas() const {
            return _matchingAreas;
        }


    private:
        const string _name;
        const TicketImage& _image;
        const vector<const Text*>& _texts;
        const vector<const BoundingBox*>& _matchingAreas;
    };
}
