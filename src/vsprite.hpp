//
//  vsprite.hpp
//  Hopscotch
//
//  Created by John Ziegler on 10/29/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#ifndef vsprite_hpp
#define vsprite_hpp

#include "zsprite.hpp"


	/* Sprites that use velocity */

class VSprite : public ZSprite {
public:
	
	void setVelocity (float x, float y) {
		
		velocity = vecF(x, y);
		if (speedClamp && mag() < speedClamp)
			velocity = vecf(0, 0);
	}

	void setVelocity (vecF vec) {
		
		velocity = vec;
		if (speedClamp && mag() < speedClamp)
			velocity = vecf(0, 0);
	}
	
	void setVelocityP (float mag, float direc) {
		
		if (speedClamp && 0 < mag && mag < speedClamp) {
			velocity = vecf(0, 0);
			return;
		}
		if (mag < 0) {
			mag = abs(mag);
			direc = fmod(direc + 180, 360);
		}
		auto vp = vecF(mag, czdg(direc));
		velocity = toRect(vp);
	 }

	void setVelocityP (vecF v) {
		
		if (speedClamp &&
				((0 < v.x && v.x < speedClamp) ||
				 (0 > v.x && abs(v.x) < speedClamp))) {
			velocity = vecf(0, 0);
			return;
		}
		v.y = czdg(v.y);
		velocity = toRect(v);
	}

	void addVelocity (float x, float y) {
		
		velocity.x += x;
		velocity.y += y;
		if (speedClamp && mag() < speedClamp)
			velocity = vecf(0, 0);
	}
	
	void addVelocity (vecf v) {
		
		velocity += v;
		if (speedClamp && mag() < speedClamp)
			velocity = vecf(0, 0);
	 }

	void addVelocityP (float mag, float direc) {

		addVelocity(toRect(abs(mag), czdg(direc + (mag < 0 ? 180 : 0))));
	}
	
	void addVelocityP (vecf v) {
		
		addVelocity(toRect(abs(v.x), czdg(v.y + (v.x < 0 ? 180 : 0))));
	}
	
	vecF pvelocity() {
		return toPolar(velocity);
	}
	
	float mag() {
		return pvelocity().x;
	}
	
	float direc() {
		return pvelocity().y;
	}
	
	float dx() {
		return velocity.x;
	}
	
	float dy() {
		return velocity.y;
	}
	
	void backUp(float units) {
		vecF v = vecF(units, czdg(direc() + 180));
		s.move(toRect(v));
	}
	
	
	vecF    velocity = vecF(0.f, 0.f);
	float   friction = .13;		// val if friction is subtracted per frame
	float   xfriction = .85;	// val if friction is multiplied per frame
	float	speedClamp = 0;
	float	maxSpeed = 999;

};	// end class





#endif /* vsprite_hpp */
