#ifndef ZSPRITE_HPP
#define ZSPRITE_HPP


#include "jwzsfml.hpp"

class ZSprite : public Drawable {
public:
		
    ZSprite () { setup(); }
	
	ZSprite (const Texture &texture) {
		
		s.setTexture(texture);
		setup();
	}

	ZSprite (const Texture &texture, const IntRect &rectangle) {
		
		s.setTexture(texture);
		s.setTextureRect(rectangle);
		setup();
	}

	virtual ~ZSprite () { }
	
	virtual void setup () {
		
		if (!statFont.loadFromFile("resources/Monaco.ttf"))
			cerr << "Couldn't load font Monaco! ";
		auto scrHt = VideoMode::getDesktopMode().height;
		statRect.sP(10, scrHt - 120);
		statRect.setSize(vecF(425, 120));
		statRect.setFillColor(Color(255, 255, 255, 80));
		statText = Text("", statFont, 11);
		statText.setPosition(statRect.gP() + vecF{3, 3});
		statText.setFillColor(Color::Black);
	}

	virtual void update (const Time& t = Time::Zero) {
		
		if (showStats) updateStats();
	}

	virtual void draw (RenderTarget& target, RenderStates states) const {
		
		target.draw(s);
		if (showStats) {
			target.draw(statRect);
			target.draw(statText);
		}
	}

	void updateStats() {
		
		FloatRect lb = gLB();
		FloatRect gb = gGB();
		IntRect tr = gTR();
		vecU ts = getTexture()->getSize();
		vecF pos = gP();
		vecF sc = getScale();
		vecF ogn = getOrigin();
		float rot = gRot();
		statText.setString(
		   "position, origin: " + fS(pos.x, 1) + ", " + fS(pos.y, 1) + "; " + fS(ogn.x, 1) + ", " + fS(ogn.y, 1) + "\n" +
		   "scale, rotation: " + fS(sc.x, 1) + ", " + fS(sc.y, 1) + "; " + fS(rot, 1) + "\n" +
		   "localBounds: " + fS(lb.left, 1) + ", " + fS(lb.top, 1) + ", " + fS(lb.width, 1) + ", " + fS(lb.height, 1) + "\n" +
		   "globalBounds: " + fS(gb.left, 1) + ", " + fS(gb.top, 1) + ", " + fS(gb.width, 1) + ", " + fS(gb.height) + "\n" +
		   "textureRect: " + tS(tr.left) + ", " + tS(tr.top) + ", " + tS(tr.width) + ", " + tS(tr.height) + "\n" +
		   "textureSize: " + tS(ts.x) + ", " + tS(ts.y) + "\n" +
		   addtlStats() + "\n" +
		   "");
	}

	virtual string addtlStats() {	// for debugging
		
		return "width(): " + fS(width());
	}

		/* Get values of hitBox() */
	float   width () {
		return hitBox().width;
	}
	
	float   height () {
		return hitBox().height;
	}
	
	float   top () {
		return hitBox().top;
	}
	
	float   bottom () {
		return hitBox().top + hitBox().height - 1;
	}
	
	float   left () {
		return hitBox().left;
	}
	
	float   right () {
		return hitBox().left + hitBox().width - 1;
	}
   
		/* Get values from getGlobalBounds() */
	float   gWidth () {
		return gGB().width;
	}
	
	float   gHeight () {
		return gGB().height;
	}
	
	float   gTop () {
		return gGB().top;
	}
	
	float   gBottom () {
		return gGB().top + gGB().height - 1;
	}
	
	float   gLeft () {
		return gGB().left;
	}
	
	float   gRight () {
		return gGB().left + gGB().width - 1;
	}
 
		/* Get corner positions with reference to hitBox() */
	vecF bottomLeft () {
		vecF v = vecF(left(), bottom());
		return v;
	}
	
	vecF bottomRight() {
		vecF v = vecF(right(), bottom());
		return v;
	}
	
	vecF topLeft() {
		return vecF (left(), top());
	}
	
	vecF topRight() {
		vecF v = vecF(right(), top());
		return v;
	}

	
		/* Set sprite position with reference to an edge of hitBox() */
	void setTop (float y) {
		sP(gP().x, y - hbofs() + getOrigin().y);
	}
	
	void setBottom (float y) {
		sP(gP().x, y + 1 + hbofs() - (height() - getOrigin().y));
	}
	
	void setLeft (float x) {
		sP(x - hbofs() + getOrigin().x, gP().y);
	}
	
	void setRight (float x) {
		sP(x + 1 + hbofs() - (width() - getOrigin().x), gP().y);
	}
    
		/* Set sprite position with reference to an edge of getGlobalBounds() */
	void setgTop (float y) {
		sP(gP().x, y + getOrigin().y);
	}
	
	void setgBottom (float y) {
		sP(gP().x, y + 1 - (height() - getOrigin().y));
	}
	
	void setgLeft (float x) {
		sP(x + getOrigin().x, gP().y);
	}
	
	void setgRight (float x) {
		sP(x + 1 - (width() - getOrigin().x), gP().y);
	}
    
	
	virtual void centerOn (float x, float y) {
		
		auto oldOgn = getOrigin();
		centerOrigin();
		sP(x, y);
		setOrigin(oldOgn);
	}

