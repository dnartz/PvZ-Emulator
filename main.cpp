#include "httplib.h"
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <cstdlib>
#include "rapidjson/reader.h"

#include "world.h"

static std::string load_file(const std::string& path) {
    std::string s;
    std::ifstream fin(path);
    if (fin.is_open()) {
        std::stringstream ss;
        ss << fin.rdbuf();
        s = ss.str();
        fin.close();
    } else {
        std::cout << "Unable to load index.html" << std::endl;
        exit(1);
    }

    return s;
}

class config_handler : public rapidjson::BaseReaderHandler<
    rapidjson::UTF8<>,
    config_handler
> {
private:
    enum class field {
        none,
        scene,
        stop_spawn,
        is_zombie_dance,
        is_future_enabled
    } curr_field;

public:
    bool is_started;
    pvz_emulator::object::scene_type type;
    bool is_zombie_dance;
    bool is_future_enabled;
    bool stop_spawn;

    config_handler(const pvz_emulator::object::scene &s) :
        is_started(false),
        curr_field(field::none),
        type(s.type),
        is_zombie_dance(s.is_zombie_dance),
        is_future_enabled(s.is_future_enabled),
        stop_spawn(s.stop_spawn) {}

    bool StartObject() {
        if (is_started) {
            return false;
        } else {
            is_started = true;
            return true;
        }
    }

    bool Key(const char* key, rapidjson::SizeType length, bool copy) {
        if (strcmp(key, "scene") == 0) {
            curr_field = field::scene;
            return true;
        } else if (strcmp(key, "is_zombie_dance") == 0) {
            curr_field = field::is_zombie_dance;
            return true;
        } else if (strcmp(key, "is_future_enabled") == 0) {
            curr_field = field::is_future_enabled;
            return true;
        } else if (strcmp(key, "stop_spawn") == 0) {
            curr_field = field::stop_spawn;
            return true;
        } else {
            return false;
        }
    }

    bool String(const Ch* str, rapidjson::SizeType length, bool copy) {
        if (curr_field == field::scene) {
            if (strcmp(str, "day") == 0) {
                type = pvz_emulator::object::scene_type::day;
                return true;
            }
            if (strcmp(str, "night") == 0) {
                type = pvz_emulator::object::scene_type::night;
                return true;
            }
            if (strcmp(str, "pool") == 0) {
                type = pvz_emulator::object::scene_type::pool;
                return true;
            }
            if (strcmp(str, "fog") == 0) {
                type = pvz_emulator::object::scene_type::fog;
                return true;
            }
            if (strcmp(str, "roof") == 0) {
                type = pvz_emulator::object::scene_type::roof;
                return true;
            }
            if (strcmp(str, "moon_night") == 0) {
                type = pvz_emulator::object::scene_type::moon_night;
                return true;
            }
        }

        return false;
    }

    bool Bool(bool b) {
        switch (curr_field) {
        case field::is_zombie_dance:
            is_zombie_dance = b;
            return true;
        case field::is_future_enabled:
            is_future_enabled = b;
            return true;
        case field::stop_spawn:
            stop_spawn = b;
            return true;
        default:
            return false;
        }
    }

    bool EndObject(rapidjson::SizeType memberCount) {
        return true;
    }
};

static void start_server(char** argv, pvz_emulator::world& w) {
    using namespace httplib;

    Server svr;

    svr.Get("/(index.html)?", [&argv](const Request& req, Response& res) {
        static const auto index = load_file(std::string(argv[1]) + "/index.html");
        res.set_content(index.c_str(), "text/html");
    });

    svr.Get("/index.js.map", [&argv](const Request& req, Response& res) {
        static const auto map = load_file(std::string(argv[1]) + "/index.js.map");
        res.set_content(map.c_str(), "application/json");
    });

    svr.Get("/status.json", [&w](const Request& req, Response& res) {
        std::string s;
        w.to_json(s);

        res.set_content(s.c_str(), "application/json");
    });

    svr.Post("/status.json", [&w](const Request& req, Response& res) {
        w.update();

        std::string s;
        w.to_json(s);

        res.set_content(s.c_str(), "application/json");
    });

    svr.Post("/config.json", [&w](const Request& req, Response& res) {
        config_handler handler(w.scene);
        rapidjson::Reader reader;
        rapidjson::StringStream ss(req.body.c_str());

        if (reader.Parse(ss, handler).IsError()) {
            res.status = 400;
        }
        else {
            w.scene.is_future_enabled = handler.is_future_enabled;
            w.scene.is_zombie_dance = handler.is_zombie_dance;
            w.scene.stop_spawn = handler.stop_spawn;
            w.scene.type = handler.type;

            std::string s;
            w.to_json(s);
            res.set_content(s.c_str(), "application/json");
        }
    });

    svr.listen("localhost", 3000);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: PvZ-Emulator path to index.html" << std::endl;
        exit(0);
    }

    using namespace pvz_emulator::object;

    // Initialize the world instance and create some objects to test the emulator.
    pvz_emulator::world w(pvz_emulator::object::scene_type::pool);

    start_server(argv, w);

    return 0;
}
