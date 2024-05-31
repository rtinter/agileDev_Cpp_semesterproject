#ifndef ATHENA_COLORS_H
#define ATHENA_COLORS_H

#include "imgui.h"

namespace commons {
/***********************************************
 * Colors provides colors as ImVec4 objects.
 * These colors are used by ColorTheme
 * (but can be accessed directly, too).
 **********************************************/
    class Colors {
    public:
        Colors() = delete; // static class

        static ImVec4 const NONE;
        static ImVec4 const LIGHT_GRAY;
        static ImVec4 const DARK_GRAY;
        static ImVec4 const ORANGE;
        static ImVec4 const kBLUE;
        static ImVec4 const RED;
        static ImVec4 const GREEN;
        static ImVec4 const INDIGO;
        static ImVec4 const SEAFOAM;
    };
}
// commons

#endif //ATHENA_COLORS_H
