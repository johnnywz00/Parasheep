
#include "sfmlTguiApp.hpp"
#include "parasheep.hpp"


void State::onCreate ()
{
    levelLabel = Text("LEVEL " + tS(level), gFont("menuButton"), 30);
    levelLabel.setOutlineColor(Color::Black);
    levelLabel.setOutlineThickness(2);
    levelLabel.setFillColor(Color(80, 160, 255));
    levelLabel.sP(50, 50);

    scoreTxt = Text("SCORE:     \nACCURACY:  " , gFont("stats"), 18);
    scoreTxt.setOutlineColor(Color::Black);
    scoreTxt.setOutlineThickness(1);
    scoreTxt.setFillColor(Color(80, 160, 255));
    scoreTxt.sP(20, 110);

	launcher.setTexture(gTexture("launcher"));
    centerOrigin(launcher);
    launcher.sP(40, scrh - 75);
    launcher.sRot(initAngle);

	deh.setTexture(gTexture("deh"));
    centerOrigin(deh);
    deh.sP(launcher.gP() + vecF(0, -8));
    deh.setScale(1.1, 1.1);

	ground.setTexture(gTexture("ground"));
    ground.setTextureRect(IntRect(0, 0, scrw, ground.gLB().height));
    ground.setOrigin(0, ground.gLB().height);
    ground.sP(0, scrh);
	
	cactus.setTexture(gTexture("cactus"));
	cactus.setOrigin({cactus.gLB().width / 2, cactus.gLB().height - 1});
	cactus.setPosition(scrcx - 50, scrh - ground.gLB().height + 5);

    platform.setSize(vecF(60, 60));
    platform.sP(0, scrh - 65);
    platform.setFillColor(Color(150, 116, 0));
    platform.setOutlineColor(Color(131, 102, 3));
    
	/* Gradient sky */
    forNum (20) {
		bkgd[i].setSize({scrw, scrh / 20});
		bkgd[i].sP(0, (scrh * (19 - i)) / 20);
    }
    forNum (10) {
		clouds[i].setTexture(gTexture("cloud"));
		clouds[i].sP(randRange(0, scrw + 400), randRange(0, scrh - 50));
		float factor = float(randRange(50, 120)) / 100;
		clouds[i].setScale(factor, factor);
		cloudVels[i] = float(randRange(5, 30)) / 100;
		int gray = randRange(230, 255);
		clouds[i].setColor(Color(gray, gray, gray));
    }
	
    powerBar[0].setSize(vecF(15, 350));
    powerBar[1].setSize(vecF(9, 344));
    powerBar[0].setOutlineThickness(1);
    powerBar[1].setOutlineThickness(2);
    powerBar[0].setOutlineColor(Color(150, 150, 150));
    powerBar[1].setOutlineColor(Color(20, 20, 255));
    powerBar[0].setFillColor(Color::Transparent);
    powerBar[1].setFillColor(Color(0, 0, 255, 120));
    powerBar[0].setOrigin(0, powerBar[0].gLB().top + powerBar[0].gLB().height);
    powerBar[1].setOrigin(0, powerBar[1].gLB().top + powerBar[1].gLB().height);
    powerBar[0].setPosition(10, scrcy + powerBar[0].gLB().height / 2);
    powerBar[1].setPosition(13, scrcy + powerBar[1].gLB().height / 2);

	menuCreate();

    resetGame();
} //end onCreate

void State::onMouseDown (int x, int y)
{
	if (mode == menu) {
		menuClick(x, y);
		return;
	}
}

void State::onKeyPress (Keyboard::Key k)
{
	switch(k) {
		case Keyboard::Escape:
			if (mode == menu)
				app->close();
			else {
				mode = menu;
				renWin->setMouseCursorVisible(true);
				auto sc = gui->get("scores");
				if (sc)
					sc->setVisible(false);
			}
			break;
		
		case Keyboard::Y:
			resetGame();
			break;
			
		case Keyboard::Space:
			if (!gunMode && canShoot) {
				pullingBack = true;
			}
			break;
			
		default:
			break;
	}
}

