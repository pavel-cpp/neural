#include "../include/ProgressBarCLI.h"

namespace CLI {
    ProgressBar::ProgressBar() : start_(0), current_(0), end_(0){
        _setmode(_fileno(stdout), _O_U16TEXT);
    }

    ProgressBar::ProgressBar(int start, int current, int end) : start_(start), current_(current_), end_(end) {}

    ProgressBar::ProgressBar(const ProgressBar &copy) : start_(copy.start_), current_(copy.current_), end_(copy.end_) {}

    void ProgressBar::SetStart(int start) {
        start_ = start;
        current_ = 0;
    }

    void ProgressBar::SetEnd(int end) {
        end_ = end;
    }

    wchar_t ProgressBar::Next() {
        if(ComputePercentage() / 2){
            ++current_;
            return L'\x5288';
        }
        return NULL_CHAR;
    }

    int ProgressBar::ComputePercentage() const {
        return static_cast<int>(static_cast<double>(current_) / (end_ - start_));
    }

    void ProgressBar::Restart() {
        current_ = 0;
    }

    ProgressBar::~ProgressBar() = default;


} // CLI