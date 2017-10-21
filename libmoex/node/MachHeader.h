//
//  Created by everettjf
//  Copyright © 2017 everettjf. All rights reserved.
//
#ifndef MACHHEADER_H
#define MACHHEADER_H

#include "Node.h"
#include "LoadCommand.h"
#include "Magic.h"
#include "MachSection.h"


MOEX_NAMESPACE_BEGIN

class LoadCommand_LC_SYMTAB;

class MachHeaderInternal : public NodeData<mach_header>{
public:
    void Init(void *offset,NodeContextPtr&ctx) override {
        NodeData::Init(offset,ctx);
        if(data_.magic == MH_CIGAM) {
            swap_mach_header(&data_, NX_LittleEndian);
        }
    }
};
using MachHeaderInternalPtr = std::shared_ptr<MachHeaderInternal>;

class MachHeader64Internal : public NodeData<mach_header_64>{
public:
    void Init(void *offset,NodeContextPtr&ctx) override {
        NodeData::Init(offset,ctx);
        if(data_.magic == MH_CIGAM_64){
            swap_mach_header_64(& data_,NX_LittleEndian);
        }
    }
};
using MachHeader64InternalPtr = std::shared_ptr<MachHeader64Internal>;


struct ParsingCacheInfo{
    uint64_t base_addr=0LL;
};

class MachHeader : public Node{
private:
    bool is64_;
    mach_header * header_; // points to mach_header and mach_header_64 (swapped)
    std::vector<LoadCommandPtr> loadcmds_;

    MachHeaderInternalPtr mh_;
    MachHeader64InternalPtr mh64_;
    Magic magic_;
    NodeContextPtr ctx_;
    void *header_start_;

    ParsingCacheInfo cache_;
private:
    void Parse(void *offset,NodeContextPtr& ctx);
public:
    bool Is64()const{return is64_;}
    mach_header * data_ptr(){return header_;}
    std::vector<LoadCommandPtr> &loadcmds_ref(){return loadcmds_;}
    NodeContextPtr & ctx(){return ctx_;}
    void * header_start(){return header_start_;}

    ParsingCacheInfo & cache(){return cache_;}

    uint64_t GetRAW(const void * addr);

    std::size_t DATA_SIZE();

    void Init(void *offset,NodeContextPtr&ctx);

    MachHeaderInternalPtr & mh(){return mh_;}
    MachHeader64InternalPtr & mh64(){return mh64_;}

    std::string GetArch();
    std::string GetFileTypeString();
    std::vector<std::tuple<uint32_t,std::string>> GetFlagsArray();
    std::string GetMagicString();

    std::string GetCpuTypeString();
    std::string GetCpuSubTypeString();
    std::vector<std::tuple<cpu_type_t,cpu_subtype_t,std::string>> GetCpuSubTypeArray();

};
using MachHeaderPtr = std::shared_ptr<MachHeader>;




MOEX_NAMESPACE_END

#endif // MACHHEADER_H
