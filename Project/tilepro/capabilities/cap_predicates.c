#include <kernel.h>
#include <capabilities.h>
#include <cap_predicates.h>
#include <offsets.h>
/* Includes: */
 
#include <stdint.h>
#include <assert.h>
#include <capabilities.h>
#include <mdb/mdb.h>
#include <stdbool.h>
 
/* Function Definitions: */
 
int8_t compare_caps(struct  capability* left,struct  capability* right,bool tiebreak) {
    uint8_t _fof_x131;
    uint8_t _fof_x132;
    genpaddr_t _fof_x133;
    genpaddr_t _fof_x134;
    gensize_t _fof_x135;
    gensize_t _fof_x136;
    struct  capability* left__125;
    struct  capability left__125__126;
    enum objtype left__125__126__129;
    union  capability_u left__125__126__137;
    struct  FCNode left__125__126__137__139;
    genpaddr_t left__125__126__137__139__141;
    uint8_t left__125__126__137__139__143;
    coreid_t left__125__126__137__139__145;
    union  capability_u left__125__126__147;
    struct  IO left__125__126__147__149;
    uint16_t left__125__126__147__149__151;
    uint16_t left__125__126__147__149__153;
    union  capability_u left__125__126__155;
    struct  Notify_RCK left__125__126__155__157;
    coreid_t left__125__126__155__157__159;
    uint16_t left__125__126__155__157__161;
    union  capability_u left__125__126__163;
    struct  Notify_IPI left__125__126__163__165;
    coreid_t left__125__126__163__165__167;
    uint16_t left__125__126__163__165__169;
    union  capability_u left__125__126__171;
    struct  ID left__125__126__171__173;
    coreid_t left__125__126__171__173__175;
    uint32_t left__125__126__171__173__177;
    struct  capability* right__127;
    struct  capability right__127__128;
    enum objtype right__127__128__130;
    union  capability_u right__127__128__138;
    struct  FCNode right__127__128__138__140;
    genpaddr_t right__127__128__138__140__142;
    uint8_t right__127__128__138__140__144;
    coreid_t right__127__128__138__140__146;
    union  capability_u right__127__128__148;
    struct  IO right__127__128__148__150;
    uint16_t right__127__128__148__150__152;
    uint16_t right__127__128__148__150__154;
    union  capability_u right__127__128__156;
    struct  Notify_RCK right__127__128__156__158;
    coreid_t right__127__128__156__158__160;
    uint16_t right__127__128__156__158__162;
    union  capability_u right__127__128__164;
    struct  Notify_IPI right__127__128__164__166;
    coreid_t right__127__128__164__166__168;
    uint16_t right__127__128__164__166__170;
    union  capability_u right__127__128__172;
    struct  ID right__127__128__172__174;
    coreid_t right__127__128__172__174__176;
    uint32_t right__127__128__172__174__178;
 
    left__125 = left;
    left__125__126 = *left__125;
    right__127 = right;
    right__127__128 = *right__127;
    left__125__126__129 = left__125__126.type;
    right__127__128__130 = right__127__128.type;
    _fof_x131 = get_type_root(left__125__126__129);
    _fof_x132 = get_type_root(right__127__128__130);
    if ((_fof_x131 != _fof_x132)){
        return ((((_fof_x131 < _fof_x132)) ? (-1) : (1)));
    } else {
    }
    _fof_x133 = get_address(left);
    _fof_x134 = get_address(right);
    if ((_fof_x133 != _fof_x134)){
        return ((((_fof_x133 < _fof_x134)) ? (-1) : (1)));
    } else {
    }
    _fof_x135 = get_size(left);
    _fof_x136 = get_size(right);
    if ((_fof_x135 != _fof_x136)){
        return ((((_fof_x135 > _fof_x136)) ? (-1) : (1)));
    } else {
    }
    if ((left__125__126__129 != right__127__128__130)){
        return ((((left__125__126__129 < right__127__128__130)) ? (-1) : (1)));
    } else {
    }
    switch (left__125__126__129) {
        case ObjType_ID: {
            left__125__126__171 = left__125__126.u;
            right__127__128__172 = right__127__128.u;
            left__125__126__171__173 = left__125__126__171.id;
            right__127__128__172__174 = right__127__128__172.id;
            left__125__126__171__173__175 = left__125__126__171__173.coreid;
            right__127__128__172__174__176 = right__127__128__172__174.coreid;
            if ((left__125__126__171__173__175 != right__127__128__172__174__176)){
                return ((((left__125__126__171__173__175 < right__127__128__172__174__176)) ? (-1) : (1)));
            } else {
            }
            left__125__126__171__173__177 = left__125__126__171__173.core_local_id;
            right__127__128__172__174__178 = right__127__128__172__174.core_local_id;
            if ((left__125__126__171__173__177 != right__127__128__172__174__178)){
                return ((((left__125__126__171__173__177 < right__127__128__172__174__178)) ? (-1) : (1)));
            } else {
            }
            break;
        }
        case ObjType_Notify_IPI: {
            left__125__126__163 = left__125__126.u;
            right__127__128__164 = right__127__128.u;
            left__125__126__163__165 = left__125__126__163.notify_ipi;
            right__127__128__164__166 = right__127__128__164.notify_ipi;
            left__125__126__163__165__167 = left__125__126__163__165.coreid;
            right__127__128__164__166__168 = right__127__128__164__166.coreid;
            if ((left__125__126__163__165__167 != right__127__128__164__166__168)){
                return ((((left__125__126__163__165__167 < right__127__128__164__166__168)) ? (-1) : (1)));
            } else {
            }
            left__125__126__163__165__169 = left__125__126__163__165.chanid;
            right__127__128__164__166__170 = right__127__128__164__166.chanid;
            if ((left__125__126__163__165__169 != right__127__128__164__166__170)){
                return ((((left__125__126__163__165__169 < right__127__128__164__166__170)) ? (-1) : (1)));
            } else {
            }
            break;
        }
        case ObjType_Notify_RCK: {
            left__125__126__155 = left__125__126.u;
            right__127__128__156 = right__127__128.u;
            left__125__126__155__157 = left__125__126__155.notify_rck;
            right__127__128__156__158 = right__127__128__156.notify_rck;
            left__125__126__155__157__159 = left__125__126__155__157.coreid;
            right__127__128__156__158__160 = right__127__128__156__158.coreid;
            if ((left__125__126__155__157__159 != right__127__128__156__158__160)){
                return ((((left__125__126__155__157__159 < right__127__128__156__158__160)) ? (-1) : (1)));
            } else {
            }
            left__125__126__155__157__161 = left__125__126__155__157.chanid;
            right__127__128__156__158__162 = right__127__128__156__158.chanid;
            if ((left__125__126__155__157__161 != right__127__128__156__158__162)){
                return ((((left__125__126__155__157__161 < right__127__128__156__158__162)) ? (-1) : (1)));
            } else {
            }
            break;
        }
        case ObjType_IO: {
            left__125__126__147 = left__125__126.u;
            right__127__128__148 = right__127__128.u;
            left__125__126__147__149 = left__125__126__147.io;
            right__127__128__148__150 = right__127__128__148.io;
            left__125__126__147__149__151 = left__125__126__147__149.start;
            right__127__128__148__150__152 = right__127__128__148__150.start;
            if ((left__125__126__147__149__151 != right__127__128__148__150__152)){
                return ((((left__125__126__147__149__151 < right__127__128__148__150__152)) ? (-1) : (1)));
            } else {
            }
            left__125__126__147__149__153 = left__125__126__147__149.end;
            right__127__128__148__150__154 = right__127__128__148__150.end;
            if ((left__125__126__147__149__153 != right__127__128__148__150__154)){
                return ((((left__125__126__147__149__153 < right__127__128__148__150__154)) ? (-1) : (1)));
            } else {
            }
            break;
        }
        case ObjType_FCNode: {
            left__125__126__137 = left__125__126.u;
            right__127__128__138 = right__127__128.u;
            left__125__126__137__139 = left__125__126__137.fcnode;
            right__127__128__138__140 = right__127__128__138.fcnode;
            left__125__126__137__139__141 = left__125__126__137__139.cnode;
            right__127__128__138__140__142 = right__127__128__138__140.cnode;
            if ((left__125__126__137__139__141 != right__127__128__138__140__142)){
                return ((((left__125__126__137__139__141 < right__127__128__138__140__142)) ? (-1) : (1)));
            } else {
            }
            left__125__126__137__139__143 = left__125__126__137__139.bits;
            right__127__128__138__140__144 = right__127__128__138__140.bits;
            if ((left__125__126__137__139__143 != right__127__128__138__140__144)){
                return ((((left__125__126__137__139__143 < right__127__128__138__140__144)) ? (-1) : (1)));
            } else {
            }
            left__125__126__137__139__145 = left__125__126__137__139.core_id;
            right__127__128__138__140__146 = right__127__128__138__140.core_id;
            if ((left__125__126__137__139__145 != right__127__128__138__140__146)){
                return ((((left__125__126__137__139__145 < right__127__128__138__140__146)) ? (-1) : (1)));
            } else {
            }
            break;
        }
        default: {
        }
    }
    if (tiebreak){
        if ((left != right)){
            return ((((left < right)) ? (-1) : (1)));
        } else {
        }
    } else {
    }
    return (0);
}
 
