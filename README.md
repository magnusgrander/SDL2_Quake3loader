# SDL2_Quake3loader
Loads a quake3 level. With textures and shadows all in an SDL2 window. Also have a walking camera with collision detection,

This example features :
Rendering of level geometry via the BSP with Potential visible set, Pre-baked in the map.​

3D camera with Quake-like side tilt thing
Loading basic level geometry, leaves, lightmap and more.
Texture loading & mapping (only if its available)
GLM math for fast math calculations

In order to use this example, You'll need some resources from Quake 3 : Arena (or Quake live).
Since including the resources in the example violates the Copyright law (and potentially dunk me in some sticky situations), You have to get them yourself.
Thankfully the bare-minimum resources you need is just a .bsp map file.. And you can grab some of them from the Quake 3 map database.
On the other hand, The assets (pak0.pk3) must be obtained by you. It can be found in quake 3's installation folder with the name [pak0.pk3], 
And you can grab the demo version of Quake 3 : Arena on steam for free, 
if you don't happen to have the full version of the game. 
Quake live's assets works just as fine as Q3A, So you can use that if you have one too.

The program uses a fixed function pipleline

Graphics implemented through a set of pre-defined functions, such as transform and lighting (glRotatef, glLightfv, etc.)
Programmer is limited to what those functions can provide (e.g., 8 lights, Blinn-Phong model, etc.) 

Will later change this too a programmable pipeline
Graphics hardware is programmable through shader programs, which are implemented in a shader language (GLSL)
Shader programs run on the GPU
This enables new possibilities for graphics – implementing things that are difficult or impossible to do using the fixed function pipeline

Any help to do this would be very appriciated....

greetings;
Magnus Grander






