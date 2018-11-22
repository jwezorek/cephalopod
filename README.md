# cephalopod
cephalopod is intended to be the minimal useful cross-platform "2D game framework" above the level of hardware abstraction layers
like SDL and SFML but below the level of game engines.

It is an implementation of 
* sprites and sprite atlases/sprite sheets
* the notion of a "sprite tree" -- which gets called by different names but is the 2D equivalent of a scene graph, a tree of sprite nodes in which each node has local transformations applied to it that then cascade down to its children
* An action system
* Scenes
* Scene transitions
* a library of usefuk functions for doing 2D collision detection.

In short it's akin to cocos2d-x minus all the crap plus modern C++. 

Cephalopod will never support 
* anything 3D -- beyond maybe some canned 3D scene transitions
* baked-in physics
* baked-in collision detection (beyond just offering some functions e.g. rectangle/rectangle intersection, line segment/circle intersection, etc.

It is a work in progress so the state is in flux. It is currently on top of GLFW for setting up the window and handling input. It has no dependencies beyond GLFW and platform libraries. If anyone is interested email me at the address found on http://jwezorek.com.
