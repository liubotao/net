#include <string>
#include <vector>
#include <map>

class Config {
public:
    Config();
    ~Config();

    bool initConfig(const char* path);
    void setContent(const std::string &content);
    bool readItem(const char *section, const char *key, char *value, size_t size, char *defaultValue);
    bool readItem(const char *section, const char *key, int *value, const int defaultValue);
    bool readItem(const char *section, const char *key, char *value, size_t size);
    bool readItem(const char *section, const char *key, int *value);

//    bool getSection(const char *name, std::vector<std::string> *section);

private:
    std::map<std::string, std::map<std::string, std::string> > config_;
    std::string content_;
};
