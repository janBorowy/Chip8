#include "Timer.h"

Timer::Timer():
    start(Clock::now()),
    givenTime(std::chrono::nanoseconds(0)) {}

void Timer::startTimer() {
    start = Clock::now();
}

void Timer::setTime(Clock::duration time) {
    givenTime = time;
}

std::chrono::steady_clock::duration Timer::getStatus() {
    auto timeLeft = givenTime - (Clock::now() - start);
    if(timeLeft < std::chrono::nanoseconds(0)) {
        return Clock::duration(0);
    }
    return timeLeft;
}

bool Timer::hasFinished() {
    return getStatus() == Clock::duration(0);
}

