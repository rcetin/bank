#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <ctime>
#include <string>

class Datetime
{
public:
    Datetime(std::time_t epoch)
        : time_(epoch)
    { }

    std::string dump(void)
    {
        return std::string{std::asctime(std::localtime(&time_))};
    }

    time_t epoch(void)
    {
        return time_;
    }

    bool operator==(const Datetime& d)
    {
        return time_ == d.time_;
    }

    bool operator!=(const Datetime& d)
    {
        return time_ != d.time_;
    }

    bool operator<=(const Datetime& d)
    {
        return time_ <= d.time_;
    }

    bool operator<(const Datetime& d)
    {
        return time_ < d.time_;
    }

    bool operator>=(const Datetime& d)
    {
        return time_ >= d.time_;
    }

    bool operator>(const Datetime& d)
    {
        return time_ > d.time_;
    }

private:
    time_t time_;
};

#endif
