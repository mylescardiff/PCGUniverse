#include "TransformComponent.h"
#include <Logic/Components/RigidBodyComponent.h>
#include <Logic/Actor.h>
#include "TransformComponent.gen.h"

using slth::TransformComponent;

slth::IComponent * slth::TransformComponent::Copy(Actor * pNewOwner)
{
	auto pTransComp = new TransformComponent(*this);
	pTransComp->m_position.x = 0;
	pTransComp->m_position.y = 0;
	return pTransComp;
}

TransformComponent::~TransformComponent()
{
	//
}

bool slth::TransformComponent::Init(float x, float y, float width, float height, float rotation)
{
	m_size = Vector2(width, height);
	m_position = Vector2(x, y);
	m_rotation = rotation;
	return true;
}

bool slth::TransformComponent::Init(XmlData* xmlNode)
{
	// transform can also take anchor positions so if you wanted the floor 
	// to always be at the bottom regardless of screen resolution, for example
	const char* testForAnchors = xmlNode->Attribute("anchor-left");
	if (testForAnchors)
	{
		m_useAnchorPosition = true;
		m_anchorRect.left = xmlNode->FloatAttribute("anchor-left");
		m_anchorRect.top = xmlNode->FloatAttribute("anchor-top");
		m_anchorRect.right = xmlNode->FloatAttribute("anchor-right");
		m_anchorRect.bottom = xmlNode->FloatAttribute("anchor-bottom");

		m_positionRect.left = (int)xmlNode->Int64Attribute("position-left");
		m_positionRect.top = (int)xmlNode->Int64Attribute("position-top");
		m_positionRect.right = (int)xmlNode->Int64Attribute("position-right");
		m_positionRect.bottom = (int)xmlNode->Int64Attribute("position-bottom");
	}
	else
	{
		m_useAnchorPosition = false;
		this->m_position.x = xmlNode->FloatAttribute("x");
		this->m_position.y = xmlNode->FloatAttribute("y");
		m_size.x = xmlNode->FloatAttribute("w");
		m_size.y = xmlNode->FloatAttribute("h");
	}
	
	this->m_rotation = xmlNode->FloatAttribute("rotation");
	this->m_velocity.x = xmlNode->FloatAttribute("velocity-x");
	this->m_velocity.y = xmlNode->FloatAttribute("velocity-y");

	return true;
}

bool slth::TransformComponent::PostInit()
{	
	return true;
}

void slth::TransformComponent::Move(Vector2 amount)
{
	m_position += amount;
	RigidBodyComponent* pRigidBody = m_pOwner->GetComponent<RigidBodyComponent>();
	if (pRigidBody)
	{
		pRigidBody->Move(amount);
	}
}

void slth::TransformComponent::Move(float x, float y)
{
	Vector2 v(x, y); 
	Move(v);
}

void slth::TransformComponent::Update(float deltaTime)
{
	m_position.x += m_velocity.x * deltaTime;
	m_position.y += m_velocity.y * deltaTime;
}


void slth::TransformComponent::RegisterToLuaState(lua_State * pState)
{
	luaL_newmetatable(pState, StringId);				// Stack: table

	// metatable.__index = 
	luaL_getmetatable(pState, StringId);
	lua_setfield(pState, -2, "__index");
	
	// Add other functions
	lua_pushcfunction(pState, &glua::TransformComponent_Move);	// Stack: table, function
	lua_setfield(pState, -2, "move");							// Stack: table

	//
	lua_pop(pState, 1);											// Stack:
							
}

slth::Vector2 slth::TransformComponent::GetPosition() const
{
	if (m_useAnchorPosition && m_pWorld)
	{
		Vector2 anchoredPosition(
			m_pWorld->GetSize().x * (float)m_anchorRect.left + m_positionRect.left * m_pWorld->GetDPI(),
			m_pWorld->GetSize().y * (float)m_anchorRect.top + m_positionRect.top * m_pWorld->GetDPI()
		);
		return anchoredPosition;
	}
	else
	{
		return m_position;
	}
}

void slth::TransformComponent::SetPosition(Vector2 position)
{
	m_position = position; 
}

void slth::TransformComponent::SetSize(Vector2 size)
{
	m_size = size;
	
}

slth::RectInt slth::TransformComponent::GetPositionRect() const
{
	return RectInt((int)m_position.x, (int)m_position.y, (int)m_position.x + (int)m_size.x, (int)m_position.y + (int)m_size.y);
}

void slth::TransformComponent::SetWorld(World* pWorld)
{
	m_pWorld = pWorld;
	if (m_useAnchorPosition)
	{
		RectInt realPositionRect(
			(int)(m_pWorld->GetSize().x * (float)m_anchorRect.left + (float)m_positionRect.left * m_pWorld->GetDPI()),
			(int)(m_pWorld->GetSize().y * (float)m_anchorRect.top + (float)m_positionRect.top * m_pWorld->GetDPI()),
			(int)(m_pWorld->GetSize().x * (float)m_anchorRect.right + (float)m_positionRect.right * m_pWorld->GetDPI()),
			(int)(m_pWorld->GetSize().y * (float)m_anchorRect.bottom + (float)m_positionRect.bottom * m_pWorld->GetDPI())

		);
		m_size.x = (float)realPositionRect.right - (float)realPositionRect.left;
		m_size.y = (float)realPositionRect.bottom - (float)realPositionRect.top;
		m_position.x = (float)realPositionRect.left;
		m_position.y = (float)realPositionRect.top;
	}
}


