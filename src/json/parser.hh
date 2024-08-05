#pragma once

#include "lex.hh"
#include "ast.hh"

namespace json
{

struct Parser
{
    adt::Allocator* _pArena;
    adt::String _sName;
    Object* _pHead;

    Parser(adt::Allocator* p) : _pArena(p), _l(p) {}

    void load(adt::String path);
    void parse();
    void print();
    static void printNode(Object* pNode, adt::String svEnd, int depth);
    Object* getHeadObj() { return _pHead; }
    void traverse(Object* pNode, bool (*pfn)(Object* p, void* a), void* args);
    void traverse(bool (*pfn)(Object* p, void* a), void* args) { traverse(_pHead, pfn, args); }

private:
    Lexer _l;
    Token _tCurr;
    Token _tNext;

    void expect(enum Token::TYPE t, adt::String svFile, int line);
    void next();
    void parseNode(Object* pNode);
    void parseIdent(TagVal* pTV);
    void parseNumber(TagVal* pTV);
    void parseObject(Object* pNode);
    void parseArray(Object* pNode); /* arrays are same as objects */
    void parseNull(TagVal* pTV);
    void parseBool(TagVal* pTV);
};

/* Linear search inside JSON object. Returns nullptr if not found */
inline Object*
searchObject(adt::Array<Object>& aObj, adt::String svKey)
{
    for (u32 i = 0; i < aObj._size; i++)
        if (aObj[i].svKey == svKey)
            return &aObj[i];

    return nullptr;
}

inline adt::Array<Object>&
getObject(Object* obj)
{
    return obj->tagVal.val.o;
}

inline adt::Array<Object>&
getArray(Object* obj)
{
    return obj->tagVal.val.a;
}

inline long
getLong(Object* obj)
{
    return obj->tagVal.val.l;
}

inline double
getDouble(Object* obj)
{
    return obj->tagVal.val.d;
}

inline adt::String
getString(Object* obj)
{
    return obj->tagVal.val.sv;
}

inline bool
getBool(Object* obj)
{
    return obj->tagVal.val.b;
}

inline Object
putObject(adt::String key, adt::Allocator* pAlloc)
{
    return {
        .svKey = key,
        .tagVal {.tag = TAG::OBJECT, .val {.o {pAlloc}}}
    };
}

inline Object
putArray(adt::String key, adt::Allocator* pAlloc)
{
    return {
        .svKey = key,
        .tagVal {.tag = TAG::ARRAY, .val {.a {pAlloc}}}
    };
}

inline Object
putLong(adt::String key, long l)
{
    return {
        .svKey = key,
        .tagVal {.tag = TAG::LONG, .val {.l = l}}
    };
}

inline Object
putDouble(adt::String key, double d)
{
    return {
        .svKey = key,
        .tagVal {.tag = TAG::DOUBLE, .val {.d = d}}
    };
}

inline Object
putString(adt::String key, adt::String s)
{
    return {
        .svKey = key,
        .tagVal {.tag = TAG::STRING, .val {.sv = s}}
    };
}

inline Object
putBool(adt::String key, bool b)
{
    return {
        .svKey = key,
        .tagVal {.tag = TAG::BOOL, .val {.b = b}}
    };
}

inline Object
putNull(adt::String key)
{
    return {
        .svKey = key,
        .tagVal {.tag = TAG::NULL_, .val {.n = nullptr}}
    };
}

inline Object*
pushToObject(Object* pObj, Object o)
{
    return pObj->tagVal.val.o.push(o);
}

inline Object*
pushToArray(Object* pObj, Object o)
{
    return pObj->tagVal.val.a.push(o);
}

} /* namespace json */
