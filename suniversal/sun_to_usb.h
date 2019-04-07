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

#include "config.h"
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

    Translations were set to the same USB scan codes that a SUN Type 7
    keyboard sends.
 */
static const uint16_t sun2usb[131] = {
/*  scan                                        */
/*  code    meaning          translation to USB */
/*  --------------------------------------------*/
/*  0x00                    */  0,
/* B 0x01    Stop            */  USB_F9,
/*  0x02    Volume_Decr     */  USB_VOLUMEDOWN,
/* B 0x03    Again           */  USB_F5,
/* B 0x04    Volume_Incr     */  USB_F3,
/* B 0x05    F1              */  USB_F1,
/* B 0x06    F2              */  USB_F2,
/* B 0x07    F10             */  USB_F12,
/*  0x08    F3              */  USB_F3,
/* B 0x09    F11             */  USB_F10,
/* B 0x0A    F4              */  USB_F8,
/* B 0x0B    F12             */  USB_F6,
/* B 0x0C    F5              */  USB_F4,
/* B 0x0D    GraphAlt        */  USB_TAB,
/* B 0x0E    F6              */  USB_GRAVE,
/*  0x0F                    */  0,
/*  0x10    F7              */  USB_F7,
/* B 0x11    F8              */  USB_MOD_LALT << 8,
/* B 0x12    F9              */  USB_MOD_LSHIFT << 8,
/*  0x13    Alt_L           */  USB_MOD_LALT << 8,
/* B 0x14    T5_Up           */  USB_MOD_LCTRL << 8,
/* B 0x15    Pause           */  USB_Q,
/* B 0x16    Pr_Sc           */  USB_1,
/*  0x17    Break/ScrollLock*/  USB_SCROLLLOCK,
/*  0x18    T5_Left         */  USB_LEFT,
/*  0x19    Props           */  CODE_OR_MACRO(USB_PROPS, MACRO_PROPS),
/* B 0x1A    Undo            */  USB_Z,
/* B 0x1B    T5_Down         */  USB_S,
/* B 0x1C    T5_Right        */  USB_A,
/* B 0x1D    Esc             */  USB_W,
/* B 0x1E    1_!             */  USB_2,
/*  0x1F    2_@             */  USB_2,
/*  0x20    3_#             */  USB_3,
/* B 0x21    4_$             */  USB_C,
/* B 0x22    5_%             */  USB_X,
/* B 0x23    6_^             */  USB_D,
/* B 0x24    7_&             */  USB_E,
/* B 0x25    8_*             */  USB_4,
/* B 0x26    9_(             */  USB_3,
/*  0x27    0_)             */  USB_0,
/*  0x28    -__             */  USB_MINUS,
/* B 0x29    =_+             */  USB_SPACE,
/* B 0x2A    ‘_~             */  USB_V,
/* B 0x2B    Backspace       */  USB_F,
/* B 0x2C    T5_Insert       */  USB_T,
/* B 0x2D    =               */  USB_R,
/* B 0x2E    /               */  USB_5,
/*  0x2F    *               */  USB_KPASTERISK,
/*  0x30    Power           */  USB_POWER,
/* B 0x31    Front           */  USB_N,
/* B 0x32    Del_.           */  USB_B,
/* B 0x33    Copy            */  USB_H,
/* B 0x34    T5_Home         */  USB_G,
/* B 0x35    Tab             */  USB_Y,
/* B 0x36    Q               */  USB_6,
/*  0x37    W               */  USB_W,
/* B 0x38    E               */  USB_F11,
/*  0x39    R               */  USB_R,
/* B 0x3A    T               */  USB_M,
/* B 0x3B    Y               */  USB_J,
/* B 0x3C    U               */  USB_U,
/*  0x3D    I               */  USB_7,
/*  0x3E    O               */  USB_8,
/*  0x3F    P               */  USB_P,
/* B 0x40    [_{             */  USB_LEFTMETA,
/* B 0x41    ]_}             */  USB_COMMA,
/* B 0x42    Delete          */  USB_K,
/* B 0x43    Compose         */  USB_I,
/* B 0x44    Home_7          */  USB_O,
/*  0x45    up-cur_8        */  USB_0,
/*  0x46    PgUp_9          */  USB_9,
/*  0x47    -               */  USB_KPMINUS,
/* B 0x48    Open            */  USB_RIGHTMETA,
/* B 0x49    Paste           */  USB_DOT,
/* B 0x4A    T5_End          */  USB_SLASH,
/* B 0x4B                    */  USB_L,
/* B 0x4C    Ctrl_L          */  USB_SEMICOLON,
/* B 0x4D    A               */  USB_P,
/* B 0x4E    S               */  USB_MINUS,
/*  0x4F    D               */  USB_D,
/*  0x50    F               */  USB_F,
/*  0x51    G               */  USB_G,
/*  0x52    H               */  USB_APOSTROPHE,
/*  0x53    J               */  USB_J,
/* B 0x54    K               */  USB_LEFTBRACE,
/* B 0x55    L               */  USB_EQUAL,
/*  0x56    ;_:             */  USB_SEMICOLON,
/*  0x57    ‘_“             */  USB_APOSTROPHE,
/* B 0x58    \               */  USB_CAPSLOCK,
/* B 0x59    Return          */  USB_MOD_RSHIFT << 8,
/* B 0x5A    Enter           */  USB_ENTER,
/* B 0x5B    Left-Cur_4      */  USB_RIGHTBRACE,
/*  0x5C    5               */  USB_KP5,
/* B 0x5D    Right-Cur_6     */  USB_BACKSLASH,
/*  0x5E    Ins_0           */  USB_KP0,
/*  0x5F    Find            */  CODE_OR_MACRO(USB_FIND, MACRO_FIND),
/*  0x60    T5_PgUp         */  USB_PAGEUP,
/*  0x61    Cut             */  CODE_OR_MACRO(USB_CUT, MACRO_CUT),
/*  0x62    Num_Lock        */  USB_NUMLOCK,
/*  0x63    Shift_L         */  USB_MOD_LSHIFT << 8,
/*  0x64    Z               */  USB_Z,
/*  0x65    X               */  USB_X,
/* B 0x66    C               */  USB_BACKSPACE,
/*  0x67    V               */  USB_V,
/*  0x68    B               */  USB_B,
/* B 0x69    N               */  USB_KP1,
/*  0x6A    M               */  USB_M,
/* B 0x6B    ,_<             */  USB_KP4,
/* B 0x6C    ._>             */  USB_KP7,
/*  0x6D    /_?             */  USB_SLASH,
/*  0x6E    Shift_R         */  USB_MOD_RSHIFT << 8,
/*  0x6F    Line_Feed       */  0,
/* B 0x70    End_1           */  USB_KP0,
/*  0x71    Dn-Cur_2        */  USB_KP2,
/* B 0x72    PgDn_3          */  USB_KP2,
/* B 0x73                    */  USB_KP5,
/* B 0x74                    */  USB_KP6,
/* B 0x75                    */  USB_KP8,
/* B 0x76    Help            */  USB_ESC,
/* B 0x77    CapsLock        */  USB_NUMLOCK,
/*  0x78    L-Triangle      */  USB_F11,
/*  0x79    SpaceBar        */  USB_SPACE,
/* B 0x7A    R-triangle      */  USB_KP3,
/*  0x7B    T5_PgDn         */  USB_PAGEDOWN,
/*  0x7C    <_>_|           */  USB_102ND,
/* B 0x7D    +               */  USB_KP9,
/*  0x7E                    */  0,
/*  0x7F                    */  0,
/*  0x81                    */  0,
/*  0x82                    */  0,
/* B 0x83                    */ USB_F7,
};

#endif
