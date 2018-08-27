# cephalopod
cephalopod is intended to be the minimal useful cross-platform "2D game framework" above the level of hardware abstaction layers
like SDL and SFML but below the level of game engines: an implementation of sprite sheets, sprites, the notion of a "sprite tree" that gets called by different names but is the 2D equivalent of a scene graph -- a tree of sprite nodes in which each node has local transformations applied to it that then cascade down to its children. An action system. Scenes. Eventuallu scene transitions and eventually a library of functions for doing 2D collision detection.

Basically, it's akin to cocos2d-x with all the crap cut out and using modern C++. Cephalopod will never support anything 3D -- beyond maybe some canned 3D scene transitions -- and will never support baked-in physics. 

It is a work in progress so the state is in flux. It is currently on top of OpenGL + GLFW for setting up the window and handling input. It has no dependencies beyonf GLFW and platform libraries.

If anyone is interested email me at the address found on http://jwezorek.com.
