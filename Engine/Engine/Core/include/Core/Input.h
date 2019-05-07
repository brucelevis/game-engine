#pragma once

#include "SDL.h"
#include <stdbool.h>
#include "Math/Vec.h"

enum class MouseButton {
	None,
	Left,
	Middle,
	Right,

};

struct ButtonState {
	bool down;
	bool just_pressed;
	bool just_released;
};



struct Mouse {
	Vec2i pos;
	Vec2i delta_pos;

	Vec2i global_pos;
	Vec2i global_delta_pos;

	Vec2i scroll;

	ButtonState mouse_button_left;
	ButtonState mouse_button_middle;
	ButtonState mouse_button_right;

};


struct Input {
	Mouse mouse;
	// Store scancodes, but when querying if a key is pressed we'll use keycodes
	ButtonState keys[SDL_NUM_SCANCODES];
};


// Our keycodes are the same as the sdl keycodes for interop reasons
// Why didn't we just use SDK_Keycodes?
// Because it is nicer to have our own keycode enums so that in the future, if we want to remove sdl, we can still do it
// and game code and other systems so have to edit all their SDLK_.... codes everywhere.
enum Keycode {
	
    KEYCODE_UNKNOWN = SDLK_UNKNOWN,

    KEYCODE_RETURN = SDLK_RETURN,
	KEYCODE_ESCAPE = SDLK_ESCAPE,
	
    KEYCODE_BACKSPACE = SDLK_BACKSPACE,
    KEYCODE_TAB = SDLK_TAB,
    KEYCODE_SPACE = SDLK_SPACE,
    KEYCODE_EXCLAIM = SDLK_EXCLAIM,
    KEYCODE_QUOTEDBL = SDLK_QUOTEDBL,
    KEYCODE_HASH = SDLK_HASH,
    KEYCODE_PERCENT = SDLK_PERCENT,
    KEYCODE_DOLLAR = SDLK_DOLLAR,
    KEYCODE_AMPERSAND = SDLK_AMPERSAND,
    KEYCODE_QUOTE = SDLK_QUOTE,
    KEYCODE_LEFTPAREN = SDLK_LEFTPAREN,
    KEYCODE_RIGHTPAREN = SDLK_RIGHTPAREN,
    KEYCODE_ASTERISK = SDLK_ASTERISK,
    KEYCODE_PLUS = SDLK_PLUS,
    KEYCODE_COMMA = SDLK_COMMA,
    KEYCODE_MINUS = SDLK_MINUS,
    KEYCODE_PERIOD = SDLK_PERIOD,
    KEYCODE_SLASH = SDLK_SLASH,
    KEYCODE_0 = SDLK_0,
    KEYCODE_1 = SDLK_1,
    KEYCODE_2 = SDLK_2,
    KEYCODE_3 = SDLK_3,
    KEYCODE_4 = SDLK_4,
    KEYCODE_5 = SDLK_5,
    KEYCODE_6 = SDLK_6,
    KEYCODE_7 = SDLK_7,
    KEYCODE_8 = SDLK_8,
    KEYCODE_9 = SDLK_9,
    KEYCODE_COLON = SDLK_COLON,
    KEYCODE_SEMICOLON = SDLK_SEMICOLON,
    KEYCODE_LESS = SDLK_LESS,
    KEYCODE_EQUALS = SDLK_EQUALS,
    KEYCODE_GREATER = SDLK_GREATER,
    KEYCODE_QUESTION = SDLK_QUESTION,
    KEYCODE_AT = SDLK_AT,
    /*
       Skip uppercase letters
     */
    KEYCODE_LEFTBRACKET = SDLK_LEFTBRACKET,
    KEYCODE_BACKSLASH = SDLK_BACKSLASH,
    KEYCODE_RIGHTBRACKET = SDLK_RIGHTBRACKET,
    KEYCODE_CARET = SDLK_CARET,
    KEYCODE_UNDERSCORE = SDLK_UNDERSCORE,
    KEYCODE_BACKQUOTE = SDLK_BACKQUOTE,
    KEYCODE_A = SDLK_a,
    KEYCODE_B = SDLK_b,
    KEYCODE_C = SDLK_c,
    KEYCODE_D = SDLK_d,
    KEYCODE_E = SDLK_e,
    KEYCODE_F = SDLK_f,
    KEYCODE_G = SDLK_g,
    KEYCODE_H = SDLK_h,
    KEYCODE_I = SDLK_i,
    KEYCODE_J = SDLK_j,
    KEYCODE_K = SDLK_k,
    KEYCODE_L = SDLK_l,
    KEYCODE_M = SDLK_m,
    KEYCODE_N = SDLK_n,
    KEYCODE_O = SDLK_o,
    KEYCODE_P = SDLK_p,
    KEYCODE_Q = SDLK_q,
    KEYCODE_R = SDLK_r,
    KEYCODE_S = SDLK_s,
    KEYCODE_T = SDLK_t,
    KEYCODE_U = SDLK_u,
    KEYCODE_V = SDLK_v,
    KEYCODE_W = SDLK_w,
    KEYCODE_X = SDLK_x,
    KEYCODE_Y = SDLK_y,
    KEYCODE_Z = SDLK_z,

