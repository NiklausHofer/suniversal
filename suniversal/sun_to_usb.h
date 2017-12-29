/*
    keymap - SUN to USB scan codes table
    Copyright (c) 2017, Alexander Vollschwitz

    This file is part of suniversal.

    suniversal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    suniversal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with suniversal. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SUN_TO_USB_h
#define SUN_TO_USB_h

#include "suniversal.h"
#include "usb_codes.h"
#include "macros.h"

#if USE_MACROS == true
#define CODE_OR_MACRO(C, M) (0xFF00 + (M))
#else
#define CODE_OR_MACRO(C, M) (C)
#endif

/*
    scan code lookup table:

     - index is SUN scan code

     - value at index is USB scan code, modifier, or macro ID, as 16 bit
       unsigned int, according to this scheme:

        - non-modifiers are stored on low byte, with high byte 0
        - modifiers are stored in high byte, with low byte 0
        - 0xFF in high byte indicates that a macro is to be used, low
          byte in this case is the ID of the macro

    The scan codes are according to the keyboard documentation. Note that
    while the documentation lists two scan sets - US and International -
    we really just need one table here. As the documentation states, the
    scan sets are about key-switch to scan code assignments, but we're
    concerned with translating scan codes. From all I can tell, those are
    the same in each of the two scan sets.
 */