void State::onKeyRelease (Keyboard::Key k)
{
	switch(k) {
			// Fire key was just released
		case Keyboard::Space:
			if (!gunMode && pullingBack) {
				needLaunch = true;
			}
			
		default:
			break;
	}
}

void State::update (const Time& time)
{
		// No processing necessary here if high scores are showing
	auto sc = gui->get("scores");
	if (sc && sc->isVisible() && sc->isFocused())
	   return;

		// Let menu screen run its own logic
	if (mode == menu) {
		menuUpdate();
		return;
	}
			   
//===============================================
		/* TESTING AIDS */
	if (iKP(G)) { gunMode = iKP(LShift) ? 0 : 1; PAUSE; }
	if (iKP(H)) { newLevel(); PAUSE; }
	if (iKP(I)) { newEnemy(time); PAUSE; }
//===============================================

		// Make clouds drift across the sky
	forNum (10) {
	   clouds[i].move(-cloudVels[i], 0);
	   if (clouds[i].gGB().left + clouds[i].gGB().width < -1)
		   clouds[i].sP(scrw + 2, clouds[i].gP().y);
	}

		// Don't process further if player is dead
	if (gameOver)
		return;

		// Aim gun based on keyboard input
	if (iKP(Left))
		gunLeft();
	if (iKP(Right))
		gunRight();

		// Orient the aiming guideline
	updateGuide();

			// Handle firing projectiles
	
	if (time.asSeconds()  > lastFireTime.asSeconds() + timeBetweenFires) {

		canShoot = true;
			// Normal mode: have to hold key to determine projectile velocity
		if (!gunMode) {
			
				// The gun is being activated but hasn't fired yet
				// onKeyRelease will handle actual firing in this mode
			if (pullingBack) {
			   power = min(power + powerRate, maxPower);
			   powerBar[1].setScale(1, power / maxPower);
			}
		}
			// Rapid-fire gun mode
		else if (gunMode == 1) {
			
				// Shoot projectiles at full power every time interval
			if (iKP(Space)) {
				power = maxPower;
				needLaunch = true;
			}
		}
	}
	if (needLaunch) {
		launch();
		lastFireTime = time;
		needLaunch = false;
	}

		// Send a new parachuting sheep if enough time has elapsed
	if (time.asSeconds() > lastEnemyTime.asSeconds() +
			timeBetweenEnemies && !(randRange(0, 60)))
	   newEnemy(time);

	
/* Update sheep sprites */
	
	for (auto& e : enemies) {
	   if (e.dead)
		   continue;
	   if (e.onGround) {
		   
				// Sheep on ground don't need updating unless the sheep are charging
		   if (!running)
			   continue;
		   else {
					// The sheep is hopping towards the player and has just landed on the ground
			   if (e.s.gP().y >= groundLevel) {
				   e.s.sP(e.s.gP().x, groundLevel);
				   e.yVel = e.jumpVel;
			   }
					// The sheep is mid-jump and needs gravity added
			   else e.yVel += sheepG;
			   
					// Move the running sheep
			   e.s.move(runRate, e.yVel);
				
					// End game if running sheep reaches player
			   if (e.s.gGB().intersects(deh.gGB())) {
				   endGame();
				   return;
			   }
		   }
	   }
			// Move shot/falling sheep downwards and erase them once below the screen
	   else if (e.hit) {
		   e.s.move(0, fallRate);
		   if (e.s.gP().y > scrh)
			   enemies.erase(enemies.begin() + indexOfRef(enemies, e));
	   }
		
			// This sheep is floating down with a parachute
	   else {
				// Create the side-to-side floating motion
		   e.floatDeg += floatDegInc;
		   if (e.floatDeg >= 360)
			   e.floatDeg -= 360;
		   float xAdj = cos(toRad(e.floatDeg)) * floatDev;
		   e.s.sP(e.xLine + xAdj, e.s.gP().y + e.floatRate);
		   
				// Rotate the sprite relative to the amount of drift
		   e.s.sRot(cos(toRad(e.floatDeg)) * floatSkew);
		   
				// The sheep landed without getting shot
		   if (e.s.gP().y > groundLevel) {
			   e.s.sP(e.s.gP().x, groundLevel);
			   e.onGround = true;
				// Lose special gun if sheep lands
			   if (gunMode)
				   gunMode = 0;
				// Change the sprite from parachuting to standing
			   Sprite s2(gTexture("sheep"));
			   centerOrigin(s2);
			   s2.sP(e.s.gP().x, e.s.gP().y + 10);
			   e.s = s2;
			   
					// Check to see if the sheep are ready to charge
			   ++numOnGround;
			   if (numOnGround >= 3 && !running) {
				   running = true;
			   }
			}
		}
	} // end for e in enemies
	
	
/* Give motion to flying projectiles */

	for (auto& p : projs) {
		if (p.isLoaded)
			continue;
		p.velocity.y += G;
		p.move(p.velocity);
		
			// Find if we've popped any sheep
		for (auto& e : enemies) {
			if (e.dead)
				continue;
			
				// It's a hit
			if (p.gGB().intersects(e.hitBox())) {
				
					// Change sheep sprite
				Sprite s2(gTexture("fallingSheep"));
				centerOrigin(s2);
				s2.sP(e.s.gP().x, e.s.gP().y + 10);
				s2.sRot(e.s.gRot());
				e.s = s2;
				
					// If the sheep was already on the ground, flip it upside down
				if (e.onGround) {
					e.dead = true;
					e.s.sRot(180);
					gSound("hitLanded").play();
					
						// If all charging sheep are popped before another one lands, newly landed sheep won't charge until 3 reached again
					--numOnGround;
					if (numOnGround < 1)
						running = false;
				}
					// This was a parachuting sheep; mark to fall off the screen
				else {
					e.hit = true;
					
						// Special green sheep earn the better gun
					if (e.isBonus)
						gunMode = 1;
					gSound(e.isBonus ? "specialGun" : "hitDropping").play();
				}
					// Handle scoring and advancing level
				++numShot;
				score += (e.isBonus ? 50 : 10) + level - 1;
				if (int(numShot) % 10 == 0)
					newLevel();
				
					// Erase the projectile that hit a target
				p.readyToErase = true;
			}
		}
		// The projectile went offscreen
		if (    p.gP().x > scrw ||
				p.gP().y > scrh - 20) {
			p.readyToErase = true;
		}
	} // end for p : projs
	
		// Delete projectiles that are out of screen or that have hit sheep
	for (auto& p : projs) {
		if (p.readyToErase)
			projs.erase(projs.begin() + indexOfRef(projs, p));
	}
		// If any projectiles were erased in the loop our curProj pointer
		// will have become invalid. The loaded proj will always be back()
	curProj = &projs.back();

	shootingPct = (totalShots > 0 ? numShot / totalShots : 0) * 100;

		// Write the score
	scoreTxt.setString("SCORE:     " + tS(score) + "\nACCURACY:  " +
					   floatStr(shootingPct, 2)   );
	
} //end update

