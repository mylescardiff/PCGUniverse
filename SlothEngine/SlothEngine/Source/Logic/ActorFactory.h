#pragma once
/** \file ActorFactory.h */

// Created by Myles Cardiff

#include <memory>
#include <functional>
#include <Logic/Components/IComponent.h>
#include <Logic/Actor.h>
#include <Common.h>
#include <Utilities/Color.h>

#include <map>
#include <lua.hpp>

//! \namespace Sloth Engine
namespace slth
{
	class ProcessManager;
	class IProcess;
	class IGraphics;
	class World;

/** \class ActorFactory */
class ActorFactory
{
	using ComponentCreator = std::function<IComponent*(Actor*) >;
	using ComponentCreatorMap = std::unordered_map<Id, ComponentCreator>;
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SLTH_API ActorFactory();

	/** Default Destructor */
	SLTH_API ~ActorFactory();

	SLTH_API void Init();

	SLTH_API void SetLuaState(lua_State* pLuaState) { m_pLuaState = pLuaState; }

	SLTH_API void CreateActors(const char* filename, std::map<Id32, Actor*>& list, IGraphics* pGraphics, World* pWorld);
	
	SLTH_API void CreateActor(const char* tag, float x, float y, float width, float height, std::map<Id32, Actor*>& list, World* pWorld);

	SLTH_API Actor* CreateBlankActor(const char* tag, std::map<Id32, Actor*>& list);

	SLTH_API void CreateActor(XmlData* pXmlElement, std::map<Id32, Actor*>& list, IGraphics* pGraphics, World* pWorld);

	SLTH_API virtual Actor* CloneActor(Actor& otherActor, Vector2 position);

	SLTH_API slth::Actor* CreateBlockActor(const char* tag, Vector2 position, Vector2 size, Color color, std::map<Id32, Actor*>& list);

	SLTH_API void RegisterComponentCreator(Id componentId, ComponentCreator creator) 
	{
		m_componentCreators[componentId] = creator;
	}

	void RegisterWithLua(Actor* pActor);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	u32 m_nextActorId;
	lua_State* m_pLuaState;

	ComponentCreatorMap m_componentCreators;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}