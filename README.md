# Graphics <sub><sub>🖌️</sub></sub>

## Introduction

This repo is all about GUIs and how to mask the core application logic with a clean user interface and connect it to the backend that lifts the entire application data.

Unlike web interfaces, GUIs are native and are very detached from the javascript noise; except if you're using a js GUI library like react-native. I will only target desktop GUI applications since they are easier and also my machine cannot meet android studio's hellish resource demands.

Along with learning about GUIs and interface development, I will also be learning about computer graphics; yes, the linear algebra, opengl, rendering and shading type of graphics. According to `Computer Graphics from scratch` by `Gabriel Gambetta`, the main topics I will cover on computer graphics will be raterization and ray tracers along other topics like light, shadows, textures, shading and linear algebra.

## Requirements

- Python
- PyQT5
- PutPixel -- `putPixel(x, y, color)`

To keep things simple, I will use python and pyqt; tkinter is a wrapper around tcl which I have never understood, pygame is just weird, you have to initialize the library and possibly some more constants, why? PyQT5 is just perfect since I have found some very promising book `Create GUI Applications with Python & Qt5` by `Martin Fitzpatrick`, also has good online documentation and I want to transfer this knowledge to my C++ applications.

The `PutPixel` requirement is more of a single function that can set the color of a pixel at the coordinate `(x, y)`. It is abstract and amazingly PyQT5 does provide a way to set a pixel's color at some coordinate on the canvas. It is the one requirement from `Gambetta's` book.

## Expectations

Now, computer graphics is a very big topic and I just wanna get some basics out. As we all know, I'm an aspiring backend developer; the guy who patches kernel bugs. But it has come to my attention that some algorithms from the frontend can really help me think about the backend, I mean, a little understanding about user interfaces and how creative thinking about it can help squeeze in new features like how python was designed. I will look into wrapping applications behind user interfaces, handle errors cleanly and most importantly, deal with missing resources like internet and caching to prevent users from killing my servers with DOS attacks from too many retries on their end. Hopefully, this will move me closer to understanding Operating Systems and Games.

## Conclusion

As I walk this path I hope to one day code the spinning doughnut animation that runs on the terminal coded in C; my main motivation to start this journey.
