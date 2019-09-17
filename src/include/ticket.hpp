#pragma once

#ifdef MAKEDLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

namespace cti {

    class Matcher;
    class Ticket;
    class Text;
    class BoundingBox;
    class Point;
    class TicketMatch;
    class Metadata;

    class EXPORT Cti {
    public:
        Cti(){}
        Cti(const Cti& other) = delete;
        Cti(Cti&& other) = delete;
        Cti& operator=(const Cti& other) = delete;
        Cti& operator=(Cti&& other) = delete;

    public:
        const Matcher* const train(const vector<const Ticket>& tickets) const;
        void hello(char*) const;
        // TODO: possibly a learn method that takes an existing matcher and a vector of tickets to train additional tickets?
    };

    class EXPORT TicketImage {
    public:
        TicketImage(string imageFilePath) {} // TODO: implement
        TicketImage(int width, int height, int bytes_per_pixel, int bytes_per_line) {} // TODO: implement
        TicketImage(const TicketImage& other) = delete;
        TicketImage(TicketImage&& other) = delete;
        TicketImage& operator=(const TicketImage& other) = delete;
        TicketImage& operator=(TicketImage&& other) = delete;

    private:
        // TODO: internal structure?
    };

    class Ticket {
    public:
        // TODO: maybe a vector of bounding boxes to be ignored in the matcher?
        // TODO: maybe a vector of bounding boxes to be used in the matcher to match on?
        Ticket(string name, TicketImage& image, vector<const Text>& texts) : name(name), image(image), texts(texts) {}
        Ticket(const Ticket& other) = delete;
        Ticket(Ticket&& other) = delete;
        Ticket& operator=(const Ticket& other) = delete;
        Ticket& operator=(Ticket&& other) = delete;

    private:
        const string name;
        const TicketImage& image;
        const vector<const Text>& texts;
    };

    class EXPORT TicketMetadata {
    public:
        TicketMetadata(string name, vector<const Text>& texts) : name(name), texts(texts) {}
        TicketMetadata(const TicketMetadata& other) = delete;
        TicketMetadata(TicketMetadata&& other) = delete;
        TicketMetadata& operator=(const TicketMetadata& other) = delete;
        TicketMetadata& operator=(TicketMetadata&& other) = delete;
        const vector<const Text>& getTexts() const {
            return texts;
        }

    private:
        const string name;
        const vector<const Text>& texts;
    };

    class EXPORT Text {
    public:
        Text(string key, BoundingBox& boundingBox) : key(key), boundingBox(boundingBox) {}
        Text(const Text& other) = delete;
        Text(Text&& other) = delete;
        Text& operator=(const Text& other) = delete;
        Text& operator=(Text&& other) = delete;

    private:
        const string key;
        const BoundingBox& boundingBox;
    };

    class EXPORT BoundingBox {
    public:
        BoundingBox(Point& topLeft, Point& bottomRight) : topLeft(topLeft), bottomRight(bottomRight) {}
        BoundingBox(const BoundingBox& other) = delete;
        BoundingBox(BoundingBox&& other) = delete;
        BoundingBox& operator=(const BoundingBox& other) = delete;
        BoundingBox& operator=(BoundingBox&& other) = delete;

    private:
        const Point& topLeft;
        const Point& bottomRight;
    };

    class EXPORT Point {
    public:
        Point(int x, int y) : x(x), y(y) {}
        Point(const Point& other) = delete;
        Point(Point&& other) = delete;
        Point& operator=(const Point& other) = delete;
        Point& operator=(Point&& other) = delete;

    private:
        const int x;
        const int y;
    };

    class EXPORT Matcher {
    public:
        Matcher(const Matcher& other) = delete;
        Matcher(Matcher&& other) = delete;
        Matcher& operator=(const Matcher& other) = delete;
        Matcher& operator=(Matcher&& other) = delete;

        virtual TicketMatch* match(const TicketImage& image) = 0;
    };

    class EXPORT TicketMatch {
    public:
        TicketMatch(string name) : name(name) {}
        TicketMatch(const TicketMatch& other) = delete;
        TicketMatch(TicketMatch&& other) = delete;
        TicketMatch& operator=(const TicketMatch& other) = delete;
        TicketMatch& operator=(TicketMatch&& other) = delete;

        string getName() const {
            return name;
        }

    private:
        const string name;
    };

    class EXPORT MetadataReader {
    public:
        MetadataReader(const MetadataReader& other) = delete;
        MetadataReader(MetadataReader&& other) = delete;
        MetadataReader& operator=(const MetadataReader& other) = delete;
        MetadataReader& operator=(MetadataReader&& other) = delete;

        virtual const Metadata* const read(const TicketImage& image, const TicketMetadata& metadata) = 0;
    };

    class EXPORT Metadata {
    public:
        const unordered_map<const string, const string>& getTexts() const {
            return texts;
        }
    private:
        const unordered_map<const string, const string>& texts;
    };





    // Auswertung pro version: Score für matching, Score für metadaten-extraktion


    // Sensitivität / Richtig-positiv-Rate / Empfindlichkeit / Trefferquote / Sensitivity / true positive rate / recall / hit rate
    // Anteil der korrekt als positiv klassifizierten Objekte an der Gesamtheit der tatsächlich positiven Objekte.
    // Anteil an tatsächlich zu erkennenden Tickets, welche auch erkannt wurden.
    // TP / (TP + FN)

    // Falsch-negativ-Rate / false negative rate / miss rate
    // Anteil der fälschlich als negativ klassifizierten Objekte an der Gesamtheit der tatsächlich positiven Objekte.
    // Anteil der fälschlicherweise nicht erkannten Tickets an der Gesamtheit der tatsächlich zu erkennenden Tickets.
    // FN / (TP + FN)


    // Spezifität / Richtig-negativ-Rate / kennzeichnende Eigenschaft / specificity / true negative rate / correct rejection rate
    // Anteil der korrekt als negativ klassifizierten Objekte an der Gesamtheit der in Wirklichkeit negativen Objekte.
    // Anteil der korrekterweise nicht erkannten Tickets an der Gesamtheit der tatsächlich nicht zu erkennenden Tickets.
    // TN / (TN + FP)

    // Falsch-positiv-Rate / Ausfallsrate / fallout / false positive rate
    // Anteil der fälschlich als positiv klassifizierten Objekte an, die in Wirklichkeit negativ sind.
    // Anteil der fälschlicherweise erkannten Tickets, welche in Wirklichkeit nicht zu erkennen sind.


    // Positiver Vorhersagewert / Relevanz / Wirksamkeit / Genauigkeit / precision / positive predictive value
    // Anteil der korrekt als positiv klassifizierten Ergenisse an der Gesamtheit der als positiv klassifizierten Ergebnisse.
    // Anteil der korrekterweise erkannten Tickets an der Gesamtheit der zu erkennenden Tickets.
    // TP / (TP + FP)

    // Negativer Vorhersagewert / Segreganz / Trennfähigkeit / negative predictive value
    // Anteil der korrekt als negativ klassifizierten Ergebnisse an der Gesamtheit der als negativ klassifizierten Ergebnisse.
    // Anteil der korrekterweise nicht erkannten Tickets an der Gesamtheit der nicht zu erkennenden Tickets.
    // TN / (TN + FN)


    // Vertrauenswahrscheinlichkeit / Treffergenauigkeit / accuracy
    // Anteil aller Objekte, welche korrekt klassifiziert wurden.
    // (TP + TN) / (TP + FP + TN + FN)


    // F1
    // Combines precision and recall
    // 2 * precision * recall / (precision + recall)

}
