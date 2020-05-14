#pragma once
/** \file IKeyboard.h */
/** Detects and holds keys pressed and down from the keyboard. (This is not a true interface, but you could inherit and override it) */
// Created by Myles Cardiff

#include <stdint.h>
#include "KeyMap.h"
#include <Common.h>

//! \namespace Sloth
namespace slth
{
	constexpr u32 kNumKeys = 999;

/** \class IKeyboard */
/** Base keyboard interface, provides basic key press and key down data */
class IKeyboard
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

	IKeyboard();

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Register all necesary functions with lua state */
	SLTH_API void RegisterWithLuaState(lua_State* pState);

	/** Detects if a key is currently being held down */
	GLUA()
	SLTH_API bool IsKeyDown(size_t keyCode);

	/** Detects if a key was pressed during this frame */
	GLUA()
	SLTH_API bool IsKeyPressed(size_t keyCode);

	/** Clears a single key press data, leaves key down in place */
	SLTH_API void ClearKey(size_t keyCode);

	/** Clears all key pressed data, leaves key down in place */
	SLTH_API void ClearAllKeys();

	/** Sets the key state for a single key */
	SLTH_API void SetKey(size_t keyCode, bool down);

	

protected:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	struct KeyData
	{
		bool m_pressed;
		bool m_down;
		u32 m_pressedTime;
	};
	KeyData m_keyData[kNumKeys];

	//static bool s_anyPressed;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}