void State::draw ()
{
	if (mode == menu) {
		menuDraw();
		return;
	}

	for (auto& b : bkgd)
		renWin->draw(b);
	for (auto& c : clouds)
		renWin->draw(c);
	renWin->draw(powerBar[0]);
	renWin->draw(powerBar[1]);
	
	// The shot/falling sheep need to be drawn first to disappear behind the scenery
	for (auto& e : enemies)
		if (e.hit)
			renWin->draw(e.s);
	renWin->draw(platform);
	renWin->draw(ground);
	renWin->draw(cactus);
	renWin->draw(deh);
	renWin->draw(launcher);
	for (auto& p : projs)
		renWin->draw(p);
	renWin->draw(levelLabel);
	renWin->draw(scoreTxt);
	for (auto& e : enemies)
		if (!e.hit)
			renWin->draw(e.s);
	renWin->draw(guideline);
}

void State::menuCreate ()
{
	/* Smatter the screen with faint PARASHEEP texts */
	bkgdRt.create(scrw, scrh);
	bkgdRt.clear(CHARCOAL);
	Text tmp {"PARASHEEP", gFont("menuTitle"), 50};
	forNum(70) {
		tmp.setFillColor(Color(0, 0, 0, randRange(50, 80)));
		tmp.setPosition((float)randRange(-20, scrw - 100), (float)randRange(scrh - 30));
		float factor = randFloat(.5, 2);
		tmp.setScale(factor + randFloat(-.1, .1), factor + randFloat(-.1, .1));
		tmp.setRotation((280 + randRange(160)) % 360); // angles stay between 280-360 or 0-80
		bkgdRt.draw(tmp);
	}
	bkgdRt.display();
	menuBkgd.setTexture(bkgdRt.getTexture());
	centerOrigin(menuBkgd);
	menuBkgd.setPosition(scrcx, scrcy);

	gameTitle.setFont(gFont("menuTitle"));
	gameTitle.setString(String("P A R A S H E E P"));
	gameTitle.setCharacterSize(130);
	gameTitle.setFillColor(decreaseSaturation(Color::Yellow, 40));
	gameTitle.setOutlineColor(Color(80, 80, 80));
	gameTitle.setOutlineThickness(5);
	centerOrigin(gameTitle);
	gameTitle.sP(scrcx, scrcy);

	/* Set up buttons */
	string str[numButtons];
	str[0] = "Commence!";
	str[numButtons-1] = "Retreat.";

	for (int i = 0; i < numButtons; ++i) {
	    rects[i].setSize(buttonSize);
		rects[i].setFillColor(Color(61, 173, 65)); // Color(225, 198, 32));
	    if (i == numButtons - 1)
			rects[i].setFillColor(Color(215, 60, 68)); // Color(225, 165, 71));
	    rects[i].setOutlineColor(Color(54, 60, 25));
	    rects[i].setOutlineThickness(5);
		centerOrigin(rects[i]);
		float ofs = 300;
		rects[i].setPosition(scrcx - ofs + i * 2 * ofs, scrcy - 250);
	    labels[i].setFont(gFont("menuButton"));
	    labels[i].setString(String(str[i]));
	    labels[i].setCharacterSize(36);
	    labels[i].setFillColor(Color(50, 50, 50));
	    FloatRect rect = labels[i].gLB();
		centerOrigin(labels[i]);
		labels[i].setPosition(rects[i].getPosition().x, rects[i].getPosition().y - 16);
	 }
	
	instrucsTxt = Text(instrucsStr, gFont("menuButton"), 22);
	instrucsTxt.setFillColor(Color(215, 220, 210, 200));
	centerOrigin(instrucsTxt);
	instrucsTxt.setPosition(scrcx, scrh - 180);
}

