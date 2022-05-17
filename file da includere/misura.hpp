#ifndef MISURA_HPP
#define MISURA_HPP

// piccola struct utile
struct misura {
    double p{}, d{};
    misura (double x = 0., double dx = 0.) : p{x}, d{dx} {}
};

#endif