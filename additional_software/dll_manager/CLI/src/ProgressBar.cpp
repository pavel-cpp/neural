#include "../include/ProgressBar.h"

namespace CLI {
    ProgressBar::ProgressBar() : start_(0), current_(0), end_(0) {
        _setmode(_fileno(stdout), _O_U16TEXT);
    }

    ProgressBar::ProgressBar(int start, int end) : start_(start), current_(0), end_(end) {
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
        current_ = 0;
    }

    std::wstring ProgressBar::Next() {
        return Next(L'\x2588');
    }

    std::wstring ProgressBar::Next(wchar_t ch) {
        if (current_ > end_) return {};
        ++current_;
        int print_cnt = (DELIMITER_COUNT_ - completed_ ) * ComputePercentage() ;
        if(print_cnt < 0) print_cnt = 0;
        completed_ += (DELIMITER_COUNT_ - completed_ ) * ComputePercentage();
        return std::wstring(print_cnt, ch);
    }

    double ProgressBar::ComputePercentage() const {
        return static_cast<double>(current_) / (end_ - start_);
    }

    void ProgressBar::Restart() {
        current_ = 0;
    }

    ProgressBar::~ProgressBar() = default;


} // CLI