#ifndef RENDERER_H
#define RENDERER_H
#include "vec3.h"
#include <windows.h>
#include <algorithm>
using namespace std;

class Renderer {
    public:
        void initBuffer(int width, int height) {
            this->width = width;
            this->height = height;
            buffer.resize(width * height, Vec3(0.0, 0.0, 0.0)); // Initialize buffer with black color
        }
        void clearBuffer() {
            for(int i = 0; i < buffer.size(); i++){
                buffer[i] = Vec3(0,0,0);
            }
        }
        void setPixel(int x, int y, const Vec3& c){
            buffer[calcIndex(x, y)] = c;
        }
        void drawTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec3& color) {
            setPixel(v1.x(), v1.y(), color);
            setPixel(v2.x(), v2.y(), color);
            setPixel(v3.x(), v3.y(), color);

            int MinX = std::min({v1.x(), v2.x(), v3.x()});
            int MinY = std::min({v1.y(), v2.y(), v3.y()});
            int MaxX = std::max({v1.x(), v2.x(), v3.x()});
            int MaxY = std::max({v1.y(), v2.y(), v3.y()});
            
            for(int i = calcIndex(MinX, MinY); i < calcIndex(MaxX, MaxY); i++){
                int x = i % width;
                int y = i / width;
                Vec3 p (x, y, 0);
                if(computeBarycentric(p, v1, v2, v3)){
                    setPixel(x, y, color);
                }
            }

        }
        void present(HDC hdc) {
            BITMAPINFO bmi = {};

            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height; 
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 24; 
            bmi.bmiHeader.biCompression = BI_RGB;

            vector <BYTE> b(width * height * 3); 

            for(int i = 0; i < buffer.size(); i++){
                Vec3 pixel = buffer[i];
                BYTE r_byte = clamp(pixel.x(), 0, 1) * 255;
                BYTE g_byte = clamp(pixel.y(), 0, 1) * 255;
                BYTE b_byte = clamp(pixel.z(), 0, 1) * 255;

                int base = i * 3;
                b[base] = b_byte;
                b[base+1] = g_byte;
                b[base+2] = r_byte;
            }
            SetDIBitsToDevice(
                hdc,              // HDC from Win32
                0, 0,             // Destination x, y (top-left)
                width, height,    // Width and height of your image
                0, 0,             // Source x, y in the image
                0,                // Start scan line
                height,           // Number of scan lines
                b.data(),         // Pointer to your BGR byte array
                &bmi,             // Pointer to your BITMAPINFO
                DIB_RGB_COLORS    // Color usage
            );

        }

    private:
        int width;
        int height;
        vector<Vec3> buffer; // Buffer to hold pixel colors

        int calcIndex(int x, int y){
            return y * width + x; // Calculate index in the buffer
        }

        bool computeBarycentric(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c){
            Vec3 v0 = b-c;
            Vec3 v1 = c-a;
            Vec3 v2 = p-a;

            double d00 = dot(v0, v0);
            double d01 = dot(v0, v1);
            double d11 = dot(v1, v1);
            double d20 = dot(v2, v0);
            double d21 = dot(v2, v1);

            double denom = d00 * d11 - d01 * d01;
            double v = (d11 * d20 - d01 *d21)/denom;
            double w = (d00 * d21 - d01 * d20)/denom;
            double u = 1 - v - w;

            if(u >= 0 && u <= 1){
                if(v >= 0 && v <= 1){
                    if(w >= 0 && w <= 1){
                        return true;
                    }
                }
            }
            return false;
        }
        double clamp(double d, double min, double max) {
            const double t = d < min ? min : d;
            return t > max ? max : t;
        }
#endif
};
