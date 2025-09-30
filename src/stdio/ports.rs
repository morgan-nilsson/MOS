use core::arch::asm;

#[inline]
pub fn port_byte_in(port: u16) -> u8 {
    let val: u8;
    unsafe {
        asm!(
            "in al, dx",
            out("al") val,
            in("dx") port,
        );
    }
    val
}

#[inline]
pub fn port_byte_out(port: u16, byte: u8) {
    unsafe {
        asm!(
            "out dx, al",
            in("al") byte,
            in("dx") port,
        )
    }
}