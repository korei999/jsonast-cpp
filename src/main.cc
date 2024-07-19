#include "logs.hh"
#include "json/parser.hh"
#include "MapAllocator.hh"

int
main(int argCount, char* paArgs[])
{
    if (argCount < 2)
    {
        COUT("jsonast version: %f\n\n", JSONASTCPP_VERSION);
        COUT("usage: %s <path to json>\n", paArgs[0]);
        exit(3);
    }

    adt::Arena arena(adt::SIZE_1M * 200);

    json::Parser p(&arena);
    p.load(paArgs[1]);
    p.parse();

    if (argCount >= 3 && adt::String(paArgs[2]) == "-p")
        p.print();

    arena.freeArena();
}
