
#ifndef sfmlTguiApp_hpp
#define sfmlTguiApp_hpp


#include "parasheep.hpp"


class FullscreenOnlyApp
{
public:
    FullscreenOnlyApp ();
       
	void run ();
	
	void update ();
	
	void close ()
	{
		isDone = true;
		window.close();
	}
	
	void setRedrawColor (const Color& c) { redrawColor = c; }

	RenderWindow            window;
	tgui::Gui        		gui;
	TimedEventManager		timedMgr;
	State            		state;
	Clock            		clock;
	Time             		elapsed;
	Image                   icon;
	Color                   redrawColor { Color::White };
	bool					isDone = false;
};

#endif /* sfmlApp_hpp */
