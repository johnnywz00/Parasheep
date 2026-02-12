
#include "sfmlTguiApp.hpp"


FullscreenOnlyApp::FullscreenOnlyApp ()
{
	VideoMode mode = VideoMode::getDesktopMode();
	auto style = Style::Default;
	auto fsmodes = VideoMode::getFullscreenModes();
	if (fsmodes.size()) {
		mode = fsmodes[0];
		style = Style::Fullscreen;
	}
	window.create(mode, "", style);
	
	window.setFramerateLimit(60);
	
	if (!icon.loadFromFile((Resources::executingDir() / "resources" / "images" / "icon.png").string()))
		cerr << "Couldn't load icon. " << endl;
	else
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	
    srand(unsigned(time(nullptr)));
		
    state.renWin = &window;
	state.app = this;
	state.timedMgr = &timedMgr;
	state.gui = &gui;

	gui.setTarget(window);
	tgui::Theme::setDefault((Resources::executingDir() / "resources" / "Black.txt").string());

    state.onCreate();
	
    clock.restart();
}

void FullscreenOnlyApp::run ()
{
	while (!isDone) {
		update();
		
		window.clear(redrawColor);
		state.draw();
		gui.draw();
		window.display();
		
		elapsed += clock.restart();
	}
}

void FullscreenOnlyApp::update ()
{
	state.oldMouse = state.mouseVec;
    Event event;
    while (window.pollEvent(event)) {
		vecF adj = window.mapPixelToCoords(vecI(event.mouseButton.x,
													   event.mouseButton.y));
        switch(event.type) {
				
            case Event::KeyPressed:
                switch(event.key.code) {
                    default:
                        state.onKeyPress(event.key.code);
						break;
                }
                break;
				
			case Event::KeyReleased:
				state.onKeyRelease(event.key.code);
                break;
				
			case Event::MouseMoved:
				state.mouseVec = toVecI(adj);
				break;
				
            case Event::MouseButtonPressed:
                state.onMouseDown(int(adj.x), int(adj.y));
				break;
				
            case Event::MouseButtonReleased:
				state.onMouseUp(int(adj.x), int(adj.y));
				break;
				
            case Event::Closed:
				close();
				break;
				
            default:
                break;
        }
		if (gui.handleEvent(event))
			/* Pass */
			;
    }
    state.update(elapsed);
}


int main (int argc, char* argv[])
{
/* XCode folly: two instances of the program are launched if
 * we customize the working directory. We can cause the extraneous
 * instance to silently quit immediately by giving it a relative
 * path that it can't find.
 */
#ifdef DEBUG
    Image img;
	if (!img.loadFromFile("resources/images/icon.png")) {
		cerr << "DEBUG preprocessor symbol is defined.\n";
		return EXIT_FAILURE;
	}
#endif
	
	cout << "Initializing Resources\n";
	Resources::initialize(argc, argv);

	FullscreenOnlyApp app;
	cout << "Preparing to launch game loop for Parasheep\n";
	app.run();
}
