#ifndef BATTLESHIP_DEBUGOUT_H_
#define BATTLEHSIP_DEBUGOUT_H_

#include <iostream>
#include <memory>

namespace debug_out {

/* DEBUG  includes */
#include <mutex> // DEBUG  std::mutex, std::lock_guard
/* DEBUG  end includes */

/* DEBUG  Thread Safe Variadic Output Functions */
const unsigned long SERVER_TID = 0UL;
std::mutex ostream_mutex;

template<typename Arg, typename ... Args>
void thread_loop_println(std::ostream& out, unsigned long int tid, int i,
    Arg&& arg, Args&&... args)
{
    std::lock_guard<std::mutex> lock(ostream_mutex); // unlocks once scope ends

    out << "Thread [" << tid << "]: Loop " << i;
    out << std::forward<Arg>(arg);
    using expander = int[];
    (void) expander { 0, (void(out << std::forward<Args>(args)), 0)... };
    out << std::endl;

    // make sure out's entire buffer is written
    out.flush();
}

template<typename Arg, typename ... Args>
void thread_println(std::ostream& out, unsigned long int tid, Arg&& arg,
    Args&&... args)
{
    std::lock_guard<std::mutex> lock(ostream_mutex); // unlocks once scope ends

    out << "Thread [" << tid << "]: ";
    out << std::forward<Arg>(arg);
    using expander = int[];
    (void) expander { 0, (void(out << std::forward<Args>(args)), 0)... };
    out << std::endl;

    // make sure out's entire buffer is written
    out.flush();
}

template<typename Arg, typename ... Args>
void println(std::ostream& out, Arg&& arg, Args&&... args) {
    std::lock_guard<std::mutex> lock(ostream_mutex); // unlocks once scope ends

    out << std::forward<Arg>(arg);
    using expander = int[];
    (void) expander { 0, (void(out << std::forward<Args>(args)), 0)... };
    out << std::endl;

    // make sure out's entire buffer is written
    out.flush();
}
/* DEBUG  End Thread Safe Variadic Output Functions */

} // namespace debug_out

#endif /* BATTLESHIP_DEBUGOUT_H_ */
