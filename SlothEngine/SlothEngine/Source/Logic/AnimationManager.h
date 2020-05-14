#pragma once
/** \file AnimationManager.h */

// Created by Myles Cardiff

#include <Common.h>
#include <unordered_map>

//! \namespace Sloth Engine
namespace slth
{
	class Animation;
	class IGraphics;
/** \class AnimationManager */

class AnimationManager
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	AnimationManager();

	/** Default Destructor */
	~AnimationManager();
	SLTH_API Animation* Find(Id id);
	SLTH_API Animation* Find(std::string& name) { return Find((HASH(name))); }
	SLTH_API void LoadAnimations(const char* filename, IGraphics* pGraphics);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	std::unordered_map<Id, Animation*> m_animations;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}