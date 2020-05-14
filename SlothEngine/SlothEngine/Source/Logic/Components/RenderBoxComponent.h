#pragma once
/** \file RenderBoxComponent.h */

// Created by Myles Cardiff

#include ".\IDrawableComponent.h"
#include <Utilities/Vector2.h>
#include <Utilities/Color.h>
#include <functional>
#include <Graphics/IGraphics.h>


//! \namespace Sloth Engine
/** Adds a renderable rectangle to the actor (requires Transform) */
namespace slth
{

	class TransformComponent;
	class SpriteComponent;
	/** \class RenderBoxComponent */
class RenderBoxComponent
	: public IDrawableComponent
{
	

public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SLTH_API RenderBoxComponent()
		: m_pSprite(nullptr)
		, m_pTransform(nullptr)
		
	{};

	/** Copy Method */
	SLTH_API virtual IComponent* Copy(Actor* pNewOwner) override { return new RenderBoxComponent(*this); }


	/** Default Destructor */
	SLTH_API ~RenderBoxComponent();

	/** Ideintifier Functions */
	static constexpr Id Identifier = 1;
	SLTH_API virtual const Id GetIdentifier() override { return Identifier; }
	SLTH_API virtual const char* GetName() override { return "RenderBox"; };

	/** Initialization Functions*/
	SLTH_API virtual bool Init(XmlData* xmlElement) override;
	SLTH_API virtual bool Init(Vector2 size, float r, float g, float b);

	SLTH_API virtual bool PostInit() override;

	/** Draw the box on screen */
	SLTH_API virtual void Draw(IGraphics* pGraphics) override;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	TransformComponent* m_pTransform;
	SpriteComponent* m_pSprite;

	Vector2 m_size;
	Color m_color;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

	TransformComponent* GetTransform();
	SpriteComponent* GetSprite();

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API Vector2 GetSize() { return m_size; }
	SLTH_API Color GetColor() { return m_color; }
	SLTH_API void SetColor(Color color) { m_color = color; }

};
}