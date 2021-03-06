#ifndef __fat32_ebpb_DEV_H
#define __fat32_ebpb_DEV_H 1
/*
 * DEVICE DEFINITION: FAT32 Extended BIOS Parameter Block (EBPB)
 * 
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * This file is distributed under the terms in the attached LICENSE
 * file. If you do not find this file, copies can be found by
 * writing to:
 * ETH Zurich D-INFK, Universitaetstr. 6, CH-8092 Zurich.
 * Attn: Systems Group.
 * 
 * THIS FILE IS AUTOMATICALLY GENERATED BY MACKEREL: DO NOT EDIT!
 */
#include <mackerel/mackerel.h>
#include <inttypes.h>
#undef __DN
#define __DN(x) fat32_ebpb ## _ ## x
/*
 * Register type: fat32_ebpb_flgs_t
 * Description: Implicit type of Flags register
 * Fields:
 *   afc	(size 5, offset 0, init 0):	RW	Active FAT Copy
 *   _anon5	(size 1, offset 5, init 0):	RSVD	_
 *   fme	(size 1, offset 6, init 0):	RW	FAT Mirror Enabled
 *   _anon7	(size 9, offset 7, init 0):	RSVD	_
 */
typedef uint16_t fat32_ebpb_flgs_t;
#define fat32_ebpb_flgs_default 0x0
static inline uint8_t fat32_ebpb_flgs_afc_extract(fat32_ebpb_flgs_t _regval) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_flgs_afc_extract(fat32_ebpb_flgs_t _regval)
{
    return((uint8_t )((_regval & 0x1f) >> 0));
}

static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_afc_insert(fat32_ebpb_flgs_t _regval, uint8_t _fieldval) __attribute__ ((always_inline));
static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_afc_insert(fat32_ebpb_flgs_t _regval, uint8_t _fieldval)
{
    return((_regval & 0xffe0) | (0x1f & (((fat32_ebpb_flgs_t )(_fieldval)) << 0)));
}

static inline uint8_t fat32_ebpb_flgs_fme_extract(fat32_ebpb_flgs_t _regval) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_flgs_fme_extract(fat32_ebpb_flgs_t _regval)
{
    return((uint8_t )((_regval & 0x40) >> 6));
}

static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_fme_insert(fat32_ebpb_flgs_t _regval, uint8_t _fieldval) __attribute__ ((always_inline));
static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_fme_insert(fat32_ebpb_flgs_t _regval, uint8_t _fieldval)
{
    return((_regval & 0xffbf) | (0x40 & (((fat32_ebpb_flgs_t )(_fieldval)) << 6)));
}

static inline int fat32_ebpb_flgs_prtval(char *_s, size_t _size, fat32_ebpb_flgs_t _regval) __attribute__ ((always_inline));
static inline int fat32_ebpb_flgs_prtval(char *_s, size_t _size, fat32_ebpb_flgs_t _regval)
{
    int _r = 0;
    int _avail;
    int _rc;
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, " afc =\t%" PRIx8 "\t(Active FAT Copy)\n", fat32_ebpb_flgs_afc_extract(_regval));
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, " fme =\t%" PRIx8 "\t(FAT Mirror Enabled)\n", fat32_ebpb_flgs_fme_extract(_regval));
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Device representation structure
 */
struct __DN(t) {
    mackerel_addr_t b;
};
typedef struct __DN(t) __DN(t);

/*
 * Initial register values (currently 0)
 */
enum fat32_ebpb_initials {
    fat32_ebpb_spf_initial = 0x0,
    fat32_ebpb_flgs_initial = 0x0,
    fat32_ebpb_vrsn_initial = 0x0,
    fat32_ebpb_rtst_initial = 0x0,
    fat32_ebpb_fsis_initial = 0x0,
    fat32_ebpb_bbss_initial = 0x0,
    fat32_ebpb_ldn_initial = 0x0,
    fat32_ebpb_rsv_initial = 0x0,
    fat32_ebpb_ebs_initial = 0x0,
    fat32_ebpb_vsn_initial = 0x0,
    fat32_ebpb_vlb_initial = 0x0,
    fat32_ebpb_fst_initial = 0x0
};

