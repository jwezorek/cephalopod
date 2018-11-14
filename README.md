# cephalopod
cephalopod is intended to be the minimal useful cross-platform "2D game framework" above the level of hardware abstraction layers
like SDL and SFML but below the level of game engines.

It is an implementation of sprite sheets, sprites, the notion of a "sprite tree" -- which gets called by different names but is the 2D equivalent of a scene graph, a tree of sprite nodes in which each node has local transformations applied to it that then cascade down to its children. An action system. Scenes. Scene transitions and a library of functions for doing 2D collision detection.

In short cocos2d-x with 
1. minus all the crap 
2. plus modern C++. 

Cephalopod will never support anything 3D -- beyond maybe some canned 3D scene transitions -- and will never support baked-in physics. 

It is a work in progress so the state is in flux. It is currently on top of GLFW for setting up the window and handling input. It has no dependencies beyond GLFW and platform libraries.

If anyone is interested email me at the address found on http://jwezorek.com.
