#ifndef PARASHEEP_H
#define PARASHEEP_H

#include "jwzsfml.hpp"
#include "objects.hpp"

//sky turns to night, sun goes down moon/stars come out
//cactus: sheep hang
//horizontal moving sheep with jetpack
//tiny ephemeral target which if hit gives you a bomb to wipe all current sheep
// "   "  gives you rapid fire bullets; target shows countdown till disappearing
//death animation
//add score if ≈10 sheep shot consec without miss
//pts based on distance sheep is from gun



// CACTUS
// CHECK HIGH SCORE LOAD


class SFGameWindow;

enum GameMode { menu, playing };


class State {

public:
    
    State () { }
    
    ~State () { }
    
    void loadTextures ();
    
    void loadSounds ();
    
    void loadFonts ();

    void onCreate ();
    
    void resetGame ();
    
    void startGame ();
    
    void endGame ();
    
    void newLevel ();
    
    void newEnemy (const Time& time);
    
    void updateGuide ();
    
    void setLoadedProjPos ();
    
    void onMouseDown (int x, int y);
    
    void menuClick (int x, int y);
    
    void onMouseUp (int x, int y);
    
    void onKeyPress (Keyboard::Key);

	void onKeyRelease (Keyboard::Key);

	void launch ();
    
    void gunLeft ();
    
    void gunRight ();
    
    void update (const Time& time);
    
    void menuUpdate ();
    
    void draw ();
    
    void menuDraw ();
    
    void showHighScores (int score, int level);

    void enterNewScore (tgui::String str);
    
    string highScoreToText (HighScore &hs);
    
    void topTenToFile ();
    
    void setUpHighScores ();

    void decrementLevelTime () { timeBetweenEnemies *= .8; }
    
    float angle () { return (angle_ = launcher.gRot()); }
    
    RenderWindow*       w;
    SFGameWindow*       gw;
    tgui::Gui*          gui;
    TimedEventManager*        timedMgr;

    Font                font,
                        levelFont;
    Text                levelLabel,
                        scoreTxt;
    
    vector<Texture>     texs;
    static const vector<string>
                        texList;
    Texture             txLauncher, txProj, txSheep, txDeh,
                        txSheepPara, txSheepHit, txGround, txCloud;
    VertexArray         guideline { Lines };
    RectangleShape      bkgd[20];
    RectangleShape      powerBar[2],
                        platform;
    Sprite              bkgdFrame,
                        ground,
                        launcher,
                        deh;
    Sprite              clouds[10];
    vector<Projectile>  projs;
    Projectile*         curProj = nullptr;
    vector<Enemy>       enemies;
    vector<HighScore>   topTen;
    vector<tgui::EditBox::Ptr>
                        eboxes;
                        
    const Color         skyTop = Color(255, 165, 54),
                        skyTopNight = Color(4, 4, 50),
                        skyBottom = Color(227, 229, 164),
                        skyBottomNight = Color(3, 50, 150);
    
    vector<SoundBuffer> buffers;
    vector<Sound>       sounds;
    static const vector<string>
                        soundList;
  
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
    float               timeBetweenEnemies = 5;
    float               timeBetweenFires = .4;
    float               G = .25,
                        sheepG = G,
                        angRate = 1,
                        initAngle = -45,
                        powerRate = .8,
                        powerScale = 360,
                        maxPower = 27,
                        projOffset = 35,
                        guideLength = 250,
                        groundLevel = ScrH - 48,
                        levelInc = .2,
                        floatDev = 20,
                        floatSkew = 5,
                        floatDegInc = 5,
                        runRate = -1,
                        runInc = -.3,
                        fallRate = 5;
    
    int                 numOnGround = 0;
    int                 score = 0;
    int                 gunMode = 0;
    int                 level = 1;
    int                 mx = 0,
                        my = 0,
                        mxOld = 0,
                        myOld = 0;
    
    bool                running = false;
    bool                pullingBack = false;
	bool				shotFiredThisFrame = false;
	bool				canShoot = true;
	bool				needLaunch = false;
    bool                gameOver = false;

 
////////////  MENU SCREEN /////////////////////

    GameMode            mode = menu;
   static constexpr int numButtons = 2;
    RectangleShape      menuBkgd,
                        rects[numButtons];
    Text                gameTitle,
                        labels[numButtons];
    Font                titleFont,
                        labelFont;
    vecF                buttonSize = vecF(500, 55);
    uint                buttonPadding = 15;
    vecF                buttonPos = vecF(ScrCX - buttonSize.x / 2 - buttonPadding / 2,
                                         ScrCY - 80);


    
};  //end class State

#endif



