#ifndef REG_ACCESS_H
#define REG_ACCESS_H

namespace Hw {
template<typename addr_type,
         typename reg_type,
         const addr_type addr = addr_type(0),
         const reg_type val = reg_type(0)>
class reg_access {
public:
    static void bit_set() {
        *reinterpret_cast<volatile reg_type*>(addr) |= reg_type(reg_type(1) << val);
    }
    static void bit_set(reg_type value) {
        *reinterpret_cast<volatile reg_type*>(addr) |= reg_type(reg_type(1) << value);
    }
    static void bit_set(addr_type address, reg_type value) {
        *reinterpret_cast<volatile reg_type*>(address) |= reg_type(reg_type(1) << value);
    }
    static void bit_clr() {
        *reinterpret_cast<volatile reg_type*>(addr) &= ~(reg_type(reg_type(1) << val));
    }
    static void bit_clr(reg_type value) {
        *reinterpret_cast<volatile reg_type*>(addr) &= ~(reg_type(reg_type(1) << value));
    }
    static void bit_clr(addr_type address, reg_type value) {
        *reinterpret_cast<volatile reg_type*>(address) &= ~(reg_type(reg_type(1) << value));
    }
    static void bit_not() {
        *reinterpret_cast<volatile reg_type*>(addr) ^= reg_type(reg_type(1) << val);
    }
    static reg_type bit_get() {
        return *reinterpret_cast<volatile reg_type*>(addr) & reg_type(reg_type(1) << val);
    }
    static void reg_or() {
        *reinterpret_cast<volatile reg_type*>(addr) |= val;
    }
    static void reg_or(reg_type value) {
        *reinterpret_cast<volatile reg_type*>(addr) |= value;
    }
    static void reg_set() {
        *reinterpret_cast<volatile reg_type*>(addr) = val;
    }
    static void reg_set(reg_type value) {
        *reinterpret_cast<volatile reg_type*>(addr) = value;
    }
    static void reg_and() {
        *reinterpret_cast<volatile reg_type*>(addr) &= val;
    }
    static void reg_and(reg_type value) {
        *reinterpret_cast<volatile reg_type*>(addr) &= value;
    }
    static reg_type reg_get() {
        return *reinterpret_cast<volatile reg_type*>(addr);
    }
};
}

#endif // REG_ACCESS_H
