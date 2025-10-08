use x86_64::structures::idt::InterruptStackFrame;
use crate::stdio::ports::port_byte_in;

use crate::print;

#[inline]
pub fn keyboard_callback(_stack_frame: InterruptStackFrame, sc: u8) {

    let vk = VirtualKey::convert_ps2_scancode_to_virtualkey(sc);

    print!("{}", vk.convert_virtual_key_to_char());

}

pub fn get_input() -> VirtualKey {

    loop {

        while (port_byte_in(0x64) & 1) == 0 {}

        let sc = port_byte_in(0x60);

        // not keyup events
        if sc & 0x80 != 0 {
            continue;
        }

        if sc != 0 {
            return VirtualKey::convert_ps2_scancode_to_virtualkey(sc);
        }
    }
}

pub enum VirtualKey {
    Undefined = 0x00,  // Undefined key
    LButton = 0x01,  // Left mouse button
    RButton = 0x02,  // Right mouse button
    Cancel = 0x03,  // Control-break processing
    MButton = 0x04,  // Middle mouse button (three-button mouse)
    XButton1 = 0x05,  // X1 mouse button
    XButton2 = 0x06,  // X2 mouse button

    Back = 0x08,  // BACKSPACE key
    Tab = 0x09,  // TAB key

    Clear = 0x0C,  // CLEAR key
    Return = 0x0D,  // ENTER key

    Shift = 0x10,  // SHIFT key
    Control = 0x11,  // CTRL key
    Menu = 0x12,  // ALT key
    Pause = 0x13,  // PAUSE key
    Capital = 0x14,  // CAPS LOCK key
    Kana = 0x15,  // IME Kana mode
    ImeOn = 0x16,  // IME On
    Junja = 0x17,  // IME Junja mode
    Final = 0x18,  // IME final mode
    Hanja = 0x19,  // IME Hanja mode
    ImeOff = 0x1A,  // IME Off
    Escape = 0x1B,  // ESC key
    Convert = 0x1C,  // IME convert
    NonConvert = 0x1D,  // IME nonconvert
    Accept = 0x1E,  // IME accept
    ModeChange = 0x1F,  // IME mode change
    Space = 0x20,  // SPACEBAR
    Prior = 0x21,  // PAGE UP key
    Next = 0x22,  // PAGE DOWN key
    End = 0x23,  // END key
    Home = 0x24,  // HOME key
    Left = 0x25,  // LEFT ARROW key
    Up = 0x26,  // UP ARROW key
    Right = 0x27,  // RIGHT ARROW key
    Down = 0x28,  // DOWN ARROW key
    Select = 0x29,  // SELECT key
    Print = 0x2A,  // PRINT key
    Execute = 0x2B,  // EXECUTE key
    Snapshot = 0x2C,  // PRINT SCREEN key
    Insert = 0x2D,  // INS key
    Delete = 0x2E,  // DEL key
    Help = 0x2F,  // HELP key
    Key0 = 0x30,  // 0 key
    Key1 = 0x31,  // 1 key
    Key2 = 0x32,  // 2 key
    Key3 = 0x33,  // 3 key
    Key4 = 0x34,  // 4 key
    Key5 = 0x35,  // 5 key
    Key6 = 0x36,  // 6 key
    Key7 = 0x37,  // 7 key
    Key8 = 0x38,  // 8 key
    Key9 = 0x39,  // 9 key
    A = 0x41,  // A key
    B = 0x42,  // B key
    C = 0x43,  // C key
    D = 0x44,  // D key
    E = 0x45,  // E key
    F = 0x46,  // F key
    G = 0x47,  // G key
    H = 0x48,  // H key
    I = 0x49,  // I key
    J = 0x4A,  // J key
    K = 0x4B,  // K key
    L = 0x4C,  // L key
    M = 0x4D,  // M key
    N = 0x4E,  // N key
    O = 0x4F,  // O key
    P = 0x50,  // P key
    Q = 0x51,  // Q key
    R = 0x52,  // R key
    S = 0x53,  // S key
    T = 0x54,  // T key
    U = 0x55,  // U key
    V = 0x56,  // V key
    W = 0x57,  // W key
    X = 0x58,  // X key
    Y = 0x59,  // Y key
    Z = 0x5A,  // Z key
    LSuper = 0x5B,  // Left Super key
    RSuper = 0x5C,  // Right Super key
    Apps = 0x5D,  // Applications key
    Sleep = 0x5F,  // Computer Sleep key
    Numpad0 = 0x60,  // Numeric keypad 0 key
    Numpad1 = 0x61,  // Numeric keypad 1 key
    Numpad2 = 0x62,  // Numeric keypad 2 key
    Numpad3 = 0x63,  // Numeric keypad 3 key
    Numpad4 = 0x64,  // Numeric keypad 4 key
    Numpad5 = 0x65,  // Numeric keypad 5 key
    Numpad6 = 0x66,  // Numeric keypad 6 key
    Numpad7 = 0x67,  // Numeric keypad 7 key
    Numpad8 = 0x68,  // Numeric keypad 8 key
    Numpad9 = 0x69,  // Numeric keypad 9 key
    Multiply = 0x6A,  // Multiply key
    Add = 0x6B,  // Add key
    Separator = 0x6C,  // Separator key
    Subtract = 0x6D,  // Subtract key
    Decimal = 0x6E,  // Decimal key
    Divide = 0x6F,  // Divide key
    F1 = 0x70,  // F1 key
    F2 = 0x71,  // F2 key
    F3 = 0x72,  // F3 key
    F4 = 0x73,  // F4 key
    F5 = 0x74,  // F5 key
    F6 = 0x75,  // F6 key
    F7 = 0x76,  // F7 key
    F8 = 0x77,  // F8 key
    F9 = 0x78,  // F9 key
    F10 = 0x79,  // F10 key
    F11 = 0x7A,  // F11 key
    F12 = 0x7B,  // F12 key
    F13 = 0x7C,  // F13 key
    F14 = 0x7D,  // F14 key
    F15 = 0x7E,  // F15 key
    F16 = 0x7F,  // F16 key
    F17 = 0x80,  // F17 key
    F18 = 0x81,  // F18 key
    F19 = 0x82,  // F19 key
    F20 = 0x83,  // F20 key
    F21 = 0x84,  // F21 key
    F22 = 0x85,  // F22 key
    F23 = 0x86,  // F23 key
    F24 = 0x87,  // F24 key
    NumLock = 0x90,  // NUM LOCK key
    Scroll = 0x91,  // SCROLL LOCK key
    LShift = 0xA0,  // Left SHIFT key
    RShift = 0xA1,  // Right SHIFT key
    LControl = 0xA2,  // Left CONTROL key
    RControl = 0xA3,  // Right CONTROL key
    LMenu = 0xA4,  // Left MENU key alt
    RMenu = 0xA5,  // Right MENU key alt

