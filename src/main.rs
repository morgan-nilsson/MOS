#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]
#![reexport_test_harness_main = "test_main"]
#![feature(abi_x86_interrupt)]

pub mod stdio;
pub mod testing;
pub mod clock;
pub mod keyboard;
pub mod types;
pub mod interrupts;
pub mod memory;

#[cfg(not(test))]
use core::panic::PanicInfo;
#[cfg(not(test))]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    use crate::println;
    println!("{}", info);
    hlt_loop();
}

use bootloader::{BootInfo, entry_point};

#[cfg(not(test))]
entry_point!( kernel_main );

#[unsafe(no_mangle)]
fn kernel_main(bootinfo: &'static BootInfo) -> ! {

    init_os();

    #[cfg(test)]
    test_main();

    hlt_loop();
}

#[cfg(test)]
entry_point!(test_kernel_main);

/// Entry point for `cargo test`
#[cfg(test)]
fn test_kernel_main(_boot_info: &'static BootInfo) -> ! {
    // like before
    init_os();
    test_main();
    hlt_loop();
}

pub fn hlt_loop() -> ! {
    loop {
        x86_64::instructions::hlt();
    }
}

#[cfg(test)]
pub fn test_runner(tests: &[&dyn testing::testing::Testable]) {
    testing::testing::test_runner(tests);
}

fn init_os() {
    interrupts::interrupts::init_idt();
    memory::gdt::init();
    unsafe { interrupts::interrupts::PICS.lock().initialize() };
    x86_64::instructions::interrupts::enable();
}