use crate::stdio::ports;
use core::{ptr::copy};
use lazy_static::lazy_static;
use spin::Mutex;
use volatile::Volatile;

const VGA_CTRL_REG: u16 = 0x3d4;
const VGA_DATA_REG: u16 = 0x3d5;
const VGA_OFFSET_LOW: u8 = 0x0f;
const VGA_OFFSET_HIGH: u8 = 0x0e;

const VGA_BUFFER_START: usize = 0xB8000;

const VGA_MAX_ROWS: usize = 25;
const VGA_MAX_COLUMNS: usize = 80;

lazy_static! {
    pub static ref VGA_WRITER: Mutex<VGAWriter> = Mutex::new(VGAWriter::new(
        unsafe {
            &mut *(VGA_BUFFER_START as *mut Buffer)
        }
    ));
}

#[macro_export]
macro_rules! print {
    ($($arg:tt)*) => ($crate::stdio::vga::_print(format_args!($($arg)*)));
}

#[macro_export]
macro_rules! println {
    () => ($crate::print!("\n"));
    ($($arg:tt)*) => ($crate::print!("{}\n", format_args!($($arg)*)));
}

#[doc(hidden)]
pub fn _print(args: core::fmt::Arguments) {
    use core::fmt::Write;
    VGA_WRITER.lock().write_fmt(args).unwrap();
}

#[allow(dead_code)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum Color {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    Pink = 13,
    Yellow = 14,
    White = 15,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(transparent)]
pub struct ColorCode(u8);

impl ColorCode {
    fn new(foreground: Color, background: Color) -> ColorCode {
        ColorCode((background as u8) << 4 | (foreground as u8))
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(C)]
struct ScreenChar {
    ascii_character: u8,
    color_code: ColorCode,
}

#[repr(transparent)]
struct Buffer {
    rows: [[Volatile<ScreenChar>; VGA_MAX_COLUMNS]; VGA_MAX_ROWS],
}

#[derive(Debug, Clone)]
pub struct VGAError {}

impl core::fmt::Display for VGAError {
    fn fmt(&self, f: &mut core::fmt::Formatter) -> core::fmt::Result {
        write!(f, "Error writing to VGA buffer")
    }
}

pub struct VGAWriter {
    cursor_location: (usize, usize),
    color_code: ColorCode,
    vga_buffer: &'static mut Buffer,

}

impl VGAWriter {
    fn new(vga_buffer: &'static mut Buffer) -> Self {
        Self { 
            vga_buffer,
            cursor_location: (0, 0), 
            color_code: ColorCode::new(Color::White, Color::Black)
        }
    }

    #[inline]
    pub fn set_cursor(&mut self, location: (usize, usize)) {

        self.cursor_location = location;

        let offset = (location.0 * VGA_MAX_COLUMNS) + location.1;

        ports::port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);
        ports::port_byte_out(VGA_DATA_REG, (offset >> 8) as u8);
        ports::port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);
        ports::port_byte_out(VGA_DATA_REG, (offset & 0xFF) as u8);
    }

    #[inline]
    pub fn get_cursor(&self) -> (usize, usize) {

        self.cursor_location

    }

    #[inline]
    pub fn set_color_code(&mut self, color_code: ColorCode) {
        self.color_code = color_code;
    }

    fn set_char_at_offset(&mut self, c: u8, location: (usize, usize)) {

        self.vga_buffer.rows[location.0][location.1].write(ScreenChar {
            ascii_character: c,
            color_code: self.color_code,
        });
    }

    fn scroll_line(&mut self) -> (usize, usize) {

        unsafe {
            copy (
                self.vga_buffer.rows[1].as_ptr(),
                self.vga_buffer.rows[0].as_mut_ptr(),
                VGA_MAX_COLUMNS * (VGA_MAX_ROWS - 1),
            );
        }

        self.fill_row(VGA_MAX_ROWS - 1, ' ' as u8);

        self.set_cursor((self.cursor_location.0 - 1, self.cursor_location.1 % VGA_MAX_COLUMNS));

        return self.cursor_location;
    }

    pub fn write_string(&mut self, s: &str) -> Result<(), VGAError> {

        let mut cursor_location: (usize, usize) = self.get_cursor();
        for i in 0..s.len() {

            if cursor_location.1 == VGA_MAX_COLUMNS {
                cursor_location.0 += 1;
                cursor_location.1 = 0;
            }

            if cursor_location.0 >= VGA_MAX_ROWS {
                cursor_location = self.scroll_line();
            }

            let c = s.chars().nth(i);
            if c.is_none() {
                return Err(VGAError{});
            }
            let c = c.unwrap();

            if c == '\n' {

                cursor_location.0 += 1;
                cursor_location.1 = 0;

            } else {

                self.set_char_at_offset(c as u8, cursor_location);
                cursor_location.1 += 1;

            }

            self.set_cursor(cursor_location);

        }

        Ok(())

    }

    pub fn clear_screen(&mut self) {

        for row in 0..VGA_MAX_ROWS {
            self.fill_row(row, ' ' as u8);
        }

        self.set_cursor((0, 0));
    }

    fn fill_row(&mut self, row: usize, c: u8) {

        for col in 0..VGA_MAX_COLUMNS {
            self.set_char_at_offset(c, (row, col));
        }

    }
}

