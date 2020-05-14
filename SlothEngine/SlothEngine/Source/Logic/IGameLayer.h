#pragma once
/** \file IGameLayer.h */

// Created by Myles Cardiff

#include "..\Common.h"
#include <memory>
#include <vector>
#include <map>
#include <Logic/Processes/ProcessManager.h>
#include <Logic/Events/EventDispatcher.h>
#include <Logic/AnimationManager.h>
#include <Logic/World.h>
#include <Logic/Tiles/Map.h>
#include <Logic/Events/CollisionEvent.h>
#include <lua.hpp>
#include <Logic/Grid/GridSystem.h>
#include <Logic/Components/ColliderComponent.h>

//struct lua_State;

namespace glua
{
	int IGameLayer_FindActor(lua_State* pState);
}

//! \namespace Engine Contains all Engine code
namespace slth
{

	class IView;
	class ApplicationLayer;
	class AnimationManager;
	class ActorFactory;
	class IGraphics;
	class IAudioSystem;

	/** \class IGameLayer */

	class IGameLayer
	{
	public:
		// --------------------------------------------------------------------- //
		// Public Member Variables
		// --------------------------------------------------------------------- //


		// --------------------------------------------------------------------- //
		// Public Member Functions
		// --------------------------------------------------------------------- //
		/** Default Constructor */
		SLTH_API IGameLayer();

		/** Default Destructor */
		SLTH_API virtual ~IGameLayer();

		SLTH_API virtual const char* GetGameName() const = 0;

		SLTH_API virtual bool Init(ApplicationLayer* pAppLayer);

		SLTH_API virtual bool PostInit();

		SLTH_API virtual void UpdateInput() {};

		SLTH_API virtual void Update(float deltaTime);

		SLTH_API virtual void AddView(IView* pView) { m_pViews.emplace_back(pView); }

		SLTH_API virtual IGameLayer* Create() = 0;

		SLTH_API virtual Actor* FindActor(Id32 id);

		SLTH_API virtual Actor* FindActor(std::string name);

		SLTH_API virtual Actor* FindActor(const char* name);

		

		SLTH_API void AddActor(Actor* pActor);

		SLTH_API virtual void KillActor(Id32 id);

		SLTH_API virtual void DebugDraw() { m_pWorld->DebugDraw(); }

		SLTH_API virtual void CollisionStart(Actor* pActorA, Actor* pActorB);

		SLTH_API virtual void CollisionEnd(Actor* pActorA, Actor* pActorB);

		SLTH_API virtual Actor* FindUIElement(std::string id);

		SLTH_API virtual Actor* FindUIElement(const char* name) { return FindUIElement(std::string(name)); }

		SLTH_API virtual void SetActorsVisibleByTag(const char* tag, bool visible);

		SLTH_API virtual void Draw() {};


	protected:
		// --------------------------------------------------------------------- //
		// Private Member Variables
		// --------------------------------------------------------------------- //

		ApplicationLayer* m_pAppLayer;

		std::vector<IView*> m_pViews;
		float m_timeAccumulator;

		ActorFactory* m_pActorFactory;
		std::map<Id32, slth::Actor*> m_actors;
		std::vector<Id32> m_actorsToDelete;
		std::vector<slth::Actor*> m_actorsToAdd;
		u32 m_playMode;

		std::map<Id32, slth::Actor*> m_uiElements;

		bool m_checkCollision; 
		float m_defaultCollideTimout;

		IGraphics* m_pGraphics;
		IAudioSystem* m_pAudio;
		World* m_pWorld;

		AnimationManager m_animationManager;
		EventDispatcher m_eventDispatcher;
		ProcessManager m_processManager;

		lua_State* m_pLuaState;
		bool m_luaTickExists;

		CollisionEvent* m_pCollisionEvent;

		slth::Map m_map;

		slth::GridSystem m_gridSystem;

		bool m_quit;
		bool m_paused;

		// --------------------------------------------------------------------- //
		// Private Member Functions
		// --------------------------------------------------------------------- //

		/** Create Lua State */
		SLTH_API void CreateLuaState();

		// ------------------------------------------------------------
// CREDIT: These two lua functions were written by Dylan Wjinen.
// He was helping me understand what's happening in the lua / c++ 
// exchange so they're being used just for educational and debug
// purposes for this project.
// ------------------------------------------------------------

/** Prints what's in the lua stack to the console */
		static void PrintLuaStack(lua_State* pLuaState)
		{
			int i = lua_gettop(pLuaState);
			printf(" ----------------  Stack Dump ----------------\n");
			while (i)
			{
				int t = lua_type(pLuaState, i);
				switch (t)
				{
				case LUA_TSTRING:
					printf("%d:'%s'\n", i, lua_tostring(pLuaState, i));
					break;
				case LUA_TBOOLEAN:
					printf("%d: %s\n", i, lua_toboolean(pLuaState, i) ? "true" : "false");
					break;
				case LUA_TNUMBER:
					printf("%d: %g\n", i, lua_tonumber(pLuaState, i));
					break;
				default:
					printf("%d: %s\n", i, lua_typename(pLuaState, t));
					break;
				}
				i--;
			}
			printf("--------------- Stack Dump Finished ---------------\n");
		}


	public:
		// --------------------------------------------------------------------- //
		// Accessors & Mutators
		// --------------------------------------------------------------------- //

		SLTH_API ActorFactory* GetActorFactory() { return m_pActorFactory; }
		SLTH_API IAudioSystem* GetAudio() { return m_pAudio; }
		SLTH_API IGraphics* GetGraphics() { return m_pGraphics; }
		SLTH_API World* GetWorld() { return m_pWorld; }
		SLTH_API std::map<Id32, slth::Actor*>& GetActors() { return m_actors; }

		SLTH_API EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }
		SLTH_API ProcessManager& GetProcessManager() { return m_processManager; }
		SLTH_API AnimationManager& GetAnimationsManager() { return m_animationManager; }

		SLTH_API Map& GetMap() { return m_map; }
		SLTH_API GridSystem& GetGrid() { return m_gridSystem; }
		SLTH_API std::map<Id32, slth::Actor*>& GetUIElements() { return m_uiElements; }

		SLTH_API void Pause() { m_paused = !m_paused; }

		SLTH_API void Quit() { m_quit = true; }
		SLTH_API bool HasQuit() { return m_quit; }

		SLTH_API void SetDefaultCollisionTimeout(float timeout) { m_defaultCollideTimout = timeout; }

		SLTH_API const u32 GetPlayMode() { return m_playMode; }
		SLTH_API void SetPlayMode(const u32 playMode) { m_playMode = playMode; }
	};
}