#pragma once
#include <math.h>
#include <cmath>
#include <Common.h>

namespace slth
{
	class Vector2
	{

	public:

		float x;
		float y;

		SLTH_API constexpr Vector2()
			: x(0)
			, y(0)
		{}

		SLTH_API constexpr Vector2(float x, float y)
			: x(x)
			, y(y)
		{}

		SLTH_API ~Vector2() {}

		// position helper functions
		SLTH_API static float IndexFromPosition(float x, float y, float width)
		{
			return (width * y) + x;
		}
		SLTH_API static Vector2 PositionFromIndex(int index, int width)
		{
			return Vector2(static_cast<float>(floor(index / width)), static_cast<float>(floor(fmod(index, width))));
		}
		SLTH_API static float Determinant(const Vector2 vectorA, const Vector2 vectorB)
		{
			return vectorA.x * vectorB.y - vectorA.y * vectorB.x;
		}

		SLTH_API static float AngleBetween(const Vector2 vectorA, const Vector2 vectorB)
		{
			return atan2(Determinant(vectorA, vectorB), DotProduct(vectorA, vectorB));
		}

		SLTH_API static float DotProduct(const Vector2 vectorA, const Vector2 vectorB)
		{
			return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
		}

		SLTH_API Vector2 Round()
		{
			return Vector2(std::round(x), std::round(y));
			
		}

		/** Makes the magnitude of the vector 1.0f */
		SLTH_API void Normalize()
		{
			float magnitude = sqrt(x * x + y * y);
			x = x / magnitude;
			y = y / magnitude;

		}

		SLTH_API float DistanceTo(const Vector2 otherVector)
		{
			return abs(sqrt((x - otherVector.x) * (x - otherVector.x) + (y - otherVector.y) * (y - otherVector.y)));
		}

		// finds the direction of another object on the screen
		SLTH_API Vector2 BearingTo(Vector2 target, float speed)
		{
			float xDiff = x - target.x;
			float yDiff = y - target.y;
			float xChange = -(xDiff / yDiff) * speed;
			return Vector2(xChange, -speed);
		}


		/** Rotates the vecror around the origin by degrees, WARNING: this is not super precicse for floats, round to int if you need it to be exact */
		SLTH_API Vector2 Rotate(float degrees)
		{
			float rads = degrees * (3.14f / 180.f); // conversion to radians
			float newX = (x * std::cos(rads)) - (y * std::sin(rads));
			float newY = (x * std::sin(rads)) + (y * std::cos(rads));

			return { newX, newY };
		}
		// credit: https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
		SLTH_API Vector2 Rotate(float degrees, Vector2 center)
		{
			float rads = degrees * (kPi / 180.f);

			Vector2 returnValue = *this;

			returnValue.x -= center.x;
			returnValue.y -= center.y;

			Vector2 newPos(
				returnValue.x * cos(rads) - returnValue.y * sin(rads),
				returnValue.x * sin(rads) + returnValue.y * cos(rads)
			);
		
			returnValue.x = newPos.x + center.x;
			returnValue.y = newPos.y + center.y;

			return returnValue;
		}

		// equality 
		SLTH_API bool operator==(const Vector2& other) const
		{
			if (x == other.x && y == other.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		SLTH_API bool operator!=(const Vector2& other) const
		{
			return !(*this == other);
		}

		// addition
		SLTH_API Vector2& operator+=(const Vector2& right)
		{
			x += right.x;
			y += right.y;
			return *this;
		}
		// subtraction
		SLTH_API Vector2& operator-=(const Vector2& right)
		{
			x -= right.x;
			y -= right.y;
			return *this;
		}

		SLTH_API friend Vector2 operator+(const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x + right.x, left.y + right.y);
		}

		SLTH_API friend Vector2 operator*(const Vector2& left, const float right)
		{
			return Vector2(left.x * right, left.y * right);
		}
		// subtraction 
		SLTH_API friend Vector2 operator-(const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x - right.x, left.y - right.y);

		}
		SLTH_API friend Vector2 operator/(const Vector2& left, const float right)
		{
			return Vector2(left.x / right, left.y / right);
		}

		/** Lerp */
		SLTH_API static float Lerp(slth::Vector2 vec, float amount)
		{
			return (1.0f - amount) * vec.x + amount * vec.y;
		}

	};
}