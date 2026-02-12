# Parasheep

## C++/SFML arcade game with gravity-affected projectiles

IMG

### ABOUT THE PROJECT
This program started off as a basic exercise (in Python, originally) when I was teaching myself projectile physics. Eventually I gamified it for the benefit of my small children, although they relied on me to aim the gun while they pressed "fire." The theme of shooting at a parachuting entity harks back to the ancient game Airborne! which I played as a child.  

The game is one of several projects that I undertook in the early days of my programming self-teaching (ca. 2020). The projects of this period were motivated primarily by the desire to know if I could "solve" the challenge of bringing this or that mechanic or concept into being through programming language, largely without consulting any other sources to see how it was done (this was also pre-AI era). The projects were characterized by a lack of concern for code organization, best practices, readability, or maintainability, because my free time was very limited and my chief objective was getting the satisfaction of knowing I got the idea to work. Most of the time, when I achieved that much of the goal, I was ready to move on to the next logical challenge rather than pore over the details of making a well-rounded product fit for public consumption. I've taken a little time to restructure some things before posting the code here; nonetheless, much about the program remains primitive or suboptimal as it was originally written. As with nearly all my projects, I can think of many things that could make the game more interesting, polished, or full-featured; whether I'll ever find/make the time to circle back around and implement them remains to be seen.

I've employed the TGUI library to facilitate entering and displaying high scores for Letter Invaders (credits to Bruno Van de Velde).

(NOTE: Linking to TGUI was causing errors in the Windows version. For now, the high scores feature is disabled in Windows. It should work fine on Mac and Linux.)

### FILE DESCRIPTIONS
* **sfmlTguiApp:**  Implements `main()` and the abstract app
* **parasheep:**  Implements primary graphical elements, physics, game logic
* **object:**  Small structs representing enemy sheep, projectiles, etc.
  
(From my "reusable modules" repo: https://github.com/johnnywz00/SFML-shared-headers)
* **jwz:**  C++ utility functions, #defines, shortcuts
* **jwzsfml:**  Like above, but SFML-specific
* **zsprite:**  Wrapper class for SFML sprite with many extra methods, particularly related to collision checking
* **vsprite:**  Subclass of ZSprite that uses velocity
* **resourcemanager:**  Static class for accessing resource files globally
* **highscore:**  Struct representing data for one high score entry

### BUILDING INSTRUCTIONS
Ready-made program files are available on the Releases page of this repository, with versions for MacOS, Windows, and Linux. If your OS isn't supported by the pre-made versions, or if you have other reasons for building from source:
- Clone this repository, and navigate to the root folder of that clone in a terminal window.
- Run:
<pre>
   cmake -B build
   cmake --build build --parallel
</pre>