    KEYCODE_CAPSLOCK = SDLK_CAPSLOCK,

    KEYCODE_F1 = SDLK_F1,
    KEYCODE_F2 = SDLK_F2,
    KEYCODE_F3 = SDLK_F3,
    KEYCODE_F4 = SDLK_F4,
    KEYCODE_F5 = SDLK_F5,
    KEYCODE_F6 = SDLK_F6,
    KEYCODE_F7 = SDLK_F7,
    KEYCODE_F8 = SDLK_F8,
    KEYCODE_F9 = SDLK_F9,
    KEYCODE_F10 = SDLK_F10,
    KEYCODE_F11 = SDLK_F11,
    KEYCODE_F12 = SDLK_F12,

    KEYCODE_PRINTSCREEN = SDLK_PRINTSCREEN,
    KEYCODE_SCROLLLOCK = SDLK_SCROLLLOCK,
    KEYCODE_PAUSE = SDLK_PAUSE,
    KEYCODE_INSERT = SDLK_INSERT,
    KEYCODE_HOME = SDLK_HOME,
    KEYCODE_PAGEUP = SDLK_PAGEUP,
    KEYCODE_DELETE = SDLK_DELETE,
    KEYCODE_END = SDLK_END,
    KEYCODE_PAGEDOWN = SDLK_PAGEDOWN,
    KEYCODE_RIGHT = SDLK_RIGHT,
    KEYCODE_LEFT = SDLK_LEFT,
    KEYCODE_DOWN = SDLK_DOWN,
    KEYCODE_UP = SDLK_UP,

    KEYCODE_NUMLOCKCLEAR = SDLK_NUMLOCKCLEAR,
    KEYCODE_KP_DIVIDE = SDLK_KP_DIVIDE,
    KEYCODE_KP_MULTIPLY = SDLK_KP_MULTIPLY,
    KEYCODE_KP_MINUS = SDLK_KP_MINUS,
    KEYCODE_KP_PLUS = SDLK_KP_PLUS,
    KEYCODE_KP_ENTER = SDLK_KP_ENTER,
    KEYCODE_KP_1 = SDLK_KP_1,
    KEYCODE_KP_2 = SDLK_KP_2,
    KEYCODE_KP_3 = SDLK_KP_3,
    KEYCODE_KP_4 = SDLK_KP_4,
    KEYCODE_KP_5 = SDLK_KP_5,
    KEYCODE_KP_6 = SDLK_KP_6,
    KEYCODE_KP_7 = SDLK_KP_7,
    KEYCODE_KP_8 = SDLK_KP_8,
    KEYCODE_KP_9 = SDLK_KP_9,
    KEYCODE_KP_0 = SDLK_KP_0,
    KEYCODE_KP_PERIOD = SDLK_KP_PERIOD,