genpaddr_t get_address(struct  capability* cap) {
    uint64_t _fof_x25;
    uint64_t _fof_x31;
    struct  capability* cap__10;
    struct  capability cap__10__11;
    union  capability_u cap__10__11__12;
    struct  RAM cap__10__11__12__13;
    genpaddr_t cap__10__11__12__13__14;
    struct  capability* cap__15;
    struct  capability cap__15__16;
    union  capability_u cap__15__16__17;
    struct  CNode cap__15__16__17__18;
    lpaddr_t cap__15__16__17__18__19;
    struct  capability* cap__2;
    struct  capability* cap__20;
    struct  capability cap__20__21;
    union  capability_u cap__20__21__22;
    struct  Dispatcher cap__20__21__22__23;
    struct dcb* cap__20__21__22__23__24;
    struct  capability* cap__26;
    struct  capability cap__26__27;
    union  capability_u cap__26__27__28;
    struct  EndPoint cap__26__27__28__29;
    struct dcb* cap__26__27__28__29__30;
    struct  capability cap__2__3;
    enum objtype cap__2__3__4;
    struct  capability* cap__32;
    struct  capability cap__32__33;
    union  capability_u cap__32__33__34;
    struct  Frame cap__32__33__34__35;
    genpaddr_t cap__32__33__34__35__36;
    struct  capability* cap__37;
    struct  capability cap__37__38;
    union  capability_u cap__37__38__39;
    struct  DevFrame cap__37__38__39__40;
    genpaddr_t cap__37__38__39__40__41;
    struct  capability* cap__42;
    struct  capability cap__42__43;
    union  capability_u cap__42__43__44;
    struct  VNode_x86_64_pml4 cap__42__43__44__45;
    genpaddr_t cap__42__43__44__45__46;
    struct  capability* cap__47;
    struct  capability cap__47__48;
    union  capability_u cap__47__48__49;
    struct  VNode_x86_64_pdpt cap__47__48__49__50;
    genpaddr_t cap__47__48__49__50__51;
    struct  capability* cap__5;
    struct  capability* cap__52;
    struct  capability cap__52__53;
    union  capability_u cap__52__53__54;
    struct  VNode_x86_64_pdir cap__52__53__54__55;
    genpaddr_t cap__52__53__54__55__56;
    struct  capability* cap__57;
    struct  capability cap__57__58;
    union  capability_u cap__57__58__59;
    struct  VNode_x86_64_ptable cap__57__58__59__60;
    genpaddr_t cap__57__58__59__60__61;
    struct  capability cap__5__6;
    union  capability_u cap__5__6__7;
    struct  PhysAddr cap__5__6__7__8;
    genpaddr_t cap__5__6__7__8__9;
    struct  capability* cap__62;
    struct  capability cap__62__63;
    union  capability_u cap__62__63__64;
    struct  VNode_x86_32_pdpt cap__62__63__64__65;
    genpaddr_t cap__62__63__64__65__66;
    struct  capability* cap__67;
    struct  capability cap__67__68;
    union  capability_u cap__67__68__69;
    struct  VNode_x86_32_pdir cap__67__68__69__70;
    genpaddr_t cap__67__68__69__70__71;
    struct  capability* cap__72;
    struct  capability cap__72__73;
    union  capability_u cap__72__73__74;
    struct  VNode_x86_32_ptable cap__72__73__74__75;
    genpaddr_t cap__72__73__74__75__76;
    struct  capability* cap__77;
    struct  capability cap__77__78;
    union  capability_u cap__77__78__79;
    struct  VNode_ARM_l1 cap__77__78__79__80;
    genpaddr_t cap__77__78__79__80__81;
    struct  capability* cap__82;
    struct  capability cap__82__83;
    union  capability_u cap__82__83__84;
    struct  VNode_ARM_l2 cap__82__83__84__85;
    genpaddr_t cap__82__83__84__85__86;
    struct  capability* cap__87;
    struct  capability cap__87__88;
    union  capability_u cap__87__88__89;
    struct  VNode_tilepro_l1 cap__87__88__89__90;
    genpaddr_t cap__87__88__89__90__91;
    struct  capability* cap__92;
    struct  capability cap__92__93;
    union  capability_u cap__92__93__94;
    struct  VNode_tilepro_l2 cap__92__93__94__95;
    genpaddr_t cap__92__93__94__95__96;
 
    cap__2 = cap;
    cap__2__3 = *cap__2;
    cap__2__3__4 = cap__2__3.type;
    switch (cap__2__3__4) {
        case ObjType_PerfMon: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_ID: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_Notify_IPI: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_Notify_RCK: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_IO: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_IRQTable: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_VNode_tilepro_l2: {
            cap__92 = cap;
            cap__92__93 = *cap__92;
            cap__92__93__94 = cap__92__93.u;
            cap__92__93__94__95 = cap__92__93__94.vnode_tilepro_l2;
            cap__92__93__94__95__96 = cap__92__93__94__95.base;
            return (cap__92__93__94__95__96);
            break;
        }
        case ObjType_VNode_tilepro_l1: {
            cap__87 = cap;
            cap__87__88 = *cap__87;
            cap__87__88__89 = cap__87__88.u;
            cap__87__88__89__90 = cap__87__88__89.vnode_tilepro_l1;
            cap__87__88__89__90__91 = cap__87__88__89__90.base;
            return (cap__87__88__89__90__91);
            break;
        }
        case ObjType_VNode_ARM_l2: {
            cap__82 = cap;
            cap__82__83 = *cap__82;
            cap__82__83__84 = cap__82__83.u;
            cap__82__83__84__85 = cap__82__83__84.vnode_arm_l2;
            cap__82__83__84__85__86 = cap__82__83__84__85.base;
            return (cap__82__83__84__85__86);
            break;
        }
        case ObjType_VNode_ARM_l1: {
            cap__77 = cap;
            cap__77__78 = *cap__77;
            cap__77__78__79 = cap__77__78.u;
            cap__77__78__79__80 = cap__77__78__79.vnode_arm_l1;
            cap__77__78__79__80__81 = cap__77__78__79__80.base;
            return (cap__77__78__79__80__81);
            break;
        }
        case ObjType_VNode_x86_32_ptable: {
            cap__72 = cap;
            cap__72__73 = *cap__72;
            cap__72__73__74 = cap__72__73.u;
            cap__72__73__74__75 = cap__72__73__74.vnode_x86_32_ptable;
            cap__72__73__74__75__76 = cap__72__73__74__75.base;
            return (cap__72__73__74__75__76);
            break;
        }
        case ObjType_VNode_x86_32_pdir: {
            cap__67 = cap;
            cap__67__68 = *cap__67;
            cap__67__68__69 = cap__67__68.u;
            cap__67__68__69__70 = cap__67__68__69.vnode_x86_32_pdir;
            cap__67__68__69__70__71 = cap__67__68__69__70.base;
            return (cap__67__68__69__70__71);
            break;
        }
        case ObjType_VNode_x86_32_pdpt: {
            cap__62 = cap;
            cap__62__63 = *cap__62;
            cap__62__63__64 = cap__62__63.u;
            cap__62__63__64__65 = cap__62__63__64.vnode_x86_32_pdpt;
            cap__62__63__64__65__66 = cap__62__63__64__65.base;
            return (cap__62__63__64__65__66);
            break;
        }
        case ObjType_VNode_x86_64_ptable: {
            cap__57 = cap;
            cap__57__58 = *cap__57;
            cap__57__58__59 = cap__57__58.u;
            cap__57__58__59__60 = cap__57__58__59.vnode_x86_64_ptable;
            cap__57__58__59__60__61 = cap__57__58__59__60.base;
            return (cap__57__58__59__60__61);
            break;
        }
        case ObjType_VNode_x86_64_pdir: {
            cap__52 = cap;
            cap__52__53 = *cap__52;
            cap__52__53__54 = cap__52__53.u;
            cap__52__53__54__55 = cap__52__53__54.vnode_x86_64_pdir;
            cap__52__53__54__55__56 = cap__52__53__54__55.base;
            return (cap__52__53__54__55__56);
            break;
        }
        case ObjType_VNode_x86_64_pdpt: {
            cap__47 = cap;
            cap__47__48 = *cap__47;
            cap__47__48__49 = cap__47__48.u;
            cap__47__48__49__50 = cap__47__48__49.vnode_x86_64_pdpt;
            cap__47__48__49__50__51 = cap__47__48__49__50.base;
            return (cap__47__48__49__50__51);
            break;
        }
        case ObjType_VNode_x86_64_pml4: {
            cap__42 = cap;
            cap__42__43 = *cap__42;
            cap__42__43__44 = cap__42__43.u;
            cap__42__43__44__45 = cap__42__43__44.vnode_x86_64_pml4;
            cap__42__43__44__45__46 = cap__42__43__44__45.base;
            return (cap__42__43__44__45__46);
            break;
        }
        case ObjType_Kernel: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_DevFrame: {
            cap__37 = cap;
            cap__37__38 = *cap__37;
            cap__37__38__39 = cap__37__38.u;
            cap__37__38__39__40 = cap__37__38__39.devframe;
            cap__37__38__39__40__41 = cap__37__38__39__40.base;
            return (cap__37__38__39__40__41);
            break;
        }
        case ObjType_Frame: {
            cap__32 = cap;
            cap__32__33 = *cap__32;
            cap__32__33__34 = cap__32__33.u;
            cap__32__33__34__35 = cap__32__33__34.frame;
            cap__32__33__34__35__36 = cap__32__33__34__35.base;
            return (cap__32__33__34__35__36);
            break;
        }
        case ObjType_EndPoint: {
            cap__26 = cap;
            cap__26__27 = *cap__26;
            cap__26__27__28 = cap__26__27.u;
            cap__26__27__28__29 = cap__26__27__28.endpoint;
            cap__26__27__28__29__30 = cap__26__27__28__29.listener;
            _fof_x31 = mem_to_local_phys(((lvaddr_t) cap__26__27__28__29__30));
            return (_fof_x31);
            break;
        }
        case ObjType_Dispatcher: {
            cap__20 = cap;
            cap__20__21 = *cap__20;
            cap__20__21__22 = cap__20__21.u;
            cap__20__21__22__23 = cap__20__21__22.dispatcher;
            cap__20__21__22__23__24 = cap__20__21__22__23.dcb;
            _fof_x25 = mem_to_local_phys(((lvaddr_t) cap__20__21__22__23__24));
            return (_fof_x25);
            break;
        }
        case ObjType_FCNode: {
            return (((genpaddr_t) 0));
            break;
        }
        case ObjType_CNode: {
            cap__15 = cap;
            cap__15__16 = *cap__15;
            cap__15__16__17 = cap__15__16.u;
            cap__15__16__17__18 = cap__15__16__17.cnode;
            cap__15__16__17__18__19 = cap__15__16__17__18.cnode;
            return (cap__15__16__17__18__19);
            break;
        }
        case ObjType_RAM: {
            cap__10 = cap;
            cap__10__11 = *cap__10;
            cap__10__11__12 = cap__10__11.u;
            cap__10__11__12__13 = cap__10__11__12.ram;
            cap__10__11__12__13__14 = cap__10__11__12__13.base;
            return (cap__10__11__12__13__14);
            break;
        }
        case ObjType_PhysAddr: {
            cap__5 = cap;
            cap__5__6 = *cap__5;
            cap__5__6__7 = cap__5__6.u;
            cap__5__6__7__8 = cap__5__6__7.physaddr;
            cap__5__6__7__8__9 = cap__5__6__7__8.base;
            return (cap__5__6__7__8__9);
            break;
        }
        case ObjType_Null: {
            return (((genpaddr_t) 0));
            break;
        }
        default: {
            assert(0);
            return (0);
        }
    }
}
 
