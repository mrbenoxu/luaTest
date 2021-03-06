#ifndef _LSTATE_H_
#define _LSTATE_H_

#include "lobject.h"

#define LUA_EXTRASPACE sizeof(void*) 
#define G(L) ((L)->l_G)

#define STEPMULADJ 200
#define GCSTEPMUL 200 
#define GCSTEPSIZE 1024  //1kb
#define GCPAUSE 100

// size for string cache
#define STRCACHE_M 53
#define STRCACHE_N 2

typedef TValue* StkId;

struct CallInfo {
    StkId func;                     // 被调用函数在栈中的位置
    StkId top;                      // 被调用函数的栈顶位置
    int nresult;                    // 有多少个返回值
    int callstatus;                 // 调用状态
    struct CallInfo* next;          // 下一个调用
    struct CallInfo* previous;      // 上一个调用
};

typedef struct lua_State {
    CommonHeader;       // gc header, all gcobject should have the commonheader
    StkId stack;
    StkId stack_last;
    StkId top;
    int stack_size;
    struct lua_longjmp* errorjmp;
    int status;
    struct lua_State* previous;
    struct CallInfo base_ci;
    struct CallInfo* ci;
    int nci;
    struct global_State* l_G;
    ptrdiff_t errorfunc;
    int ncalls;
    struct GCObject* gclist;
} lua_State;

// only for short string
struct stringtable {
    struct TString** hash;
    unsigned int nuse;
    unsigned int size;
};

typedef struct global_State {
    struct lua_State* mainthread;
    lua_Alloc frealloc;
    void* ud; 
    lua_CFunction panic;
    
    struct stringtable strt;
    TString* strcache[STRCACHE_M][STRCACHE_N];  // 缓存所有长短字符，超出就覆盖
    unsigned int seed;              // hash seed, just for string hash
    TString* memerrmsg;
    TValue l_registry;

    //gc fields
    lu_byte gcstate;
    lu_byte currentwhite;
    struct GCObject* allgc;         // gc root set
    struct GCObject** sweepgc;
    struct GCObject* gray;
    struct GCObject* grayagain;
    struct GCObject* fixgc;         // objects can not collect by gc
    lu_mem totalbytes;
    l_mem GCdebt;                   // GCdebt will be negative
    lu_mem GCmemtrav;               // 每次GC操作（调用一次singlestep接口）清除的数据大小
    lu_mem GCestimate;              // after finish a gc cycle,it records total memory bytes (totalbytes + GCdebt)
    int GCstepmul;
} global_State;

// GCUnion
union GCUnion {
    struct GCObject gc;
    lua_State th;
    TString ts;
    struct Table tbl;
};

struct lua_longjmp;

#endif // _LSTATE_H_
