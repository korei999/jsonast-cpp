#include "logs.hh"
#include "json/parser.hh"
#include "ArrayAllocator.hh"

int
main(int argCount, char* paArgs[])
{
    if (argCount < 2)
    {
        COUT("jsonast version: %f\n\n", JSONASTCPP_VERSION);
        COUT("usage: %s <path to json>\n", paArgs[0]);
        exit(3);
    }

    adt::ArrayAllocator alloc(adt::SIZE_8M);

    json::Parser p(&alloc);
    p.load(paArgs[1]);
    p.parse();

    if (argCount >= 3 && adt::String(paArgs[2]) == "-p")
        p.print();

#ifndef NDEBUG
    alloc.freeAll();
#endif
}
