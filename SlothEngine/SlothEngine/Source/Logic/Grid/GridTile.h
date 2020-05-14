#pragma once
/** \file GridTile.h */
/** TODO: File Purpose */
// Created by Myles Cardiff

#include <Utilities/Color.h>
#include <Utilities/Vector2.h>
#include <Logic/Actor.h>
#include <Logic/Components/RenderBoxComponent.h>

//! \namespace TODO: Namespace Description
namespace slth
{

	class GridSystem;

/** \class GridTile */
/** Creates a tiled grid on the screen for navigation / graph based games */
class GridTile
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SLTH_API GridTile();
	
	/** Explicit Constructor */
	SLTH_API GridTile(int x, int y, float weight, Color color);



private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	
	float m_weight;
	Vector2 m_coordinates;
	u32 m_identifier; // room ID, etc. 
	u32 m_type; // floor, wall, door, etc. 

	Actor* m_pActor;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API float GetWeight() { return m_weight; }
	
	SLTH_API void SetWeight(float weight) { m_weight = weight; }

	SLTH_API Vector2 GetCoordinates() { return m_coordinates; }

	SLTH_API void SetColor(Color color) { if (m_pActor) { m_pActor->GetComponent<RenderBoxComponent>()->SetColor(color); } }

	SLTH_API void SetIdentifier(u32 identifier) { m_identifier = identifier; }

	SLTH_API u32 GetIdentifier() { return m_identifier; }

	SLTH_API void SetType(u32 type) { m_type = type; }

	SLTH_API u32 GetType() { return m_type; }

	SLTH_API Actor* GetActor() { return m_pActor; }

	SLTH_API void SetActor(Actor* pActor) { m_pActor = pActor; }

	 

};
}