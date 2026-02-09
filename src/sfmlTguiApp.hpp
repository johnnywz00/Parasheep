//
//  sfmlApp.hpp
//  Hopscotch
//
//  Created by John Ziegler on 10/22/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#ifndef sfmlApp_hpp
#define sfmlApp_hpp


#include "parasheep.hpp"


inline const string defaultTitle { "Parasheep" };
inline const string iconPath { "resources/sheeppara.png" };


class SFGameWindow {

	friend class Game;
public:
    
    SFGameWindow ();

    SFGameWindow (const string& title, const vecU& size);
    
	~SFGameWindow () { destroy(); }
    
	void draw (Drawable& d) { window.draw(d); }
    
	void beginDraw () { window.clear(redrawColor); }
    
	void endDraw () { window.display(); }
    
	bool isDone () { return _isDone; }
    
	bool isFullscreen () { return _isFullscreen; }
    
    bool isStretched () { return _isStretched; }
    
	bool isFocused () { return _isFocused; }
    
	vecU getWindowSize () { return windowSize; };
    
	RenderWindow* getRenderWindow () { return &window; };
    
	void close () { _isDone = true; };
    
	void toggleFullscreen ();
	
	void toggleStretchGraphics ();
    
    void setToggledView (bool);
     
    
    vecf screenOffsetFrom1440x900;
    
private:
    
    void destroy () { window.close(); };
    
	void setup (const string& title, const vecU& size);
    
	void create ();
    
    Image                   icon;
    RenderWindow            window;
    Color                   redrawColor { Color::Black };
    vecU                    windowSize;
    static const int        defaultWidth { 1280 };
    static const int        defaultHeight { 720 };
    string                  windowTitle;

    bool                    _isDone;
    bool                    _isFullscreen;
    bool                    _isFocused;
    bool                    _isStretched;
};




class Game {
public:
    
    Game ();
    
    ~Game () { }
       
	void update ();
    
	void render () {
        window.beginDraw();
        state.draw();
        gui.draw();
        window.endDraw();
	}
    
	void lateUpdate () { restartClock(); }
    
	SFGameWindow* getWindow () { return &window; };
    
	Time getElapsed () { return elapsed; };
    
	void restartClock () { elapsed += clock.restart(); };

private:

    SFGameWindow     window;
    tgui::Gui        gui;
    Clock            clock;
    Time             elapsed;
    TimedEventManager      timedMgr;
    State            state;
};




#endif /* sfmlApp_hpp */
