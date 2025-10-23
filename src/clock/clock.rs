use crate::print;
use crate::stdio::ports::port_byte_out;
use x86_64::structures::idt::InterruptStackFrame;

const PIT_NUMERATOR: usize = 1193180;

pub struct PIT {
    frequency: usize,
    _tick: usize,
}

impl PIT {

    fn new(frequency: usize) -> Self {

        let mut me = Self { frequency: 0, _tick: 0 };

        me.set_frequency(frequency);

        return me;


    }

    fn set_frequency(&mut self, frequency: usize) {
        self.frequency = frequency;

        let divisor = PIT_NUMERATOR / frequency;

        port_byte_out(0x43, 0x36);

        let l = divisor & 0xFF;
        let h = (divisor >> 8) & 0xFF;

        port_byte_out(0x40, l as u8);
        port_byte_out(0x40, h as u8);

    }

    pub fn timer_callback(&self, _stack_frame: InterruptStackFrame) {

        let r = self.frequency.checked_add(1);
        if r.is_none() {
            panic!("Timer overflow")
        }

        print!(".");
    }

}

lazy_static::lazy_static!{
    pub static ref PIT_TIMER: spin::Mutex<PIT> = spin::Mutex::new(
        PIT::new(100)
    );
}