#ifndef NEURALDLLMANAGER_PROGRESSBARCLI_H
#define NEURALDLLMANAGER_PROGRESSBARCLI_H

#include <fcntl.h>
#include <io.h>
#include <iostream>

namespace CLI {

    const wchar_t NULL_CHAR = 0;

    class ProgressBar {
    public:
        ProgressBar();

        ProgressBar(int start, int current, int end);

        ProgressBar(const ProgressBar& copy);

        virtual ~ProgressBar();

        wchar_t Next();

        void Restart();

        void SetStart(int start);

        void SetEnd(int end);

    private:

        int start_, current_, end_;

        int ComputePercentage() const;
    };

}

#endif //NEURALDLLMANAGER_PROGRESSBARCLI_H
