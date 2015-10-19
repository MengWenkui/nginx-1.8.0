
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct {
    void             *value;  /* key ��Ӧ��ֵ */
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


typedef struct {
    ngx_hash_elt_t  **buckets;  /* bucket ָ������ */
    ngx_uint_t        size;     /* �����С */
} ngx_hash_t;


typedef struct {
    ngx_hash_t        hash;
    void             *value;
} ngx_hash_wildcard_t;


typedef struct {
    ngx_str_t         key;		 /* hashkey */
    ngx_uint_t        key_hash;  /* hashkey ֵ */
    void             *value;
} ngx_hash_key_t;


typedef ngx_uint_t (*ngx_hash_key_pt) (u_char *data, size_t len);


typedef struct {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


typedef struct {
	
    ngx_hash_t       *hash;
    /* ָ����ַ�������hash ֵ��hash ������nginx ��Դ�������ṩ��Ĭ�ϵ�ʵ�ֺ��� ngx_hash_key_lc */
    ngx_hash_key_pt   key;
	/* hash ���е�Ͱ�ĸ��� */
    ngx_uint_t        max_size;
    /* ÿ��Ͱ��������ƴ�С����λ���ֽ� */
    ngx_uint_t        bucket_size;
	/* hash������ */
    char             *name;
    /* ��hash ������ڴ�ʹ�õ�pool */
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_KEY     1
#define NGX_HASH_READONLY_KEY     2


typedef struct {
	/* hash size */
    ngx_uint_t        hsize; 

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
    
	/* ������з�ͨ���key ������ */
    ngx_array_t       keys;			
     /* ���Ǹ���ά���飬��һ��ά�ȴ������bucket �ı�ţ���ôkeys_hash[i] �д�ŵ�
�����е�key �������hash ֵ��hsize ȡģ�Ժ��ֵΪi ��key */
    ngx_array_t      *keys_hash;   
	/* ��ǰ��ͨ���key ����������Ժ��ֵ�����磺��*.abc.com������������Ժ󣬱�
�ɡ�com.abc.��������ڴ������� */
    ngx_array_t       dns_wc_head;
    /* ��ֵ�ڵ��õĹ�������������ͼ���Ƿ��г�ͻ��ǰ��ͨ�����key ֵ��Ҳ
�����Ƿ����ظ�*/
    ngx_array_t      *dns_wc_head_hash;
	/* ��ź���ͨ���key ����������Ժ��ֵ�����磺��mail.xxx.*������������Ժ󣬱�
�ɡ�mail.xxx.��������ڴ������� */
    ngx_array_t       dns_wc_tail;
    /* ��ֵ�ڵ��õĹ�������������ͼ���Ƿ��г�ͻ�ĺ���ͨ�����key ֵ��Ҳ��
���Ƿ����ظ� */
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_keys_arrays_t;


typedef struct {
    ngx_uint_t        hash;
    ngx_str_t         key;
    ngx_str_t         value;
    u_char           *lowcase_key;
} ngx_table_elt_t;


void *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t key, u_char *name, size_t len);
void *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);

#define ngx_hash(key, c)   ((ngx_uint_t) key * 31 + c)
ngx_uint_t ngx_hash_key(u_char *data, size_t len);
ngx_uint_t ngx_hash_key_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_key(ngx_hash_keys_arrays_t *ha, ngx_str_t *key,
    void *value, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
