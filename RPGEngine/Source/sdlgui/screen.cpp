/*
	screen.cpp -- Top-level widget and interface between sdlgui and SDL

	A significant redesign of this code was contributed by Christian Schueller.

	Based on NanoGUI by Wenzel Jakob <wenzel@inf.ethz.ch>.
	Adaptation for SDL by Dalerank <dalerankn8@gmail.com>

	All rights reserved. Use of this source code is governed by a
	BSD-style license that can be found in the LICENSE.txt file.
*/

#include "screen.h"
#include "theme.h"
#include "window.h"
#include "popup.h"
#include <iostream>
#include <map>

#include <SDL.h>

namespace GUI
{

	std::map<SDL_Window*, Screen*> __sdlgui_screens;

	Screen::Screen(SDL_Window* window, const Vector2i& size, const std::string& caption,
		bool resizable, bool fullscreen)
		: Widget(nullptr), _window(nullptr), m_sdl_renderer(nullptr), m_caption(caption)
	{
		SDL_SetWindowTitle(window, caption.c_str());
		initialize(window);
	}

	bool Screen::onEvent(const SDL_Event& event)
	{
		auto it = __sdlgui_screens.find(_window);
		if (it == __sdlgui_screens.end())
			return false;

		switch (event.type)
		{
		case SDL_MOUSEWHEEL:
		{
			if (!m_process_events)
				return false;
			return scrollCallbackEvent(event.wheel.x, event.wheel.y);
		}
		break;

		case SDL_MOUSEMOTION:
		{
			if (!m_process_events)
				return false;
			return cursorPosCallbackEvent(event.motion.x, event.motion.y);
		}
		break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			if (!m_process_events)
				return false;

			SDL_Keymod mods = SDL_GetModState();
			return mouseButtonCallbackEvent(event.button.button, event.button.type, mods);
		}
		break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			if (!m_process_events)
				return false;

			const auto mods = SDL_GetModState();
			return keyCallbackEvent(event.key.keysym.sym, event.key.keysym.scancode, event.key.state, mods);
		}
		break;

