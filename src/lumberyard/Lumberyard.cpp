//
// Created by yretenai on 5/31/2020.
//

#include "Lumberyard.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

#ifdef USE_NOESIS
const char* g_pPluginName = "lumberyard";
const char* g_pPluginDesc = "Lumberyard Asset Handler by yretenai";

using namespace dragon::lumberyard;

bool NPAPI_InitLocal() {
    open_dragon_log();
    write_dragon_log("lumberyard (fmt_dragon) v1.0.0\n");

    write_dragon_log("Initializing \"%s\" handler...\n", "Lumberyard Model");
    int handle =
        g_nfn->NPAPI_Register((char*)"Lumberyard Model", (char*)".cgf");
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Model::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Model::noesis_check);

    write_dragon_log("Initializing \"%s\" handler...\n", "Lumberyard Texture");
    handle = g_nfn->NPAPI_Register((char*)"Lumberyard Texture", (char*)".a;.1");
    g_nfn->NPAPI_SetTypeHandler_LoadRGBA(handle, Texture::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Texture::noesis_check);

    write_dragon_log("Initializing \"%s\" handler...\n",
                     "Cook Lumberyard Texture tool");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Cook Lumberyard Texture",
                                       Texture::noesis_tool, nullptr);
    g_nfn->NPAPI_SetToolHelpText(
        handle, (char*)"Merges streamed Lumberyard textures into one DDS");
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_CONTEXTITEM);
    g_nfn->NPAPI_SetToolVisibleCallback(handle,
                                        Texture::noesis_tool_visibility);

    write_dragon_log("Initialized handlers!\n");
    flush_dragon_log();
    return true;
}

void NPAPI_ShutdownLocal() {
    flush_dragon_log();
    close_dragon_log();
}

#endif // USE_NOESIS

#pragma clang diagnostic pop
