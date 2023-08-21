#include "Timer.hpp"

Timer::Timer() : m_duration(0.0) {}

void Timer::start(double duration) {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_duration = duration;
}

bool Timer::isRunning() const {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - m_startTime;
    return elapsed.count() < m_duration;
}

double Timer::getTimeRemaining() const {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - m_startTime;
    return m_duration - elapsed.count();
}

void Timer::addTime(double duration) {
    m_duration += duration;
}

void Timer::stop() {
    m_duration = 0.0;
}