/*
 * Device Initialization function
 */
static inline void fat32_ebpb_initialize(__DN(t) *_dev, mackerel_addr_t b) __attribute__ ((always_inline));
static inline void fat32_ebpb_initialize(__DN(t) *_dev, mackerel_addr_t b)
{
    _dev->b = b;
}

/*
 * Register spf: Sectors per FAT
 * Type: fat32_ebpb.uint32 (primitive type)
 */
static inline uint32_t fat32_ebpb_spf_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint32_t fat32_ebpb_spf_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_32(_dev->b, 0x24));
}

static inline uint32_t fat32_ebpb_spf_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint32_t fat32_ebpb_spf_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_32(_dev->b, 0x24));
}

static inline void fat32_ebpb_spf_rawwr(__DN(t) *_dev, uint32_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_spf_rawwr(__DN(t) *_dev, uint32_t _regval)
{
    mackerel_write_addr_32(_dev->b, 0x24, _regval);
}

static inline void fat32_ebpb_spf_wr(__DN(t) *_dev, uint32_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_spf_wr(__DN(t) *_dev, uint32_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_32(_dev->b, 0x24, _regval);
}

static inline int fat32_ebpb_spf_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_spf_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint32_t _regval = mackerel_read_addr_32(_dev->b, 0x24);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register spf (Sectors per FAT): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx32 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register flgs: Flags
 * Type: fat32_ebpb.flgs (Implicit type of Flags register)
 *   afc	(size 5, offset 0, init 0):	RW	Active FAT Copy
 *   _anon5	(size 1, offset 5, init 0):	RSVD	_
 *   fme	(size 1, offset 6, init 0):	RW	FAT Mirror Enabled
 *   _anon7	(size 9, offset 7, init 0):	RSVD	_
 */
static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x28));
}

static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline fat32_ebpb_flgs_t fat32_ebpb_flgs_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x28));
}

static inline void fat32_ebpb_flgs_rawwr(__DN(t) *_dev, fat32_ebpb_flgs_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_flgs_rawwr(__DN(t) *_dev, fat32_ebpb_flgs_t _regval)
{
    mackerel_write_addr_16(_dev->b, 0x28, _regval);
}

static inline void fat32_ebpb_flgs_wr(__DN(t) *_dev, fat32_ebpb_flgs_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_flgs_wr(__DN(t) *_dev, fat32_ebpb_flgs_t _regval)
{
    _regval = (_regval & 0x5f);
    // No MB1 fields present
    _regval = (_regval | (0xffa0 & mackerel_read_addr_16(_dev->b, 0x28)));
    mackerel_write_addr_16(_dev->b, 0x28, _regval);
}

static inline int fat32_ebpb_flgs_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_flgs_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    fat32_ebpb_flgs_t _regval = mackerel_read_addr_16(_dev->b, 0x28);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register flgs (Flags): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\n");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, " afc =\t%" PRIx8 "\t(Active FAT Copy)\n", fat32_ebpb_flgs_afc_extract(_regval));
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    // _anon5 is anonymous
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, " fme =\t%" PRIx8 "\t(FAT Mirror Enabled)\n", fat32_ebpb_flgs_fme_extract(_regval));
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    // _anon7 is anonymous
    return(_r);
}