static const uint16_t sun2usb[128] = {
/*  scan                                        */
/*  code    meaning          translation to USB */
/*  --------------------------------------------*/
/*  0x00                    */  0,
/*  0x01    Stop            */  CODE_OR_MACRO(USB_STOP, MACRO_STOP),
/*  0x02    Volume_Decr     */  USB_VOLUMEDOWN,
/*  0x03    Again           */  CODE_OR_MACRO(USB_AGAIN, MACRO_AGAIN),
/*  0x04    Volume_Incr     */  USB_VOLUMEUP,
/*  0x05    F1              */  USB_F1,
/*  0x06    F2              */  USB_F2,
/*  0x07    F10             */  USB_F10,
/*  0x08    F3              */  USB_F3,
/*  0x09    F11             */  USB_F11,
/*  0x0A    F4              */  USB_F4,
/*  0x0B    F12             */  USB_F12,
/*  0x0C    F5              */  USB_F5,
/*  0x0D    GraphAlt        */  USB_MOD_RALT << 8,
/*  0x0E    F6              */  USB_F6,
/*  0x0F                    */  0,
/*  0x10    F7              */  USB_F7,
/*  0x11    F8              */  USB_F8,
/*  0x12    F9              */  USB_F9,
/*  0x13    Alt_L           */  USB_MOD_LALT << 8,
/*  0x14    T5_Up           */  USB_UP,
/*  0x15    Pause           */  USB_PAUSE,
/*  0x16    Pr_Sc           */  USB_SYSRQ,
/*  0x17    Break/ScrollLock*/  USB_SCROLLLOCK,
/*  0x18    T5_Left         */  USB_LEFT,
/*  0x19    Props           */  CODE_OR_MACRO(USB_PROPS, MACRO_PROPS),
/*  0x1A    Undo            */  CODE_OR_MACRO(USB_UNDO, MACRO_UNDO),
/*  0x1B    T5_Down         */  USB_DOWN,
/*  0x1C    T5_Right        */  USB_RIGHT,
/*  0x1D    Esc             */  USB_ESC,
/*  0x1E    1_!             */  USB_1,
/*  0x1F    2_@             */  USB_2,
/*  0x20    3_#             */  USB_3,
/*  0x21    4_$             */  USB_4,
/*  0x22    5_%             */  USB_5,
/*  0x23    6_^             */  USB_6,
/*  0x24    7_&             */  USB_7,
/*  0x25    8_*             */  USB_8,
/*  0x26    9_(             */  USB_9,
/*  0x27    0_)             */  USB_0,
/*  0x28    -__             */  USB_MINUS,
/*  0x29    =_+             */  USB_EQUAL,
/*  0x2A    ‘_~             */  USB_APOSTROPHE,
/*  0x2B    Backspace       */  USB_BACKSPACE,
/*  0x2C    T5_Insert       */  USB_INSERT,
/*  0x2D    =               */  USB_MUTE,
/*  0x2E    /               */  USB_KPSLASH,
/*  0x2F    *               */  USB_KPASTERISK,
/*  0x30    Power           */  USB_POWER,
/*  0x31    Front           */  CODE_OR_MACRO(USB_FRONT, MACRO_FRONT),
/*  0x32    Del_.           */  USB_KPDOT,
/*  0x33    Copy            */  CODE_OR_MACRO(USB_COPY, MACRO_COPY),
/*  0x34    T5_Home         */  USB_HOME,
/*  0x35    Tab             */  USB_TAB,
/*  0x36    Q               */  USB_Q,
/*  0x37    W               */  USB_W,
/*  0x38    E               */  USB_E,
/*  0x39    R               */  USB_R,
/*  0x3A    T               */  USB_T,
/*  0x3B    Y               */  USB_Y,
/*  0x3C    U               */  USB_U,
/*  0x3D    I               */  USB_I,
/*  0x3E    O               */  USB_O,
/*  0x3F    P               */  USB_P,
/*  0x40    [_{             */  USB_LEFTBRACE,
/*  0x41    ]_}             */  USB_RIGHTBRACE,
/*  0x42    Delete          */  USB_DELETE,
/*  0x43    Compose         */  USB_COMPOSE,
/*  0x44    Home_7          */  USB_KP7,
/*  0x45    up-cur_8        */  USB_KP8,
/*  0x46    PgUp_9          */  USB_KP9,
/*  0x47    -               */  USB_KPMINUS,
/*  0x48    Open            */  CODE_OR_MACRO(USB_OPEN, MACRO_OPEN),
/*  0x49    Paste           */  CODE_OR_MACRO(USB_COPY, MACRO_PASTE),
/*  0x4A    T5_End          */  USB_END,
/*  0x4B                    */  0,
/*  0x4C    Ctrl_L          */  USB_MOD_LCTRL << 8,
/*  0x4D    A               */  USB_A,
/*  0x4E    S               */  USB_S,
/*  0x4F    D               */  USB_D,
/*  0x50    F               */  USB_F,
/*  0x51    G               */  USB_G,
/*  0x52    H               */  USB_H,
/*  0x53    J               */  USB_J,
/*  0x54    K               */  USB_K,
/*  0x55    L               */  USB_L,
/*  0x56    ;_:             */  USB_SEMICOLON,
/*  0x57    ‘_“             */  USB_APOSTROPHE,
/*  0x58    \               */  USB_BACKSLASH,
/*  0x59    Return          */  USB_ENTER,
/*  0x5A    Enter           */  USB_ENTER,
/*  0x5B    Left-Cur_4      */  USB_KP4,
/*  0x5C    5               */  USB_KP5,
/*  0x5D    Right-Cur_6     */  USB_KP6,
/*  0x5E    Ins_0           */  USB_KP0,
/*  0x5F    Find            */  CODE_OR_MACRO(USB_FIND, MACRO_FIND),
/*  0x60    T5_PgUp         */  USB_PAGEUP,
/*  0x61    Cut             */  CODE_OR_MACRO(USB_CUT, MACRO_CUT),
/*  0x62    Num_Lock        */  USB_NUMLOCK,
/*  0x63    Shift_L         */  USB_MOD_LSHIFT << 8,
/*  0x64    Z               */  USB_Z,
/*  0x65    X               */  USB_X,
/*  0x66    C               */  USB_C,
/*  0x67    V               */  USB_V,
/*  0x68    B               */  USB_B,
/*  0x69    N               */  USB_N,
/*  0x6A    M               */  USB_M,
/*  0x6B    ,_<             */  USB_COMMA,
/*  0x6C    ._>             */  USB_DOT,
/*  0x6D    /_?             */  USB_SLASH,
/*  0x6E    Shift_R         */  USB_MOD_RSHIFT << 8,
/*  0x6F    Line_Feed       */  0,
/*  0x70    End_1           */  USB_KP1,
/*  0x71    Dn-Cur_2        */  USB_KP2,
/*  0x72    PgDn_3          */  USB_KP3,
/*  0x73                    */  0,
/*  0x74                    */  0,
/*  0x75                    */  0,
/*  0x76    Help            */  CODE_OR_MACRO(USB_HELP, MACRO_HELP),
/*  0x77    CapsLock        */  USB_CAPSLOCK,
/*  0x78    L-Triangle      */  USB_MOD_LMETA << 8,
/*  0x79    SpaceBar        */  USB_SPACE,
/*  0x7A    R-triangle      */  USB_MOD_RMETA << 8,
/*  0x7B    T5_PgDn         */  USB_PAGEDOWN,
/*  0x7C    <_>_|           */  USB_102ND,
/*  0x7D    +               */  USB_KPPLUS,
/*  0x7E                    */  0,
/*  0x7F                    */  0
};

#endif
