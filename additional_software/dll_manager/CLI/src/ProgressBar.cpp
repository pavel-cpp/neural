#include "../include/ProgressBar.h"

namespace CLI {
    ProgressBar::ProgressBar() : start_(0), current_(0), end_(0){
        _setmode(_fileno(stdout), _O_U16TEXT);
    }

    ProgressBar::ProgressBar(int start, int current, int end) : start_(start), current_(current_), end_(end) {
        _setmode(_fileno(stdout), _O_U16TEXT);
    }

    ProgressBar::ProgressBar(const ProgressBar &copy) : start_(copy.start_), current_(copy.current_), end_(copy.end_) {
        _setmode(_fileno(stdout), _O_U16TEXT);
    }

    void ProgressBar::SetStart(int start) {
        start_ = start;
        current_ = 0;
    }

    void ProgressBar::SetEnd(int end) {
        end_ = end;
    }

    std::wstring ProgressBar::Next() {
            ++current_;
            return std::wstring(ComputePercentage() / 8, L'\x2588');
    }

    int ProgressBar::ComputePercentage() const {
        return static_cast<int>((static_cast<double>(current_) / (end_ - start_)) * 100);
    }

    void ProgressBar::Restart() {
        current_ = 0;
    }

    ProgressBar::~ProgressBar() = default;


} // CLI