#pragma once

namespace cti::reader {
    class ConfusionMatrix {
    public:
        ConfusionMatrix(int truePositives, int trueNegatives, int falsePositives, int falseNegatives)
                : _truePositives(truePositives), _trueNegatives(trueNegatives),
                _falsePositives(falsePositives), _falseNegatives(falseNegatives) {};
        ConfusionMatrix(const ConfusionMatrix&) = default;
        ConfusionMatrix(ConfusionMatrix&&) = default;
        ConfusionMatrix &operator=(const ConfusionMatrix& other) = delete;
        ConfusionMatrix &operator=(ConfusionMatrix&&) = default;

        ConfusionMatrix operator+(const ConfusionMatrix& other) {
            ConfusionMatrix result {
                this->truePositives() + other.truePositives(),
                this->trueNegatives() + other.trueNegatives(),
                this->falsePositives() + other.falsePositives(),
                this->falseNegatives() + other.falseNegatives()
            };
            return result;
        }

        const double accuracy() const {
            return (double)(truePositives() + trueNegatives()) / total();
        }

        const int truePositives() const {
            return _truePositives;
        }

        const int trueNegatives() const {
            return _trueNegatives;
        }

        const int falsePositives() const {
            return _falsePositives;
        }

        const int falseNegatives() const {
            return _falseNegatives;
        }

        const int total() const {
            return truePositives() + trueNegatives() + falsePositives() + falseNegatives();
        }

    private:
        int _truePositives;
        int _trueNegatives;
        int _falsePositives;
        int _falseNegatives;
    };
}