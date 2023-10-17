#include <chrono>

typedef std::chrono::steady_clock Clock;

class Timer {
    Clock::time_point start;
    Clock::duration givenTime;
    public:
    Timer();
    void startTimer();
    bool hasFinished();
    void setTime(Clock::duration time);
    std::chrono::steady_clock::duration getStatus();
};