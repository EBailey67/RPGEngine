#pragma once

#include <array>
#include <utility>
#include <vector>
#include <SDL_render.h>

#include "config.hpp"
#include "fwd.hpp"

#define PROFILING 1  // NOLINT(cppcoreguidelines-macro-usage)
#include "../Utility/Instrumentor.h"
#include "../Utility/Primitives.h"
#include "../Utility/Vector2D.h"

enum class Layer
{
    None = -1,
    Floor = 0,
    Walls = 1,
    Objects = 2,
    Mobs = 3,
	Lights = 4,
    Debug = 5,
    UI = 6,
};

class Graphics  // NOLINT(cppcoreguidelines-special-member-functions)
{
    friend Game;

    static constexpr const auto reset_layer = Layer::None;

private:
	struct Edge
	{
      int yUpper;
      float xIntersect;
      float dxPerScan;
      std::shared_ptr<Edge> next;
    };

	static int yNext(const int k, const int cnt, const std::vector<Vector2D>& pts)
    {
	    int j;

	    if ((k + 1) > (cnt - 1))
	    {
		    j = 0;
		}
	    else
	    {
		    j = (k + 1);
		}

	    while (pts[k].y == pts[j].y)
	    {
		    if ((j + 1) > (cnt - 1))
			    j = 0;
		    else
			    j++;
	    }

	    return static_cast<int>(pts[j].y);
    }

	static void insertEdge(const std::shared_ptr<Edge> list, const std::shared_ptr<Edge>& edge)
	{
		auto q = list;
		auto p = q->next;

		while (p != nullptr)
		{
			if (edge->xIntersect < p->xIntersect)
			{
				p = nullptr;
			}
			else
			{
				q = p;
				p = p->next;
			}
		}

		edge->next = q->next;
		q->next = edge;
	}

	static void makeEdgeRec(const Vector2D& lower, const Vector2D& upper,
		const int yComp, const std::shared_ptr<Edge>& edge, std::vector<std::shared_ptr<Edge>>& edges)
	{
		edge->dxPerScan = ((upper.x - lower.x) / (upper.y - lower.y));
		edge->xIntersect = lower.x;

		if (static_cast<int>(upper.y) < yComp)
			edge->yUpper = static_cast<int>(upper.y - 1);
		else
			edge->yUpper = static_cast<int>(upper.y);

		insertEdge(edges[static_cast<int>(lower.y)], edge);
	}

	static void buildEdgeList(const int cnt, const std::vector<Vector2D>& pts, std::vector<std::shared_ptr<Edge>>& edges)
	{
		Vector2D v1;

		auto yPrev = static_cast<int>(pts[static_cast<size_t>(cnt) - 2].y);

		v1.x = pts[static_cast<size_t>(cnt) - 1].x;
		v1.y = pts[static_cast<size_t>(cnt) - 1].y;

		for (auto count = 0; count < cnt; count++)
		{
			auto v2 = pts[count];

			if (v1.y != v2.y)
			{
				auto const edge = std::make_shared<Edge>();

				if (v1.y < v2.y)
					makeEdgeRec(v1, v2, yNext(count, cnt, pts), edge, edges);
				else
					makeEdgeRec(v2, v1, yPrev, edge, edges);
			}

			yPrev = static_cast<int>(v1.y);
			v1 = v2;
		}
	}

	static void buildActiveList(const int row, const std::shared_ptr<Edge>& active, const std::vector<std::shared_ptr<Edge>>& edges)
	{
		auto p = edges[row]->next;

		while (p)
		{
			auto q = p->next;
			insertEdge(active, p);
			p = q;
		}
	}

	static void fillScan(const int row, const std::shared_ptr<Edge> active)
	{
		auto p1 = active->next;

		while (p1)
		{
			auto p2 = p1->next;
			if (p2 != nullptr)
			{
				DrawLineToLayer(m_currentLayer, static_cast<int>(p1->xIntersect), row, static_cast<int>(p2->xIntersect), row);
				p1 = p2->next;
			}
			else
			{
				p1 = nullptr;
			}
		}
	}

