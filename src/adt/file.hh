#pragma once

#include "String.hh"
#include "Array.hh"

namespace adt
{

inline String
loadFile(Allocator* pAlloc, String path)
{
    String ret;

    auto sn = makeString(pAlloc, path);

    FILE* pf = fopen(sn._pData, "rb");
    if (pf)
    {
        fseek(pf, 0, SEEK_END);
        long size = ftell(pf) + 1;
        rewind(pf);

        ret._pData = (char*)(pAlloc->alloc(size, sizeof(char)));
        ret._size = size - 1;
        fread(ret._pData, 1, ret._size, pf);

        fclose(pf);
    }

    return ret;
}

inline Array<u8>
loadFileToCharArray(Allocator* pAlloc, String path)
{
    Array<u8> ret(pAlloc);

    FILE* pf = fopen(path.data(), "rb");
    if (pf)
    {
        fseek(pf, 0, SEEK_END);
        long size = ftell(pf);
        rewind(pf);

        ret.resize(size + 1);
        ret._size = size;
        fread(ret._pData, 1, size, pf);

        fclose(pf);
    }

    return ret;
}

inline String
replacePathSuffix(Allocator* pAlloc, adt::String path, adt::String suffix)
{
    auto lastSlash = adt::findLastOf(path, '/') + 1;
    adt::String pathToMtl = {&path[0], lastSlash};
    auto r = adt::concat(pAlloc, pathToMtl, suffix);
    return r;
}

} /* namespace adt */
