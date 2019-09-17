#pragma once

namespace cti {
    using std:string;
    using std::vector;
    using std::unordered_map;

    class Cti {
        Cti(const Cti& other) = delete;
        Cti(Cti&& other) = delete;
        operator=(const Cti& other) = delete;
        operator=(Cti&& other) = delete;

    public:
        const Matcher* const train(const vector<const Ticket>& tickets) const;
        // TODO: possibly a learn method that takes an existing matcher and a vector of tickets to train additional tickets?
    };

    class TicketImage {
    public:
        TicketImage(string imageFilePath) {} // TODO: implement
        TicketImage(int width, int height, int bytes_per_pixel, int bytes_per_line) {} // TODO: implement
        TicketImage(const TicketImage& other) = delete;
        TicketImage(TicketImage&& other) = delete;
        operator=(const TicketImage& other) = delete;
        operator=(TicketImage&& other) = delete;

    private:
        // TODO: internal structure?
    };

    class Ticket {
    public:
        // TODO: maybe a vector of bounding boxes to be ignored in the matcher?
        // TODO: maybe a vector of bounding boxes to be used in the matcher to match on?
        Ticket(string name, TicketImage& image) : name(name), image(image) {}
        Ticket(const Ticket& other) = delete;
        Ticket(Ticket&& other) = delete;
        operator=(const Ticket& other) = delete;
        operator=(Ticket&& other) = delete;

    private:
        const string name;
        const TicketImage& image;
        const vector<const Text>& texts;
    };

    class TicketMetadata {
    public:
        TicketMetadata(string name, vector<Text>& texts) : name(name), texts(texts) {}
        TicketMetadata(const TicketMetadata& other) = delete;
        TicketMetadata(TicketMetadata&& other) = delete;
        operator=(const TicketMetadata& other) = delete;
        operator=(TicketMetadata&& other) = delete;
        const vector<const Text>& texts() {
            return texts;
        }

    private:
        const string name;
        const vector<const Text>& texts;
    };

    class Text {
    public:
        Text(string key, BoundingBox& boundingBox) : key(key), boundingBox(boundingBox) {}
        Text(const Text& other) = delete;
        Text(Text&& other) = delete;
        operator=(const Text& other) = delete;
        operator=(Text&& other) = delete;

    private:
        const string key;
        const BoundingBox& boundingBox;
    };

    class BoundingBox {
    public:
        BoundingBox(Point& topLeft, Point& bottomRight) : topLeft(topLeft), bottomRight(bottomRight) {}
        BoundingBox(const BoundingBox& other) = delete;
        BoundingBox(BoundingBox&& other) = delete;
        operator=(const BoundingBox& other) = delete;
        operator=(BoundingBox&& other) = delete;

    private:
        const Point& topLeft;
        const Point& bottomRight;
    };

    class Point {
    public:
        Point(int x, int y) : x(x), y(y) {}
        Point(const Point& other) = delete;
        Point(Point&& other) = delete;
        operator=(const Point& other) = delete;
        operator=(Point&& other) = delete;

    private:
        const int x;
        const int y;
    };

    class Matcher {
    public:
        Matcher(const Matcher& other) = delete;
        Matcher(Matcher&& other) = delete;
        operator=(const Matcher& other) = delete;
        operator=(Matcher&& other) = delete;

        virtual TicketMatch* match(const TicketImage& image) = 0;
    };

    class TicketMatch {
    public:
        TicketMatch(string name) : name(name) {}
        TicketMatch(const TicketMatch& other) = delete;
        TicketMatch(TicketMatch&& other) = delete;
        operator=(const TicketMatch& other) = delete;
        operator=(TicketMatch&& other) = delete;
        boolean matched() {
            return name != nullptr;
        }
        string name() {
            return name;
        }

    private:
        const string name;
    };

    class MetadataReader {
    public:
        MetadataReader(const MetadataReader& other) = delete;
        MetadataReader(MetadataReader&& other) = delete;
        operator=(const MetadataReader& other) = delete;
        operator=(MetadataReader&& other) = delete;

        virtual const Metadata* const read(const TicketImage& image, const TicketMetadata& metadata) = 0;
    };

    class Metadata {
    public:
        const unordered_map<const string, const string>& texts() {
            return texts;
        }
    private:
        const unordered_map<const string, const string>& texts;
    };

    void test() {
        using cti;

        TicketImage newTicket { "ticket1.jpg" }

        vector<Ticket> knownTickets;
        knownTickets.push_back(Ticket { "ticket1", TicketImage { "ticket1.jpg" } });

        Cti cti;
        TicketMatcher* matcher = cti.train(&knownTickets);
        TicketMatch* match = matcher.match(newTicket);

        if(match->matched()) {
            cout << "Image matched. Reading metadata." << endl;
            TicketMetadata* ticketMeta = getMeta(match.name());

            MetadataReader reader;
            Metadata* metadata = reader.read(newTicket, ticketMeta);

            if(metadata->texts().size() != ticketMeta->texts().size()) {
                cout << "Unable to read all metadata from image" << endl;
            }

            cout << "Read the following metadata from image:" << endl;
            for (auto x : metadata->texts()) {
                cout << x.first << "=" << x.second << endl;
            }
        } else {
            cout << "Unable to match image" << endl;
        }
    }

    const TicketMetadata * const getMeta(string name) {
        const vector<Text> texts1;
        texts1.push_back(Text { "text1", BoundingBox { Point { 0 , 0 }, Point { 100, 100 } } });
        texts1.push_back(Text { "text2", BoundingBox { Point { 200 , 150 }, Point { 300, 400 } } });
        const TicketMetadata ticketMeta { "ticket1", texts1 };
        return ticketMeta;
    }

    void test2() {



    }

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
