#pragma once
#ifdef PVZEMU_BUILD_DEBUGGER
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#endif // PVZEMU_BUILD_DEBUGGER

namespace pvz_emulator::object {

struct rect {
    int x;
    int y;
    int width;
    int height;

    bool is_overlap_with_circle(int px, int py, int r);

    float get_overlap_len(const rect& r);

#ifdef PVZEMU_BUILD_DEBUGGER
    void to_json(rapidjson::Writer<rapidjson::StringBuffer>& writer);
#endif // PVZEMU_BUILD_DEBUGGER
};

enum class reanim_type {
    repeat = 0,
    once = 1
};

struct reanim_frame_status {
    float frame_progress;
    unsigned int frame;
    unsigned int next_frame;
};

struct reanim {
    float fps;
    float prev_fps;
    unsigned int begin_frame;
    unsigned int n_frames;
    unsigned int n_repeated;
    float progress;
    float prev_progress;
    reanim_type type;

    bool is_in_progress(double progress);

    void get_frame_status(reanim_frame_status& rfs) const;

#ifdef PVZEMU_BUILD_DEBUGGER
    void to_json(rapidjson::Writer<rapidjson::StringBuffer>& writer);
#endif // PVZEMU_BUILD_DEBUGGER
};

}