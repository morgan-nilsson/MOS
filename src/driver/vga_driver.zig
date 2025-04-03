const stdio = @import("stdio");

const VGA_CTRL_REG = 0x3d4;
const VGA_DATA_REG = 0x3d5;
const VGA_OFFSET_LOW = 0x0f;
const VGA_OFFSET_HIGH = 0x0e;

const VGA_ADDRESS = 0xB8000;
const VGA_MAX_ROWS = 25;
const VGA_MAX_COLUMNS = 80;

const WHITE_ON_BLACK = 0x0f;

pub fn set_cursor(offset: u32) void {
    const real_offset = offset / 2;

    stdio.port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);
    stdio.port_byte_out(VGA_DATA_REG, @intCast(real_offset >> 8));

    stdio.port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);
    stdio.port_byte_out(VGA_DATA_REG, @intCast(real_offset & 0xFF));
}
