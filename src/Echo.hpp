//
// Created by Jeramy on 4/19/2015.
//

#ifndef TOYBOXESCAPE_ECHO_HPP
#define TOYBOXESCAPE_ECHO_HPP

#ifndef NDEBUG
#include <iostream>
template <typename Head>
void Echo(const Head& head) {
    std::clog << head << std::endl;
}
template <typename Head, typename... Tail>
void Echo(const Head& head, const Tail&... tail) {
    std::clog << head << " ";
    Echo(tail...);
}
#else
template <typename... Ts>
void Echo(Ts&&...) {}
#endif

#endif //TOYBOXESCAPE_ECHO_HPP
