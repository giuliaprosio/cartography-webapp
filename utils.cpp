#include <cstdlib>

char *getenvs(const char *name, const char *def)
{
    char *value = getenv(name);
    if (value == NULL) {
        return (char *) def;
    }
    return value;
}

int getenvi(const char *name, int def)
{
    char *value = getenv(name);
    if (value == NULL) {
        return def;
    }
    return atoi(value);
}
