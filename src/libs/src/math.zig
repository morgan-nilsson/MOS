export fn abs(n: c_int) callconv(.C) c_int {
    if (n < 0) {
        return -n;
    } else return n;
}
