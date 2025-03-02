#include "../../driver/include/keyboard.h"

/**
 * Translates a virtual key to a character.
 */
char Virtual_key_to_char(VirtualKey vkey) {
    switch (vkey) {
        case VK_1: return '1';
        case VK_2: return '2';
        case VK_3: return '3';
        case VK_4: return '4';
        case VK_5: return '5';
        case VK_6: return '6';
        case VK_7: return '7';
        case VK_8: return '8';
        case VK_9: return '9';
        case VK_0: return '0';

        case VK_A: return 'A';
        case VK_B: return 'B';
        case VK_C: return 'C';
        case VK_D: return 'D';
        case VK_E: return 'E';
        case VK_F: return 'F';
        case VK_G: return 'G';
        case VK_H: return 'H';
        case VK_I: return 'I';
        case VK_J: return 'J';
        case VK_K: return 'K';
        case VK_L: return 'L';
        case VK_M: return 'M';
        case VK_N: return 'N';
        case VK_O: return 'O';
        case VK_P: return 'P';
        case VK_Q: return 'Q';
        case VK_R: return 'R';
        case VK_S: return 'S';
        case VK_T: return 'T';
        case VK_U: return 'U';
        case VK_V: return 'V';
        case VK_W: return 'W';
        case VK_X: return 'X';
        case VK_Y: return 'Y';
        case VK_Z: return 'Z';

        case VK_SPACE: return ' ';
        case VK_RETURN: return '\n';
        case VK_BACK: return '\b';
        case VK_TAB: return '\t';

        case VK_OEM_1: return ';';
        case VK_OEM_PLUS: return '=';
        case VK_OEM_COMMA: return ',';
        case VK_OEM_MINUS: return '-';
        case VK_OEM_PERIOD: return '.';
        case VK_OEM_2: return '/';
        case VK_OEM_3: return '`';
        case VK_OEM_4: return '[';
        case VK_OEM_5: return '\\';
        case VK_OEM_6: return ']';
        case VK_OEM_7: return '\'';
        case VK_OEM_8: return '`';
        case VK_OEM_102: return '\\';

    }
}