	static void deleteAfter(std::shared_ptr<Edge> q)
	{
		auto const p = q->next;
		q->next = p->next;
		// delete p;
	}

	static void updateActiveList(const int scan, std::shared_ptr<Edge>& active)
	{
		auto q = active;
		auto p = active->next;

		while (p)
		{
			if (scan >= p->yUpper)
			{
				p = p->next;
				deleteAfter(q);
			}
			else
			{
				p->xIntersect = (p->xIntersect + p->dxPerScan);
				q = p;
				p = p->next;
			}
		}
	}

	static void resortActiveList(std::shared_ptr<Edge>& active)
	{
		auto p = active->next;

		active->next = nullptr;

		while (p)
		{
			auto const q = p->next;
			insertEdge(active, p);
			p = q;
		}
	}

	// https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
	static bool linelineIntersection(RPGEngine::LineSegment& l1, RPGEngine::LineSegment& l2, Vector2D& intersectionPoint)
	{
		// Line AB represented as a1x + b1y = c1 
		const auto a1 = l1.b.y - l1.a.y;
		const auto b1 = l1.a.x - l1.b.x;
		const auto c1 = a1 * l1.a.x + b1 * l1.a.y;

		// Line CD represented as a2x + b2y = c2 
		const auto a2 = l2.b.y - l2.a.y;
		const auto b2 = l2.a.x - l2.b.x;
		const auto c2 = a2 * (l2.a.x) + b2 * (l2.a.y);

		const auto determinant = a1 * b2 - a2 * b1;

		if (determinant == 0)
		{
			// The lines are parallel. This is simplified 
			// by returning a pair of FLT_MAX
			intersectionPoint.x = FLT_MAX;
			intersectionPoint.y = FLT_MAX;
			return false;
		}
		else
		{
			intersectionPoint.x = floorf( (b2 * c1 - b1 * c2) / determinant);
			intersectionPoint.y = floorf((a1 * c2 - a2 * c1) / determinant);
			return true;
		}
	}

	static bool isInsideClipWindow(const Vector2D p, const int edge_id, RPGEngine::LineSegment& edge)
    {
		switch(edge_id)
		{
		case 0:	// TOPEDGE
           return (p.y >= edge.a.y);
 		case 1: // RIGHTEDGE
           return (p.x <= edge.b.x);
 		case 2: // BOTTOMEDGE
           return (p.y <= edge.b.y);
 		case 3: // LEFTEDGE
           return (p.x >= edge.a.x);
 		default:
			return false;
		}
    }

	// http://www.sunshine2k.de/coding/java/SutherlandHodgman/SutherlandHodgman.html
	static std::vector<Vector2D> PolygonClipToRect(SDL_Rect& rClip, std::vector<Vector2D> coordinates)
	{
		std::vector<Vector2D> clippedPolygon;
		std::vector<Vector2D> polygon;

		if(coordinates.size() < 3)
			return polygon;

		// Clean up the coordinates
		for (auto& point : coordinates)
		{
			point.x = floorf(point.x);
			point.y = floorf(point.y);
			polygon.emplace_back(point);
		}

		enum
		{
			TOPEDGE  = 0,
			RIGHTEDGE = 1,
			BOTTOMEDGE = 2,
			LEFTEDGE = 3,
		};
		
		std::vector<RPGEngine::LineSegment> edges(4);
		edges[TOPEDGE] = RPGEngine::LineSegment(Vector2D(rClip.x, rClip.y), Vector2D(rClip.x + rClip.w, rClip.y));
		edges[RIGHTEDGE] = RPGEngine::LineSegment(Vector2D(rClip.x + rClip.w, rClip.y), Vector2D(rClip.x + rClip.w, rClip.y + rClip.h));
		edges[BOTTOMEDGE] = RPGEngine::LineSegment(Vector2D(rClip.x, rClip.y + rClip.h), Vector2D(rClip.x + rClip.w, rClip.y + rClip.h));
		edges[LEFTEDGE] = RPGEngine::LineSegment(Vector2D(rClip.x, rClip.y), Vector2D(rClip.x, rClip.y + rClip.h));
		
		// Do this for each edge
		for (auto j = 0; j < 4; j++)
		{
			clippedPolygon.clear();
			for(auto i = 0; i < static_cast<int>(polygon.size()) - 1; i++)
			{
				const auto pi = polygon[i];
				const auto pi1 = polygon[static_cast<size_t>(i) + 1];
				if (isInsideClipWindow(pi, j, edges[j]))
				{
	               if (isInsideClipWindow(pi1, j, edges[j]))
	               {
	                    clippedPolygon.emplace_back(pi1);
				   }
	               else
	               {
	               		Vector2D ptInter;
						RPGEngine::LineSegment ls(pi, pi1);
	               		if (linelineIntersection(ls, edges[j], ptInter))
							clippedPolygon.emplace_back(ptInter);
				   }
				}
				else
				{
	               if (isInsideClipWindow(pi1, j, edges[j]))
	               {
	               		Vector2D ptInter;
						RPGEngine::LineSegment ls(pi, pi1);
	               		if (linelineIntersection(ls, edges[j], ptInter))
	               		{
		                    clippedPolygon.emplace_back(ptInter);
						}
	                    clippedPolygon.emplace_back(pi1);
				   }
				}
			}

			if (clippedPolygon.size() < 3)
			{
				polygon.clear();
				return polygon;
			}
			
			if(clippedPolygon[0] != clippedPolygon[clippedPolygon.size() - 1])
				clippedPolygon.emplace_back(clippedPolygon[0]);
			std::swap(clippedPolygon, polygon);
		}

		for(auto& point : polygon)
		{
			assert(point.InRect(rClip) == true);
		}
		
		return polygon;
	}
	
