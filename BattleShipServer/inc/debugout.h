#ifndef BATTLESHIP_DEBUGOUT_H_
#define BATTLESHIP_DEBUGOUT_H_

/* DEBUG  includes */
#include <iostream>
#include <memory>
#include <mutex> // DEBUG  std::mutex, std::lock_guard
/* DEBUG  end includes */

namespace debug_out {

const unsigned long SERVER_TID = 0UL;
// std::mutex out_mux;
/* DEBUG  Thread Safe Variadic Output Functions */



template<typename Arg, typename ... Args>
void thread_loop_println(std::ostream& out, unsigned long int tid,
    int i, Arg&& arg, Args&&... args)
{
    static std::mutex out_mux;
    std::lock_guard<std::mutex> lock(out_mux); // unlocks once scope ends

    out << "Thread [" << tid << "]: Loop " << i;
    out << std::forward<Arg>(arg);
    using expander = int[];
    (void) expander { 0, (void(out << std::forward<Args>(args)), 0)... };
    out << std::endl;

    // make sure out's entire buffer is written
    out.flush();
}

template<typename Arg, typename ... Args>
void thread_println(std::ostream& out, unsigned long int tid,
    Arg&& arg, Args&&... args)
{
    static std::mutex out_mux;
    std::lock_guard<std::mutex> lock(out_mux); // unlocks once scope ends

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
    static std::mutex out_mux;
    std::lock_guard<std::mutex> lock(out_mux); // unlocks once scope ends

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