gensize_t get_size(struct  capability* cap) {
    struct  capability* cap__100;
    struct  capability cap__100__101;
    union  capability_u cap__100__101__102;
    struct  PhysAddr cap__100__101__102__103;
    uint8_t cap__100__101__102__103__104;
    struct  capability* cap__105;
    struct  capability cap__105__106;
    union  capability_u cap__105__106__107;
    struct  RAM cap__105__106__107__108;
    uint8_t cap__105__106__107__108__109;
    struct  capability* cap__110;
    struct  capability cap__110__111;
    union  capability_u cap__110__111__112;
    struct  CNode cap__110__111__112__113;
    uint8_t cap__110__111__112__113__114;
    struct  capability* cap__115;
    struct  capability cap__115__116;
    union  capability_u cap__115__116__117;
    struct  Frame cap__115__116__117__118;
    uint8_t cap__115__116__117__118__119;
    struct  capability* cap__120;
    struct  capability cap__120__121;
    union  capability_u cap__120__121__122;
    struct  DevFrame cap__120__121__122__123;
    uint8_t cap__120__121__122__123__124;
    struct  capability* cap__97;
    struct  capability cap__97__98;
    enum objtype cap__97__98__99;
 
    cap__97 = cap;
    cap__97__98 = *cap__97;
    cap__97__98__99 = cap__97__98.type;
    switch (cap__97__98__99) {
        case ObjType_PerfMon: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_ID: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_Notify_IPI: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_Notify_RCK: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_IO: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_IRQTable: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_VNode_tilepro_l2: {
            return ((((gensize_t) 1) << 16));
            break;
        }
        case ObjType_VNode_tilepro_l1: {
            return ((((gensize_t) 1) << 16));
            break;
        }
        case ObjType_VNode_ARM_l2: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_ARM_l1: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_x86_32_ptable: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_x86_32_pdir: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_x86_32_pdpt: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_x86_64_ptable: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_x86_64_pdir: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_x86_64_pdpt: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_VNode_x86_64_pml4: {
            return ((((gensize_t) 1) << 12));
            break;
        }
        case ObjType_Kernel: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_DevFrame: {
            cap__120 = cap;
            cap__120__121 = *cap__120;
            cap__120__121__122 = cap__120__121.u;
            cap__120__121__122__123 = cap__120__121__122.devframe;
            cap__120__121__122__123__124 = cap__120__121__122__123.bits;
            return ((((gensize_t) 1) << cap__120__121__122__123__124));
            break;
        }
        case ObjType_Frame: {
            cap__115 = cap;
            cap__115__116 = *cap__115;
            cap__115__116__117 = cap__115__116.u;
            cap__115__116__117__118 = cap__115__116__117.frame;
            cap__115__116__117__118__119 = cap__115__116__117__118.bits;
            return ((((gensize_t) 1) << cap__115__116__117__118__119));
            break;
        }
        case ObjType_EndPoint: {
            return (0);
            break;
        }
        case ObjType_Dispatcher: {
            return ((((gensize_t) 1) << 11));
            break;
        }
        case ObjType_FCNode: {
            return (((gensize_t) 0));
            break;
        }
        case ObjType_CNode: {
            cap__110 = cap;
            cap__110__111 = *cap__110;
            cap__110__111__112 = cap__110__111.u;
            cap__110__111__112__113 = cap__110__111__112.cnode;
            cap__110__111__112__113__114 = cap__110__111__112__113.bits;
            return ((((gensize_t) 1) << (cap__110__111__112__113__114 + 8)));
            break;
        }
        case ObjType_RAM: {
            cap__105 = cap;
            cap__105__106 = *cap__105;
            cap__105__106__107 = cap__105__106.u;
            cap__105__106__107__108 = cap__105__106__107.ram;
            cap__105__106__107__108__109 = cap__105__106__107__108.bits;
            return ((((gensize_t) 1) << cap__105__106__107__108__109));
            break;
        }
        case ObjType_PhysAddr: {
            cap__100 = cap;
            cap__100__101 = *cap__100;
            cap__100__101__102 = cap__100__101.u;
            cap__100__101__102__103 = cap__100__101__102.physaddr;
            cap__100__101__102__103__104 = cap__100__101__102__103.bits;
            return ((((gensize_t) 1) << cap__100__101__102__103__104));
            break;
        }
        case ObjType_Null: {
            return (((gensize_t) 0));
            break;
        }
        default: {
            assert(0);
            return (0);
        }
    }
}
 