	virtual void centerOn (vecF pos) {
		
		centerOn(pos.x, pos.y);
	}

	virtual void centerOrigin () {
		
		::centerOrigin(s);
	}

	float getRadRotation() const {
        
		return toRad(getRotation());
	}
	
	float gRotR () {
        
		return toRad(getRotation());
	}
	
	virtual void setRadRotation (float rot) {
        
		s.setRotation(toDeg(rot));
	}
	
	virtual void setRotR (float rot) {
        
		s.setRotation(toDeg(rot));
	}


	virtual FloatRect clickBox () {
        
		return gGB();
	}

	/* hitBox() offset, for use when we want the
		 * collision capsule to simply be a certain number
		 * of pixels in from the borders of getGlobalBounds
		 */
	virtual float hbofs () {
		return 0;
	}
    
	virtual FloatRect hitBox () {
    	
		FloatRect gb = gGB();
    	return FloatRect(gb.left + hbofs(), gb.top + hbofs(),
						 gb.width - 2 * hbofs(), gb.height - 2 * hbofs());
	}
	
	FloatRect baseline () {
		
		FloatRect hb = hitBox();
		return FloatRect(hb.left, hb.top + hb.height - 1, hb.width, 1);
	}

	
		/* Sprite methods: making them virtual to allow
		 * for the possibility of composite object subclasses that may
		 * want to manipulate more than one sf::Drawable/Transformable
		 */
	virtual void setTexture (const Texture& tx, bool resetRect = false) {
		s.setTexture(tx, resetRect);
	}
	
	virtual void setTextureRect(const IntRect & r) {
		s.setTextureRect(r);
	}
	
	virtual void setColor (const Color& c) {
		s.setColor(c);
	}
	
	virtual const Texture* getTexture () const {
		return s.getTexture();
	}
	
	virtual const IntRect& getTextureRect () const {
		return s.getTextureRect();
	}
	
	virtual const Color& getColor () const {
		return s.getColor();
	}
	
	virtual FloatRect getLocalBounds () const {
		return s.getLocalBounds();
	}
	
	virtual FloatRect getGlobalBounds () const {
		return s.getGlobalBounds();
	}
	
	virtual void setPosition (float x, float y) {
		s.setPosition(x, y);
	}
	
	virtual void setPosition (const Vector2f& pos) {
		s.setPosition(pos);
	}
	
	virtual void setRotation (float ang) {
		s.setRotation(ang);
	}
	
	virtual void setScale (float x, float y) {
		s.setScale(x, y);
	}
	
	virtual void setScale (const Vector2f& factors) {
		s.setScale(factors);
	}
	
	virtual void setOrigin (float x, float y) {
		s.setOrigin(x, y);
	}
	
	virtual void setOrigin (const Vector2f ogn) {
		s.setOrigin(ogn);
	}
	
	virtual const Vector2f& getPosition () const {
		return s.getPosition();
	}
	
	virtual float getRotation () const {
		return s.getRotation();
	}
	
	virtual const Vector2f& getScale () const {
		return s.getScale();
	}
	
	virtual const Vector2f& getOrigin () const {
		return s.getOrigin();
	}
	
	virtual void move (float x, float y) {
		s.move(x, y);
	}
	
	virtual void move (const Vector2f& ofs) {
		s.move(ofs);
	}
	
	virtual void rotate (float ang) {
		s.rotate(ang);
	}
	
	virtual void scale (float x, float y) {
		s.scale(x, y);
	}
	
	virtual void scale (const Vector2f& factors) {
		s.scale(factors);
	}
	
	virtual const Transform& getTransform () const {
		return s.getTransform();
	}
	
	virtual const Transform& getInverseTransform () const {
		return s.getInverseTransform();
	}

	
	Sprite 	s;

	unsigned int    id = 0;
	bool    		clickedOn { false };

//============= D E B U G   S T A T S ==================
		
	RectangleShape  statRect;
	Text            statText;
	Font            statFont;
	bool            showStats { false };
	
}; //end class ZSprite




inline void centerOrigin (ZSprite& obj) {
    
	obj.centerOrigin();
}

inline bool hitL (ZSprite& z, FloatRect& isct) {
    
	return isOrBetween(z.left(), isct.left, isct.left + isct.width - 1);
}

inline bool hitR (ZSprite& z, FloatRect& isct) {
    
	return isOrBetween(z.right(), isct.left, isct.left + isct.width - 1);
}

inline bool hitTop (ZSprite& z, FloatRect& isct) {
    
	return isOrBetween(z.top(), isct.top, isct.top + isct.height - 1);
}

inline bool hitBot (ZSprite& z, FloatRect& isct) {
    
	return isOrBetween(z.bottom(), isct.top, isct.top + isct.height - 1);
}

inline bool hitTopL (ZSprite& z, FloatRect& isct) {
    
	return hitTop(z, isct) && hitL(z, isct);
}

inline bool hitTopR (ZSprite& z, FloatRect& isct) {
    
	return hitTop(z, isct) && hitR(z, isct);
}

inline bool hitBotR (ZSprite& z, FloatRect& isct) {
    
	return hitBot(z, isct) && hitR(z, isct);
}

inline bool hitBotL (ZSprite& z, FloatRect& isct) {
    
	return hitBot(z, isct) && hitL(z, isct);
}




#endif /* ZSPRITE_HPP */