static inline uint8_t fat32_ebpb_flgs_afc_rdf(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_flgs_afc_rdf(__DN(t) *_dev)
{
    fat32_ebpb_flgs_t _regval = mackerel_read_addr_16(_dev->b, 0x28);
    return(fat32_ebpb_flgs_afc_extract(_regval));
}

static inline uint8_t fat32_ebpb_flgs_fme_rdf(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_flgs_fme_rdf(__DN(t) *_dev)
{
    fat32_ebpb_flgs_t _regval = mackerel_read_addr_16(_dev->b, 0x28);
    return(fat32_ebpb_flgs_fme_extract(_regval));
}

static inline void fat32_ebpb_flgs_afc_wrf(__DN(t) *_dev, uint8_t _fieldval) __attribute__ ((always_inline));
static inline void fat32_ebpb_flgs_afc_wrf(__DN(t) *_dev, uint8_t _fieldval)
{
    fat32_ebpb_flgs_t _regval = 0x1f & (((fat32_ebpb_flgs_t )(_fieldval)) << 0);
    _regval = (_regval | (0xffe0 & mackerel_read_addr_16(_dev->b, 0x28)));
    // No read of register shadow required
    // No MB0 fields present
    // No MB1 fields present
    mackerel_write_addr_16(_dev->b, 0x28, _regval);
    // No shadow register to write to
}

static inline void fat32_ebpb_flgs_fme_wrf(__DN(t) *_dev, uint8_t _fieldval) __attribute__ ((always_inline));
static inline void fat32_ebpb_flgs_fme_wrf(__DN(t) *_dev, uint8_t _fieldval)
{
    fat32_ebpb_flgs_t _regval = 0x40 & (((fat32_ebpb_flgs_t )(_fieldval)) << 6);
    _regval = (_regval | (0xffbf & mackerel_read_addr_16(_dev->b, 0x28)));
    // No read of register shadow required
    // No MB0 fields present
    // No MB1 fields present
    mackerel_write_addr_16(_dev->b, 0x28, _regval);
    // No shadow register to write to
}

/*
 * Register vrsn: Version
 * Type: fat32_ebpb.uint16 (primitive type)
 */
static inline uint16_t fat32_ebpb_vrsn_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint16_t fat32_ebpb_vrsn_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x2a));
}

static inline uint16_t fat32_ebpb_vrsn_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint16_t fat32_ebpb_vrsn_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x2a));
}

static inline void fat32_ebpb_vrsn_rawwr(__DN(t) *_dev, uint16_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_vrsn_rawwr(__DN(t) *_dev, uint16_t _regval)
{
    mackerel_write_addr_16(_dev->b, 0x2a, _regval);
}

static inline void fat32_ebpb_vrsn_wr(__DN(t) *_dev, uint16_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_vrsn_wr(__DN(t) *_dev, uint16_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_16(_dev->b, 0x2a, _regval);
}

static inline int fat32_ebpb_vrsn_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_vrsn_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint16_t _regval = mackerel_read_addr_16(_dev->b, 0x2a);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register vrsn (Version): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx16 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register rtst: Start cluster of root directory
 * Type: fat32_ebpb.uint32 (primitive type)
 */
static inline uint32_t fat32_ebpb_rtst_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint32_t fat32_ebpb_rtst_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_32(_dev->b, 0x2c));
}

static inline uint32_t fat32_ebpb_rtst_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint32_t fat32_ebpb_rtst_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_32(_dev->b, 0x2c));
}

static inline void fat32_ebpb_rtst_rawwr(__DN(t) *_dev, uint32_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_rtst_rawwr(__DN(t) *_dev, uint32_t _regval)
{
    mackerel_write_addr_32(_dev->b, 0x2c, _regval);
}

static inline void fat32_ebpb_rtst_wr(__DN(t) *_dev, uint32_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_rtst_wr(__DN(t) *_dev, uint32_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_32(_dev->b, 0x2c, _regval);
}

static inline int fat32_ebpb_rtst_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_rtst_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint32_t _regval = mackerel_read_addr_32(_dev->b, 0x2c);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register rtst (Start cluster of root directory): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx32 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register fsis: Sector of the File System Information Sector
 * Type: fat32_ebpb.uint16 (primitive type)
 */
static inline uint16_t fat32_ebpb_fsis_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint16_t fat32_ebpb_fsis_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x30));
}