uint8_t get_type_root(enum objtype type) {
 
    switch (type) {
        case ObjType_PerfMon: {
            return (9);
            break;
        }
        case ObjType_ID: {
            return (8);
            break;
        }
        case ObjType_Notify_IPI: {
            return (7);
            break;
        }
        case ObjType_Notify_RCK: {
            return (6);
            break;
        }
        case ObjType_IO: {
            return (5);
            break;
        }
        case ObjType_IRQTable: {
            return (4);
            break;
        }
        case ObjType_VNode_tilepro_l2: {
            return (1);
            break;
        }
        case ObjType_VNode_tilepro_l1: {
            return (1);
            break;
        }
        case ObjType_VNode_ARM_l2: {
            return (1);
            break;
        }
        case ObjType_VNode_ARM_l1: {
            return (1);
            break;
        }
        case ObjType_VNode_x86_32_ptable: {
            return (1);
            break;
        }
        case ObjType_VNode_x86_32_pdir: {
            return (1);
            break;
        }
        case ObjType_VNode_x86_32_pdpt: {
            return (1);
            break;
        }
        case ObjType_VNode_x86_64_ptable: {
            return (1);
            break;
        }
        case ObjType_VNode_x86_64_pdir: {
            return (1);
            break;
        }
        case ObjType_VNode_x86_64_pdpt: {
            return (1);
            break;
        }
        case ObjType_VNode_x86_64_pml4: {
            return (1);
            break;
        }
        case ObjType_Kernel: {
            return (3);
            break;
        }
        case ObjType_DevFrame: {
            return (1);
            break;
        }
        case ObjType_Frame: {
            return (1);
            break;
        }
        case ObjType_EndPoint: {
            return (1);
            break;
        }
        case ObjType_Dispatcher: {
            return (1);
            break;
        }
        case ObjType_FCNode: {
            return (2);
            break;
        }
        case ObjType_CNode: {
            return (1);
            break;
        }
        case ObjType_RAM: {
            return (1);
            break;
        }
        case ObjType_PhysAddr: {
            return (1);
            break;
        }
        case ObjType_Null: {
            return (0);
            break;
        }
        default: {
            assert(0);
            return (0);
        }
    }
}
 
