pub fn port_byte_in(port: u16) u8 {
    var value: u8 = undefined;
    asm volatile ("inb %[port], %[value]"
        : [value] "={al}" (value), // Read from port into AL
        : [port] "{dx}" (port), // Load port into DX register
    );
    return value;
}

pub fn port_byte_out(port: u16, data: u8) void {
    asm volatile ("outb %[value], %[port]"
        :
        : [value] "{al}" (data), // Send data via AL register
          [port] "{dx}" (port), // Load port into DX register
    );
}