    KEYCODE_APPLICATION = SDLK_APPLICATION,
    KEYCODE_POWER = SDLK_POWER,
    KEYCODE_KP_EQUALS = SDLK_KP_EQUALS,
    KEYCODE_F13 = SDLK_F13,
    KEYCODE_F14 = SDLK_F14,
    KEYCODE_F15 = SDLK_F15,
    KEYCODE_F16 = SDLK_F16,
    KEYCODE_F17 = SDLK_F17,
    KEYCODE_F18 = SDLK_F18,
    KEYCODE_F19 = SDLK_F19,
    KEYCODE_F20 = SDLK_F20,
    KEYCODE_F21 = SDLK_F21,
    KEYCODE_F22 = SDLK_F22,
    KEYCODE_F23 = SDLK_F23,
    KEYCODE_F24 = SDLK_F24,
    KEYCODE_EXECUTE = SDLK_EXECUTE,
    KEYCODE_HELP = SDLK_HELP,
    KEYCODE_MENU = SDLK_MENU,
    KEYCODE_SELECT = SDLK_SELECT,
    KEYCODE_STOP = SDLK_STOP,
    KEYCODE_AGAIN = SDLK_AGAIN,
    KEYCODE_UNDO = SDLK_UNDO,
    KEYCODE_CUT = SDLK_CUT,
    KEYCODE_COPY = SDLK_COPY,
    KEYCODE_PASTE = SDLK_PASTE,
    KEYCODE_FIND = SDLK_FIND,
    KEYCODE_MUTE = SDLK_MUTE,
    KEYCODE_VOLUMEUP = SDLK_VOLUMEUP,
    KEYCODE_VOLUMEDOWN = SDLK_VOLUMEDOWN,
    KEYCODE_KP_COMMA = SDLK_KP_COMMA,
    KEYCODE_KP_EQUALSAS400 = SDLK_KP_EQUALSAS400,

    KEYCODE_ALTERASE = SDLK_ALTERASE,
    KEYCODE_SYSREQ = SDLK_SYSREQ,
    KEYCODE_CANCEL = SDLK_CANCEL,
    KEYCODE_CLEAR = SDLK_CLEAR,
    KEYCODE_PRIOR = SDLK_PRIOR,
    KEYCODE_RETURN2 = SDLK_RETURN2,
    KEYCODE_SEPARATOR = SDLK_SEPARATOR,
    KEYCODE_OUT = SDLK_OUT,
    KEYCODE_OPER = SDLK_OPER,
    KEYCODE_CLEARAGAIN = SDLK_CLEARAGAIN,
    KEYCODE_CRSEL = SDLK_CRSEL,
    KEYCODE_EXSEL = SDLK_EXSEL,

