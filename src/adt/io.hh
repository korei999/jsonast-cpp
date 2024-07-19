#pragma once

#include "String.hh"

namespace adt
{

String
loadFile(BaseAllocator* pAlloc, String path)
{
    String ret;

    FILE* pf = fopen(path.data(), "rb");
    if (pf)
    {
        fseek(pf, 0, SEEK_END);
        size_t size = ftell(pf) + 1;
        rewind(pf);

        ret.pData = static_cast<char*>(pAlloc->alloc(size, sizeof(char)));
        ret.size = size - 1;
        fread(ret.pData, 1, ret.size, pf);

        fclose(pf);
    }

    return ret;
}

} /* namespace adt */
