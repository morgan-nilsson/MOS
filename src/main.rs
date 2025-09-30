#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]
#![reexport_test_harness_main = "test_main"]

use core::panic::PanicInfo;
mod testing;

pub mod stdio;

#[cfg(test)]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    serial_println!("[failed]\n");
    serial_println!("Error: {}\n", info);
    exit_qemu(QemuExitCode::Failed);
    loop {}
}

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {

    #[cfg(test)]
    test_main();

    println!("Hello, world!");

    loop {}
}

#[cfg(test)]
pub fn test_runner(tests: &[&dyn testing::Testable]) {
    testing::test_runner(tests);
}



#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u32)]
pub enum QemuExitCode {
    Success = 0x10,
    Failed = 0x11,
}

pub fn exit_qemu(exit_code: QemuExitCode) {
    use stdio::ports::port_byte_out;
    const PORT: u16 = 0xf4;
    port_byte_out(PORT, exit_code as u8);
}