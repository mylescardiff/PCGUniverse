#include "GridTile.h"
#include <Utilities/RectInt.h>

using slth::GridTile;

GridTile::GridTile()
	: m_identifier(0)
	, m_coordinates(Vector2(0.f, 0.f))
	, m_pActor(nullptr)
	, m_type(0)
	, m_weight(0)
{
	//
}

slth::GridTile::GridTile(int x, int y, float weight, Color color)
	: m_coordinates(Vector2((float)x, (float)y))
	, m_type(0)
	, m_pActor(nullptr)
	, m_identifier(0)
	, m_weight(weight)
{

}




