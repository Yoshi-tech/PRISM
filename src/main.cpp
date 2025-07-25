#include "vec3.h"
#include "vec4.h"
#include "Ray.h"
#include "Renderer.h"
#include "camera.h"
#include <windows.h>
#include <cmath>
#include <chrono>
#include <thread>


// Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // Optional: handle more messages here

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Entry point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    // Register the window class
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    // Create the window
    HWND hwnd = CreateWindowExW(
        0,                          // Optional window styles
        CLASS_NAME,                 // Window class name
        L"PRISM v1.0.0",         // Window title
        WS_OVERLAPPEDWINDOW,        // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    Renderer r;
    camera cam;
    Vec3 model_a(100, 100, 0);
    Vec3 model_b(300, 200, 0);
    Vec3 model_c(150, 400, 0);

    Vec3 color(1, 0, 0); 


    // Run the message loop
    MSG msg = {};
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) goto exit;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
            cam.azimuth += 0.01;
            cam.updatePosition();

            r.initBuffer(800, 600);
            r.clearBuffer();
            mat4 lookAt = cam.lookAt(cam.position, cam.target, cam.up);
            Vec4 a_4 = Vec4(model_a.x(), model_a.y(), model_a.z(), 1.0);
            Vec4 b_4 = Vec4(model_b.x(), model_b.y(), model_b.z(), 1.0);
            Vec4 c_4 = Vec4(model_c.x(), model_c.y(), model_c.z(), 1.0);


            Vec4 view_a = lookAt * a_4;
            Vec4 view_b = lookAt * b_4;
            Vec4 view_c = lookAt * c_4;

            Vec3 a = Vec3(view_a.x(), view_a.y(), view_a.z());
            Vec3 b = Vec3(view_b.x(), view_b.y(), view_b.z());
            Vec3 c = Vec3(view_c.x(), view_c.y(), view_c.z()); // Note: This should probably use view_c, but your code overwrites view_a

            r.drawTriangle(a, b, c, color);

            HDC hdc = GetDC(hwnd);

            r.present(hdc);
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // cap to ~60 FPS
        

    }

    exit:
    return 0;
}