static inline uint16_t fat32_ebpb_fsis_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint16_t fat32_ebpb_fsis_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x30));
}

static inline void fat32_ebpb_fsis_rawwr(__DN(t) *_dev, uint16_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_fsis_rawwr(__DN(t) *_dev, uint16_t _regval)
{
    mackerel_write_addr_16(_dev->b, 0x30, _regval);
}

static inline void fat32_ebpb_fsis_wr(__DN(t) *_dev, uint16_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_fsis_wr(__DN(t) *_dev, uint16_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_16(_dev->b, 0x30, _regval);
}

static inline int fat32_ebpb_fsis_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_fsis_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint16_t _regval = mackerel_read_addr_16(_dev->b, 0x30);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register fsis (Sector of the File System Information Sector): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx16 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register bbss: Sector of backup boot sector, 0 implies no backup
 * Type: fat32_ebpb.uint16 (primitive type)
 */
static inline uint16_t fat32_ebpb_bbss_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint16_t fat32_ebpb_bbss_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x32));
}

static inline uint16_t fat32_ebpb_bbss_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint16_t fat32_ebpb_bbss_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_16(_dev->b, 0x32));
}

static inline void fat32_ebpb_bbss_rawwr(__DN(t) *_dev, uint16_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_bbss_rawwr(__DN(t) *_dev, uint16_t _regval)
{
    mackerel_write_addr_16(_dev->b, 0x32, _regval);
}

static inline void fat32_ebpb_bbss_wr(__DN(t) *_dev, uint16_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_bbss_wr(__DN(t) *_dev, uint16_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_16(_dev->b, 0x32, _regval);
}

static inline int fat32_ebpb_bbss_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_bbss_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint16_t _regval = mackerel_read_addr_16(_dev->b, 0x32);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register bbss (Sector of backup boot sector, 0 implies no backup): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx16 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register ldn: Logical Drive Number of partition
 * Type: fat32_ebpb.uint8 (primitive type)
 */
static inline uint8_t fat32_ebpb_ldn_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_ldn_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_8(_dev->b, 0x40));
}

static inline uint8_t fat32_ebpb_ldn_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_ldn_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_8(_dev->b, 0x40));
}

static inline void fat32_ebpb_ldn_rawwr(__DN(t) *_dev, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_ldn_rawwr(__DN(t) *_dev, uint8_t _regval)
{
    mackerel_write_addr_8(_dev->b, 0x40, _regval);
}

static inline void fat32_ebpb_ldn_wr(__DN(t) *_dev, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_ldn_wr(__DN(t) *_dev, uint8_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_8(_dev->b, 0x40, _regval);
}

static inline int fat32_ebpb_ldn_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_ldn_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint8_t _regval = mackerel_read_addr_8(_dev->b, 0x40);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register ldn (Logical Drive Number of partition): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx8 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register rsv: Reserved
 * Type: fat32_ebpb.uint8 (primitive type)
 */
static inline uint8_t fat32_ebpb_rsv_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_rsv_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_8(_dev->b, 0x41));
}

static inline uint8_t fat32_ebpb_rsv_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_rsv_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_8(_dev->b, 0x41));
}

static inline void fat32_ebpb_rsv_rawwr(__DN(t) *_dev, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_rsv_rawwr(__DN(t) *_dev, uint8_t _regval)
{
    mackerel_write_addr_8(_dev->b, 0x41, _regval);
}

static inline void fat32_ebpb_rsv_wr(__DN(t) *_dev, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_rsv_wr(__DN(t) *_dev, uint8_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_8(_dev->b, 0x41, _regval);
}

static inline int fat32_ebpb_rsv_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_rsv_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint8_t _regval = mackerel_read_addr_8(_dev->b, 0x41);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register rsv (Reserved): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx8 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register ebs: Extended boot signature
 * Type: fat32_ebpb.uint8 (primitive type)
 */
static inline uint8_t fat32_ebpb_ebs_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_ebs_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_8(_dev->b, 0x42));
}