void State::menuClick (int x, int y)
{
	float halfx = buttonSize.x / 2.;
	float halfy = buttonSize.y / 2.;
	for (int i = 0; i <  numButtons; ++i) {
		if (x >= rects[i].getPosition().x - halfx &&
				x <= rects[i].getPosition().x + halfx &&
				y >= rects[i].getPosition().y - halfy &&
				y <= rects[i].getPosition().y + halfy) {
			if (i == 0)
				startGame();
			else if (i == numButtons-1)
				app->close();
		}
	}
}

void State::menuUpdate ()
{
   // potential animations
}

void State::menuDraw ()
{
   renWin->draw(menuBkgd);
   renWin->draw(gameTitle);
   for (auto& b : rects)
	   renWin->draw(b);
   for (auto& b : labels)
	   renWin->draw(b);
	renWin->draw(instrucsTxt);
}

void State::setUpHighScores ()
{
	ifstream scores { Resources::executingDir() / "resources" / "parasheepscores.txt" };
	if (scores.is_open()) {
		topTen.clear();
    	string line;
    	while (getline(scores, line)) {
	    	HighScore hs;
	    	stringstream ss(line);
	    	ss >> hs.score >> hs.player >> hs.date >> hs.level;
	    	topTen.push_back(hs);
    	}
    	scores.close();
    }

    auto p = tgui::Panel::create();
	p->setSize(800, 700);
	p->setPosition(scrcx - (p->getSize().x / 2), scrcy - (p->getSize().y / 2));
    gui->add(p, "scores");
    auto l = tgui::Label::create();
    l->setOrigin(.5, .5);
    l->setPosition("50 % ", 50);
    l->setTextSize(48);
    l->setText("HALL OF HEROES");
    p->add(l, "scoreslabel");
    auto vl = tgui::VerticalLayout::create();
    vl->setSize("90 % ", "80 % ");
    vl->setPosition("50 % ", 100);
    vl->setOrigin(.5, 0);
    p->add(vl, "vl");
    forNum(10) {
	    auto tt = topTen[i];
	    auto eb = tgui::EditBox::create();
	    eb->setTextSize(24);
	    eb->setText(highScoreToText(tt));
	    eb->onReturnKeyPress(&State::enterNewScore, this);
	    vl->add(eb, "EBox" + tS(i));
	    eboxes.push_back(eb);
	    if (i != 9)
	 		vl->addSpace(.1);
    }
    p->setVisible(false);
}

