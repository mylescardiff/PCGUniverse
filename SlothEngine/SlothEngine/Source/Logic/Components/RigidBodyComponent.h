#pragma once
/** \file RigidBodyComponent.h */

// Created by Myles Cardiff

#include ".\IComponent.h"
#include <Box2D.h>
#include <Utilities\Vector2.h>
#include <Logic\World.h>
#include <Common.h>
#include "RigidBodyComponent.gen.h"

//! \namespace Sloth Engine
namespace slth
{
	class TransformComponent;
	class SpriteComponent;

/** \class RigidBodyComponent */
/** Adds physics to the actor (requires Sprite and Transform) */
	class RigidBodyComponent
	: public IComponent
{

public:

	enum class BodyType
	{
		Static,
		Kinematic,
		Dynamic
	};
	
	enum class BodyShape
	{
		Box,
		Circle,
		Polygon
	};

	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor  */
	SLTH_API RigidBodyComponent();

	/** Copy Method */
	SLTH_API virtual IComponent* Copy(Actor* pNewOwner) override;

	/** Default Destructor */
	SLTH_API ~RigidBodyComponent();

	/** Ideintifier Functions */
	static constexpr Id Identifier = 5; //Get the compile time unique id we generated on top of this file.
	SLTH_API virtual const Id GetIdentifier() override { return Identifier; }
	SLTH_API inline static const char* StringId = "RigidBody";
	SLTH_API virtual const char* GetName() override { return StringId; }

	/** Initialization Functions */
	SLTH_API bool Init(BodyType type, BodyShape shape);
	SLTH_API virtual bool Init(XmlData* xmlElement) override;
	SLTH_API virtual bool PostInit() override;

	/** Creates the physics body and places it in the Box2D world */
	SLTH_API virtual void SetBody(World* pWorld);

	/** Apply directional force on the object */
	SLTH_API virtual void ApplyLinearImpulse(Vector2& directionalForce);

	/** Apply directional force on the object */
	GLUA()
	SLTH_API virtual void ApplyLinearImpulse(float x, float y);

	/** Move the object by a factor of x/y */
	SLTH_API virtual void Move(Vector2 amount);
	
	/** Set the current position of the object to x/y */
	SLTH_API virtual void SetPosition(Vector2 position);

	/** Register the necessary functions for this component to Lua */
	SLTH_API static void RegisterToLuaState(lua_State* pState);

protected:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	b2Vec2 m_size;
	World* m_pWorld;
	b2Body* m_pPhysicsBody;
	BodyType m_type;
	BodyShape m_shape;
	std::vector<b2Vec2> m_polyVertices;
	Vector2 m_offset;
	bool m_fixedRotation;
	float m_bouncyness;

	// cached components
	TransformComponent* m_pTransform;
	SpriteComponent* m_pSprite;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
	

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
	SLTH_API TransformComponent* GetTransform();
	SLTH_API SpriteComponent* GetSprite();
	SLTH_API b2Body* GetPhysicsBody() const { return m_pPhysicsBody; }
	SLTH_API BodyType GetBodyType() const { return m_type; }
	SLTH_API BodyShape GetBodyShape() const { return m_shape; }
	
	SLTH_API Vector2 GetOffset() const { return m_offset; }
};
}