    KEYCODE_KP_00 = SDLK_KP_00,
    KEYCODE_KP_000 = SDLK_KP_000,
    KEYCODE_THOUSANDSSEPARATOR = SDLK_THOUSANDSSEPARATOR,
    KEYCODE_DECIMALSEPARATOR = SDLK_DECIMALSEPARATOR,
    KEYCODE_CURRENCYUNIT = SDLK_CURRENCYUNIT,
    KEYCODE_CURRENCYSUBUNIT = SDLK_CURRENCYSUBUNIT,
    KEYCODE_KP_LEFTPAREN = SDLK_KP_LEFTPAREN,
    KEYCODE_KP_RIGHTPAREN = SDLK_KP_RIGHTPAREN,
    KEYCODE_KP_LEFTBRACE = SDLK_KP_LEFTBRACE,
    KEYCODE_KP_RIGHTBRACE = SDLK_KP_RIGHTBRACE,
    KEYCODE_KP_TAB = SDLK_KP_TAB,
    KEYCODE_KP_BACKSPACE = SDLK_KP_BACKSPACE,
    KEYCODE_KP_A = SDLK_KP_A,
    KEYCODE_KP_B = SDLK_KP_B,
    KEYCODE_KP_C = SDLK_KP_C,
    KEYCODE_KP_D = SDLK_KP_D,
    KEYCODE_KP_E = SDLK_KP_E,
    KEYCODE_KP_F = SDLK_KP_F,
    KEYCODE_KP_XOR = SDLK_KP_XOR,
    KEYCODE_KP_POWER = SDLK_KP_POWER,
    KEYCODE_KP_PERCENT = SDLK_KP_PERCENT,
    KEYCODE_KP_LESS = SDLK_KP_LESS,
    KEYCODE_KP_GREATER = SDLK_KP_GREATER,
    KEYCODE_KP_AMPERSAND = SDLK_KP_AMPERSAND,
    KEYCODE_KP_DBLAMPERSAND = SDLK_KP_DBLAMPERSAND,
    KEYCODE_KP_VERTICALBAR = SDLK_KP_VERTICALBAR,
    KEYCODE_KP_DBLVERTICALBAR = SDLK_KP_DBLVERTICALBAR,
    KEYCODE_KP_COLON = SDLK_KP_COLON,
    KEYCODE_KP_HASH = SDLK_KP_HASH,
    KEYCODE_KP_SPACE = SDLK_KP_SPACE,
    KEYCODE_KP_AT = SDLK_KP_AT,
    KEYCODE_KP_EXCLAM = SDLK_KP_EXCLAM,
    KEYCODE_KP_MEMSTORE = SDLK_KP_MEMSTORE,
    KEYCODE_KP_MEMRECALL = SDLK_KP_MEMRECALL,
    KEYCODE_KP_MEMCLEAR = SDLK_KP_MEMCLEAR,
    KEYCODE_KP_MEMADD = SDLK_KP_MEMADD,
    KEYCODE_KP_MEMSUBTRACT = SDLK_KP_MEMSUBTRACT,
    KEYCODE_KP_MEMMULTIPLY = SDLK_KP_MEMMULTIPLY,
    KEYCODE_KP_MEMDIVIDE = SDLK_KP_MEMDIVIDE,
    KEYCODE_KP_PLUSMINUS = SDLK_KP_PLUSMINUS,
    KEYCODE_KP_CLEAR = SDLK_KP_CLEAR,
    KEYCODE_KP_CLEARENTRY = SDLK_KP_CLEARENTRY,
    KEYCODE_KP_BINARY = SDLK_KP_BINARY,
    KEYCODE_KP_OCTAL = SDLK_KP_OCTAL,
    KEYCODE_KP_DECIMAL = SDLK_KP_DECIMAL,
    KEYCODE_KP_HEXADECIMAL = SDLK_KP_HEXADECIMAL,

    KEYCODE_LCTRL = SDLK_LCTRL,
    KEYCODE_LSHIFT = SDLK_LSHIFT,
    KEYCODE_LALT = SDLK_LALT,
    KEYCODE_LGUI = SDLK_LGUI,
    KEYCODE_RCTRL = SDLK_RCTRL,
    KEYCODE_RSHIFT = SDLK_RSHIFT,
    KEYCODE_RALT = SDLK_RALT,
    KEYCODE_RGUI = SDLK_RGUI,

    KEYCODE_MODE = SDLK_MODE,