impl core::fmt::Write for VGAWriter {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        self.write_string(s).map_err(|_| core::fmt::Error)
    }
}

#[cfg(test)]
fn get_screen_char_at_location(location: (usize, usize)) -> ScreenChar {
    unsafe {
        (*(VGA_BUFFER_START as *const Buffer)).rows[location.0][location.1].read()
    }
}

#[cfg(test)]
fn get_cursor() -> (usize, usize) {
    ports::port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);

    let mut offset = (ports::port_byte_in(VGA_DATA_REG) as usize) << 8;
    ports::port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);

    offset += ports::port_byte_in(VGA_DATA_REG as u16) as usize;

    return (offset / VGA_MAX_COLUMNS, offset % VGA_MAX_COLUMNS);
}

#[test_case]
fn test_get_set_cursor() {
    
    VGA_WRITER.lock().set_cursor((5, 5));
    assert_eq!(VGA_WRITER.lock().get_cursor(), (5, 5));
    assert_eq!(get_cursor(), (5, 5));

    VGA_WRITER.lock().set_cursor((0, 0));
    assert_eq!(VGA_WRITER.lock().get_cursor(), (0, 0));
    assert_eq!(get_cursor(), (0, 0));

    VGA_WRITER.lock().set_cursor((24, 79));
    assert_eq!(VGA_WRITER.lock().get_cursor(), (24, 79));
    assert_eq!(get_cursor(), (24, 79));
}

#[test_case]
fn test_write_char_at() {

    VGA_WRITER.lock().clear_screen();

    VGA_WRITER.lock().set_char_at_offset('a' as u8, (0, 0));
    assert_eq!(get_screen_char_at_location((0, 0)).ascii_character, 'a' as u8);

    VGA_WRITER.lock().set_char_at_offset('b' as u8, (5, 5));
    assert_eq!(get_screen_char_at_location((5, 5)).ascii_character, 'b' as u8);

    VGA_WRITER.lock().set_char_at_offset('c' as u8, (24, 79));
    assert_eq!(get_screen_char_at_location((24, 79)).ascii_character, 'c' as u8);
}

#[test_case]
fn test_clear_screen() {

    VGA_WRITER.lock().clear_screen();

    for row in 0..VGA_MAX_ROWS {
        for col in 0..VGA_MAX_COLUMNS {
            assert_eq!(get_screen_char_at_location((row, col)).ascii_character, ' ' as u8);
        }
    }

}

#[test_case]
fn test_write_string() {

    // write a string thay fits on one line
    VGA_WRITER.lock().clear_screen();

    VGA_WRITER.lock().write_string("Hello, World!").unwrap();

    for (i, c) in "Hello, World!".chars().enumerate() {
        assert_eq!(get_screen_char_at_location((0, i)).ascii_character, c as u8);
    }

    // write a string that has a newline
    VGA_WRITER.lock().clear_screen();

    VGA_WRITER.lock().write_string("Hello,\nWorld!").unwrap();
    for (i, c) in "Hello,".chars().enumerate() {
        assert_eq!(get_screen_char_at_location((0, i)).ascii_character, c as u8);
    }
    for (i, c) in "World!".chars().enumerate() {
        assert_eq!(get_screen_char_at_location((1, i)).ascii_character, c as u8);
    }

    // write a string that exceeds one line
    VGA_WRITER.lock().clear_screen();
    let long_string = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";

    VGA_WRITER.lock().write_string(long_string).unwrap();

    for (i, c) in long_string.chars().enumerate() {
        assert_eq!(get_screen_char_at_location((i / VGA_MAX_COLUMNS, i % VGA_MAX_COLUMNS)).ascii_character, c as u8);
    }

    // write a string that needs to be scrolled
    VGA_WRITER.lock().clear_screen();

    for i in 0..30 {
        println!("{}", i);
    }

    // first 5 lines should be scrolled off
    assert_eq!(get_screen_char_at_location((0, 0)).ascii_character, '5' as u8);

    assert_eq!(get_screen_char_at_location((VGA_MAX_ROWS - 1, 0)).ascii_character, '2' as u8);
    assert_eq!(get_screen_char_at_location((VGA_MAX_ROWS - 1, 1)).ascii_character, '9' as u8);

}