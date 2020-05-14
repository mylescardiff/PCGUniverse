#include "IGameLayer.h"
#include <Logger/Logger.h>
#include <Views/IView.h>
#include <Logic/Actor.h>
#include <Logic/World.h>
#include <Logic/Components/RigidBodyComponent.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Components/AnimationComponent.h>
#include <Logic/Components/SpriteComponent.h>
#include <Logic/ActorFactory.h>
#include <lua.hpp>
#include <Application/ApplicationLayer.h>
#include <Graphics/IGraphics.h>
#include <sstream>
#include <lua.hpp>
#include <Logic/IGameLayer.gen.h>

using slth::IGameLayer;

IGameLayer::IGameLayer()
	: m_pActorFactory(nullptr)
	, m_pAudio(nullptr)
	, m_pGraphics(nullptr)
	, m_pLuaState(nullptr)
	, m_pWorld(nullptr)
	, m_timeAccumulator(0.f)
	, m_quit(false)
	, m_defaultCollideTimout(2.f)
	, m_luaTickExists(false)
	, m_pAppLayer(nullptr)
	, m_paused(false)
{
	m_pCollisionEvent = new CollisionEvent;
}

IGameLayer::~IGameLayer()
{
	// clear actors
	for (auto pActor : m_actors)
	{
		delete pActor.second;
	}

	//clear 
	for (auto pView : m_pViews)
	{
		delete pView;
	}

	// clear anything pending in the add/delete lists
	for (Actor* pActor : m_actorsToAdd)
	{
		delete pActor;
	}
	m_actors.clear();
	m_actorsToDelete.clear();

	m_actors.clear();
	
	// clear ui
	for (auto pActor : m_uiElements)
	{
		delete pActor.second;
	}
	lua_close(m_pLuaState);

	delete m_pCollisionEvent;
	delete m_pActorFactory;
}

bool slth::IGameLayer::Init(ApplicationLayer* pAppLayer)
{
	// stuff pulled from application layer
	m_pAudio = pAppLayer->GetAudio();
	m_pGraphics = pAppLayer->GetGraphics();
	m_pWorld = pAppLayer->GetWorld();
	m_pAppLayer = pAppLayer;

	m_pWorld->SetGameLayer(this);

	// create lua state and register libs
	CreateLuaState();
	luaL_openlibs(m_pLuaState);

	// register keyboard and mouse classes with lua
	pAppLayer->GetKeyboard()->RegisterWithLuaState(m_pLuaState);
	pAppLayer->GetMouse()->RegisterWithLuaState(m_pLuaState);

	// register components with lua
	TransformComponent::RegisterToLuaState(m_pLuaState);
	RigidBodyComponent::RegisterToLuaState(m_pLuaState);
	SpriteComponent::RegisterToLuaState(m_pLuaState);

	// push necessary functions to lua
	lua_pushlightuserdata(m_pLuaState, this);
	lua_setglobal(m_pLuaState, "game");

	// get actor
	lua_pushcfunction(m_pLuaState, &glua::IGameLayer_FindActor);
	lua_setglobal(m_pLuaState, "getActor");

	// kill actor
	lua_pushcfunction(m_pLuaState, &glua::IGameLayer_KillActor);
	lua_setglobal(m_pLuaState, "killActor");

	// actor factory init
	m_pActorFactory = new slth::ActorFactory;
	m_pActorFactory->Init();

	m_pActorFactory->SetLuaState(m_pLuaState);

	return true;
}

bool slth::IGameLayer::PostInit()
{
	// check for the presence of the tick function in Lua
	lua_getglobal(m_pLuaState, "tick");
	m_luaTickExists = !lua_isnil(m_pLuaState, -1);
	lua_pop(m_pLuaState, 1);

	SetDefaultCollisionTimeout(2.f);



	return true;
}

