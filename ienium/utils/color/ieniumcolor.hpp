#pragma once

#include <algorithm>

namespace ienium::utils
{
    class Color
    {
        private:
        const float rValue;
        const float gValue;
        const float bValue;
        const float aValue;

        public:
        Color (const float _r, const float _g, const float _b, const float _a = 1)
            :   rValue(std::clamp<float>(_r, 0.f, 1.f)),
                gValue(std::clamp<float>(_g, 0.f, 1.f)),
                bValue(std::clamp<float>(_b, 0.f, 1.f)),
                aValue(std::clamp<float>(_a, 0.f, 1.f)) {}

        Color (const int _R, const int _G, const int _B, const int _A = 255)
            :   rValue(std::clamp<int>(_R, 0, 255) / 255.f),
                gValue(std::clamp<int>(_G, 0, 255) / 255.f),
                bValue(std::clamp<int>(_B, 0, 255) / 255.f),
                aValue(std::clamp<int>(_A, 0, 255) / 255.f) {}


        inline unsigned char R () const { return rValue * 255;  }
        inline unsigned char G () const { return gValue * 255; }
        inline unsigned char B () const { return bValue * 255; }
        inline unsigned char A () const { return aValue * 255; }

        inline float r () const { return rValue; }
        inline float g () const { return gValue; }
        inline float b () const { return bValue; }
        inline float a () const { return aValue; }
    
        // Basic colors
        static inline Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
        static inline Color White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
        
        // Grey tones
        static inline Color DarkGrey()      { return Color (0.25f,   0.25f,     0.25f,   1.0f); }
        static inline Color Grey()          { return Color (0.5f,    0.5f,      0.5f,    1.0f); }
        static inline Color LightGrey()     { return Color (0.75f,   0.75f,     0.75f,   1.0f); }
        static inline Color Silver()        { return Color (0.753f,  0.753f,    0.753f,  1.0f); }
        
        // Basic colors
        static inline Color Red()           { return Color (1.0f,    0.0f,      0.0f,    1.0f); }
        static inline Color Green()         { return Color (0.0f,    1.0f,      0.0f,    1.0f); }
        static inline Color Blue()          { return Color (0.0f,    0.0f,      1.0f,    1.0f); }
        
        // Secondary colors
        static inline Color Yellow()        { return Color (1.0f,    1.0f,      0.0f,    1.0f); }
        static inline Color Cyan()          { return Color (0.0f,    1.0f,      1.0f,    1.0f); }
        static inline Color Magenta()       { return Color (1.0f,    0.0f,      1.0f,    1.0f); }
        
        // Warm colors
        static inline Color Orange()        { return Color (1.0f,    0.647f,    0.0f,    1.0f); }
        static inline Color Coral()         { return Color (1.0f,    0.498f,    0.314f,  1.0f); }
        static inline Color Crimson()       { return Color (0.863f,  0.078f,    0.235f,  1.0f); }
        static inline Color Pink()          { return Color (1.0f,    0.753f,    0.796f,  1.0f); }
        static inline Color Gold()          { return Color (1.0f,    0.843f,    0.0f,    1.0f); }
        
        // Cold colors
        static inline Color Navy()          { return Color (0.0f,    0.0f,      0.502f,  1.0f); }
        static inline Color Teal()          { return Color (0.0f,    0.502f,    0.502f,  1.0f); }
        static inline Color SkyBlue()       { return Color (0.529f,  0.808f,    0.922f,  1.0f); }
        static inline Color Indigo()        { return Color (0.294f,  0.0f,      0.510f,  1.0f); }
        
        // Nature colors
        static inline Color ForestGreen()   { return Color (0.133f,  0.545f,    0.133f,  1.0f); }
        static inline Color Olive()         { return Color (0.502f,  0.502f,    0.0f,    1.0f); }
        static inline Color Maroon()        { return Color (0.502f,  0.0f,      0.0f,    1.0f); }
        static inline Color Purple()        { return Color (0.502f,  0.0f,      0.502f,  1.0f); }
        
        // Utility colors
        static inline Color Transparent()   { return Color (0.0f,    0.0f,      0.0f,    0.0f); }
    };
}