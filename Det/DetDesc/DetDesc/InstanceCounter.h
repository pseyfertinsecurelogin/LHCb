#ifndef    DETDESC_INSTANCECOUNTER_H
#define    DETDESC_INSTANCECOUNTER_H 1

namespace Details {
    template <typename T>
    class InstanceCounter {
        static std::size_t s_counter;
    public:
        InstanceCounter() { ++s_counter; }
        InstanceCounter(const InstanceCounter&) { ++s_counter; }
        ~InstanceCounter() { --s_counter; }
        static std::size_t count() { return s_counter; }
    };

    template <typename T> std::size_t InstanceCounter<T>::s_counter = 0;
}
#endif
