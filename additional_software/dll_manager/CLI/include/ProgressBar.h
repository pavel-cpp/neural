#ifndef NEURALDLLMANAGER_PROGRESSBAR_H
#define NEURALDLLMANAGER_PROGRESSBAR_H

#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <limits>
#include <windows.h>

namespace CLI {

    class ProgressBar {
    public:
        ProgressBar();

        ProgressBar(int start, int end);

        ProgressBar(const ProgressBar &copy);

        virtual ~ProgressBar();

        std::wstring Next();

        //TODO: FIX: Incorrect char count
        std::wstring Next(wchar_t ch);

        void Restart();

        void SetStart(int start);

        void SetEnd(int end);

    private:

        int start_, current_, end_;

        double completed_;

        const float DELIMITER_COUNT_ = 25.f;

        double ComputePercentage() const;
    };

}

#endif //NEURALDLLMANAGER_PROGRESSBAR_H
