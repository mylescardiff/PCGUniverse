#pragma once
/** \file IDrawableComponent.h */

// Created by Myles Cardiff

#include ".\IComponent.h"


//! \namespace Sloth Engine
namespace slth
{
	class IGraphics;

/** \class IDrawableComponent */

class IDrawableComponent
	: public IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //


	/** Default Destructor */
	SLTH_API virtual ~IDrawableComponent();

	SLTH_API virtual const char* GetName() = 0;
	SLTH_API virtual void Draw(IGraphics* pGraphics) = 0;
	SLTH_API virtual bool PostInit() { return true; }

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

};
}