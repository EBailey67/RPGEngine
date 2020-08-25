#pragma once
#include <algorithm>
#include <vector>


#include "../Component/Map.h"
#include "../Utility/Vector2D.h"
#include "../sdlgui/common.h"

namespace RPGEngine
{
	/*
	Implementation of "FOV using recursive shadowcasting - improved" as
	described on http://roguebasin.roguelikedevelopment.org/index.php?title=FOV_using_recursive_shadowcasting_-_improved

	The FOV code is contained in the region "FOV Algorithm".
	The method GetVisibleCells() is called to calculate the cells
	visible to the player by examing each octant sequantially.
	The generic list VisiblePoints contains the cells visible to the player.

	GetVisibleCells() is called everytime the player moves, and the event playerMoved
	is called when a successful move is made (the player moves into an empty cell)
	*/
	class FOVRecurse
	{

	public:
		FOVRecurse(const int px, const int py, const int range, Map* _map)
		{
			map = _map;
			MapSize = Vector2Di(map->mapWidth, map->mapHeight);
			player.x = px;
			player.y = py;
			VisualRange = range;
		}

		// Get the value of the point at the specified location
		int PointGet(const int x, const int y) const
		{
			return map->cell[y][x].isWalkable;
		}


		//  Octant data
		//
		//    \ 1 | 2 /
		//   8 \  |  / 3
		//   -----+-----
		//   7 /  |  \ 4
		//    / 6 | 5 \
		//
		//  1 = NNW, 2 =NNE, 3=ENE, 4=ESE, 5=SSE, 6=SSW, 7=WSW, 8 = WNW

		/// <summary>
		/// Start here: go through all the octants which surround the player to
		/// determine which open cells are visible
		/// </summary>
		void GetVisibleCells()
		{
			VisiblePoints.clear();
			for (auto o : visible_octants)
				ScanOctant(1, o, 1.0, 0.0);

			VisiblePoints.emplace_back(player.x, player.y);     // This spot is always considered visible
		}


		// Move the player in the specified direction provided the cell is valid and empty
		void CalculateFOV()
		{
			GetVisibleCells();
		}


		Vector2Di MapSize;
		int VisualRange; // Radius of the player's circle of vision
		std::vector<Vector2Di> VisiblePoints;   // List of points visible to the player
		[[nodiscard]] Vector2Di GetPlayerPosition() const { return player; }
		void SetPlayerPosition(Vector2Di& point) { player = point; }

	private:
		Map* map;
		Vector2Di player;
		int visible_octants[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };  // The octants which a player can see


		/****************************************************************************************************
												map point code
		****************************************************************************************************/

		// Check if the provided coordinate is within the bounds of the mapp array
		[[nodiscard]] bool Point_Valid(const int pX, const int pY) const
		{
			return pX >= 0 && pX < static_cast<int>(MapSize.x) && pY >= 0 && pY < static_cast<int>(MapSize.y);
		}

		/****************************************************************************************************
												FOV algorithm
		****************************************************************************************************/


	protected:
		/// <summary>
		/// Examine the provided octant and calculate the visible cells within it.
		/// </summary>
		/// <param name="pDepth">Depth of the scan</param>
		/// <param name="pOctant">Octant being examined</param>
		/// <param name="pStartSlope">Start slope of the octant</param>
		/// <param name="pEndSlope">End slope of the octanct</param>
		void ScanOctant(int pDepth, int pOctant, double pStartSlope, double pEndSlope)
		{
			int visrange2 = VisualRange * VisualRange;
			int x = 0;
			int y = 0;

			switch (pOctant)
			{
			case 1: //nnw
				y = player.y - pDepth;
				if (y < 0) return;

				x = player.x - static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (x < 0) x = 0;

				while (GetSlope(x, y, player.x, player.y, false) >= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent) //current cell blocked
						{
							if (x - 1 >= 0 && map->cell[y][x - 1].isTransparent) //prior cell within range AND open...
								//...incremenet the depth, adjust the endslope and recurse
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x - 0.5, y + 0.5, player.x, player.y, false));
						}
						else
						{

							if (x - 1 >= 0 && !map->cell[y][x - 1].isTransparent) //prior cell within range AND open...
								//..adjust the startslope
								pStartSlope = GetSlope(x - 0.5, y - 0.5, player.x, player.y, false);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					x++;
				}
				x--;
				break;

			case 2: //nne
				y = player.y - pDepth;
				if (y < 0) return;

				x = player.x + static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (x >= MapSize.x)
					x = MapSize.x - 1;

