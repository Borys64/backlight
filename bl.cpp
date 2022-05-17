#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// simplify reading display parameters
long int read_file_int(std::string path) {
    std::string line;
    std::string content;

    std::ifstream file(path);
    if(file.is_open()) {
        while(std::getline(file, line)) {
            content += line;
        };
    };

    long int num = atoi(content.c_str());
    return num;
};

int main(int argc, char* argv[]) {

    // check if user is root for /sys file i/o
    if(getuid()) 
    {
        std::cout << "this program needs to be run as root\n";
        return 1;
    };


    // check args
    if(argc < 2) 
    {
        std::cout << "usage: bl <level> - level: integer between 1 - 100\n"; 
        return 1;
    } else if(!atoi(argv[1])) {
        std::cout << "level not an integer\n";
        return 1;
    };

    // level <= 100
    int level = atoi(argv[1]);
    if(!level || level > 100)
    {
        std::cout << "invalid brightness level (between 1 - 100)\n";
        return 1;
    };

    // get display capabilities and current brightness
    long int max = read_file_int("/sys/class/backlight/intel_backlight/max_brightness");
    long int cur = read_file_int("/sys/class/backlight/intel_backlight/brightness");
    

    // ensure parameters are in tact
    if(!max) {
        std::cout << "Could not determine max brightness";
        return 1;
    };

    // calculate desired screen brightness as a percentage of display maximum
    double des = max * (double)level / 100;
    std::string set = std::to_string((int)des);

    // determine firmware
    struct stat info;
    std::string firmware = "intel_backlight";
    if(stat("/sys/class/backlight/acpi_video0", &info) == 0) {
        firmware = "acpi_video0";
    } else {
        std::cout << "error: unsupported interface" << std::endl;
        return 1;
    }
    
    // set brightness using the sysfs backlight interface
    std::string path = "/sys/class/backlight/" + firmware + "/brightness";
    std::ofstream brightness(path.c_str(), std::ios::out);
    while(brightness.is_open()) {
        brightness << set << std::endl;
        brightness.close();
    };

    // confirmation
    std::cout << "brightness set 100%" << std::endl;
    return 0;
};