    static void Polygon(std::vector<Vector2D> coordinates)
	{
		if (coordinates.size() >= 2)
		{
			DrawLineToLayer(m_currentLayer, static_cast<int>(coordinates[0].x), static_cast<int>(coordinates[0].y),
				static_cast<int>(coordinates[coordinates.size() - 1].x), static_cast<int>(coordinates[coordinates.size() - 1].y));

			for (auto count = 0; count < coordinates.size() - 1; count++)
				DrawLineToLayer(m_currentLayer, static_cast<int>(coordinates[count].x), static_cast<int>(coordinates[count].y),
					static_cast<int>(coordinates[count + 1].x),	static_cast<int>(coordinates[count + 1].y));
		}
	}

	// https://www.codepoc.io/blog/cpp/2830/program-to-fill-a-polygon-using-scan-line-polygon-fill-algorithm
	// 
	
	static void FillPolygon(const std::vector<Vector2D>& pointsIn)
	{
		// Make sure we're given a valid polygon
		if (pointsIn.size() < 3)
			return;

		PROFILE_FUNCTION();
		std::vector<std::shared_ptr<Edge>> edges(2000);

		SDL_Rect clipRegion;
		clipRegion.x = 0;
		clipRegion.y = 0;
		clipRegion.w = 3000;
		clipRegion.h = 1600;
		std::vector<Vector2D> points = PolygonClipToRect(clipRegion, pointsIn);

		if (points.size() < 3)
			return;
		
		for(auto& point : points)
		{
			assert(point.InRect(clipRegion) == true);
		}

		
		std::vector<Vector2D> pts;
		std::copy(points.begin(), points.end(), std::back_inserter(pts));

		// Initialize the edges
		for (auto& edge : edges)
		{
			edge = std::make_shared<Edge>();
			edge->next = nullptr;
		}

		buildEdgeList(static_cast<int>(points.size()), pts, edges);

		auto active = std::make_shared<Edge>(); 
		active->next = nullptr;
		
		for (auto row = 0; row < 1600; row++)
		{
			buildActiveList(row, active, edges);

			if (active->next)
			{
				fillScan(row, active);
				updateActiveList(row, active);
				resortActiveList(active);
			}
		}

		Polygon(points);
	}


	Graphics() = default;

    ~Graphics() = default;

    static void AssignWindow(SDL_Window *window, const Uint32 rendererFlags = 0)
    {
        DestroyData();
        m_window = window;

        m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
        if (!m_renderer)
        {
            DebugError("Graphics", SDL_GetError());
            SDL_THROW();
        }

        if (SDL_RenderTargetSupported(m_renderer) == SDL_FALSE)
        {
            SDL_THROW();
        }

    	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        CreateLayers();
    }

