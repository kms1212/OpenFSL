#include "drivers.h"

static ps2kbdKeyMap ps2kbdMappingTable[89];

static uint8_t ps2kbdLockStatus;
static uint8_t ps2kbdSpecialKeyDown;
static uint8_t ps2kbdCodeSkip;
static bool ps2kbdExtendedCode;

bool ps2kbdCheckOutputBuffer()
{
	if (portInB(0x0064) & 0x01)
		return true;
	return false;
}

bool ps2kbdCheckInputBuffer()
{
	if (portInB(0x0064) & 0x02)
		return true;
	return false;
}

bool ps2kbdWaitInput()
{
	for (int i = 0; i < 0xFFFF; i++)
	{
		if(!ps2kbdCheckInputBuffer())
			return false;
	}
	return true;
}

bool ps2kbdWaitOutput()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 0xFFFF; j++)
		{
			if(ps2kbdCheckOutputBuffer())
				break;
		}
		
		if (portInB(0x0060) == 0xFA)
			return false;
	}
	return true;
}

bool ps2kbdActivate()
{
	portOutB(0x0064, 0xAE);
	if (ps2kbdWaitInput())
		return true;
	
	portOutB(0x0060, 0xF4);
	if (ps2kbdWaitOutput())
		return true;
	return false;
}

uint8_t ps2kbdGetCode()
{
	while (!ps2kbdCheckOutputBuffer());
	return portInB(0x0060);
}

bool ps2kbdSetLED(uint8_t led)
{
	if (ps2kbdWaitInput())
		return true;
	
	portOutB(0x0060, 0xED);
	if (ps2kbdWaitInput())
		return true;
	
	if (ps2kbdWaitOutput())
		return true;
		
	portOutB(0x0060, led);
	if (ps2kbdWaitInput())
		return true;
	
	if (ps2kbdWaitOutput())
		return true;
	
	return false;
}

bool ps2kbdCheckUseAltKey(uint8_t code)
{
	uint8_t scanCode;
	bool useAltKey = false;
	
	scanCode = code & 0x7F;
	if (ps2kbdIsAlphabetCode(scanCode))
	{
		if((ps2kbdSpecialKeyDown & PS2KBD_SPECIALKEY_LSHIFT) ^
			(ps2kbdSpecialKeyDown & PS2KBD_SPECIALKEY_RSHIFT) ^
			(ps2kbdLockStatus & PS2KBD_STATUS_CAPSLOCK))
			useAltKey = true;
		else
			useAltKey = false;
	}
	else if (ps2kbdIsNumberSymbolCode(scanCode))
	{
		if((ps2kbdSpecialKeyDown & PS2KBD_SPECIALKEY_LSHIFT) ^
			(ps2kbdSpecialKeyDown & PS2KBD_SPECIALKEY_RSHIFT))
			useAltKey = true;
		else
			useAltKey = false;
	}
	else if (ps2kbdIsNumpadCode(scanCode) && !ps2kbdExtendedCode)
	{
		if(ps2kbdLockStatus & PS2KBD_STATUS_NUMLOCK)
			useAltKey = true;
		else
			useAltKey = false;
	}
	
	return useAltKey;
}

bool ps2kbdIsAlphabetCode(uint8_t code)
{
	if (('a' <= ps2kbdMappingTable[code].kbdAsciiNom) && 
		(ps2kbdMappingTable[code].kbdAsciiNom <= 'z'))
		return true;
	return false;
}

bool ps2kbdIsNumberSymbolCode(uint8_t code)
{
	if ((2 <= code) && (code <= 53) && !ps2kbdIsAlphabetCode(code))
		return true;
	return false;
}

bool ps2kbdIsNumpadCode(uint8_t code)
{
	if ((71 <= code) && (code <= 83))
		return true;
	return false;
}

