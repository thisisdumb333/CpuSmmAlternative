#ifndef PAGE_H_
#define PAGE_H_

#pragma warning(push, 0)
#include <Uefi.h>
#pragma warning(pop)

typedef union PCIDE
{

    struct
    {
        UINT64 Reserved1 : 3;
        UINT64 Pwt : 1;
        UINT64 Pcd : 1;
        UINT64 Reserved2 : 7;
    }DISABLED;

    UINT64 ENABLED : 12;

}PCIDE;

typedef union _CR3
{
    struct
    {
        UINT64 Reserved1 : 3;
        UINT64 Pwt : 1;
        UINT64 Pcd : 1;
        UINT64 Reserved2 : 7;
        UINT64 TopPageMapLevel : 40;
        UINT64 Reserved3 : 12;
    }Disabled;

    struct
    {
        UINT64 Pcide : 12;
        UINT64 TopPageMapLevel : 40;
        UINT64 Reserved3 : 12;
    }Enabled;

    UINT64 Flags;
}CR3;
static_assert(sizeof(CR3) == sizeof(UINT64), "CR3 Size Mismatch");

typedef union _VIRTUAL_ADDRESS
{
    struct
    {
        UINT64 Unused : 12; //< [11:0]
        UINT64 PtIndex : 9; //< [20:12]
        UINT64 PdIndex : 9; //< [29:21]
        UINT64 PdptIndex : 9;  //< [38:30]
        UINT64 Pml4Index : 9; //< [47:39]
    }Bits;
    UINT64 Flags;
}VIRTUAL_ADDRESS;


typedef union _PML5E_4KB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Ignore : 1;
        UINT64 MustBeZero : 2;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 11;
        UINT64 NoExecute : 1;
    }Bits;

    UINT64 Flags;
}PML5E_4KB;

typedef union _PML4E_4KB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Ignore : 1;
        UINT64 MustBeZero : 2;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 11;
        UINT64 NoExecute : 1;
    }Bits;

    UINT64 Flags;
}PML4E_4KB;

typedef union _PDPE_4KB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Ignore : 1;
        UINT64 PageSize : 1;
        UINT64 Ignore2 : 1;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 11;
        UINT64 NoExecute : 1;

    }Bits;

    UINT64 Flags;

}PDPE_4KB;

typedef union _PDE_4KB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Ignore : 1;
        UINT64 PageSize : 1;
        UINT64 Ignore2 : 1;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 11;
        UINT64 NoExecute : 1;
    }Bits;

    UINT64 Flags;

}PDE_4KB;

typedef struct _PTE_4KB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Dirty : 1;
        UINT64 Pat : 1;
        UINT64 Global : 1;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 7;
        UINT64 Mpk : 4;
        UINT64 NoExecute : 1;
    }Bits;

    UINT64 Flags;

}PTE_4KB;

//
// 2MB Tables
//
typedef union _PML5E_2MB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Ignore : 1;
        UINT64 MustBeZero : 2;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 11;
        UINT64 NoExecute : 1;
    }Bits;

    UINT64 Flags;
}PML5E_2MB;

typedef union _PML4E_2MB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Ignore : 1;
        UINT64 MustBeZero : 2;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 11;
        UINT64 NoExecute : 1;
    }Bits;

    UINT64 Flags;
}PML4E_2MB;

typedef union _PDPE_2MB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Ignore : 1;
        UINT64 PageSize : 1;
        UINT64 Ignore2 : 1;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Avl2 : 11;
        UINT64 NoExecute : 1;
    }Bits;

    UINT64 Flags;
}PDPE_2MB;

typedef union _PDE_2MB
{
    struct
    {
        UINT64 Present : 1;
        UINT64 ReadWrite : 1;
        UINT64 UserSupervisor : 1;
        UINT64 Writethrough : 1;
        UINT64 CacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Dirty : 1;
        UINT64 PageSize : 1;
        UINT64 Global : 1;
        UINT64 Avl : 3;
        UINT64 Pat : 1;
        UINT64 Reserved1 : 8;
        UINT64 PageFrameNumber : 31;
        UINT64 Reserved2 : 11;
        UINT64 NoExecute : 1;
    } Bits;

    UINT64 Flags;
}PDE_2MB;

#endif