    KEYCODE_AUDIONEXT = SDLK_AUDIONEXT,
    KEYCODE_AUDIOPREV = SDLK_AUDIOPREV,
    KEYCODE_AUDIOSTOP = SDLK_AUDIOSTOP,
    KEYCODE_AUDIOPLAY = SDLK_AUDIOPLAY,
    KEYCODE_AUDIOMUTE = SDLK_AUDIOMUTE,
    KEYCODE_MEDIASELECT = SDLK_MEDIASELECT,
    KEYCODE_WWW = SDLK_WWW,
    KEYCODE_MAIL = SDLK_MAIL,
    KEYCODE_CALCULATOR = SDLK_CALCULATOR,
    KEYCODE_COMPUTER = SDLK_COMPUTER,
    KEYCODE_AC_SEARCH = SDLK_AC_SEARCH,
    KEYCODE_AC_HOME = SDLK_AC_HOME,
    KEYCODE_AC_BACK = SDLK_AC_BACK,
    KEYCODE_AC_FORWARD = SDLK_AC_FORWARD,
    KEYCODE_AC_STOP = SDLK_AC_STOP,
    KEYCODE_AC_REFRESH = SDLK_AC_REFRESH,
    KEYCODE_AC_BOOKMARKS = SDLK_AC_BOOKMARKS,

    KEYCODE_BRIGHTNESSDOWN = SDLK_BRIGHTNESSDOWN,
    KEYCODE_BRIGHTNESSUP = SDLK_BRIGHTNESSUP,
    KEYCODE_DISPLAYSWITCH = SDLK_DISPLAYSWITCH,
    KEYCODE_KBDILLUMTOGGLE = SDLK_KBDILLUMTOGGLE,
    KEYCODE_KBDILLUMDOWN = SDLK_KBDILLUMDOWN,
    KEYCODE_KBDILLUMUP = SDLK_KBDILLUMUP,
    KEYCODE_EJECT = SDLK_EJECT,
    KEYCODE_SLEEP = SDLK_SLEEP,
    KEYCODE_APP1 = SDLK_APP1,
    KEYCODE_APP2 = SDLK_APP2,

    KEYCODE_AUDIOREWIND = SDLK_AUDIOREWIND,
    KEYCODE_AUDIOFASTFORWARD = SDLK_AUDIOFASTFORWARD,

};

// Our keymods are the same as the sdl keymods for interop reasons
// Why didn't we just use SDK_Keymod?
// Because it is nicer to have our own keycode enums so that in the future, if we want to remove sdl, we can still do it
// and game code and other systems so have to edit all their KMOD_.... codes everywhere.
enum Keymod {
	KEYMOD_NONE = KMOD_NONE,
	KEYMOD_LSHIFT = KMOD_LSHIFT,
	KEYMOD_RSHIFT = KMOD_RSHIFT,
	KEYMOD_LCTRL = KMOD_LCTRL,
	KEYMOD_RCTRL = KMOD_RCTRL,
	KEYMOD_LALT = KMOD_LALT,
	KEYMOD_RALT = KMOD_RALT,
	KEYMOD_LGUI = KMOD_LGUI,
	KEYMOD_RGUI = KMOD_RGUI,
	KEYMOD_NUM = KMOD_NUM,
	KEYMOD_CAPS = KMOD_CAPS,
	KEYMOD_MODE = KMOD_MODE,
	KEYMOD_CTRL = KMOD_CTRL,
	KEYMOD_SHIFT = KMOD_SHIFT,
	KEYMOD_ALT = KMOD_ALT,
	KEYMOD_GUI = KMOD_GUI,
};


static inline SDL_Keycode keycode_to_sdl_keycode(Keycode keycode) {
	// Since our mapping is the same as the keycodes, we just need to cast it
	SDL_Keycode sdl_keycode = (SDL_Keycode)keycode;
	return sdl_keycode;
}


bool is_key_down(Input* input, Keycode keycode);
bool is_key_released(Input* input, Keycode keycode);
bool is_key_pressed(Input* input, Keycode keycode);
bool is_keymod_down(Input* input, Keymod keymod);

bool is_mouse_down(Input* input, MouseButton which);
bool is_mouse_released(Input* input, MouseButton which);
bool is_mouse_pressed(Input* input, MouseButton which);

Vec2i get_scroll_delta(Input* input);

Vec2i get_mouse_pos(Input* input);
Vec2i get_mouse_delta(Input* input);

Vec2i get_global_mouse_pos(Input* input);
Vec2i get_global_mouse_delta(Input* input);
