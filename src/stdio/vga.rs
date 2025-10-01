use crate::stdio::ports;
use core::{ptr::copy_nonoverlapping};
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


pub struct VGAWriter {
    cursor_offset: usize,
    color_code: ColorCode,
    vga_buffer: &'static mut Buffer,

}

impl core::fmt::Write for VGAWriter {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        self.write_string(s).map_err(|_| core::fmt::Error)
    }
}

impl VGAWriter {
    fn new(vga_buffer: &'static mut Buffer) -> Self {
        Self { 
            vga_buffer,
            cursor_offset: 0, 
            color_code: ColorCode::new(Color::White, Color::Black)
        }
    }

    #[inline]
    pub fn set_cursor(&mut self, offset: usize) {

        self.cursor_offset = offset;

        let offset = offset / 2;

        ports::port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);
        ports::port_byte_out(VGA_DATA_REG, (offset >> 8) as u8);
        ports::port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);
        ports::port_byte_out(VGA_DATA_REG, (offset & 0xFF) as u8);
    }

    #[inline]
    pub fn get_cursor(&self) -> usize {

        self.cursor_offset

    }

    #[inline]
    pub fn set_color_code(&mut self, color_code: ColorCode) {
        self.color_code = color_code;
    }

    #[inline]
    pub fn calculate_row_from_offset(offset: usize) -> usize {
        offset / (2 * VGA_MAX_COLUMNS)
    }

    #[inline]
    pub fn calculate_column_from_offset(offset: usize) -> usize {
        offset / 2 % VGA_MAX_COLUMNS
    }

    #[inline]
    pub fn calculate_offset(column: usize, row: usize) -> usize {
        2 * (row * VGA_MAX_COLUMNS + column)
    }

    #[inline]
    fn calculate_newline_offset(offset: usize) -> usize {
        Self::calculate_offset(0, Self::calculate_row_from_offset(offset) + 1)
    }

    fn set_char_at_offset(&mut self, c: u8, offset: usize) {

        let row = Self::calculate_row_from_offset(offset);
        let column = Self::calculate_column_from_offset(offset);
        self.vga_buffer.rows[row][column].write(ScreenChar {
            ascii_character: c,
            color_code: self.color_code,
        });
    }

    fn scroll_line(&mut self, offset: usize) -> usize {

        unsafe {
            copy_nonoverlapping(
                self.vga_buffer.rows[1].as_ptr(),
                self.vga_buffer.rows[0].as_mut_ptr(),
                VGA_MAX_COLUMNS,
            );
        }

        for i in 0..VGA_MAX_COLUMNS {
            Self::set_char_at_offset(self, ' ' as u8, Self::calculate_offset(i, VGA_MAX_ROWS - 1));
        }

        offset - 2 * VGA_MAX_COLUMNS
    }

    pub fn write_string(&mut self, s: &str) -> Result<(), VGAError> {

        let mut offset: usize = self.get_cursor();
        for i in 0..s.len() {

            if offset >= VGA_MAX_COLUMNS * VGA_MAX_ROWS * 2 {
                offset = self.scroll_line(offset);
            }

            let c = s.chars().nth(i);
            if c.is_none() {
                return Err(VGAError{});
            }
            let c = c.unwrap();

            if c == '\n' {

                offset = Self::calculate_newline_offset(offset)

            } else {

                self.set_char_at_offset(c as u8, offset);
                offset += 2;

            }

            self.set_cursor(offset);

        }

        Ok(())

    }

    pub fn clear_screen(&mut self) {

        for i in 0..(VGA_MAX_COLUMNS * VGA_MAX_ROWS) {
            self.set_char_at_offset(' ' as u8, i);
        }

        self.set_cursor(Self::calculate_offset(0, 0));

    }
}

#[derive(Debug, Clone)]
pub struct VGAError {}

impl core::fmt::Display for VGAError {
    fn fmt(&self, f: &mut core::fmt::Formatter) -> core::fmt::Result {
        write!(f, "invalid first item to double")
    }
}