bool is_ancestor(struct  capability* child,struct  capability* parent) {
    bool _fof_x196;
    genpaddr_t _fof_x197;
    genpaddr_t _fof_x198;
    gensize_t _fof_x199;
    gensize_t _fof_x200;
    struct  capability* child__190;
    struct  capability child__190__191;
    enum objtype child__190__191__194;
    struct  capability* parent__192;
    struct  capability parent__192__193;
    enum objtype parent__192__193__195;
 
    child__190 = child;
    child__190__191 = *child__190;
    parent__192 = parent;
    parent__192__193 = *parent__192;
    child__190__191__194 = child__190__191.type;
    parent__192__193__195 = parent__192__193.type;
    _fof_x196 = is_well_founded(parent__192__193__195,child__190__191__194);
    if ((! _fof_x196)){
        return (0);
    } else {
    }
    _fof_x197 = get_address(parent);
    _fof_x198 = get_address(child);
    _fof_x199 = get_size(parent);
    _fof_x200 = get_size(child);
    if ((child__190__191__194 == parent__192__193__195)){
        return ((((_fof_x197 < _fof_x198) & ((_fof_x197 + _fof_x199) >= (_fof_x198 + _fof_x200))) | ((_fof_x197 <= _fof_x198) & ((_fof_x197 + _fof_x199) > (_fof_x198 + _fof_x200)))));
    } else {
        return (((_fof_x197 <= _fof_x198) & ((_fof_x197 + _fof_x199) >= (_fof_x198 + _fof_x200))));
    }
}
 
