#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <ctime>
#include <string>

class Datetime
{
public:
    Datetime() = default;
    Datetime(std::time_t epoch)
        : time_(epoch)
    { }

    std::string dump(void) const
    {
        std::string date{std::asctime(std::localtime(&time_))};
        date.pop_back();
        return date;
    }

    void set(std::time_t epoch)
    {
        time_ = epoch;
    }

    time_t epoch(void) const
    {
        return time_;
    }

    bool operator==(const Datetime& d) const
    {
        return time_ == d.time_;
    }

    bool operator!=(const Datetime& d) const
    {
        return time_ != d.time_;
    }

    bool operator<=(const Datetime& d) const
    {
        return time_ <= d.time_;
    }

    bool operator<(const Datetime& d) const
    {
        return time_ < d.time_;
    }

    bool operator>=(const Datetime& d) const
    {
        return time_ >= d.time_;
    }

    bool operator>(const Datetime& d) const
    {
        return time_ > d.time_;
    }

    static time_t now(void)
    {
        return std::time(NULL);
    }

private:
    time_t time_ = 0;
};

#endif