void ps2kbdUpdateLockStatus(uint8_t code)
{
	bool isDown;
	uint8_t scanCode;
	bool ledChanged = false;
	
	if (code & 0x80)
	{
		isDown = false;
		scanCode = code & 0x7F;
	}
	else
	{
		isDown = true;
		scanCode = code;
	}
	
	if (scanCode == 42)
	{
		if (isDown)
			ps2kbdSpecialKeyDown |= PS2KBD_SPECIALKEY_LSHIFT;
		else
			ps2kbdSpecialKeyDown &= ~PS2KBD_SPECIALKEY_LSHIFT;
	}
	else if (scanCode == 54)
	{
		if (isDown)
			ps2kbdSpecialKeyDown |= PS2KBD_SPECIALKEY_RSHIFT;
		else
			ps2kbdSpecialKeyDown &= ~PS2KBD_SPECIALKEY_RSHIFT;
	}
	else if (scanCode == 56)
	{
		if (isDown)
			ps2kbdSpecialKeyDown |= PS2KBD_SPECIALKEY_LALT;
		else
			ps2kbdSpecialKeyDown &= ~PS2KBD_SPECIALKEY_LALT;
	}
	else if ((scanCode == 29) && isDown)
	{
		ps2kbdLockStatus ^= PS2KBD_STATUS_CAPSLOCK;
		ledChanged = true;
	}
	else if ((scanCode == 69) && isDown)
	{
		ps2kbdLockStatus ^= PS2KBD_STATUS_NUMLOCK;
		ledChanged = true;
	}
	else if ((scanCode == 70) && isDown)
	{
		ps2kbdLockStatus ^= PS2KBD_STATUS_SCRLLOCK;
		ledChanged = true;
	}
	
	if(ledChanged == true)
	{
		ps2kbdSetLED(ps2kbdLockStatus);
	}
}

bool ps2kbdConvertToAscii(uint8_t scanCode, uint8_t* asciiCode, uint8_t* flags)
{
	bool useAltKey;
	
	if(ps2kbdCodeSkip > 0)
	{
		ps2kbdCodeSkip--;
		return false;
	}
	
	if (scanCode == 0xE1)
	{
		*asciiCode = PS2KBD_KEY_PAUSE;
		*flags = PS2KBD_KEYSTAT_DOWN;
		ps2kbdCodeSkip = 2;
		return true;
	}
	else if (scanCode == 0xE0)
	{
		ps2kbdExtendedCode = true;
		return false;
	}
	
	useAltKey = ps2kbdCheckUseAltKey(scanCode);
	
	if (useAltKey)
		*asciiCode = ps2kbdMappingTable[scanCode & 0x7F].kbdAsciiAlt;
	else
		*asciiCode = ps2kbdMappingTable[scanCode & 0x7F].kbdAsciiNom;
	
	if (ps2kbdExtendedCode)
	{
		*flags = PS2KBD_KEYSTAT_EXTENDED;
		ps2kbdExtendedCode = false;
	}
	else
		*flags = 0;
		
	if (!(scanCode & 0x80))
		*flags |= PS2KBD_KEYSTAT_DOWN;
		
	ps2kbdUpdateLockStatus(scanCode);
	return true;
}