    static void CreateLayers()
    {
        auto [w, h] = WindowSize();
        for (auto &layer : m_layers)
        {
            layer = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
            if (!layer)
            {
                SDL_THROW();
            }
            SDL_SetTextureBlendMode(layer, SDL_BLENDMODE_BLEND);
        }
    }

    static void DestroyLayers()
    {
        for (auto &layer : m_layers)
        {
            if (layer)
            {
                SDL_DestroyTexture(layer);
            }
        }
    }

    static void OnRenderStart()
    {
        // clear all layers textures.
        for (auto &layer : m_layers)
        {
            if (SDL_SetRenderTarget(m_renderer, layer))
            {
                SDL_THROW();
            }
            TargetClear();
        }
        // reset renderer
        ResetRenderer();

        m_currentLayer = reset_layer;
    }

    static void RenderPresent()
    {
        SDL_RenderPresent(m_renderer);
    }

    static void DrawLayers()
    {
        ResetRenderer();
        TargetClear();
        for (auto* layer : m_layers)
        {
            if (SDL_RenderCopy(m_renderer, layer, nullptr, nullptr))
            {
                SDL_THROW();
            }
        }
    }

    static void DestroyData()
    {
        if (m_window)
        {
            DestroyLayers();
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
        }
    }

public:
    static void RenderTarget(Layer layer)
    {
        if (SDL_SetRenderTarget(m_renderer, m_layers.at(static_cast<int>(layer))))
        {
            SDL_THROW();
        }
        m_currentLayer = layer;
    }

    static void ResetRenderer()
    {
        if (SDL_SetRenderTarget(m_renderer, nullptr))
        {
            SDL_THROW();
        }
    }

    static void TargetClear()
    {
    	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        SDL_RenderClear(m_renderer);
    }

