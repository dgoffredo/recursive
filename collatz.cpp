
#include <iostream>
#include <functional>
#include <ostream>
#include <utility>  // std::max

int plainFunction(int n, int* max) {
    if (n == 1) {
        return n;
    }

    const int next = n % 2 
        ? 3 * n + 1
        : n / 2;

    *max = std::max(*max, next);
    return plainFunction(next, max);
}

void help(const char* argv0, std::ostream& out) {
    out << "usage: " << argv0 << " <integer>\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        help(argv[0], std::cerr);
        return 1;
    }

    std::string arg(argv[1]);
    if (arg == "--help" || arg == "-h") {
        help(argv[0], std::cout);
        return 0;
    }

    const int n = std::stoi(arg);
    int max = 0;
    // the old fashioned way
    plainFunction(n, &max);
    std::cout << "plain function: " << max << "\n";

    max = 0;
    // lambda with type template parameter (using special "auto" syntax)
    const auto visit = [&max](int n, const auto& recur) {
        if (n == 1) {
            return n;
        }

        const int next = n % 2 
            ? 3 * n + 1
            : n / 2;

        max = std::max(max, next);
        return recur(next, recur);
    };
    visit(n, visit);
    std::cout << "generic lambda: " << max << "\n";

    max = 0;
    // Who needs a type template parameter when you already have `this`?
    class Lambda {
        int* _max;
      public:
        explicit Lambda(int* max)
        : _max(max) {}

        int operator()(int n) const {
            if (n == 1) {
                return n;
            }

            const int next = n % 2 
                ? 3 * n + 1
                : n / 2;

            *_max = std::max(*_max, next);
            return (*this)(next);
        }
    };
    Lambda func(&max);
    func(n);
    std::cout << "callable object: " << max << "\n";

    max = 0;
    // Here's what I did.  Unnecessary, considering the `auto` syntax above.
    std::function<int(int)> recur;
    const auto visit2 = [&](int n) {
        if (n == 1) {
            return n;
        }

        const int next = n % 2 
            ? 3 * n + 1
            : n / 2;

        max = std::max(max, next);
        return recur(next);
    };
    recur = visit2;
    visit2(n);
    std::cout << "non-generic lambda: " << max << "\n";
}