bool is_copy(struct  capability* left,struct  capability* right) {
    int8_t _fof_x189;
    struct  capability* left__183;
    struct  capability left__183__184;
    enum objtype left__183__184__187;
    struct  capability* right__185;
    struct  capability right__185__186;
    enum objtype right__185__186__188;
 
    left__183 = left;
    left__183__184 = *left__183;
    right__185 = right;
    right__185__186 = *right__185;
    left__183__184__187 = left__183__184.type;
    right__185__186__188 = right__185__186.type;
    if ((left__183__184__187 != right__185__186__188)){
        return (0);
    } else {
    }
    switch (left__183__184__187) {
        case ObjType_PerfMon: {
            return (1);
            break;
        }
        case ObjType_IRQTable: {
            return (1);
            break;
        }
        case ObjType_Kernel: {
            return (1);
            break;
        }
        case ObjType_Null: {
            return (0);
            break;
        }
        default: {
        }
    }
    _fof_x189 = compare_caps(left,right,0);
    return ((_fof_x189 == 0));
}
 
bool is_equal_type(enum objtype left_type,enum objtype right_type) {
 
    return ((left_type == right_type));
}
 
bool is_revoked_first(struct cte * src_cte,enum objtype src_type) {
    bool _fof_x179;
    bool _fof_x180;
    bool _fof_x181;
    bool _fof_x182;
 
    switch (src_type) {
        case ObjType_DevFrame: {
            _fof_x182 = has_descendants(src_cte);
            if (_fof_x182){
                return (0);
            } else {
                return (1);
            }
            break;
        }
        case ObjType_Frame: {
            _fof_x181 = has_descendants(src_cte);
            if (_fof_x181){
                return (0);
            } else {
                return (1);
            }
            break;
        }
        case ObjType_Dispatcher: {
            return (1);
            break;
        }
        case ObjType_RAM: {
            _fof_x180 = has_descendants(src_cte);
            if (_fof_x180){
                return (0);
            } else {
                return (1);
            }
            break;
        }
        case ObjType_PhysAddr: {
            _fof_x179 = has_descendants(src_cte);
            if (_fof_x179){
                return (0);
            } else {
                return (1);
            }
            break;
        }
        default: {
            return (0);
        }
    }
}
 
