//
// Created by yretenai on 5/28/2020.
//

#include "Model.h"

using namespace dragon::lumberyard::chunk::model;

#define CAST_MODEL_CHUNK(chunk)                                                \
    std::shared_ptr<AbstractModelChunk>(                                       \
        dynamic_cast<AbstractModelChunk*>(chunk))

namespace dragon::lumberyard {
    Model::Model(Array<char>* buffer) {
        super_assert_dragon_log(
            buffer->size() >= sizeof(CRCH_HEADER),
            "Assertion failed -> length > sizeof CrChHeader\n");
        Header = buffer->cast<CRCH_HEADER>(0);
        if (Header.ChunkCount == 0)
            return;
        super_assert_dragon_log(
            buffer->size() >= Header.ChunkTablePointer +
                                  sizeof(CRCH_CHUNK_HEADER) * Header.ChunkCount,
            "Assertion failed -> length > chunks * sizeof CrChChunkHeader\n");
        ChunkTable = buffer->cast<CRCH_CHUNK_HEADER>(Header.ChunkTablePointer,
                                                     Header.ChunkCount);
        Chunks = std::map<uint32_t, std::shared_ptr<AbstractModelChunk>>();
        for (CRCH_CHUNK_HEADER chunk_header : ChunkTable) {
            super_assert_dragon_log(
                buffer->size() >= chunk_header.Pointer + chunk_header.Size,
                "Assertion failed -> length > chunk pointer + chunk size\n");
            Array<char> chunk_buffer =
                buffer->slice(chunk_header.Pointer, chunk_header.Size);
            switch (chunk_header.Type) {
            case CRCH_CHUNK_HEADER::ModelChunkMesh:
                write_dragon_log("Found ModelChunkMesh\n");
                break;
            case CRCH_CHUNK_HEADER::ModelChunkMaterialList:
                write_dragon_log("Found ModelChunkMaterialList\n");
                break;
            case CRCH_CHUNK_HEADER::ModelChunkNode:
                write_dragon_log("Found ModelChunkNode\n");
                break;
            case CRCH_CHUNK_HEADER::ModelChunkMaterial:
                write_dragon_log("Found ModelChunkMaterial\n");
                break;
            case CRCH_CHUNK_HEADER::ModelChunkMeta:
                write_dragon_log("Found ModelChunkMeta\n");
                break;
            case CRCH_CHUNK_HEADER::ModelChunkMaterialName:
                write_dragon_log("Found ModelChunkMaterialName\n");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(
                    new MaterialName(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::ModelChunkFlags:
                write_dragon_log("Found ModelChunkFlags\n");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(
                    new ExportFlags(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::ModelChunkData:
                write_dragon_log("Found ModelChunkData\n");
                break;
            case CRCH_CHUNK_HEADER::ModelChunkSubmesh:
                write_dragon_log("Found ModelChunkSubmesh\n");
                Chunks[chunk_header.Id] =
                    CAST_MODEL_CHUNK(new Submesh(&chunk_buffer, chunk_header));
                break;
            default:
                write_dragon_log("%s (%X)\n", "Unhandled model chunk!",
                                 chunk_header.Type);
                break;
            }
            flush_dragon_log();
        }
    }

    bool Model::check(Array<char>* buffer) {
        uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer->data());
        return buffer->size() >= sizeof(CRCH_HEADER) &&
               pointer[0] == FOURCC_CRCH && pointer[1] == 0x746;
    }

    bool Model::get_chunk_header(uint32_t id, CRCH_CHUNK_HEADER& chunk) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (ChunkTable[i].Id == id) {
                chunk = ChunkTable[i];
                return true;
            }
        }
        return false;
    }

#ifdef USE_NOESIS

    noesisModel_t* Model::noesis_load(BYTE* buffer, int length, int& num_mdl,
                                      noeRAPI_t* rapi) {
        Array<char> data_buffer =
            Array<char>(reinterpret_cast<char*>(buffer), length);
        Model model(&data_buffer);
        return nullptr;
    }

    bool Model::noesis_check(BYTE* buffer, int length,
                             [[maybe_unused]] noeRAPI_t* rapi) {
        Array<char> data_buffer =
            Array<char>(reinterpret_cast<char*>(buffer), length);
        return Model::check(&data_buffer);
    }

#endif // USE_NOESIS
} // namespace dragon::lumberyard