static inline uint8_t fat32_ebpb_ebs_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_ebs_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_8(_dev->b, 0x42));
}

static inline void fat32_ebpb_ebs_rawwr(__DN(t) *_dev, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_ebs_rawwr(__DN(t) *_dev, uint8_t _regval)
{
    mackerel_write_addr_8(_dev->b, 0x42, _regval);
}

static inline void fat32_ebpb_ebs_wr(__DN(t) *_dev, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_ebs_wr(__DN(t) *_dev, uint8_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_8(_dev->b, 0x42, _regval);
}

static inline int fat32_ebpb_ebs_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_ebs_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint8_t _regval = mackerel_read_addr_8(_dev->b, 0x42);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register ebs (Extended boot signature): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx8 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register vsn: Volume serial number
 * Type: fat32_ebpb.uint32 (primitive type)
 */
static inline uint32_t fat32_ebpb_vsn_rawrd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint32_t fat32_ebpb_vsn_rawrd(__DN(t) *_dev)
{
    return(mackerel_read_addr_32(_dev->b, 0x43));
}

static inline uint32_t fat32_ebpb_vsn_rd(__DN(t) *_dev) __attribute__ ((always_inline));
static inline uint32_t fat32_ebpb_vsn_rd(__DN(t) *_dev)
{
    return(mackerel_read_addr_32(_dev->b, 0x43));
}

static inline void fat32_ebpb_vsn_rawwr(__DN(t) *_dev, uint32_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_vsn_rawwr(__DN(t) *_dev, uint32_t _regval)
{
    mackerel_write_addr_32(_dev->b, 0x43, _regval);
}

static inline void fat32_ebpb_vsn_wr(__DN(t) *_dev, uint32_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_vsn_wr(__DN(t) *_dev, uint32_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_32(_dev->b, 0x43, _regval);
}

static inline int fat32_ebpb_vsn_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_vsn_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint32_t _regval = mackerel_read_addr_32(_dev->b, 0x43);
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register vsn (Volume serial number): ");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx32 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

/*
 * Register array vlb: Volume label
 * Type: fat32_ebpb.uint8 (primitive type)
 */
static const size_t fat32_ebpb_vlb_length = 11;
static inline uint8_t fat32_ebpb_vlb_rawrd(__DN(t) *_dev, int _i) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_vlb_rawrd(__DN(t) *_dev, int _i)
{
    return(mackerel_read_addr_8(_dev->b, 0x47 + (_i * (8 / 8))));
}

static inline uint8_t fat32_ebpb_vlb_rd(__DN(t) *_dev, int _i) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_vlb_rd(__DN(t) *_dev, int _i)
{
    return(mackerel_read_addr_8(_dev->b, 0x47 + (_i * (8 / 8))));
}

static inline void fat32_ebpb_vlb_rawwr(__DN(t) *_dev, int _i, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_vlb_rawwr(__DN(t) *_dev, int _i, uint8_t _regval)
{
    mackerel_write_addr_8(_dev->b, 0x47 + (_i * (8 / 8)), _regval);
}

static inline void fat32_ebpb_vlb_wr(__DN(t) *_dev, int _i, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_vlb_wr(__DN(t) *_dev, int _i, uint8_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_8(_dev->b, 0x47 + (_i * (8 / 8)), _regval);
}

static inline int fat32_ebpb_vlb_pri(char *_s, size_t _size, __DN(t) *_dev, int _i) __attribute__ ((always_inline));
static inline int fat32_ebpb_vlb_pri(char *_s, size_t _size, __DN(t) *_dev, int _i)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint8_t _regval = mackerel_read_addr_8(_dev->b, 0x47 + (_i * (8 / 8)));
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register %s[%d] (%s): ", "vlb", _i, "Volume label");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx8 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

static inline int fat32_ebpb_vlb_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_vlb_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    int _i;
    for( _i = 0; _i < 11; _i++) {
        _avail = ((_r > _size) ? 0 : (_size - _r));
        _rc = fat32_ebpb_vlb_pri(_s + _r, _avail, _dev, _i);
        if ((_rc > 0) && (_rc < _avail)) {
            _r = (_r + _rc);
        }
    }
    return(_r);
}

/*
 * Register array fst: File system type, informative
 * Type: fat32_ebpb.uint8 (primitive type)
 */
static const size_t fat32_ebpb_fst_length = 8;
static inline uint8_t fat32_ebpb_fst_rawrd(__DN(t) *_dev, int _i) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_fst_rawrd(__DN(t) *_dev, int _i)
{
    return(mackerel_read_addr_8(_dev->b, 0x52 + (_i * (8 / 8))));
}

static inline uint8_t fat32_ebpb_fst_rd(__DN(t) *_dev, int _i) __attribute__ ((always_inline));
static inline uint8_t fat32_ebpb_fst_rd(__DN(t) *_dev, int _i)
{
    return(mackerel_read_addr_8(_dev->b, 0x52 + (_i * (8 / 8))));
}

static inline void fat32_ebpb_fst_rawwr(__DN(t) *_dev, int _i, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_fst_rawwr(__DN(t) *_dev, int _i, uint8_t _regval)
{
    mackerel_write_addr_8(_dev->b, 0x52 + (_i * (8 / 8)), _regval);
}

static inline void fat32_ebpb_fst_wr(__DN(t) *_dev, int _i, uint8_t _regval) __attribute__ ((always_inline));
static inline void fat32_ebpb_fst_wr(__DN(t) *_dev, int _i, uint8_t _regval)
{
    // No MB0 or RSVD fields present
    // No MB1 fields present
    // No pre-read of register required
    mackerel_write_addr_8(_dev->b, 0x52 + (_i * (8 / 8)), _regval);
}

static inline int fat32_ebpb_fst_pri(char *_s, size_t _size, __DN(t) *_dev, int _i) __attribute__ ((always_inline));
static inline int fat32_ebpb_fst_pri(char *_s, size_t _size, __DN(t) *_dev, int _i)
{
    int _r = 0;
    int _avail;
    int _rc;
    uint8_t _regval = mackerel_read_addr_8(_dev->b, 0x52 + (_i * (8 / 8)));
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Register %s[%d] (%s): ", "fst", _i, "File system type, informative");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "\t%" PRIx8 "\n", _regval);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

static inline int fat32_ebpb_fst_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_fst_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    int _i;
    for( _i = 0; _i < 8; _i++) {
        _avail = ((_r > _size) ? 0 : (_size - _r));
        _rc = fat32_ebpb_fst_pri(_s + _r, _avail, _dev, _i);
        if ((_rc > 0) && (_rc < _avail)) {
            _r = (_r + _rc);
        }
    }
    return(_r);
}

static inline int fat32_ebpb_pr(char *_s, size_t _size, __DN(t) *_dev) __attribute__ ((always_inline));
static inline int fat32_ebpb_pr(char *_s, size_t _size, __DN(t) *_dev)
{
    int _r = 0;
    int _avail;
    int _rc;
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "-------------------------\n");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "Dump of device fat32_ebpb (FAT32 Extended BIOS Parameter Block (EBPB)):\n");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_spf_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_flgs_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_vrsn_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_rtst_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_fsis_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_bbss_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_ldn_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_rsv_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_ebs_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_vsn_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_vlb_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = fat32_ebpb_fst_pr(_s + _r, _avail, _dev);
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "End of dump of device fat32_ebpb\n");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    _avail = ((_r > _size) ? 0 : (_size - _r));
    _rc = snprintf(_s + _r, _avail, "-------------------------\n");
    if ((_rc > 0) && (_rc < _avail)) {
        _r = (_r + _rc);
    }
    return(_r);
}

#undef __DN
#endif // __fat32_ebpb_DEV_H