    Oem1 = 0xBA,  // For the US standard keyboard, the ';:' key
    OemPlus = 0xBB,  // For any country/region, the '+' key
    OemComma = 0xBC,  // For any country/region, the ',' key
    OemMinus = 0xBD,  // For any country/region, the '-' key
    OemPeriod = 0xBE,  // For any country/region, the '.' key
    Oem2 = 0xBF,  // For the US standard keyboard, the '/?' key
    Oem3 = 0xC0,  // For the US standard keyboard, the '`~' key
    Oem4 = 0xDB,  // For the US standard keyboard, the '[{' key
    Oem5 = 0xDC,  // For the US standard keyboard, the '\|' key
    Oem6 = 0xDD,  // For the US standard keyboard, the ']}' key
    Oem7 = 0xDE,  // For the US standard keyboard, the 'single-quote/double-quote' key
    Oem8 = 0xDF,  // Used for miscellaneous characters; it can vary by keyboard.
    Oem102 = 0xE2,  // Either the angle bracket key or the backslash key on the RT 102-key keyboard
    ProcessKey = 0xE5,  // IME PROCESS key
    Packet = 0xE7,  // Used to pass Unicode characters as if they were keystrokes.
}

impl VirtualKey {

    pub fn convert_virtual_key_to_char(self) -> char {

        match self {
            VirtualKey::A => 'a',
            VirtualKey::B => 'b',
            VirtualKey::C => 'c',
            VirtualKey::D => 'd',
            VirtualKey::E => 'e',
            VirtualKey::F => 'f',
            VirtualKey::G => 'g',
            VirtualKey::H => 'h',
            VirtualKey::I => 'i',
            VirtualKey::J => 'j',
            VirtualKey::K => 'k',
            VirtualKey::L => 'l',
            VirtualKey::M => 'm',
            VirtualKey::N => 'n',
            VirtualKey::O => 'o',
            VirtualKey::P => 'p',
            VirtualKey::Q => 'q',
            VirtualKey::R => 'r',
            VirtualKey::S => 's',
            VirtualKey::T => 't',
            VirtualKey::U => 'u',
            VirtualKey::V => 'v',
            VirtualKey::W => 'w',
            VirtualKey::X => 'x',
            VirtualKey::Y => 'y',
            VirtualKey::Z => 'z',
            VirtualKey::Space => ' ',
            _ => '\0'
        }

    }

}

