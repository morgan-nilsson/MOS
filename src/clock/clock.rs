use crate::stdio::ports::port_byte_out;

struct PIT {
    frequency: u32,
    tick: usize,
}