				while (GetSlope(x, y, player.x, player.y, false) <= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent)
						{
							if (x + 1 < MapSize.x && map->cell[y][x + 1].isTransparent)
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x + 0.5, y + 0.5, player.x, player.y, false));
						}
						else
						{
							if (x + 1 < MapSize.x && !map->cell[y][x + 1].isTransparent)
								pStartSlope = -GetSlope(x + 0.5, y - 0.5, player.x, player.y, false);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					x--;
				}
				x++;
				break;

			case 3:
				x = player.x + pDepth;
				if (x >= MapSize.x) return;

				y = player.y - static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (y < 0)
					y = 0;

				while (GetSlope(x, y, player.x, player.y, true) <= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent)
						{
							if (y - 1 >= 0 && map->cell[y - 1][x].isTransparent)
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x - 0.5, y - 0.5, player.x, player.y, true));
						}
						else
						{
							if (y - 1 >= 0 && !map->cell[y - 1][x].isTransparent)
								pStartSlope = -GetSlope(x + 0.5, y - 0.5, player.x, player.y, true);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					y++;
				}
				y--;
				break;

			case 4:
				x = player.x + pDepth;
				if (x >= MapSize.x) return;

				y = player.y + static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (y >= MapSize.y)
					y = MapSize.y - 1;

				while (GetSlope(x, y, player.x, player.y, true) >= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent)
						{
							if (y + 1 < MapSize.y && map->cell[y + 1][x].isTransparent)
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x - 0.5, y + 0.5, player.x, player.y, true));
						}
						else
						{
							if (y + 1 < MapSize.y && !map->cell[y + 1][x].isTransparent)
								pStartSlope = GetSlope(x + 0.5, y + 0.5, player.x, player.y, true);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					y--;
				}
				y++;
				break;

			case 5:
				y = player.y + pDepth;
				if (y >= MapSize.y) return;

				x = player.x + static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (x >= MapSize.x)
					x = MapSize.x - 1;

				while (GetSlope(x, y, player.x, player.y, false) >= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent)
						{
							if (x + 1 < MapSize.y && map->cell[y][x + 1].isTransparent)
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x + 0.5, y - 0.5, player.x, player.y, false));
						}
						else
						{
							if (x + 1 < MapSize.y && !map->cell[y][x + 1].isTransparent)
								pStartSlope = GetSlope(x + 0.5, y + 0.5, player.x, player.y, false);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					x--;
				}
				x++;
				break;

			case 6:
				y = player.y + pDepth;
				if (y >= MapSize.y) return;

				x = player.x - static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (x < 0) x = 0;

				while (GetSlope(x, y, player.x, player.y, false) <= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent)
						{
							if (x - 1 >= 0 && map->cell[y][x - 1].isTransparent)
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x - 0.5, y - 0.5, player.x, player.y, false));
						}
						else
						{
							if (x - 1 >= 0 && !map->cell[y][x - 1].isTransparent)
								pStartSlope = -GetSlope(x - 0.5, y + 0.5, player.x, player.y, false);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					x++;
				}
				x--;
				break;

			case 7:
				x = static_cast<int>(player.x) - pDepth;
				if (x < 0) return;

				y = static_cast<int>(player.y) + static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (y >= MapSize.y)
					y = MapSize.y - 1;

				while (GetSlope(x, y, player.x, player.y, true) <= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent)
						{
							if (y + 1 < MapSize.y && map->cell[y + 1][x].isTransparent)
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x + 0.5, y + 0.5, player.x, player.y, true));
						}
						else
						{
							if (y + 1 < MapSize.y && !map->cell[y + 1][x].isTransparent)
								pStartSlope = -GetSlope(x - 0.5, y + 0.5, player.x, player.y, true);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					y--;
				}
				y++;
				break;

			case 8: //wnw
				x = static_cast<int>(player.x) - pDepth;
				if (x < 0)
					return;

				y = static_cast<int>(player.y) - static_cast<int>((pStartSlope * static_cast<double>(pDepth)));
				if (y < 0)
					y = 0;

				while (GetSlope(x, y, player.x, player.y, true) >= pEndSlope)
				{
					if (GetVisDistance(x, y, player.x, player.y) <= visrange2)
					{
						if (!map->cell[y][x].isTransparent)
						{
							if (y - 1 >= 0 && map->cell[y - 1][x].isTransparent)
								ScanOctant(pDepth + 1, pOctant, pStartSlope, GetSlope(x + 0.5, y - 0.5, player.x, player.y, true));
						}
						else
						{
							if (y - 1 >= 0 && !map->cell[y - 1][x].isTransparent)
								pStartSlope = GetSlope(x - 0.5, y - 0.5, player.x, player.y, true);

							VisiblePoints.emplace_back(Vector2Di(x, y));
						}
					}
					y++;
				}
				y--;
				break;
			}

			x = std::clamp(x, 0, static_cast<int>(MapSize.x - 1));
			y = std::clamp(y, 0, static_cast<int>(MapSize.y - 1));

			if (pDepth < VisualRange && map->cell[y][x].isTransparent)
				ScanOctant(pDepth + 1, pOctant, pStartSlope, pEndSlope);
		}

		// Get the gradient of the slope formed by the two points
		static double GetSlope(const double pX1, const double pY1, const double pX2, const double pY2, const bool pInvert)
		{
			if (pInvert)
				return (pY1 - pY2) / (pX1 - pX2);

			return (pX1 - pX2) / (pY1 - pY2);
		}


		// Calculate the distance between the two points
		static int GetVisDistance(const int pX1, const int pY1, const int pX2, const int pY2)
		{
			return (pX1 - pX2) * (pX1 - pX2) + (pY1 - pY2) * (pY1 - pY2);
		}
	};
};
