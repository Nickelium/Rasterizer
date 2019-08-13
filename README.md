# Realtime CPU Rasterizer/Renderer

This is a simple a realtime software renderer that emulates the OpenGL graphics pipeline. 
It utilizes few dependencies and is built mostly from scratch and written in C++98/C++11. 
The goal of this project is to build a renderer without the help of any Graphics API such as DirectX, OpenGL or Vulkan. 
Therefore it doesn't utilize any hardware acceleration provided by the GPU.

<p float="left">                                                   
    <img width="99%" src="https://raw.githubusercontent.com/Nickelium/Rasterizer/master/Output/26.png">
</p> 

## Features
* Wireframe rendering
* Flat Shading
* Gouraud Shading
* Phong Shading
* Diffuse Texture Mapping
* Scene Switching 
* Rendermode Switching
* OBJ file parser
* JSON file scene description

## Back End Features
* Backface culling
* Depth Buffering
* Perspective correct interpolation
* Extendable vertex and fragment shaders
* Linear Algebra library 
* Rasterizer
* Line Rendering

## Dependencies
* SDL2 : Window context and to pass color buffer
* stbi_image: Load textures
* nlohmann json: Parse scenes file

## Getting Started
The JSON file config.json, located in the directory Resources, allows the user to describe one or multiple scenes and objects.
The scene description file should follow the format below:

	{
		"myscene_name":
		{
		  "myobject_name":
		  {
			  "model": "mypathto/myobjectmodel.obj",
			  "diffusemap": "mypathto/mytexture.png",
			  "material":
			  {
				"ambient": [0.15, 0.55, 0.85],
				"diffuse": [0.55, 0.12, 0.23],
				"specular": [1.0, 1.0, 1.0],
				"exponent": 20.0
			  },
			  "transform":
			  {
				"position": [0.0, -0.75, -10.0],
				"rotation": [0.0, 0.0, 45.0],
				"scale": [1.5, 1.5, 1.5]
			  }
		  },
		  
		  ... more objects
		  
		},
		
		... more scenes
		
	}

In the same directory, there is already a filled config file which can be used as an example. 
Currently only triangulated obj models are supported and for textures JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC (stbi_image).

Once the scene description filled in, the program can be started.
Use the arrow keys (left and right) to switch between the different scenes. 
The spacebar on the other hand will switch between the different render modes and shaders.

## Notes
The renderer in it current state has very little optimizations because it was first intended to get things working.
As consequence the renderer might have a hard time to keep a decent framerate when provided with a high poly count mesh. 
Optimizations are definetely on my todo list.
Mtl files are currently not supported.
Normals and texture coordinates are required in the mesh models.

## Render Samples
<p float="left">                                                   
    <img width="49%" src="https://raw.githubusercontent.com/Nickelium/Rasterizer/master/Output/21.png">
    <img width="49%" src="https://raw.githubusercontent.com/Nickelium/Rasterizer/master/Output/22.png">
    <img width="49%" src="https://raw.githubusercontent.com/Nickelium/Rasterizer/master/Output/16.png">
    <img width="49%" src="https://raw.githubusercontent.com/Nickelium/Rasterizer/master/Output/19.png">
</p>           

https://www.youtube.com/watch?v=7sPPdtgjI90
      
## References Litterature
* [Scratch A Pixel](https://www.scratchapixel.com/)
* [Tiny Renderer](https://github.com/ssloy/tinyrenderer/wiki)
* [Leanrn OpenGL](https://learnopengl.com/)
* [Mathematics for 3D Game Programming and Computer Graphics, Eric Lengyel](http://mathfor3dgameprogramming.com/)

## Reference Models and Textures
* Utah Teapot, The University of Utah
* Suzanne Blender, Blender Foundation
* African Head, Vidar Rapp
* Diablo, Samuel (arshlevon) Sharit
* [Illidan Stormrage Legion](https://sketchfab.com/models/48a695c9239c4002be032a48d83e65a9)
* [Wolf's Edge - World of warcraft](https://sketchfab.com/models/b2f1035bec8e4f608f55806134703cb6)
* [Low Poly Guitar](https://sketchfab.com/models/757b3b5aaf6d4052933edc329c4f5aa5)
* [Low Poly Helmet](https://sketchfab.com/models/ae3a2b2ea8e9492fae8a4a6bea9f3505)
* [Low Poly RaceCar](https://sketchfab.com/models/45840e2136c44080b4c1e7521cce8db3)
* [Cerberus Gun](http://artisaverb.info/Cerberus.html)