impl VirtualKey {
    pub fn convert_ps2_scancode_to_virtualkey(sc: u8) -> VirtualKey {
        return match sc {
            0x01 => VirtualKey::Escape,
            0x02 => VirtualKey::Key1,
            0x03 => VirtualKey::Key2,
            0x04 => VirtualKey::Key3,
            0x05 => VirtualKey::Key4,
            0x06 => VirtualKey::Key5,
            0x07 => VirtualKey::Key6,
            0x08 => VirtualKey::Key7,
            0x09 => VirtualKey::Key8,
            0x0A => VirtualKey::Key9,
            0x0B => VirtualKey::Key0,
            0x0C => VirtualKey::OemMinus,
            0x0D => VirtualKey::OemPlus,
            0x0E => VirtualKey::Back,
            0x0F => VirtualKey::Tab,
            0x10 => VirtualKey::Q,
            0x11 => VirtualKey::W,
            0x12 => VirtualKey::E,
            0x13 => VirtualKey::R,
            0x14 => VirtualKey::T,
            0x15 => VirtualKey::Y,
            0x16 => VirtualKey::U,
            0x17 => VirtualKey::I,
            0x18 => VirtualKey::O,
            0x19 => VirtualKey::P,
            0x1A => VirtualKey::Oem4,
            0x1B => VirtualKey::Oem6,
            0x1C => VirtualKey::Return,
            0x1D => VirtualKey::LControl,
            0x1E => VirtualKey::A,
            0x1F => VirtualKey::S,
            0x20 => VirtualKey::D,
            0x21 => VirtualKey::F,
            0x22 => VirtualKey::G,
            0x23 => VirtualKey::H,
            0x24 => VirtualKey::J,
            0x25 => VirtualKey::K,
            0x26 => VirtualKey::L,
            0x27 => VirtualKey::Oem1,
            0x28 => VirtualKey::Oem7,
            0x29 => VirtualKey::Oem3,
            0x2A => VirtualKey::LShift,
            0x2B => VirtualKey::Oem5,
            0x2C => VirtualKey::Z,
            0x2D => VirtualKey::X,
            0x2E => VirtualKey::C,
            0x2F => VirtualKey::V,
            0x30 => VirtualKey::B,
            0x31 => VirtualKey::N,
            0x32 => VirtualKey::M,
            0x33 => VirtualKey::OemComma,
            0x34 => VirtualKey::OemPeriod,
            0x35 => VirtualKey::Oem2,
            0x36 => VirtualKey::RShift,
            0x37 => VirtualKey::Multiply,
            0x38 => VirtualKey::LMenu,
            0x39 => VirtualKey::Space,
            0x3A => VirtualKey::Capital,
            0x3B => VirtualKey::F1,
            0x3C => VirtualKey::F2,
            0x3D => VirtualKey::F3,
            0x3E => VirtualKey::F4,
            0x3F => VirtualKey::F5,
            0x40 => VirtualKey::F6,
            0x41 => VirtualKey::F7,
            0x42 => VirtualKey::F8,
            0x43 => VirtualKey::F9,
            0x44 => VirtualKey::F10,
            0x45 => VirtualKey::NumLock,
            0x46 => VirtualKey::Scroll,
            0x47 => VirtualKey::Home,
            0x48 => VirtualKey::Up,
            0x49 => VirtualKey::Prior,
            0x4B => VirtualKey::Left,
            0x4D => VirtualKey::Right,
            0x4F => VirtualKey::End,
            0x50 => VirtualKey::Down,
            0x51 => VirtualKey::Next,
            0x52 => VirtualKey::Insert,
            0x53 => VirtualKey::Delete,
            0x54 => VirtualKey::Oem102,
            0x56 => VirtualKey::F11,
            0x57 => VirtualKey::F12,
            0x58 => VirtualKey::F13,
            0x59 => VirtualKey::F14,
            0x5A => VirtualKey::F15,
            0x5B => VirtualKey::F16,
            0x5C => VirtualKey::F17,
            0x5D => VirtualKey::F18,
            0x5E => VirtualKey::F19,
            0x5F => VirtualKey::F20,
            0x60 => VirtualKey::F21,
            0x61 => VirtualKey::F22,
            0x62 => VirtualKey::F23,
            0x63 => VirtualKey::F24,
            0x6B => VirtualKey::Oem4,
            0x6C => VirtualKey::Oem6,
            0x6D => VirtualKey::Oem1,
            0x6E => VirtualKey::Oem7,
            0x6F => VirtualKey::Oem3,
            0x70 => VirtualKey::Oem5,
            0x71 => VirtualKey::OemComma,
            0x72 => VirtualKey::OemPeriod,
            0x73 => VirtualKey::Oem2,
            0x74 => VirtualKey::Oem102,
            0x75 => VirtualKey::ProcessKey,
            _ => VirtualKey::Undefined,
        }
    }
}