#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

static int lept_parse_equal_dest_string(lept_context* c, lept_value* v , const char* dest , size_t length , lept_type test_type) {
    EXPECT(c, dest[0]);
    dest++;
    for (int i = 1; i < length -1; ++i) {
        if(c->json[i] != dest[i])
            return  LEPT_PARSE_INVALID_VALUE;
    }
    c->json += length-1;
    v->type = test_type;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 'n':  return lept_parse_equal_dest_string(c,v,"null",4,LEPT_NULL);
        case 'f': return lept_parse_equal_dest_string(c,v,"false",5 ,LEPT_FALSE);
        case 't': return lept_parse_equal_dest_string(c,v,"true",4 , LEPT_TRUE);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    size_t result = lept_parse_value(&c, v);

    if (result == LEPT_PARSE_OK )
    {
        lept_parse_whitespace(&c);
        if (*c.json != '\0')
//            result = LEPT_PARSE_ROOT_NOT_SINGULAR;
        result =  LEPT_PARSE_ROOT_NOT_SINGULAR;
    }

    return result;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
