#define STR(expr) #expr

constexpr bool is_digit(char ch) {
    return ch <= '9' && ch >= '0';
}

constexpr unsigned count_split(const char* str, char delim) {
    unsigned count = 1u;
    for (unsigned i = 0; str[i]; i++) {
        if (str[i] == delim) {
            count++;
        }
    }
    return count;
}


struct const_str_wrapper;
constexpr unsigned int stou_impl(const_str_wrapper str, unsigned value, unsigned processed);
constexpr unsigned int stou(const_str_wrapper str);

struct const_str_wrapper {
    const char* const begin;
    const unsigned size;
    
    template<unsigned N> constexpr
    const_str_wrapper(const char(&arr)[N]) 
    : begin(arr), size(N - 1) {
        static_assert(N >= 1, "wrong string length");
    }
    constexpr 
    const_str_wrapper(const char* start, unsigned length) 
    : begin(start), size(length) {}

    constexpr
    unsigned tou() {
        return stou(*this);
    }
};

constexpr unsigned int stou_impl(const_str_wrapper str, unsigned value = 0, unsigned processed = 0) {
    if (processed == str.size) return value;
    return !is_digit(str.begin[processed]) ? 
        value :
        stou_impl(str, str.begin[processed] - '0' + 10 * value, processed + 1);
}

constexpr const char* lstrip(const char* str) {
    while (!is_digit(*str)) {
        str++;
    }
    return str;
}

constexpr unsigned int stou(const_str_wrapper str) {
    return stou_impl(str);
}

template <unsigned N>
struct split_str_to_u {
    constexpr static unsigned length = N;
    unsigned nums[N] {};
    // State Machine:
    // NORM + [0-9]: IN_NUM
    // IN_NUM + [0-9]: IN_NUM
    // IN_NUM + [^0-9]: NORM
    // NORM + [^0-9]: NORM
    constexpr 
    split_str_to_u(const char* str, char delim) {
        str = lstrip(str);
        if (count_split(str, delim) != N) 
            throw "wrong split count!";
        const char* lpos = str;
        unsigned len = 0;
        unsigned processed = 0;
        enum {NORM, IN_NUM} mode = NORM;
        while (*str) {
            if (mode == NORM && is_digit(*str)) {
                lpos = str;
                mode = IN_NUM;
                len = 1;
                str++;
            } else if (mode == IN_NUM && is_digit(*str)) {
                len++;
                str++;
            } else if (mode == IN_NUM && !is_digit(*str)) {
                if (lpos == nullptr) 
                    throw "unexpected lpos";
                const_str_wrapper wp(lpos, len);
                nums[processed] = wp.tou();
                processed++;
                len = 0;
                mode = NORM;
                lpos = nullptr;
                str = lstrip(str+1);
            } else {
                str++;
            }
        }
        if (mode == IN_NUM) {
            if (lpos == nullptr) 
                throw "unexpected lpos";
            const_str_wrapper wp(lpos, len);
            nums[processed] = wp.tou();
            processed++;
            mode = NORM;
                
        }
        if (processed != N) {
            throw "error parsing str";
        };
    }
};