bool is_well_founded(enum objtype src_type,enum objtype dest_type) {
 
    switch (dest_type) {
        case ObjType_VNode_tilepro_l2: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_tilepro_l1: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_ARM_l2: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_ARM_l1: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_x86_32_ptable: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_x86_32_pdir: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_x86_32_pdpt: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_x86_64_ptable: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_x86_64_pdir: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_x86_64_pdpt: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_VNode_x86_64_pml4: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_DevFrame: {
            return (((src_type == ObjType_PhysAddr) | (src_type == ObjType_DevFrame)));
            break;
        }
        case ObjType_Frame: {
            return (((src_type == ObjType_RAM) | (src_type == ObjType_Frame)));
            break;
        }
        case ObjType_EndPoint: {
            return (((src_type == ObjType_Dispatcher) | 0));
            break;
        }
        case ObjType_Dispatcher: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_CNode: {
            return (((src_type == ObjType_RAM) | 0));
            break;
        }
        case ObjType_RAM: {
            return (((src_type == ObjType_PhysAddr) | (src_type == ObjType_RAM)));
            break;
        }
        case ObjType_PhysAddr: {
            return ((0 | (src_type == ObjType_PhysAddr)));
            break;
        }
        default: {
            return (0);
        }
    }
}
 
 
 
