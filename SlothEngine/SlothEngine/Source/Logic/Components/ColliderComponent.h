#pragma once
/** \file ColliderComponent.h */
/** TODO: File Purpose */
// Created by Myles Cardiff

#include ".\IComponent.h"
#include <Utilities/Vector2.h>
#include <Utilities/RectInt.h>
#include <Common.h>


//! \namespace TODO: Namespace Description
namespace slth
{
/** \class ColliderComponent */
/** TODO: Class Purpose */
class ColliderComponent
	: public IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SLTH_API ColliderComponent();

	/** Default Destructor */
	SLTH_API ~ColliderComponent();

	/** Copy Method */
	SLTH_API virtual IComponent* Copy(Actor* pNewOwner) override { return new ColliderComponent(*this); }

	/** Ideintifier Functions */
	static constexpr Id Identifier = 6;
	SLTH_API virtual const Id GetIdentifier() override { return Identifier; }
	SLTH_API virtual const char* GetName() override { return "Collider"; };

	/** Initialization Functions*/
	SLTH_API virtual bool Init(XmlData* xmlElement) override;
	SLTH_API virtual bool PostInit() override;


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	Vector2 m_size;
	Vector2 m_offset;
	float m_collisionTimeout;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API Vector2 GetSize() { return m_size; }
	SLTH_API Vector2 GetOffset() { return m_offset; }
	SLTH_API RectInt GetRect();

	SLTH_API float GetTimeout() { return m_collisionTimeout; }
	SLTH_API void SetTimout(float time) { m_collisionTimeout = time; }
	SLTH_API void DecreaseTimeout(float time) { m_collisionTimeout -= time; }

};
}