#ifndef RENDERER_H
#define RENDERER_H
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include <windows.h>
#include <algorithm>
using namespace std;

class Renderer {
    public:
        vector<float> depthbuffer;
        bool wireframe = false; // Toggle for wireframe mode

        void initBuffer(int width, int height) {
            this->width = width;
            this->height = height;
            buffer.resize(width * height, Vec3(0.0, 0.0, 0.0)); // Initialize buffer with black color
            depthbuffer.resize(width * height, std::numeric_limits<float>::infinity()); // Initialize depthbuffer with infinity
        }
        void clearBuffer() {
            for(int i = 0; i < buffer.size(); i++){
                buffer[i] = Vec3(0,0,0);
            }
            std::fill(depthbuffer.begin(), depthbuffer.end(), std::numeric_limits<float>::infinity());
        }
        void setPixel(int x, int y, const Vec3& c, float depth = 0.0f){
            if(x >= 0 && x < width && y >= 0 && y < height){
                int idx = calcIndex(x, y);
                if(depth < depthbuffer[idx]){
                    buffer[idx] = c;
                    depthbuffer[idx] = depth;
                }
            }
        }
        void drawTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec3& color) {
            if (wireframe) {
                drawLine(v1, v2, color);
                drawLine(v2, v3, color);
                drawLine(v3, v1, color);
                return;
            }

            setPixel(v1.x(), v1.y(), color, v1.z());
            setPixel(v2.x(), v2.y(), color, v2.z());
            setPixel(v3.x(), v3.y(), color, v3.z());

            int MinX = std::min({v1.x(), v2.x(), v3.x()});
            int MinY = std::min({v1.y(), v2.y(), v3.y()});
            int MaxX = std::max({v1.x(), v2.x(), v3.x()});
            int MaxY = std::max({v1.y(), v2.y(), v3.y()});
            
            for(int y = MinY; y <= MaxY; y++){
                for (int x = MinX; x <= MaxX; x++) {
                    Vec3 p(x, y, 0);
                    if (computeBarycentric(p, v1, v2, v3)) {
                        // Interpolate depth using barycentric coordinates
                        float depth = interpolateDepth(p, v1, v2, v3);
                        setPixel(x, y, color, depth);
                    }
                }
            }
        }

        void drawCube(const mat4& transform, const Vec3& color){
            Vec3 vertices[8] = {
                Vec3(-1, -1, -1), Vec3(1, -1, -1),
                Vec3(1, 1, -1),   Vec3(-1, 1, -1),
                Vec3(-1, -1, 1),  Vec3(1, -1, 1),
                Vec3(1, 1, 1),    Vec3(-1, 1, 1)
            };

            int indices[36] = {
                4, 5, 6,  4, 6, 7,
                0, 2, 1,  0, 3, 2,
                1, 5, 6,  1, 6, 2,
                0, 4, 7,  0, 7, 3,
                3, 2, 6,  3, 6, 7,
                0, 1, 5,  0, 5, 4
            };

            for(int i =0; i < 36; i += 3){
                int i0 = indices[i];
                int i1 = indices[i + 1];
                int i2 = indices[i + 2];

                Vec3 v0 = vertices[i0];
                Vec3 v1 = vertices[i1];
                Vec3 v2 = vertices[i2];

                Vec4 v0_4(v0.x(), v0.y(), v0.z(), 1.0);
                Vec4 v1_4(v1.x(), v1.y(), v1.z(), 1.0);
                Vec4 v2_4(v2.x(), v2.y(), v2.z(), 1.0);

                Vec4 view_v0 = transform * v0_4;
                Vec4 view_v1 = transform * v1_4;
                Vec4 view_v2 = transform * v2_4;

                if (view_v0.w() != 0) {
                    view_v0.vec[0] /= view_v0.w();
                    view_v0.vec[1] /= view_v0.w();
                    view_v0.vec[2] /= view_v0.w();
                }

                if (view_v1.w() != 0) {
                    view_v1.vec[0] /= view_v1.w();
                    view_v1.vec[1] /= view_v1.w();
                    view_v1.vec[2] /= view_v1.w();
                }

                if (view_v2.w() != 0) {
                    view_v2.vec[0] /= view_v2.w();
                    view_v2.vec[1] /= view_v2.w();
                    view_v2.vec[2] /= view_v2.w();
                }

                auto ndcToScreenX = [&](double x) {
                    x = clamp(x, -1.0, 1.0);
                    return int((x + 1.0) * 0.5 * width);
                };

                auto ndcToScreenY = [&](double y) {
                    y = clamp(y, -1.0, 1.0);
                    return int((1.0 - (y + 1.0) * 0.5) * height);
                };

                Vec3 a1(ndcToScreenX(view_v0.x()), ndcToScreenY(view_v0.y()), view_v0.z());
                Vec3 b2(ndcToScreenX(view_v1.x()), ndcToScreenY(view_v1.y()), view_v1.z());
                Vec3 c3(ndcToScreenX(view_v2.x()), ndcToScreenY(view_v2.y()), view_v2.z());

                drawTriangle(a1, b2, c3, color);
            }
        }

        void drawLine(Vec3 p1, Vec3 p2, Vec3 color){
            double dx = abs(p1.x() - p2.x());
            double dy = abs(p1.y() - p2.y());
            double sx = 0;
            double sy = 0;
            if(p1.x() > p2.x()){
                sx = -1;
            }
            else{
                sx = +1;
            }
            if(p1.y() > p2.y()){
                sy = -1;
            }
            else{
                sy = +1;
            }

            double y = p1.y();
            double x = p1.x();
            if (dx > dy) {
                double slope = dy/dx;
                for(int i = p1.x(); i != p2.x(); i += sx){
                    setPixel(i, round(y), color);
                    y += sy*slope;
                }
            } 
            else {
                double slope = dx/dy;
                for(int i = p1.y(); i != p2.y(); i += sy){
                    setPixel(round(x), i, color);
                    x += sx*slope;
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

        bool computeBarycentric(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c) {
            double denom = ((b.y() - c.y()) * (a.x() - c.x()) + (c.x() - b.x()) * (a.y() - c.y()));
            double u = ((b.y() - c.y()) * (p.x() - c.x()) + (c.x() - b.x()) * (p.y() - c.y())) / denom;
            double v = ((c.y() - a.y()) * (p.x() - c.x()) + (a.x() - c.x()) * (p.y() - c.y())) / denom;
            double w = 1.0 - u - v;
            return (u >= 0) && (v >= 0) && (w >= 0);
        }
        double clamp(double d, double min, double max) {
            const double t = d < min ? min : d;
            return t > max ? max : t;
        }

        float interpolateDepth(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c) {
            Vec3 v0 = b - a;
            Vec3 v1 = c - a;
            Vec3 v2 = p - a;

            float d00 = dot(v0, v0);
            float d01 = dot(v0, v1);
            float d11 = dot(v1, v1);
            float d20 = dot(v2, v0);
            float d21 = dot(v2, v1);

            float denom = d00 * d11 - d01 * d01;
            float v = (d11 * d20 - d01 * d21) / denom;
            float w = (d00 * d21 - d01 * d20) / denom;
            float u = 1.0f - v - w;

            return a.z() * u + b.z() * v + c.z() * w;
        }
};
#endif