    static void LayerClear(const Layer layer, const SDL_Color color)
    {

    	if (m_currentLayer != layer)
    	{
    		RenderTarget(layer);
    	}
    	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_renderer);
    }
		
	static std::pair<int, int> WindowSize() noexcept
    {
        SSECS_ASSERT(m_window);
        int w, h;
        SDL_GetWindowSize(m_window, &w, &h);
        return std::make_pair(w, h);
    }

    static SDL_Window *Window() noexcept
    {
        return m_window;
    }

    static SDL_Renderer *Renderer() noexcept
    {
        return m_renderer;
    }

    static void RenderToLayer(const Layer layer, SDL_Texture *texture, const SDL_Rect *src_rect = nullptr,
                              const SDL_Rect *dst_rect = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderCopyEx(m_renderer, texture, src_rect, dst_rect, 0, nullptr, flip))
        {
            SDL_THROW();
        }
    }

    static void RenderToLayerF(const Layer layer, SDL_Texture *texture, const SDL_Rect *src_rect = nullptr,
                               const SDL_FRect *dst_rect = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }
        if (SDL_RenderCopyExF(m_renderer, texture, src_rect, dst_rect, 0, nullptr, flip))
        {
            SDL_THROW();
        }
    }

    static void DrawRectToLayer(Layer layer, const SDL_Rect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderDrawRect(m_renderer, rect))
        {
            SDL_THROW();
        }
    }

    static void DrawRectToLayerF(Layer layer, const SDL_FRect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderDrawRectF(m_renderer, rect))
        {
            SDL_THROW();
        }
    }

    static void DrawFillRectToLayer(Layer layer, const SDL_Rect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }
        if (SDL_RenderFillRect(m_renderer, rect))
        {
            SDL_THROW();
        }
    }
    static void DrawFillRectToLayerF(Layer layer, const SDL_FRect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderFillRectF(m_renderer, rect))
        {
            SDL_THROW();
        }
    }

    static void DrawLineToLayer(Layer layer, int x1, int y1, int x2, int y2)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2))
        {
            SDL_THROW();
        }
    }

    static void SetDrawColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = SDL_ALPHA_OPAQUE) noexcept
    {
        SDL_SetRenderDrawColor(Graphics::Renderer(), r, g, b, a);
    }

    static void ResetDrawColor() noexcept
    {
        SDL_SetRenderDrawColor(Graphics::Renderer(), 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    }

	static int DrawCircleToLayer(const Layer layer, const int x, const int y, const int radius)
	{
	    int offsetx = 0;
    	int offsety = radius;
    	int d = radius - 1;
	    int status = 0;

        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }
    	
	    while (offsety >= offsetx) 
        {
	        status += SDL_RenderDrawPoint(m_renderer, x + offsetx, y + offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x + offsety, y + offsetx);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsetx, y + offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsety, y + offsetx);
	        status += SDL_RenderDrawPoint(m_renderer, x + offsetx, y - offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x + offsety, y - offsetx);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsetx, y - offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsety, y - offsetx);

	        if (status < 0) 
            {
	            status = -1;
	            break;
	        }

	        if (d >= 2*offsetx) 
            {
	            d -= 2*offsetx + 1;
	            offsetx +=1;
	        }
	        else if (d < 2 * (radius - offsety)) 
            {
	            d += 2 * offsety - 1;
	            offsety -= 1;
	        }
	        else 
            {
	            d += 2 * (offsety - offsetx - 1);
	            offsety -= 1;
	            offsetx += 1;
	        }
	    }

	    return status;
	}


	static int DrawFillCircleToLayer(const Layer layer, const int x, const int y, const int radius)
	{
	    int offsetx = 0;
    	int offsety = radius;
    	int d = radius - 1;
	    int status = 0;

        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

	    while (offsety >= offsetx) 
        {

	        status += SDL_RenderDrawLine(m_renderer, x - offsety, y + offsetx,
	                                     x + offsety, y + offsetx);
	        status += SDL_RenderDrawLine(m_renderer, x - offsetx, y + offsety,
	                                     x + offsetx, y + offsety);
	        status += SDL_RenderDrawLine(m_renderer, x - offsetx, y - offsety,
	                                     x + offsetx, y - offsety);
	        status += SDL_RenderDrawLine(m_renderer, x - offsety, y - offsetx,
	                                     x + offsety, y - offsetx);

	        if (status < 0) 
            {
	            status = -1;
	            break;
	        }

	        if (d >= 2*offsetx) 
            {
	            d -= 2*offsetx + 1;
	            offsetx +=1;
	        }
	        else if (d < 2 * (radius - offsety)) 
            {
	            d += 2 * offsety - 1;
	            offsety -= 1;
	        }
	        else 
            {
	            d += 2 * (offsety - offsetx - 1);
	            offsety -= 1;
	            offsetx += 1;
	        }
	    }

	    return status;
	}

    static void DrawPolygonToLayer(const Layer layer, std::vector<Vector2D>& vertices)
    {
	    if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

    	Polygon(vertices);
    }

    static void DrawFillPolygonToLayer(const Layer layer, std::vector<Vector2D>& vertices)
    {
	    if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

	    FillPolygon(vertices);
    }
	
	static void DrawTriangleToLayer(const Layer layer, Vector2D& pos1, Vector2D& pos2, Vector2D& pos3)
	{
		DrawTriangleToLayer(layer, 
            static_cast<int>(pos1.x), static_cast<int>(pos1.y), 
            static_cast<int>(pos2.x), static_cast<int>(pos2.y), 
            static_cast<int>(pos3.x), static_cast<int>(pos3.y));
	}

	static void DrawTriangleToLayer(const Layer layer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
	{
		DrawLineToLayer(layer, x1, y1, x2, y2);
		DrawLineToLayer(layer, x2, y2, x3, y3);
		DrawLineToLayer(layer, x3, y3, x1, y1);
	}

	static void DrawFillTriangleToLayer(const Layer layer, Vector2D& pos1, Vector2D& pos2, Vector2D& pos3)
	{
		DrawFillTriangleToLayer(layer, 
            static_cast<int>(pos1.x), static_cast<int>(pos1.y), 
            static_cast<int>(pos2.x), static_cast<int>(pos2.y), 
            static_cast<int>(pos3.x), static_cast<int>(pos3.y));
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
	static void DrawFillTriangleToLayer(const Layer layer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
	{
		auto drawline = [&](const int sx, const int ex, const int ny) { DrawLineToLayer(layer, sx, ny, ex, ny); };

		int t1_x, t2_x, y, minx, maxx, t1xp, t2xp;
		auto changed1 = false;
		auto changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;

		// Sort vertices
		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1_x = t2_x = x1; y = y1;   // Starting points
		dx1 = static_cast<int>(x2 - x1);
		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
		{
			signx1 = 1;
		}
    	
		dy1 = static_cast<int>(y2 - y1);

		dx2 = static_cast<int>(x3 - x1);
		if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = static_cast<int>(y3 - y1);

		if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
		if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		e2 = static_cast<int>(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) 
			goto next;  // NOLINT(cppcoreguidelines-avoid-goto)
		e1 = static_cast<int>(dx1 >> 1);

		for (auto i = 0; i < dx1;) 
		{
			t1xp = 0; t2xp = 0;
			if (t1_x < t2_x)
			{
				minx = t1_x;
				maxx = t2_x;
			}
			else
			{
				minx = t2_x;
				maxx = t1_x;
			}
			
			// process first line until y value is about to change
			while (i < dx1) 
			{
				i++;
				e1 += dy1;
				while (e1 >= dx1) 
				{
					e1 -= dx1;
					if (changed1) 
						t1xp = signx1;//t1x += signx1;
					else          
						goto next1;
				}
				if (changed1) break;
				t1_x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (true) 
			{
				e2 += dy2;
				while (e2 >= dx2) 
				{
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				t2_x += signx2;
			}
		next2:
			if (minx > t1_x) minx = t1_x;
			if (minx > t2_x) minx = t2_x;
			if (maxx < t1_x) maxx = t1_x;
			if (maxx < t2_x) maxx = t2_x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y

			// Now increase y
			if (!changed1) t1_x += signx1;
			t1_x += t1xp;
			if (!changed2) t2_x += signx2;
			t2_x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = static_cast<int>(x3 - x2);
    	if (dx1 < 0)
    	{
    		dx1 = -dx1; signx1 = -1;
    	}
		else
		{
			signx1 = 1;
		}
		dy1 = static_cast<int>(y3 - y2);
		t1_x = x2;

		if (dy1 > dx1) 
		{   // swap values
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = static_cast<int>(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) 
		{
			t1xp = 0; t2xp = 0;
			if (t1_x < t2_x)
			{
				minx = t1_x;
				maxx = t2_x;
			}
			else
			{
				minx = t2_x;
				maxx = t1_x;
			}
			
			// process first line until y value is about to change
			while (i < dx1) 
			{
				e1 += dy1;
				while (e1 >= dx1) 
				{
					e1 -= dx1;
					if (changed1)
					{
						t1xp = signx1;
						break;
					}//t1x += signx1;
					goto next3;
				}
				if (changed1) 
					break;
				t1_x += signx1;
				if (i < dx1) 
					i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2_x != x3) 
			{
				e2 += dy2;
				while (e2 >= dx2) 
				{
					e2 -= dx2;
					if (changed2) 
						t2xp = signx2;
					else
						goto next4;
				}
				if (changed2)
					break;
				t2_x += signx2;
			}
		next4:

			if (minx > t1_x) minx = t1_x;
			if (minx > t2_x) minx = t2_x;
			if (maxx < t1_x) maxx = t1_x;
			if (maxx < t2_x) maxx = t2_x;
			drawline(minx, maxx, y);
			if (!changed1) t1_x += signx1;
			t1_x += t1xp;
			if (!changed2) t2_x += signx2;
			t2_x += t2xp;
			y += 1;
			if (y > y3) return;
		}
	}
	
private:
    static inline SDL_Window *m_window = nullptr;
    static inline SDL_Renderer *m_renderer = nullptr;
    static inline std::array<SDL_Texture *, SDL_RENDER_LAYERS> m_layers{};
    static inline Layer m_currentLayer = reset_layer;
};