void State::resetGame ()
{
	enemies.clear();
	gunMode = 0;
	power = 0;
	numOnGround = 0;
	numShot = 0;
	totalShots = 0;
	shootingPct = 0.0f;
	score = 0;
	lastEnemyTime = Time();
	lastFireTime = Time();
	timeBetweenEnemies = 5;
	Enemy::gFloatRate = 1;
	runRate = -1;
	level = 1;
	gameOver = false;
	running = false;
	pullingBack = false;
	shotFiredThisFrame = false;
	launcher.sRot(initAngle);
	powerBar[1].setScale(1, 0);
	
	projs.clear();
	projs.emplace_back();
	curProj = &projs.back();
	setLoadedProjPos();
	
	newEnemy(Time());
	levelLabel.setString("LEVEL 1");
	auto sc = gui->get("scores");
	if (sc)
		sc->setVisible(false);
	
	forNum (20) {
		Color c = Color(255, 165, 54);
		Color c2 = Color(227, 229, 164);
		bkgd[i].setFillColor(Color(c.r + ((c2.r - c.r) / 20) * i,
								   c.g + ((c2.g - c.g) / 20) * i,
								   c.b + ((c2.b - c.b) / 20) * i ));
	}
	
#ifndef WINDOWS
	setUpHighScores();
#endif
}

void State::startGame ()
{
   resetGame();
   mode = playing;
   renWin->setMouseCursorVisible(false);
}

void State::newLevel ()
{
    ++level;
    Enemy::gFloatRate += levelInc;
    runRate += runInc;
    decrementLevelTime();
    levelLabel.setString("LEVEL " + tS(level));
	
	/* Make the sky gradually transition from dusk to night
	 * as levels are completed
	 */
	if (level < 11) {
		Color    c,
				 c2,
				 t = skyTop,
				 t2 = skyTopNight,
				 b = skyBottom,
				 b2 = skyBottomNight;
				 c = Color(t.r + ((t2.r - t.r) / 10) * (level - 1),
						   t.g + ((t2.g - t.g) / 10) * (level - 1),
						   t.b + ((t2.b - t.b) / 10) * (level - 1) );
			     c2 = Color(b.r + ((b2.r - b.r) / 10) * (level - 1),
						   b.g + ((b2.g - b.g) / 10) * (level - 1),
						   b.b + ((b2.b - b.b) / 10) * (level - 1) );
	    for (int i = 0; i < 20; ++i) {
		   bkgd[i].setFillColor(Color(c.r + ((c2.r - c.r) / 20) * i,
									  c.g + ((c2.g - c.g) / 20) * i,
									  c.b + ((c2.b - c.b) / 20) * i ));
		}
	}
}

void State::setLoadedProjPos ()
{
	vecf offsetPolar { projOffset, angle() };
	curProj->sP(launcher.gP().x + toRect(offsetPolar).x,
				launcher.gP().y + toRect(offsetPolar).y);
}

void State::newEnemy (const Time& time)
{
	Sprite  s(gTexture("parasheep"));
    centerOrigin(s);
    float x = randRange(70, scrw - 30);
    s.sP(x, -10);
    Enemy e;
    e.xLine = x;
    e.s = s;
    if (!randRange(15)) {
		e.isBonus = 1;
		e.s.setColor(Color::Green);
		e.floatRate = 2.5;
    }
    enemies.push_back(e);
    lastEnemyTime = time;
}

