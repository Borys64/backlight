#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

const std::string base_path = "/sys/class/backlight/";

// CLI options
struct options {
    int level;
    bool list;
    int device = -1;
};

class display {
    public:
        int id;
        std::string path;
        display(int _id, std::string _path) {
            id = _id;
            path = _path;
        };
};

struct options parse_args(int argc, char* argv[]) {
    options options;

    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];

        // help/usage
        if(argc < 2 || arg == "-h" || arg == "-help") {
            std::cout << "backlight is a simple display backlight utility\n"
                    << "usage: backlight <-l[evel> <level>]\n"
                    << "<-l[evel]> <level>  brightness level as a percentage\n"
                    << "[-d[evices]] list all available display devices (by default all devices are adjusted)\n"
                    << "[-D[evice] <id>] set a specific display brightness\n";

            exit(1);
        };
        
        // level
        if (arg == "-l" || arg == "-level") {
            // check presence of level argument
            if(!argv[i + 1]) {
                std::cerr << "error: no level provided\n";
                exit(1);
            } else {
                // parse level
                options.level = atoi(argv[i + 1]);
            };
        };

        // device id list
        options.list = (arg == "-d" || arg == "-devices");

        // set specific id
        if(arg == "-D" || arg == "-Device") {
            if(!argv[i + 1]) {
                std::cerr << "error: no device ID provided, use -d to list all available displays" << std::endl;
                exit(1);
            } else {
                // parse device ID
                options.device = atoi(argv[i + 1]);
            };
        };
    };  
    
    return options;
}

// simplify reading display parameters
long int read_file_int(std::string path) {
    std::string val;
    std::ifstream file(path);

    if(file.is_open() == 0) return 1;
    else std::getline(file, val);

    if(val.empty()) 
        return -1;
    
    return std::stoi(val);
}

int main(int argc, char* argv[]) {
    int last_id = -1; // keep track of displays

    // check if argc sufficient
    if(argc < 2) {
        std::cerr << "usage: backlight <-l[evel]> <level>\n"
                << "use -h for more options\n";
        return 1;
    };

    // parse arguments into options struct
    options opts = parse_args(argc, argv);

    // check if level provided and check range 
    if(opts.device != -1) {
        if(!opts.level) {
            std::cerr << "error: no brightness level provided\n";
            exit(1);
        } else if(opts.level < 1 || opts.level > 100) {
            std::cerr << "error: invalid level range (1 - 100)\n";
            exit(1);
        };
    };

    // index all sysfs backlight interfaces
    std::vector <display> displays; // index of interfaces in /sys/class/backlight
    DIR *sysfs = opendir(base_path.c_str()); // sysfs directory
    struct dirent *sysfs_ent; // sysfs directory entry
    while((sysfs_ent = readdir(sysfs)) != NULL) {
        std::string path(sysfs_ent->d_name);
        if(path == "." || path == "..") continue; // skip links
        else last_id++;
        
        display d(last_id, path);
        displays.push_back(d);
    };

    for (display disp : displays) {
        // list displays 
        if(opts.list == true) {
            std::cout << disp.id << ": " << disp.path << std::endl;
            continue;
        };

        // specific device
        if(opts.device != -1 && disp.id != opts.device) continue; // skip if not selected ID

        // get display capabilities and current brightness
        int max = read_file_int(base_path + disp.path + "/max_brightness");
        
        // ensure parameters are in tact
        if(!max) {
            std::cerr << "error: could not determine max brightness of device: " << disp.path << std::endl;
            return 1;
        };

        // calculate desired screen brightness as a percentage of display maximum
        double des = max * (double)opts.level / 100;
        std::string set = std::to_string((int)des);
        
        // set brightness using the sysfs backlight interface
        std::ofstream brightness(base_path + disp.path + "/brightness");

        if(brightness.is_open()) {
            brightness << set << std::endl;
            brightness.close();
        } else if(brightness.fail()) {
            std::cerr << "error: failed to write to " << disp.path << std::endl;
            return 1;
        };
    };

    return 0;
}