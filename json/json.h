
#define JSON_INVALID (0)
#define JSON_TRUE    (1 << 0)
#define JSON_FALSE   (1 << 1)
#define JSON_NULL    (1 << 2)
#define JSON_NUMBER  (1 << 3)
#define JSON_STRING  (1 << 4)
#define JSON_ARRAY   (1 << 5)
#define JSON_OBJECT  (1 << 6)

typedef struct JSON {
    struct JSON *next;
    struct JSON *prev;
    struct JSON *child; 

    int type;
    char *name;

    double value_number;
    char *value_string;
} JSON;

JSON *json_parse(const char *raw_json);