void slth::IGameLayer::Update(float deltaTime)
{
	if (!m_paused)
	{
		if (m_luaTickExists)
		{
			// run lua tick function
			// TODO: get rid of this it's horribly slow
			lua_getglobal(m_pLuaState, "tick");
			lua_pushnumber(m_pLuaState, deltaTime);

			if (lua_pcall(m_pLuaState, 1, 0, 0) != LUA_OK)
			{
				const char* errorString = lua_tostring(m_pLuaState, -1); // lua pushes an error string it is good to log this.
				//_LOGL(errorString);
				lua_pop(m_pLuaState, 1); // pop the error string. As soon as this pops errorString = nullptr; 
			}
		}
		

#if _DEBUG
		if (deltaTime > kMaxFrameDelay)
		{
			_LOGW("Delta time was larger than max frame time");
			deltaTime = 0.016f;
		}
#endif

		m_processManager.UpdateProcesses(deltaTime);

		for (slth::IView* view : m_pViews)
		{
			view->Update(deltaTime);
		}

		// hyper Update
		m_timeAccumulator -= deltaTime;

		if (m_timeAccumulator <= 0)
		{
			m_timeAccumulator += kFixedUpdateDelay;
		}

		for (slth::IView* view : m_pViews)
		{
			view->ViewScene();
		}

		// update physics world
		constexpr float timeStep = 1.0f / 60.0f;
		static float timeSinceLastFrame = 0;
		timeSinceLastFrame += deltaTime;

		if (timeSinceLastFrame >= timeStep)
		{
			m_pWorld->Step(timeStep, 3, 8);
			timeSinceLastFrame -= timeStep;
		}

		// update actors 
		for (auto pActor : m_actors)
		{
			AnimationComponent* pAnimComp = pActor.second->GetComponent<AnimationComponent>();
			if (pAnimComp && pAnimComp->GetDefaultName().size() > 0 && !pAnimComp->GetDefault())
			{
				// there's a default animation that hasn't been started yet, start it
				std::string copyName = pAnimComp->GetDefaultName();
				Animation* pAnim = m_animationManager.Find(HASH(copyName));
				pAnimComp->SetDefault(pAnim);
				pAnimComp->DoAnimation(pAnim, m_processManager);
			}

			RigidBodyComponent* pRigidBody = pActor.second->GetComponent<RigidBodyComponent>();
			TransformComponent* pTransform = pActor.second->GetComponent<TransformComponent>();
			if (pRigidBody)
			{
				// can safely call this because it's being checked in PostInit that one exists
			

				// cache the size 
				Vector2 objectSize = pTransform->GetSize();

				// update the position according to where Box2D thinks it should be
				b2Vec2 physPosition = pRigidBody->GetPhysicsBody()->GetPosition();

				Vector2 newPosition(
					physPosition.x * kPixelsPerMeter - pRigidBody->GetOffset().x,
					physPosition.y * kPixelsPerMeter - pRigidBody->GetOffset().y
				);

				pTransform->SetPosition(newPosition);
				pTransform->m_rotation = pRigidBody->GetPhysicsBody()->GetAngle() * 57.6f;
			}
			if (pTransform)
			{
				pTransform->Update(deltaTime);
			}
		}

		// collision detection: O(n^2)
		for (auto pOuterActor : m_actors)
		{
			ColliderComponent* pOuterCollider = pOuterActor.second->GetComponent<ColliderComponent>();
			if (pOuterCollider)
			{
				// decrement the collider
				pOuterCollider->DecreaseTimeout(deltaTime);

				for (auto pInnerActor : m_actors)
				{
					ColliderComponent* pInnerCollider = pInnerActor.second->GetComponent<ColliderComponent>();

					// check for self and hidden
					if (pInnerCollider && pInnerCollider != pOuterCollider && !pInnerActor.second->IsHidden() && !pOuterActor.second->IsHidden())
					{
						RectInt innerRect = pInnerCollider->GetRect();
						RectInt outerRect = pOuterCollider->GetRect();

						// check if the collision timers on both objects are zero, otherwise it's too soon to hit again
						if (pInnerCollider->GetTimeout() <= 0.f && pOuterCollider->GetTimeout() <= 0.f && outerRect.Overlaps(innerRect))
						{
							// we have a collision, set the timouts and call the collision start function
							pOuterCollider->SetTimout(m_defaultCollideTimout);
							pInnerCollider->SetTimout(m_defaultCollideTimout);
							CollisionStart(pOuterCollider->GetOwner(), pInnerCollider->GetOwner());
						}
					}
				}
			}
			
		}

		// add any new actors
		for (auto pActor : m_actorsToAdd)
		{
			m_actors.emplace(pActor->GetId(), pActor);
		}
		m_actorsToAdd.clear();

		// delete any dead actors
		for (auto actorId : m_actorsToDelete)
		{
			auto pActor = FindActor(actorId);
			delete pActor;
			m_actors.erase(actorId);
		}
		m_actorsToDelete.clear();
	}
	
}

