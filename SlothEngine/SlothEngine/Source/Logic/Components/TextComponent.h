#pragma once
/** \file TextComponent.h */
// Created by Myles Cardiff

#include ".\IDrawableComponent.h"
#include <Common.h>
#include <string>
#include <Utilities/Vector2.h>
#include <Utilities/Color.h>

//! \namespace Sloth Engine
namespace slth
{

	class TransformComponent;

/** \class TextComponent */
/** Adds rendered text to the actor (requires Transform) */
class TextComponent
	: public IDrawableComponent
{
public:

	enum class HorizontalAlignment
	{
		Left,
		Right,
		Center
	};
	enum class VerticalAlignment
	{
		Top,
		Middle,
		Bottom
	};
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	SLTH_API TextComponent()
		: m_alignHorizontal(HorizontalAlignment::Left)
		, m_alignVertical(VerticalAlignment::Middle)
		, m_fontSize(9)
		, m_color(slth::Color::Black())
		, m_pTransformComponent(nullptr)
	{}

	/** Copy Method */
	SLTH_API virtual IComponent* Copy(Actor* pNewOwner) override { return new TextComponent(*this); }

	/** Default Destructor */
	SLTH_API ~TextComponent();

	/** Ideintifier Functions */
	static constexpr Id Identifier = 4;
	SLTH_API virtual const Id GetIdentifier() override { return Identifier; }
	SLTH_API virtual const char* GetName() override { return "Text"; };

	/** Initialization Functions */
	SLTH_API virtual bool Init(XmlData* xmlElement) override;
	SLTH_API virtual bool PostInit() override { return true; }

	/** Draw the text on screen */
	SLTH_API virtual void Draw(IGraphics* pGraphics) override;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	std::string m_text;
	Color m_color;
	unsigned short m_fontSize;
	HorizontalAlignment m_alignHorizontal;
	VerticalAlignment m_alignVertical;
	TransformComponent* m_pTransformComponent;
	Vector2 m_offset;
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //



public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API void SetText(std::string text) { m_text = text; }

};
}