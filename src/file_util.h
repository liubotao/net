#include <string>

class FileUtils {

public:
    static bool readFile(const char *path, std::string *content);

private:
    FileUtils();
    ~FileUtils();
};
