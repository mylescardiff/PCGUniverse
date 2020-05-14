#pragma once
/** \file Animation.h */

// Created by Myles Cardiff

#include <Common.h>
#include <string>
#include <unordered_map>

//! \namespace Sloth Engine
namespace slth
{
	class IGraphics;
	class IProcess;
	class AnimationProcess;
/** \class Animation */

class Animation
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SLTH_API Animation();

	/** Default Destructor */
	SLTH_API ~Animation();

	SLTH_API void Init(XmlData* pElement);

	SLTH_API AnimationProcess* CreateProcess();



private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

	std::string m_name;
	std::string m_path;
	float m_frameTimer;
	unsigned short m_frameLength;
	unsigned short m_numFrames;
	bool m_loop;
	



public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API std::string& GetPath() { return m_path; }
	SLTH_API std::string& GetName() { return m_name; }

};
}