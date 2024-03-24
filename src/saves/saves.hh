#pragma once
#include <glog/logging.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>

// I don't like declaring a namespace in header files, so I use defines for the same purpose
#define fs std::filesystem

#define SAVE_DIR "./data/saves/"
#define MAIN_SAVE "/save.xml"
#define WORLD_SAVE "/world.xml"

namespace Engine {
    namespace Saves {
        struct PlayerSaveState {
            int pos_x = -1;
            int pos_y = -1;
            bool smooth_cam;
            bool no_clip;
        };

        struct SaveState {
            PlayerSaveState player;
        };

        class Save {
           private:
            std::string path;

           public:
            Save(std::string path);
            void SaveState();
            void LoadState();
            void CheckState();
            bool ChechSave();
        };
    }  // namespace Saves
}  // namespace Engine
