#pragma once
/** \file SpriteComponent.h */

// Created by Myles Cardiff

#include ".\IDrawableComponent.h"
#include <Utilities/Vector2.h>
#include <Utilities/Color.h>
#include <functional>
#include <Graphics/IGraphics.h>
#include <Graphics/ITexture.h>

//! \namespace Sloth Engine
namespace slth
{
/** \class SpriteComponent */
/** Adds renderable texture to the actor (requires Transform) */
	class SpriteComponent
	: public IDrawableComponent
{
public:

	enum class FacingDirection
	{
		kLeft,
		kRight
	};
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SLTH_API SpriteComponent();

	/** Copy Method */
	SLTH_API virtual IComponent* Copy(Actor* pNewOwner) override { return new SpriteComponent(*this); }

	/** Default Destructor */
	SLTH_API ~SpriteComponent();

	/** Ideintifier Functions */
	static constexpr Id Identifier = 2;
	SLTH_API inline static const char* StringId = "Sprite";
	SLTH_API virtual const Id GetIdentifier() override { return Identifier; }
	SLTH_API virtual const char* GetName() override { return StringId; };

	/** Initialization Functions */
	SLTH_API virtual bool Init(XmlData* xmlElement) override;
	SLTH_API virtual bool PostInit() override;

	/** Draw the texture on screen */
	SLTH_API virtual void Draw(IGraphics* pGraphics) override;

	/** Load the texture from disk */
	SLTH_API virtual void LoadTexture(IGraphics* pGraphics);
	
	/** Change the direction the texture is drawn (left or right) */
	SLTH_API void TurnAround();

	/** Register necessary functions for use in Lua */
	SLTH_API static void RegisterToLuaState(lua_State* pState);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	//std::string m_name;
	std::string m_path;
	ITexture* m_pTexture;
	RectInt m_textureRect;
	FacingDirection m_direction;
	Vector2 m_origin;
	bool m_repeat;
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API RectInt& GetTextureRect() { return m_textureRect; }
	SLTH_API void SetTextureRect(RectInt& rect) { m_textureRect = rect; }
	SLTH_API void SetDirection(FacingDirection direction) { m_direction = direction; }
	SLTH_API void SetTexturePath(std::string path) { m_path = path; }
	SLTH_API bool GetRepeat() { return m_repeat; }
	SLTH_API void SetRepeat(bool repeat) { m_repeat = repeat; }
};
}