		case SDL_TEXTINPUT:
		{
			if (!m_process_events)
				return false;
			return charCallbackEvent(event.text.text[0]);
		}
		break;
		}
		return false;
	}

	void Screen::initialize(SDL_Window* window)
	{
		_window = window;
		SDL_GetWindowSize(window, &mSize.x, &mSize.y);
		SDL_GetWindowSize(window, &m_fb_size.x, &m_fb_size.y);
		m_sdl_renderer = SDL_GetRenderer(window);

		if (m_sdl_renderer == nullptr)
			throw std::runtime_error("Could not initialize NanoVG!");

		mVisible = true;
		mTheme = new Theme();
		m_mouse_pos = { 0, 0 };
		m_mouse_state = m_modifiers = 0;
		m_drag_active = false;
		m_last_interaction = SDL_GetTicks();
		m_process_events = true;
		m_background = Color(0.3f, 0.3f, 0.32f, 1.0f);
		__sdlgui_screens[_window] = this;
	}

	Screen::~Screen()
	{
		__sdlgui_screens.erase(_window);
	}

	void Screen::setVisible(bool visible)
	{
		if (mVisible != visible)
		{
			mVisible = visible;

			if (visible)
				SDL_ShowWindow(_window);
			else
				SDL_HideWindow(_window);
		}
	}

	void Screen::setCaption(const std::string& caption)
	{
		if (caption != m_caption)
		{
			SDL_SetWindowTitle(_window, caption.c_str());
			m_caption = caption;
		}
	}

	void Screen::setSize(const Vector2i& size)
	{
		Widget::setSize(size);
		SDL_SetWindowSize(_window, size.x, size.y);
	}

	void Screen::drawAll()
	{
		drawContents();
		drawWidgets();
	}

	void Screen::drawWidgets()
	{
		if (!mVisible)
			return;

		/* Calculate pixel ratio for hi-dpi devices. */
		m_pixel_ratio = static_cast<float>(m_fb_size.x) / static_cast<float>(mSize.x);

		const auto renderer = SDL_GetRenderer(_window);
		draw(renderer);

		const auto elapsed = SDL_GetTicks() - m_last_interaction;
		if (elapsed > 0.5f)
		{
			/* Draw tooltips */
			const Widget* widget = findWidget(m_mouse_pos);
			if (widget && !widget->tooltip().empty())
			{
				int tooltipWidth = 150;

				if (_lastTooltip != widget->tooltip())
				{
					_lastTooltip = widget->tooltip();
					mTheme->getTexAndRectUtf8(renderer, _tooltipTex, 0, 0, _lastTooltip.c_str(), "sans", 15, Color(1.f, 1.f));
				}

				if (_tooltipTex.tex)
				{
					Vector2i pos = widget->absolutePosition() + Vector2i(widget->width() / 2, widget->height() + 10);

					const float alpha = (std::min(1.0, 2 * (elapsed - 0.5f)) * 0.8) * 255;
					SDL_SetTextureAlphaMod(_tooltipTex.tex, alpha);

					SDL_Rect bgrect{ pos.x - 2, pos.y - 2 - _tooltipTex.h(), _tooltipTex.w() + 4, _tooltipTex.h() + 4 };

					SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
					SDL_RenderFillRect(renderer, &bgrect);
					SDL_RenderCopy(renderer, _tooltipTex, Vector2i(pos.x, pos.y - _tooltipTex.h()));
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
					SDL_RenderDrawLine(renderer, bgrect.x, bgrect.y, bgrect.x + bgrect.w, bgrect.y);
					SDL_RenderDrawLine(renderer, bgrect.x + bgrect.w, bgrect.y, bgrect.x + bgrect.w, bgrect.y + bgrect.h);
					SDL_RenderDrawLine(renderer, bgrect.x, bgrect.y + bgrect.h, bgrect.x + bgrect.w, bgrect.y + bgrect.h);
					SDL_RenderDrawLine(renderer, bgrect.x, bgrect.y, bgrect.x, bgrect.y + bgrect.h);
				}
			}
		}
	}

	bool Screen::keyboardEvent(int key, int scancode, int action, int modifiers)
	{
		if (!m_focus_path.empty())
		{
			for (auto it = m_focus_path.rbegin() + 1; it != m_focus_path.rend(); ++it)
				if ((*it)->focused() && (*it)->keyboardEvent(key, scancode, action, modifiers))
					return true;
		}

		return false;
	}

	bool Screen::keyboardCharacterEvent(unsigned int codepoint)
	{
		if (!m_focus_path.empty())
		{
			for (auto it = m_focus_path.rbegin() + 1; it != m_focus_path.rend(); ++it)
				if ((*it)->focused() && (*it)->keyboardCharacterEvent(codepoint))
					return true;
		}
		return false;
	}

	bool Screen::cursorPosCallbackEvent(double x, double y)
	{
		Vector2i p((int)x, (int)y);
		bool ret = false;
		m_last_interaction = SDL_GetTicks();
		try
		{
			p -= Vector2i(1, 2);

			if (!m_drag_active)
			{
				Widget* widget = findWidget(p);
				/*if (widget != nullptr && widget->cursor() != mCursor) {
					mCursor = widget->cursor();
					glfwSetCursor(mGLFWWindow, mCursors[(int) mCursor]);
				}*/
			}
			else
			{
				ret = m_drag_widget->mouseDragEvent(
					p - m_drag_widget->parent()->absolutePosition(), p - m_mouse_pos,
					m_mouse_state, m_modifiers);
			}

			if (!ret)
				ret = mouseMotionEvent(p, p - m_mouse_pos, m_mouse_state, m_modifiers);

			m_mouse_pos = p;

			return ret;
		}
		catch (const std::exception& e) {
			std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
			abort();
		}

		return false;
	}

	bool Screen::mouseButtonCallbackEvent(int button, int action, int modifiers) {
		m_modifiers = modifiers;
		m_last_interaction = SDL_GetTicks();
		try {
			if (m_focus_path.size() > 1)
			{
				const Window* window =
					dynamic_cast<Window*>(m_focus_path[m_focus_path.size() - 2]);
				if (window && window->modal())
				{
					if (!window->contains(m_mouse_pos))
						return false;
				}
			}

			if (action == SDL_MOUSEBUTTONDOWN)
				m_mouse_state |= 1 << button;
			else
				m_mouse_state &= ~(1 << button);

			auto dropWidget = findWidget(m_mouse_pos);
			if (m_drag_active && action == SDL_MOUSEBUTTONUP &&
				dropWidget != m_drag_widget)
				m_drag_widget->mouseButtonEvent(
					m_mouse_pos - m_drag_widget->parent()->absolutePosition(), button,
					false, m_modifiers);

			/*if (dropWidget != nullptr && dropWidget->cursor() != mCursor) {
				mCursor = dropWidget->cursor();
				glfwSetCursor(mGLFWWindow, mCursors[(int) mCursor]);
			}*/

			if (action == SDL_MOUSEBUTTONDOWN && button == SDL_BUTTON_LEFT) 
			{
				m_drag_widget = findWidget(m_mouse_pos);
				if (m_drag_widget == this)
					m_drag_widget = nullptr;
				m_drag_active = m_drag_widget != nullptr;
				if (!m_drag_active)
					updateFocus(nullptr);
			}
			else 
			{
				m_drag_active = false;
				m_drag_widget = nullptr;
			}

			return mouseButtonEvent(m_mouse_pos, button, action == SDL_MOUSEBUTTONDOWN,
				m_modifiers);
		}
		catch (const std::exception& e) 
		{
			std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
			abort();
		}

		return false;
	}

	bool Screen::keyCallbackEvent(const int key, const int scancode, const int action, const int mods)
	{
		m_last_interaction = SDL_GetTicks();
		try 
		{
			return keyboardEvent(key, scancode, action, mods);
		}
		catch (const std::exception& e) 
		{
			std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
			abort();
		}
	}

	bool Screen::charCallbackEvent(unsigned int codepoint)
	{
		m_last_interaction = SDL_GetTicks();
		try 
		{
			return keyboardCharacterEvent(codepoint);
		}
		catch (const std::exception& e) 
		{
			std::cerr << "Caught exception in event handler: " << e.what()
				<< std::endl;
			abort();
		}
	}

	bool Screen::dropCallbackEvent(const int count, const char** filenames)
	{
		std::vector<std::string> arg(count);
		for (int i = 0; i < count; ++i)
			arg[i] = filenames[i];
		return dropEvent(arg);
	}

	bool Screen::scrollCallbackEvent(double x, double y)
	{
		m_last_interaction = SDL_GetTicks();
		try {
			if (m_focus_path.size() > 1) 
			{
				const Window* window = dynamic_cast<Window*>(m_focus_path[m_focus_path.size() - 2]);
				if (window && window->modal()) 
				{
					if (!window->contains(m_mouse_pos))
						return false;
				}
			}
			return scrollEvent(m_mouse_pos, Vector2f(x, y));
		}
		catch (const std::exception& e) {
			std::cerr << "Caught exception in event handler: " << e.what()
				<< std::endl;
			abort();
		}

		return false;
	}

	bool Screen::resizeCallbackEvent(int, int)
	{
		Vector2i fbSize, size;
		//glfwGetFramebufferSize(mGLFWWindow, &fbSize[0], &fbSize[1]);
		SDL_GetWindowSize(_window, &size.x, &size.y);

		if (m_fb_size == Vector2i(0, 0) || size == Vector2i(0, 0))
			return false;

		m_fb_size = fbSize;
		mSize = size;
		m_last_interaction = SDL_GetTicks();

		try
		{
			return resizeEvent(mSize);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Caught exception in event handler: " << e.what()
				<< std::endl;
			abort();
		}
	}

	void Screen::updateFocus(Widget* widget)
	{
		for (auto* w : m_focus_path) 
		{
			if (!w->focused())
				continue;
			w->focusEvent(false);
		}
		m_focus_path.clear();
		Widget* window = nullptr;
		while (widget) 
		{
			m_focus_path.push_back(widget);
			if (dynamic_cast<Window*>(widget))
				window = widget;
			widget = widget->parent();
		}

		for (auto it = m_focus_path.rbegin(); it != m_focus_path.rend(); ++it)
			(*it)->focusEvent(true);

		if (window)
			moveWindowToFront(dynamic_cast<Window*>(window));
	}

	void Screen::disposeWindow(Window* window)
	{
		if (std::find(m_focus_path.begin(), m_focus_path.end(), window) != m_focus_path.end())
			m_focus_path.clear();
		if (m_drag_widget == window)
			m_drag_widget = nullptr;
		removeChild(window);
	}

	void Screen::centerWindow(Window* window) const
	{
		if (window->size() == Vector2i{ 0, 0 })
		{
			window->setSize(window->preferredSize(m_sdl_renderer));
			window->performLayout(m_sdl_renderer);
		}
		window->setPosition((mSize - window->size()) / 2);
	}

	void Screen::moveWindowToFront(Window* window)
	{
		mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), window), mChildren.end());
		mChildren.push_back(window);
		/* Brute force topological sort (no problem for a few windows..) */
		bool changed = false;
		do {
			size_t baseIndex = 0;
			for (size_t index = 0; index < mChildren.size(); ++index)
				if (mChildren[index] == window)
					baseIndex = index;
			changed = false;
			for (size_t index = 0; index < mChildren.size(); ++index) 
			{
				Popup* pw = dynamic_cast<Popup*>(mChildren[index]);
				if (pw && pw->parentWindow() == window && index < baseIndex) 
				{
					moveWindowToFront(pw);
					changed = true;
					break;
				}
			}
		} while (changed);
	}

	void Screen::performLayout(SDL_Renderer* renderer)
	{
		Widget::performLayout(renderer);
	}

	void Screen::performLayout()
	{
		Widget::performLayout(m_sdl_renderer);
	}

}
