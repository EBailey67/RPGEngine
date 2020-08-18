#pragma once

#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <sstream>


#include "config.hpp"
#include "scene.hpp"

#include "events.hpp"
#include "graphics.hpp"
#include "../Terminal/sdl_context.hpp"

#include "../Utility/FramerateCounter.h"
#include "../Utility/Timer.h"
#include "../Terminal/terminate.hpp"

class Game
{
public:
    FramerateCounter fps_counter;
	static Game* GetInstance()
	{
		static Game game;
		return &game;
	}

private:
	Game() = default;
    ~Game()
    {
        Graphics::DestroyData();

        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    };

    void InputUpdate()
    {
        if (SDL_PollEvent(&Events::m_event))
        {
	        switch(Events::Event().type)
	        {
            case SDL_QUIT:      // Check for terminate
                Quit();
                break;
            default: break;
	        }

            if (Events::Event().window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                Graphics::DestroyLayers();
                Graphics::CreateLayers();
            	m_scene->ResizeEvent();
            }

        	m_scene->InputUpdate();
        }
    }

    void Update(const float dt)
    {
        fps_counter.Update(dt);
        m_scene->Update(dt);
    }

    void FixedUpdate() const
    {
        m_scene->FixedUpdate();
    }

    void Render() const
    {
        Graphics::OnRenderStart();
        m_scene->Render();
    	m_scene->RenderUI();
        Graphics::DrawLayers();
        Graphics::RenderPresent();
    }


public:

	Term::Color RandomColor()
    {
    return Term::Color (
        static_cast<Uint8>(rand()%255),
        static_cast<Uint8>(rand()%255),
        static_cast<Uint8>(rand()%255) );
    }

    void RandomColors( Term::String& str )
    {
    for( Term::Char& ch : str )
        ch.PriColor( Term::Color::Black ).SecColor( RandomColor() );
    }


	void SetupConsole()
    {
	    // Term::SDL::Context term( 48, 15 );
		
		term.Tilemap( "resources/sprites/tileset.png" );
		term.Framebuffer().Clear();
		Term::TTY tty( term.Framebuffer() );

	    Term::String hello = Term::MakeString("Hello, terminal!");
	    Term::String wrapStr = Term::MakeString( "Wraaaaaaaap" );
	    Term::String newLineStr = Term::MakeString( "Eeeey, I'll change line\nNewline!" );
	    RandomColors(hello);
	    RandomColors(wrapStr);
            
	    using Term::TTY;
	    tty.Set( TTY::Wrap ).PriColor( Term::Color(255,200,0) ).SecColor( Term::Color(0,0,0) );
	    tty.Place( 15,7 ).Put( wrapStr );
	    tty.Place( 0, 3 ).Put( "Done!" );
	    tty.Place( 6,10 ).Put( newLineStr );
	    tty.Place( 0,16 ).Put( "Insert between angles ><" );
	    tty.Set( TTY::Insert ).Place( 23, 16 ).Put( "!inserted!" );
	    std::stringstream ss;
	    ss << "Char size: " << sizeof(Term::Char);
	    tty.Place( 0,14 ).Put( ss.str() );
	    tty.Place(0,2);
	    for( int i=0; i < 20; ++i )
	        tty.Put( (char) i+127 );
    }
	
    // Initialization SDL's subsystems. 
    void InitializeSubsystems()
    {
        if (m_hadInitialization)
        {
            return;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING) == -1 || TTF_Init() == -1 || IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0 ||
            Mix_Init(MIX_INIT_MP3) == 0)
        {
            SDL_THROW();
        }

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        m_hadInitialization = true;
    }

    [[nodiscard]] bool HadInitialization() const noexcept
    {
        return m_hadInitialization;
    }
    
    // Assign the window to graphic engine and create render from it.
    // Also create all the render layers
    static void AssignWindow(SDL_Window *window, const Uint32 rendererFlags = 0)
    {
        Graphics::AssignWindow(window, rendererFlags);
    }

    
    void Run()
    {
        m_isRunning = true;
        auto* timer = RPGEngine::Timer::GetInstance();
        const auto frameRate = 60.0f;
    	
        while (m_isRunning)
        {
            timer->Tick();
        	
        	if (timer->DeltaTime() >= 1.0f / frameRate)
            {
                timer->Reset();
                // Variable-timed update:
                Update(timer->DeltaTime());

        		FixedUpdate();
                InputUpdate();
                Render();
            }
        }
    }

    void Quit() noexcept
    {
        m_isRunning = false;
    }

    [[nodiscard]] bool IsRunning() const noexcept
    {
        return m_isRunning;
    }

    [[nodiscard]] auto Scene() const noexcept
    {
        return m_scene.get();
    }

    
    template <typename TScene, typename... Args>
    auto CreateScene(Args... args)
    {
        m_scene = std::make_unique<TScene>(std::forward<Args>(args)...);
    }

	Term::SDL::Context term = Term::SDL::Context( 80, 40 );

private:
    bool m_hadInitialization = false;
    bool m_isRunning = false;
    std::unique_ptr<BasicScene> m_scene = std::make_unique<BasicScene>();
};


