/*
 TO DO:
 
 sun goes down moon/stars come out
 cactus: sheep hang
 horizontal moving sheep with jetpack
 tiny ephemeral target which if hit gives you a bomb to wipe all current sheep
 "   "  gives you rapid fire bullets; target shows countdown till disappearing
 death animation
 add score if ≈10 sheep shot consec without miss
 pts based on distance sheep is from gun
 
 */

#ifndef PARASHEEP_H
#define PARASHEEP_H

#include "objects.hpp"

// (Hopefully) temporary disabling of TGUI/high scores on Windows
#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS
#endif

class FullscreenOnlyApp;


enum GameMode { menu, playing };


class State
{
public:
    void onCreate ();
    
	void onMouseDown (int x, int y);

	void onMouseUp (int x, int y) { }
	
	void onKeyPress (Keyboard::Key);
	
	void onKeyRelease (Keyboard::Key);

	void update (const Time& time);
	
	void draw ();

	FullscreenOnlyApp*	app;
	RenderWindow*       renWin;
	tgui::Gui*          gui;
	TimedEventManager*  timedMgr;

	vecI				oldMouse
						, mouseVec
	;

private:
	const Color         skyTop = Color(255, 165, 54)
						, skyTopNight = Color(4, 4, 50)
						, skyBottom = Color(227, 229, 164)
						, skyBottomNight = Color(3, 50, 150)
	;
	const float     	G = .25	// Gravity
						, sheepG = G
						, angRate = 1
						, initAngle = -45
						, powerRate = .8
						, powerScale = 360
						, maxPower = 29
						, projOffset = 35
						, guideLength = 250
						, groundLevel = scrh - 48
						, levelInc = .2
						, floatDev = 20
						, floatSkew = 5
						, floatDegInc = 5
						, runInc = -.3
						, fallRate = 5
						, timeBetweenFires = .4
	;
 
	void menuCreate ();
	
	void menuClick (int x, int y);
	
	void menuUpdate ();
	
	void menuDraw ();
	
	
	void setUpHighScores ();

	void resetGame ();
    
    void startGame ();
    
	void newLevel ();
	
	void setLoadedProjPos ();
	
	void newEnemy (const Time& time);
	
    void updateGuide ();
    
	void gunLeft ();
	
	void gunRight ();
    
	float angle () { return (angle_ = launcher.gRot()); }

	void launch ();
    
	void decrementLevelTime () { timeBetweenEnemies *= .8; }
	
	void endGame ();
    
    void showHighScores (int score, int level);

    void enterNewScore (tgui::String str);
    
    string highScoreToText (HighScore &hs);
    
    void topTenToFile ();
    
	
	Sprite              ground
						, launcher
						, deh
						, cactus
	;
	Sprite              clouds[10];
	vector<Projectile>  projs;
	vector<Enemy>       enemies;
	RectangleShape      bkgd[20];
	RectangleShape      powerBar[2]
						, platform
	;
	VertexArray         guideline { Lines };
    Text                levelLabel
                        , scoreTxt
	;
    vector<HighScore>   topTen;
    vector<tgui::EditBox::Ptr>
                        eboxes;
	
	Projectile*         curProj = nullptr;
                        
	GameMode            mode = menu;
  
    Time                lastEnemyTime;
    Time                lastFireTime;
    vecF                velocity{0, 0};
    vecF                vp{0, 0};
    float               cloudVels[10];
    float               numShot = 0;
    float               totalShots = 0;
    float               power;
    float               angle_ = 0;
    float               shootingPct = 0.0f;
    float               timeBetweenEnemies;
	float				runRate;
   
    int                 numOnGround;
    int                 score;
    int                 gunMode;
    int                 level;
    
    bool                running = false;
    bool                pullingBack = false;
	bool				shotFiredThisFrame = false;
	bool				canShoot = true;
	bool				needLaunch = false;
    bool                gameOver = false;

 
////////////  MENU SCREEN /////////////////////

	static constexpr int numButtons = 2;
	const vecF         	buttonSize = vecF(500, 55);
	const string		instrucsStr =
		"Use the Left/Right keys to aim the projectile launcher.\n"
		"Hold Space proportionate to the amount of power for the shot.\n"
		"Shoot the 'chuting sheep.\n"
		"(From high scores, Y will restart and Esc will return to the menu.)\n"
		" \n \nJohn Ziegler, 2020-2024   johnnywz00@yahoo.com"
	;
	
	RenderTexture		bkgdRt;
	Sprite				menuBkgd;
    RectangleShape      rects[numButtons]
	;
    Text                gameTitle
						, instrucsTxt
                        , labels[numButtons]
	;
};  //end class State

#endif



