#include "saves.hh"

namespace Engine {
    namespace Saves {
        Save::Save(std::string path) { this->path = path; }

        void Save::SaveState() {
            if (!fs::exists(SAVE_DIR + this->path)) {
                LOG(INFO) << "Creating save directory " << SAVE_DIR + this->path;
                fs::create_directory(SAVE_DIR + this->path);
            } else {
                LOG(INFO) << "Save directory " << SAVE_DIR + this->path << " already exists";
            }
        }

        void Save::LoadState() {}

        void Save::CheckState() {}

        bool Save::ChechSave() {
            bool valid = true;

            if (!fs::exists(SAVE_DIR + this->path) || !fs::is_directory(SAVE_DIR + this->path)) {
                valid = false;
                LOG(WARNING) << "Save directory " << SAVE_DIR + this->path << " does not exist or is not a directory";
            }

            std::string save_files[] = {MAIN_SAVE, WORLD_SAVE};
            for (std::string save_file : save_files) {
                if (!fs::exists(SAVE_DIR + save_file) || !fs::is_regular_file(SAVE_DIR + save_file)) {
                    valid = false;
                    LOG(WARNING) << "Save file " << save_file << " does not exist or is not a regular file";
                }
            }

            if (valid) {
                LOG(INFO) << "Save " << this->path << " is valid";
            }

            return valid;
        }
    }  // namespace Saves
}  // namespace Engine
