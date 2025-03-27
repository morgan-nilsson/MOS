export fn abs(n: c_int) callconv(.C) c_int {
    if (n < 0) {
        return -n;
    } else return n;
}

export fn labs(n: c_long) callconv(.C) c_long {
    if (n < 0) {
        return -n;
    } else return n;
}

export fn llabs(n: c_longlong) callconv(.C) c_longlong {
    if (n < 0) {
        return -n;
    } else return n;
}

export fn max(a: c_int, b: c_int) callconv(.C) c_int {
    return if (a > b) a else b;
}

export fn min(a: c_int, b: c_int) callconv(.C) c_int {
    return if (a > b) b else a;
}

export fn lmax(a: c_long, b: c_long) callconv(.C) c_long {
    return if (a > b) a else b;
}

export fn lmin(a: c_long, b: c_long) callconv(.C) c_long {
    return if (a > b) b else a;
}

export fn llmax(a: c_longlong, b: c_longlong) callconv(.C) c_longlong {
    return if (a > b) a else b;
}

export fn llmin(a: c_longlong, b: c_longlong) callconv(.C) c_longlong {
    return if (a > b) b else a;
}