slth::Actor* slth::IGameLayer::FindActor(Id32 id)
{
	auto it = m_actors.find(id);
	if (it != m_actors.end())
	{
		return it->second;
	}
	return nullptr;
}

slth::Actor* slth::IGameLayer::FindActor(std::string name)
{
	for (auto it : m_actors)
	{
		if (it.second->GetName() == name)
			return it.second;
	}
	return nullptr;
}

slth::Actor* slth::IGameLayer::FindActor(const char* name)
{
	return FindActor(std::string(name));
}



void slth::IGameLayer::AddActor(Actor* pActor)
{
	m_actorsToAdd.emplace_back(pActor);
}

void slth::IGameLayer::KillActor(Id32 id)
{
	Actor* pActor = FindActor(id);
	AnimationComponent* pAnim = pActor->GetComponent<AnimationComponent>();
	if (pAnim)
	{
		pAnim->KillAll(m_processManager);
	}
	m_actorsToDelete.emplace_back(id);
}

void slth::IGameLayer::CollisionStart(Actor* pActorA, Actor* pActorB)
{
	//run lua collisionStart function
	lua_getglobal(m_pLuaState, "collisionStart");
	lua_pushstring(m_pLuaState, pActorA->GetTag().c_str());
	lua_pushstring(m_pLuaState, pActorB->GetTag().c_str());
	if (lua_pcall(m_pLuaState, 2, 0, 0) != LUA_OK)
	{
		const char* errorString = lua_tostring(m_pLuaState, -1); //Lua pushes an error string it is good to log this.
		_LOGL(errorString);
		lua_pop(m_pLuaState, 1); //Pop the error string. As soon as this pops errorString = nullptr; yes you're required to pop it otherwise you fuck up your stack.
	}
}

void slth::IGameLayer::CollisionEnd(Actor* pActorA, Actor* pActorB)
{
	// don't have a need for this at the moment.
	// plus i don't really have a way to do it for the simple AABB collisions, only Box2D can even detect it
}

slth::Actor * slth::IGameLayer::FindUIElement(std::string name)
{
	for (auto it : m_uiElements)
	{
		if (it.second->GetName() == name)
			return it.second;
	}
	return nullptr;
}

void slth::IGameLayer::SetActorsVisibleByTag(const char* tag, bool visible)
{
	for (auto it : m_actors)
	{
		if (it.second->GetTag() == tag)
			it.second->SetVisible(visible);
	}
}



void slth::IGameLayer::CreateLuaState()
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);

	// set up the initial table stucture
	
	// engine table
	lua_createtable(m_pLuaState, 0, 10); // [+1]

	// world table
	lua_createtable(m_pLuaState, 0, 100);// [+2]

	// add world to engine
	lua_setfield(m_pLuaState, -2, "world"); // [+1]

	// add engine to our global table
	lua_setglobal(m_pLuaState, "engine"); // [+0]

}





