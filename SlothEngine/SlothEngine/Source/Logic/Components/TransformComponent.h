#pragma once
/** \file TransformComponent.h */

// Created by Myles Cardiff

#include ".\IComponent.h"
#include <Utilities/Vector2.h>
#include <functional>
#include <Utilities/RectInt.h>
#include <Utilities/RectFloat.h>



//! \namespace Sloth Engine
namespace slth
{
	class World;
/** \class TransformComponent */
/** Adds position and size data to the actor */
	class TransformComponent
	: public IComponent
{

	Vector2 m_position;
	Vector2 m_size;
	RectFloat m_anchorRect;
	bool m_useAnchorPosition;
	RectInt m_positionRect;
	World* m_pWorld;
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables	
	float m_rotation;
	Vector2 m_velocity;

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Copy Method */
	SLTH_API virtual IComponent* Copy(Actor* pNewOwner) override;

	/** Default Destructor */
	SLTH_API ~TransformComponent();

	/** Ideintifier Functions */
	SLTH_API static constexpr Id Identifier = 0; //Get the compile time unique id we generated on top of this file.
	SLTH_API inline static const char* StringId = "Transform"; 
	SLTH_API virtual const char* GetName() override { return StringId; }
	SLTH_API virtual const Id GetIdentifier() override { return Identifier; }

	/** Initialization Functions */
	SLTH_API virtual bool Init(float x, float y, float width, float height, float rotation);
	SLTH_API virtual bool Init(XmlData* xmlElement) override;
	SLTH_API virtual bool PostInit() override;

	/** Move the item on screen */
	SLTH_API virtual void Move(Vector2 amount);
	
	/** Move the item on screen */
	SLTH_API virtual void Move(float x, float y);

	/** Update the transform based on it's velocity */
	SLTH_API virtual void Update(float deltaTime);

	/** Register necessary functions for use in Lua */
	SLTH_API static void RegisterToLuaState(lua_State* pState);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API Vector2 GetVelocity() const { return m_velocity; }
	SLTH_API void SetVelocity(Vector2 velocity) { m_velocity = velocity; }
	SLTH_API Vector2 GetPosition() const;
	SLTH_API void SetPosition(Vector2 position);
	SLTH_API Vector2 GetSize() const { return m_size; }
	SLTH_API void SetSize(Vector2 size);
	
	SLTH_API RectInt GetPositionRect() const;
	SLTH_API void SetWorld(World* pWorld);
	
};
}