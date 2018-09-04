#ifndef VTR_TIME_H
#define VTR_TIME_H
#include <chrono>
#include <string>

namespace vtr {

    //Class for tracking time elapsed since construction
    class ScopedTimer {
        public:
            ScopedTimer();
            virtual ~ScopedTimer() = default;

            //No copy
            ScopedTimer(ScopedTimer&) = delete;
            ScopedTimer& operator=(ScopedTimer&) = delete;

            //No move
            ScopedTimer(ScopedTimer&&) = delete;
            ScopedTimer& operator=(ScopedTimer&&) = delete;

            //Return elapsed time in seconds
            float elapsed_sec() const;

            //Return peak memory resident set size (in MiB)
            float max_rss_mib() const;

            //Return change in peak memory resident set size (in MiB)
            float delta_max_rss_mib() const;
        private:
            using clock = std::chrono::steady_clock;
            std::chrono::time_point<clock> start_;

            size_t initial_max_rss_; //Maximum resident set size In bytes
            constexpr static float BYTE_TO_MIB = 1024*1024;
    };

    //Scoped elapsed time class which prints the time elapsed for
    //the specified action when it is destructed.
    //
    //For example:
    //
    //      {
    //          vtr::ScopedFinishTimer timer("my_action");
    //
    //          //Do other work
    //
    //          //Will print: 'my_action took X.XX seconds' when out-of-scope
    //      }
    class ScopedFinishTimer : public ScopedTimer {
        public:
            ScopedFinishTimer(const std::string action);
            ~ScopedFinishTimer();

            void quiet(bool value);
            bool quiet() const;
            std::string action() const;

        private:
            const std::string action_;
            bool quiet_ = false;
    };

    //Scoped elapsed time class which prints out the action when
    //initialized and again both the action and elapsed time
    //when destructed.
    //For example:
    //
    //      {
    //          vtr::ScopedActionTimer timer("my_action") //Will print: 'my_action'
    //
    //          //Do other work
    //
    //          //Will print 'my_action took X.XX seconds' when out of scope
    //      }
    class ScopedActionTimer : public ScopedFinishTimer {
        public:
            ScopedActionTimer(const std::string action);
    };
}

#endif
