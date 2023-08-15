#ifndef NEURALDLLMANAGER_PROGRESSBARCLI_H
#define NEURALDLLMANAGER_PROGRESSBARCLI_H

#include <fcntl.h>
#include <io.h>

namespace CLI {

    class ProgressBar {
    public:
        ProgressBar();

        ProgressBar(int start, int current, int anEnd);

        ProgressBar(const ProgressBar& copy);

        virtual ~ProgressBar();

        int Next();

        int Restart();

        int GetStart() const;

        void SetStart(int start);

        int SetStart() const;

        void SetEnd(int end);

    private:
        int start_, current_, end_;
    };

}

#endif //NEURALDLLMANAGER_PROGRESSBARCLI_H
