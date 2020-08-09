#include "Ray.h"

namespace RPGEngine
{
	bool Ray::HitsWall(const Boundary& wall, Ray& rOut) const
	{
		const auto numeratorT = (wall.bounds.x - pos.x) * (-dir.y) - (wall.bounds.y - pos.y) * (-dir.x);
		const auto numeratorU = -wall.bounds.w * (wall.bounds.y - pos.y) - (-wall.bounds.h) * (wall.bounds.x - pos.x);
		const auto denominator = -wall.bounds.w * (-dir.y) - (-wall.bounds.h) * (-dir.x);

		if (denominator == 0)
			return false;

		const auto t = numeratorT / denominator;
		const auto u = -numeratorU / denominator;

		if (t >= 0.0f && t <= 1.0f && u > 0.0f)
		{
			rOut.pos.x = static_cast<float>(wall.bounds.x);
			rOut.pos.y = static_cast<float>(wall.bounds.y);
			rOut.dir.x = static_cast<float>(wall.bounds.w * t);
			rOut.dir.y = static_cast<float>(wall.bounds.h * t);

			return true;
		}

		return false;
	}
}