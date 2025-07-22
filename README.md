# PRISM v1.0.0 Alpha

**Pixel Rendering & Imaging Software Module**

---

## Overview

PRISM is a lightweight, educational software renderer implemented in C++ using the Win32 API.  
Its core focus is on pixel-level manipulation and triangle rasterization using barycentric coordinates, implemented from the ground up without relying on GPU acceleration or external graphics libraries.

This alpha release demonstrates the foundational building blocks of a software renderer, including:

- A custom vector math class (`Vec3`) for 3D vector operations  
- A `Renderer` class managing a pixel buffer and rasterizing triangles  
- Basic rendering pipeline implemented on the CPU  
- Displaying rendered output directly to a Win32 window using GDI  

---

## Features

- **Triangle Rasterization:**  
  PRISM renders triangles by calculating barycentric coordinates to determine if pixels lie inside the triangle, enabling correct filling.

- **Pixel Buffer Management:**  
  Internal color buffer represented as a vector of `Vec3` colors with RGB components in the range `[0,1]`.

- **Color Clamping and Conversion:**  
  Colors are clamped and converted to 24-bit RGB format for display.

- **Win32 API Integration:**  
  PRISM creates a native Windows window and draws pixels using `SetDIBitsToDevice` for efficient pixel transfer.

- **Modular and Readable Codebase:**  
  Organized into separate classes with clear responsibilities (vector math, rendering, window management).

---

## Code Structure

### `Vec3` Class (`vec3.h`)

- Represents 3D vectors and RGB colors using a simple array of three doubles.
- Implements vector arithmetic: addition, subtraction, multiplication, division, dot and cross products, length, and unit vector calculation.
- Overloaded operators for intuitive math syntax.
- Safety checks for invalid values (like infinity).

### `Renderer` Class (`Renderer.h`)

- Manages a pixel buffer representing the screen pixels.
- Initializes buffer size and clears it.
- Provides `setPixel()` to color individual pixels.
- Implements `drawTriangle()` using barycentric coordinate tests to fill triangles.
- Converts internal color representation to byte format and draws directly to the Win32 device context (HDC) via `present()`.

### Main Application (`main.cpp`)

- Creates and registers a Win32 window class.
- Handles the Windows message loop.
- Instantiates the `Renderer`, sets up buffer size, clears it.
- Defines a single red triangle and renders it.
- Presents the rendered image to the window.
- Runs until the window is closed.

---

## How It Works

1. **Window Initialization:**  
   Using Win32 API, a window class is registered and a window is created with standard overlapped styles.

2. **Rendering Setup:**  
   The renderer initializes a color buffer sized to the window dimensions (800x600).

3. **Drawing Triangles:**  
   Using 2D coordinates (stored as `Vec3` with z=0), the renderer draws the vertices of the triangle, then scans the bounding box around the triangle.

4. **Barycentric Coordinate Check:**  
   For each pixel in the bounding box, the renderer computes barycentric coordinates to determine if the pixel lies inside the triangle. Pixels inside are colored.

5. **Presenting Pixels:**  
   The color buffer is converted from floating-point RGB (0.0 to 1.0) to 24-bit RGB byte format. Then it is blitted to the window using GDI’s `SetDIBitsToDevice()` function.

6. **Message Loop:**  
   The application enters the standard Windows message loop until the window is closed by the user.

---

## Usage

- Compile the project using a MinGW or compatible C++ compiler with Windows API and GDI libraries linked.
- Run the executable to see a window displaying a red triangle rendered purely by software on the CPU.
- Extend the renderer by adding more drawing primitives, shading, transformations, or event handling.

---

## Future Plans

- Add support for other primitives (lines, circles).
- Implement depth buffering and multiple triangles rendering.
- Add shading and lighting models.
- Create an animation loop for real-time rendering.
- Design a splash screen and window icon.
- Explore porting to other platforms or graphics APIs.

---

## Build Instructions (Example)

```powershell
g++ main.cpp Renderer.cpp Vec3.cpp -o prism.exe -lgdi32 -mwindows

## Acknowledgments

Inspired by classic software rasterizers and low-level graphics programming tutorials.
Designed and developed by Yash Joshi.