static ps2kbdKeyMap ps2kbdMappingTable[89] = {
	{PS2KBD_KEY_NONE      , PS2KBD_KEY_NONE      },
	{PS2KBD_KEY_ESC       , PS2KBD_KEY_ESC       },
	{'1'                  , '!'                  },
	{'2'                  , '@'                  },
	{'3'                  , '#'                  },
	{'4'                  , '$'                  },
	{'5'                  , '%'                  },
	{'6'                  , '^'                  },
	{'7'                  , '&'                  },
	{'8'                  , '*'                  },
	{'9'                  , '('                  },
	{'0'                  , ')'                  },
	{'-'                  , '_'                  },
	{'='                  , '+'                  },
	{PS2KBD_KEY_BACKSPACE , PS2KBD_KEY_BACKSPACE },
	{PS2KBD_KEY_TAB       , PS2KBD_KEY_TAB       },
	{'q'                  , 'Q'                  },
	{'w'                  , 'W'                  },
	{'e'                  , 'E'                  },
	{'r'                  , 'R'                  },
	{'t'                  , 'T'                  },
	{'y'                  , 'Y'                  },
	{'u'                  , 'U'                  },
	{'i'                  , 'I'                  },
	{'o'                  , 'O'                  },
	{'p'                  , 'P'                  },
	{'['                  , '{'                  },
	{']'                  , '}'                  },
	{'\n'                 , '\n'                 },
	{PS2KBD_KEY_CTRL      , PS2KBD_KEY_CTRL      },
	{'a'                  , 'A'                  },
	{'s'                  , 'S'                  },
	{'d'                  , 'D'                  },
	{'f'                  , 'F'                  },
	{'g'                  , 'G'                  },
	{'h'                  , 'H'                  },
	{'j'                  , 'J'                  },
	{'k'                  , 'K'                  },
	{'l'                  , 'L'                  },
	{';'                  , ':'                  },
	{'\''                 , '\"'                 },
	{'`'                  , '~'                  },
	{PS2KBD_KEY_LSHIFT    , PS2KBD_KEY_LSHIFT    },
	{'\\'                 , '|'                  },
	{'z'                  , 'Z'                  },
	{'x'                  , 'X'                  },
	{'c'                  , 'C'                  },
	{'v'                  , 'V'                  },
	{'b'                  , 'B'                  },
	{'n'                  , 'N'                  },
	{'m'                  , 'M'                  },
	{','                  , '<'                  },
	{'.'                  , '>'                  },
	{'/'                  , '?'                  },
	{PS2KBD_KEY_RSHIFT    , PS2KBD_KEY_RSHIFT    },
	{'*'                  , '*'                  },
	{PS2KBD_KEY_LALT      , PS2KBD_KEY_LALT      },
	{' '                  , ' '                  },
	{PS2KBD_KEY_CAPSLOCK  , PS2KBD_KEY_CAPSLOCK  },
	{PS2KBD_KEY_F1        , PS2KBD_KEY_F1        },
	{PS2KBD_KEY_F2        , PS2KBD_KEY_F2        },
	{PS2KBD_KEY_F3        , PS2KBD_KEY_F3        },
	{PS2KBD_KEY_F4        , PS2KBD_KEY_F4        },
	{PS2KBD_KEY_F5        , PS2KBD_KEY_F5        },
	{PS2KBD_KEY_F6        , PS2KBD_KEY_F6        },
	{PS2KBD_KEY_F7        , PS2KBD_KEY_F7        },
	{PS2KBD_KEY_F8        , PS2KBD_KEY_F8        },
	{PS2KBD_KEY_F9        , PS2KBD_KEY_F9        },
	{PS2KBD_KEY_F10       , PS2KBD_KEY_F10       },
	{PS2KBD_KEY_NUMLOCK   , PS2KBD_KEY_NUMLOCK   },
	{PS2KBD_KEY_SCROLLLOCK, PS2KBD_KEY_SCROLLLOCK},
	{PS2KBD_KEY_HOME      , '7'                  },
	{PS2KBD_KEY_UP        , '8'                  },
	{PS2KBD_KEY_PAGEUP    , '9'                  },
	{'-'                  , '-'                  },
	{PS2KBD_KEY_LEFT      , '4'                  },
	{PS2KBD_KEY_CENTER    , '5'                  },
	{PS2KBD_KEY_RIGHT     , '6'                  },
	{'+'                  , '+'                  },
	{PS2KBD_KEY_END       , '1'                  },
	{PS2KBD_KEY_DOWN      , '2'                  },
	{PS2KBD_KEY_PAGEDOWN  , '3'                  },
	{PS2KBD_KEY_INS       , '0'                  },
	{PS2KBD_KEY_DEL       , '.'                  },
	{PS2KBD_KEY_NONE      , PS2KBD_KEY_NONE      },
	{PS2KBD_KEY_NONE      , PS2KBD_KEY_NONE      },
	{PS2KBD_KEY_NONE      , PS2KBD_KEY_NONE      },
	{PS2KBD_KEY_F11       , PS2KBD_KEY_F11       },
	{PS2KBD_KEY_F12       , PS2KBD_KEY_F12       }
};