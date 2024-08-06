#include "logs.hh"
#include "json/parser.hh"
#include "ArenaAllocator.hh"

int
main(int argCount, char* paArgs[])
{
    adt::ArenaAllocator alloc(adt::SIZE_1M * 50);

    if (argCount < 1)
    {
        COUT("jsonast version: %f\n\n", JSONASTCPP_VERSION);
        COUT("usage: %s <path to json> [-p(print)|-e(json creation example)]\n", paArgs[0]);
        exit(3);
    }

    if (argCount >= 2 && adt::String(paArgs[1]) == "-e")
    {
        json::Object oHead = json::putObject({}, &alloc);
        json::pushToObject(&oHead, json::putLong("n0", 0));
        json::pushToObject(&oHead, json::putLong("n1", 1));
        json::pushToObject(&oHead, json::putLong("n2", 2));
        json::pushToObject(&oHead, json::putString("what", "what"));
        auto* pNewObj = json::pushToObject(&oHead, json::putObject("newObj", &alloc));
        json::pushToObject(pNewObj, json::putBool("b0", true));
        json::pushToObject(pNewObj, json::putBool("b1", false));
        json::pushToObject(pNewObj, json::putNull("null"));
        auto* pArr0 = json::pushToObject(&oHead, json::putArray("arr0", &alloc));
        json::pushToArray(pArr0, json::putDouble({}, 0.1));
        json::pushToArray(pArr0, json::putDouble({}, 2.2));
        json::pushToArray(pArr0, json::putDouble({}, 3.3));
        json::pushToArray(pArr0, json::putDouble({}, -4.4));
        json::pushToArray(pArr0, json::putDouble({}, -5.5));

        json::printNode(&oHead, {}, 0);
        COUT("\n");
    }

    if (argCount >= 3 && adt::String(paArgs[2]) == "-p")
    {
        json::Parser p(&alloc);
        p.load(paArgs[1]);
        p.parse();
        p.print();
    }

    alloc.freeAll();
}
