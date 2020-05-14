#pragma once
/** \file IComponent.h */

// Created by Myles Cardiff

#include <Common.h>

struct lua_State;

//! \namespace Sloth Engine
namespace slth
{

	class Actor;

/** \class IComponent */
/** Base interface for all components */
class IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Copy Constructor (not really but acts as one) */
	SLTH_API virtual IComponent* Copy(Actor* pNewOwner) = 0;

	/** Default Destructor */
	SLTH_API virtual ~IComponent() {}

	/** Ideintifier Functions */
	SLTH_API virtual const Id GetIdentifier() = 0;
	SLTH_API virtual const char* GetName() = 0;

	/** Initialization Functions*/
	SLTH_API virtual bool Init(XmlData* xmlElement) = 0;
	SLTH_API virtual bool PostInit() = 0;
	
	/** Register necessary functions for use in Lua */
	SLTH_API virtual void RegisterWithLua(lua_State* pLuaState, const char* name);

protected:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	Actor* m_pOwner = nullptr;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
	
	SLTH_API Actor* GetOwner() { return m_pOwner;  }
	SLTH_API void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }

};
}