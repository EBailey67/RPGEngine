/*
    sdlgui/colorwheel.cpp -- fancy analog widget to select a color value

    This widget was contributed by Dmitriy Morozov.

    Based on NanoGUI by Wenzel Jakob <wenzel@inf.ethz.ch>.
    Adaptation for SDL by Dalerank <dalerankn8@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "colorwheel.h"
#include "theme.h"
#include <SDL.h>
#include <cmath>

#include "nanovg.h"
#include "nanovg_rt.h"

namespace GUI
{
	struct ColorWheel::AsyncTexture
    {
        int id;
        Texture tex;
        NVGcontext* ctx = nullptr;

        AsyncTexture(int _id) : id(_id) {};

        void load(ColorWheel* ptr)
        {
            ColorWheel* colorWheel = ptr;
            AsyncTexture* self = this;
            std::thread tgr([=]() {
                std::lock_guard<std::mutex> guard(colorWheel->theme()->loadMutex);

                NVGcontext* ctx = nullptr;
                int realw, realh;
                colorWheel->renderBodyTexture(ctx, realw, realh);
                self->tex.rrect = { 0, 0, realw, realh };
                self->ctx = ctx;
                });

            tgr.detach();
        }

        void perform(SDL_Renderer* renderer)
        {
            if (!ctx)
                return;

            unsigned char* rgba = nvgReadPixelsRT(ctx);

            tex.tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, tex.w(), tex.h());

            int pitch;
            uint8_t* pixels;
            int ok = SDL_LockTexture(tex.tex, nullptr, (void**)&pixels, &pitch);
            memcpy(pixels, rgba, sizeof(uint32_t) * tex.w() * tex.h());
            SDL_SetTextureBlendMode(tex.tex, SDL_BLENDMODE_BLEND);
            SDL_UnlockTexture(tex.tex);

            nvgDeleteRT(ctx);
            ctx = nullptr;
        }
    };

    ColorWheel::ColorWheel(Widget* parent, const Color& rgb)
        : Widget(parent), m_drag_region(None)
    {
        setColor(rgb);
    }

    Vector2i ColorWheel::preferredSize(SDL_Renderer*) const
    {
        return { 100, 100 };
    }

    void ColorWheel::draw(SDL_Renderer* renderer)
    {
            Widget::draw(renderer);

            if (!mVisible)
                return;

    	drawBody(renderer);
    }

    void ColorWheel::drawBody(SDL_Renderer* renderer)
    {
        int id = 0;

        auto atx = std::find_if(_txs.begin(), _txs.end(), [id](AsyncTexturePtr p) { return p->id == id; });

        if (atx != _txs.end())
        {
            Vector2i ap = absolutePosition();
            (*atx)->perform(renderer);
            if ((*atx)->tex.tex)
                SDL_RenderCopy(renderer, (*atx)->tex, ap);
            else
                drawBodyTemp(renderer);
        }
        else
        {
            AsyncTexturePtr newtx = std::make_shared<AsyncTexture>(id);
            newtx->load(this);
            _txs.push_back(newtx);

            drawBodyTemp(renderer);
        }
    }

        void ColorWheel::drawBodyTemp(SDL_Renderer* renderer)
    {
        Vector2i ap = absolutePosition();
        SDL_Color bodyclr = mTheme->mTextColorShadow.toSdlColor();

        SDL_Rect bodyRect{ ap.x + 1, ap.y + 1, width() - 2, height() - 2 };
        SDL_SetRenderDrawColor(renderer, bodyclr.r, bodyclr.g, bodyclr.b, bodyclr.a);
        SDL_RenderFillRect(renderer, &bodyRect);

        SDL_Rect btnRect{ ap.x - 1, ap.y - 1, width() + 2, height() + 1 };
        SDL_Color bl = (mTheme->mBorderLight).toSdlColor();
        SDL_SetRenderDrawColor(renderer, bl.r, bl.g, bl.b, bl.a);
        SDL_Rect blr{ ap.x, ap.y + 2, width() - 1, height() - 2 };
        SDL_RenderDrawLine(renderer, blr.x, blr.y, blr.x + blr.w, blr.y);
        SDL_RenderDrawLine(renderer, blr.x, blr.y, blr.x, blr.y + blr.h - 1);

        SDL_Color bd = (mTheme->mBorderDark).toSdlColor();
        SDL_SetRenderDrawColor(renderer, bd.r, bd.g, bd.b, bd.a);
        SDL_Rect bdr{ ap.x, ap.y + 1, width() - 1, height() - 2 };
        SDL_RenderDrawLine(renderer, bdr.x, bdr.y + bdr.h, bdr.x + bdr.w, bdr.y + bdr.h);
        SDL_RenderDrawLine(renderer, bdr.x + bdr.w, bdr.y, bdr.x + bdr.w, bdr.y + bdr.h);

        bd = mTheme->mBorderDark.toSdlColor();
        SDL_SetRenderDrawColor(renderer, bd.r, bd.g, bd.b, bd.a);
        SDL_RenderDrawRect(renderer, &btnRect);
    }

	
	void ColorWheel::renderBodyTexture(NVGcontext*& ctx, int& realw, int&realh)
    {
        int ww = width();
        int hh = height();
        ctx = nvgCreateRT(NVG_DEBUG, ww + 2, hh + 2, 0);

        float pxRatio = 1.0f;
        realw = ww + 2;
        realh = hh + 2;
        nvgBeginFrame(ctx, realw, realh, pxRatio);
    	
        float x = _pos.x,
              y = _pos.y,
              w = mSize.x,
              h = mSize.y;

    	x = 0;
    	y = 0;
        int i;
        float r0, r1, ax,ay, bx,by, cx,cy, aeps, r;
        float hue = m_hue;
        NVGpaint paint;

        nvgSave(ctx);

        cx = x + w*0.5f;
        cy = y + h*0.5f;
        r1 = (w < h ? w : h) * 0.5f - 5.0f;
        r0 = r1 * .75f;

        aeps = 0.5f / r1;   // half a pixel arc length in radians (2pi cancels out).

        for (i = 0; i < 6; i++) 
        {
            auto a0 = static_cast<float>(i) / 6.0f * M_PI * 2.0f - aeps;
            auto a1 = static_cast<float>(i + 1.0f) / 6.0f * M_PI * 2.0f + aeps;
            nvgBeginPath(ctx);
            nvgArc(ctx, cx,cy, r0, a0, a1, NVG_CW);
            nvgArc(ctx, cx,cy, r1, a1, a0, NVG_CCW);
            nvgClosePath(ctx);
            ax = cx + cosf(a0) * (r0+r1)*0.5f;
            ay = cy + sinf(a0) * (r0+r1)*0.5f;
            bx = cx + cosf(a1) * (r0+r1)*0.5f;
            by = cy + sinf(a1) * (r0+r1)*0.5f;
            paint = nvgLinearGradient(ctx, ax, ay, bx, by,
                                      nvgHSLA(a0 / (M_PI * 2), 1.0f, 0.55f, 255),
                                      nvgHSLA(a1 / (M_PI * 2), 1.0f, 0.55f, 255));
            nvgFillPaint(ctx, paint);
            nvgFill(ctx);
        }

        nvgBeginPath(ctx);
        nvgCircle(ctx, cx,cy, r0-0.5f);
        nvgCircle(ctx, cx,cy, r1+0.5f);
        nvgStrokeColor(ctx, nvgRGBA(0,0,0,64));
        nvgStrokeWidth(ctx, 1.0f);
        nvgStroke(ctx);

        // Selector
        nvgSave(ctx);
        nvgTranslate(ctx, cx,cy);
        nvgRotate(ctx, hue*NVG_PI*2);

        // Marker on
        float u = std::max(r1/50, 1.5f);
              u = std::min(u, 4.f);
        nvgStrokeWidth(ctx, u);
        nvgBeginPath(ctx);
        nvgRect(ctx, r0-1,-2*u,r1-r0+2,4*u);
        nvgStrokeColor(ctx, nvgRGBA(255,255,255,192));
        nvgStroke(ctx);

        paint = nvgBoxGradient(ctx, r0-3,-5,r1-r0+6,10, 2,4, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
        nvgBeginPath(ctx);
        nvgRect(ctx, r0-2-10,-4-10,r1-r0+4+20,8+20);
        nvgRect(ctx, r0-2,-4,r1-r0+4,8);
        nvgPathWinding(ctx, NVG_HOLE);
        nvgFillPaint(ctx, paint);
        nvgFill(ctx);

        // Center triangle
        r = r0 - 6;
        ax = cosf(120.0f/180.0f*NVG_PI) * r;
        ay = sinf(120.0f/180.0f*NVG_PI) * r;
        bx = cosf(-120.0f/180.0f*NVG_PI) * r;
        by = sinf(-120.0f/180.0f*NVG_PI) * r;
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, r,0);
        nvgLineTo(ctx, ax, ay);
        nvgLineTo(ctx, bx, by);
        nvgClosePath(ctx);
        paint = nvgLinearGradient(ctx, r, 0, ax, ay, nvgHSLA(hue, 1.0f, 0.5f, 255),
                                  nvgRGBA(255, 255, 255, 255));
        nvgFillPaint(ctx, paint);
        nvgFill(ctx);
        paint = nvgLinearGradient(ctx, (r + ax) * 0.5f, (0 + ay) * 0.5f, bx, by,
                                  nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 0, 0, 255));
        nvgFillPaint(ctx, paint);
        nvgFill(ctx);
        nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 64));
        nvgStroke(ctx);

        // Select circle on triangle
        auto sx = r*(1 - m_white - m_black) + ax * m_white + bx * m_black;
        auto sy =                             ay * m_white + by * m_black;

        nvgStrokeWidth(ctx, u);
        nvgBeginPath(ctx);
        nvgCircle(ctx, sx,sy,2*u);
        nvgStrokeColor(ctx, nvgRGBA(255,255,255,192));
        nvgStroke(ctx);

        nvgRestore(ctx);

        nvgRestore(ctx);

    	nvgEndFrame(ctx);
    }

	
    bool ColorWheel::mouseButtonEvent(const Vector2i& p, int button, bool down, int modifiers)
    {
        Widget::mouseButtonEvent(p, button, down, modifiers);
        if (!mEnabled || button != SDL_BUTTON_LEFT)
            return false;

        if (down) {
            m_drag_region = adjustPosition(p);
            return m_drag_region != None;
        }
        else {
            m_drag_region = None;
            return true;
        }
    }

    bool ColorWheel::mouseDragEvent(const Vector2i& p, const Vector2i&,
        int, int)
    {
        return adjustPosition(p, m_drag_region) != None;
    }

    ColorWheel::Region ColorWheel::adjustPosition(const Vector2i& p, Region consideredRegions)
    {
        /*   float x = p.x - _pos.x,
                 y = p.y - _pos.y,
                 w = mSize.x,
                 h = mSize.y;

           float cx = w*0.5f;
           float cy = h*0.5f;
           float r1 = (w < h ? w : h) * 0.5f - 5.0f;
           float r0 = r1 * .75f;

           x -= cx;
           y -= cy;

           float mr = std::sqrt(x*x + y*y);

           if ((consideredRegions & OuterCircle) &&
               ((mr >= r0 && mr <= r1) || (consideredRegions == OuterCircle))) {
               if (!(consideredRegions & OuterCircle))
                   return None;
               mHue = std::atan(y / x);
               if (x < 0)
                   mHue += NVG_PI;
               mHue /= 2*NVG_PI;

               if (mCallback)
                   mCallback(color());

               return OuterCircle;
           }

           float r = r0 - 6;

           float ax = std::cos( 120.0f/180.0f*NVG_PI) * r;
           float ay = std::sin( 120.0f/180.0f*NVG_PI) * r;
           float bx = std::cos(-120.0f/180.0f*NVG_PI) * r;
           float by = std::sin(-120.0f/180.0f*NVG_PI) * r;

           typedef Eigen::Matrix<float,2,2>        Matrix2f;

           Eigen::Matrix<float, 2, 3> triangle;
           triangle << ax,bx,r,
                       ay,by,0;
           triangle = Eigen::Rotation2D<float>(mHue * 2 * NVG_PI).matrix() * triangle;

           Matrix2f T;
           T << triangle(0,0) - triangle(0,2), triangle(0,1) - triangle(0,2),
                triangle(1,0) - triangle(1,2), triangle(1,1) - triangle(1,2);
           Vector2f pos { x - triangle(0,2), y - triangle(1,2) };

           Vector2f bary = T.colPivHouseholderQr().solve(pos);
           float l0 = bary[0], l1 = bary[1], l2 = 1 - l0 - l1;
           bool triangleTest = l0 >= 0 && l0 <= 1.f && l1 >= 0.f && l1 <= 1.f &&
                               l2 >= 0.f && l2 <= 1.f;

           if ((consideredRegions & InnerTriangle) &&
               (triangleTest || consideredRegions == InnerTriangle)) {
               if (!(consideredRegions & InnerTriangle))
                   return None;
               l0 = std::min(std::max(0.f, l0), 1.f);
               l1 = std::min(std::max(0.f, l1), 1.f);
               l2 = std::min(std::max(0.f, l2), 1.f);
               float sum = l0 + l1 + l2;
               l0 /= sum;
               l1 /= sum;
               mWhite = l0;
               mBlack = l1;
               if (mCallback)
                   mCallback(color());
               return InnerTriangle;
           }
           */
        return None;

    }

    Color ColorWheel::hue2rgb(float h) const
    {
        float s = 1., v = 1.;

        if (h < 0) h += 1;

        int i = int(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        float r = 0, g = 0, b = 0;
        switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
        }

        return { r, g, b, 1.f };
    }

    Color ColorWheel::color() const
    {
        Color rgb = hue2rgb(m_hue);
        Color black{ 0.f, 0.f, 0.f, 1.f };
        Color white{ 1.f, 1.f, 1.f, 1.f };
        return rgb * (1 - m_white - m_black) + black * m_black + white * m_white;
    }

    void ColorWheel::setColor(const Color& rgb)
    {
        /*    float r = rgb[0], g = rgb[1], b = rgb[2];

            float max = std::max({ r, g, b });
            float min = std::min({ r, g, b });
            float l = (max + min) / 2;

            if (max == min) {
                mHue = 0.;
                mBlack = 1. - l;
                mWhite = l;
            } else {
                float d = max - min, h;
                if (max == r)
                    h = (g - b) / d + (g < b ? 6 : 0);
                else if (max == g)
                    h = (b - r) / d + 2;
                else
                    h = (r - g) / d + 4;
                h /= 6;

                mHue = h;

                Eigen::Matrix<float, 4, 3> M;
                M.topLeftCorner<3, 1>() = hue2rgb(h).head<3>();
                M(3, 0) = 1.;
                M.col(1) = Vector4f{ 0., 0., 0., 1. };
                M.col(2) = Vector4f{ 1., 1., 1., 1. };

                Vector4f rgb4{ rgb[0], rgb[1], rgb[2], 1. };
                Vector3f bary = M.colPivHouseholderQr().solve(rgb4);

                mBlack = bary[1];
                mWhite = bary[2];
            }
            */
    }

}

