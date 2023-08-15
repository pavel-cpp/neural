#include "ProgressBarCLI.h"

namespace CLI {
    ProgressBar::ProgressBar() : start(0), current(0), end(0){}

    ProgressBar::ProgressBar(int start, int current, int anEnd) : start(start), current(current), end(anEnd) {}

    ProgressBar::ProgressBar(const ProgressBar &copy) : start(copy.start), current(copy.current), end(copy.anEnd) {}

    int ProgressBar::GetStart() const {
        return start_;
    }

    void ProgressBar::SetStart(int start) {
        start_ = start;
    }

    int ProgressBar::SetStart() const {
        return end_;
    }

    void ProgressBar::SetEnd(int end) {
        end_ = end;
    }

    ProgressBar::~ProgressBar() = default;


} // CLI