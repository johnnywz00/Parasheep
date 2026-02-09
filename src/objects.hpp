//
//  objects.hpp
//  Launcher
//
//  Created by John Ziegler on 11/9/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#ifndef objects_hpp
#define objects_hpp

#include "jwzsfml.hpp"
#include "timedeventmanager.hpp"
#include "highscore.hpp"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "vsprite.hpp"

//#define _EMBEDDING
#include "embedded.hpp"


inline float floatRate = 1;


struct Enemy {
    
    Enemy () {
        
        float i = randRange(-350, -200);
        jumpVel = i / 100;
        floatRate = ::floatRate * (float(randRange(90, 110)) / 100);
    }
    
    FloatRect hitBox () {
        
        int ofs = 4;
        FloatRect fr = s.gGB();
        return FloatRect(fr.left + ofs, fr.top + ofs,
                         fr.width - 2 * ofs, fr.height - 2 * ofs);
    }
    
    Sprite          s;
    vecF            velocity{ 0, 0 };
    float           xLine = ScrW - 15;
    float           yVel = 0;
    float           jumpVel = -2;
    float           floatDeg = 90;
    float           floatRate;
    int             isBonus = 0;
    bool            onGround = false;
    bool            hit = false;
    bool            dead = false;
    
};


struct Projectile: public VSprite {
    
    Projectile (const Texture& tx) {
        
        s.setTexture(tx);
        ::centerOrigin(s);
    }

    float mass () {
        
        float r = gGB().width / 2;
        return (4/3) * pi * r * r * r;
    }
    
    bool readyToErase = false;
	bool isLoaded = true;
};
#endif /* objects_hpp */
