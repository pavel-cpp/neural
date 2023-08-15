#ifndef NEURALDLLMANAGER_PROGRESSBAR_H
#define NEURALDLLMANAGER_PROGRESSBAR_H

#include <fcntl.h>
#include <io.h>
#include <iostream>

namespace CLI {

    class ProgressBar {
    public:
        ProgressBar();

        ProgressBar(int start, int current, int end);

        ProgressBar(const ProgressBar& copy);

        virtual ~ProgressBar();

        std::wstring Next();

        void Restart();

        void SetStart(int start);

        void SetEnd(int end);

    private:

        int start_, current_, end_;

        int ComputePercentage() const;
    };

}

#endif //NEURALDLLMANAGER_PROGRESSBAR_H