void State::updateGuide ()
{
	guideline.clear();
	vecF ogn = launcher.gP() + toRect(launcher.gLB().width, angle());
	int gray = 0;
	for (int i = 0; i <  guideLength; i  += 8) {
		Vertex v { vecF(ogn + toRect(i, angle())), Color(gray, gray, gray) };
		guideline.append(v);
        v = Vertex(vecF(ogn + toRect(i + 2, angle())), Color(gray, gray, gray));
        guideline.append(v);
	}
}

void State::gunLeft ()
{
	launcher.rotate(iKP(LShift) ? -angRate / 2 : -angRate);
	if (angle() < 270)
       launcher.sRot(270);
	setLoadedProjPos();
}

void State::gunRight ()
{
	launcher.rotate(iKP(LShift) ? angRate / 2 : angRate);
	if (angle() > 342 || angle() < 270)
       launcher.sRot(342);
	setLoadedProjPos();
}

void State::launch ()
{
	pullingBack = false;
	canShoot = false;
	curProj->isLoaded = false;
	curProj->setVelocity(toRect(power * powerScale / curProj->mass(), angle()));
	powerBar[1].setScale(1, 0);
	power = 0;
	++totalShots;
	gSound("shoot").play();
		// Reload
	projs.emplace_back();
	curProj = &projs.back();
	setLoadedProjPos();
}

void State::endGame ()
{
   gSound("gameOver").play();
   gameOver = true;
   renWin->setMouseCursorVisible(true);
#ifndef WINDOWS
   showHighScores(score, level);
#endif
}

void State::showHighScores(int score, int level)
{
	renWin->setMouseCursorVisible(true);
	auto sc = gui->get<tgui::Panel>("scores");
	if (!sc)
		return;
	
	sc->setVisible(true);
	if (score >= topTen[9].score) {
		string date {LocalTime().slashDate()};
		HighScore hs {score, level, date};
		auto p = find_if(topTen.begin(), topTen.end(),
						 [&](auto x){ return hs.score >= x.score; });
		if (p != topTen.end())
			topTen.insert(p, hs);
		topTen.resize(10);
		forNum(10) {
			HighScore& tt = topTen[i];
			auto eb = gui->get<tgui::EditBox>("EBox" + tS(i));
			if (tt.player == "") {
				eb->setText(tS(hs.score));
				eb->selectText();
				eb->setEnabled(true);
				eb->setFocused(true);
			}
			else {
				eb->setText(highScoreToText(tt));
				eb->setEnabled(false);
			}
		}
		sc->setFocused(true);
	}
	else {
		sc->setFocused(false);
		for (auto& e : eboxes) {
			e->setFocused(false);
			e->setEnabled(false);
		}
	}
}

void State::enterNewScore (tgui::String str)
{
	string s = str.toStdString();
	/* Truncate the entered name if it's too long */
	if (s.length() > 18)
		s.resize(18);
	/* Get rid of potential spaces in the entered name
	 * just to keep our parsing simple.
	 */
	for (auto& ch : s)
		if (ch == ' ')
			ch = '-';
	
    forNum(10) {
        if (topTen[i].player == "") {
            topTen[i].player = s == "" ? "NamelessOne" : s;
            eboxes[i]->setText(highScoreToText(topTen[i]));
            eboxes[i]->setEnabled(false);
            gui->get("scores")->setFocused(false);
        }
    }
    topTenToFile();
}

string State::highScoreToText (HighScore &hs)
{
   stringstream ss {};
   ss << "  " << std::left << std::setw(10) << tS(hs.score) << std::setw(20) << hs.player << std::setw(10)
           << hs.date << std::right << std::setw(10) << tS(hs.level);
   return ss.str();
}

void State::topTenToFile ()
{
	ofstream f { Resources::executingDir() / "resources" / "parasheepscores.txt", ios_base::trunc };
    if (!f.is_open())
        return;
    for (auto& t : topTen) {
        f << t.score << " " << t.player << " " <<
        t.date << " " << t.level << "\n";
    }
	f.close();
}
