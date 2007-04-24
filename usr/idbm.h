/*
 * iSCSI Discovery Database Library
 *
 * Copyright (C) 2004 Dmitry Yusupov, Alex Aizman
 * Copyright (C) 2006 Mike Christie
 * Copyright (C) 2006 Red Hat, Inc. All rights reserved.
 * maintained by open-iscsi@@googlegroups.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * See the file COPYING included with this distribution for more details.
 */

#ifndef IDBM_H
#define IDBM_H

#include <sys/types.h>
#include "initiator.h"
#include "config.h"

#define HASH_MAXLEN	NI_MAXHOST + NI_MAXSERV

#define TYPE_INT	0
#define TYPE_INT_O	1
#define TYPE_STR	2
#define MAX_KEYS	256   /* number of keys total(including CNX_MAX) */
#define NAME_MAXVAL	128   /* the maximum length of key name */
#define VALUE_MAXVAL	256   /* the maximum length of 223 bytes in the RFC. */
#define OPTS_MAXVAL	8
typedef struct recinfo {
	int		type;
	char		name[NAME_MAXVAL];
	char		value[VALUE_MAXVAL];
	void		*data;
	int		data_len;
	int		visible;
	char*		opts[OPTS_MAXVAL];
	int		numopts;
} recinfo_t;

typedef struct idbm {
	void		*discdb;
	void		*nodedb;
	char		*configfile;
	int             refs;
	iface_rec_t	irec_iface[ISCSI_IFACE_MAX];
	recinfo_t	iinfo[MAX_KEYS];
	node_rec_t	nrec;
	recinfo_t	ninfo[MAX_KEYS];
	discovery_rec_t	drec_st;
	recinfo_t	dinfo_st[MAX_KEYS];
	discovery_rec_t	drec_slp;
	recinfo_t	dinfo_slp[MAX_KEYS];
	discovery_rec_t	drec_isns;
	recinfo_t	dinfo_isns[MAX_KEYS];
} idbm_t;

struct db_set_param {
	char *name;
	char *value;
	struct idbm  *db;
};

extern char* get_iscsi_initiatorname(char *pathname);
extern char* get_iscsi_initiatoralias(char *pathname);
extern idbm_t* idbm_init(char *configfile);
extern void idbm_node_setup_from_conf(idbm_t *db, node_rec_t *rec);
extern void idbm_terminate(idbm_t *db);
extern int idbm_print_node(void *data, node_rec_t *rec);
extern int idbm_for_each_node(idbm_t *db, void *data,
			     int (* fn)(void *data, node_rec_t *rec));
extern int idbm_for_each_portal(char *buf, char *targetname, idbm_t *db,
				void *data,
				int (* fn)(void *data, node_rec_t *rec));
extern int idbm_for_each_iface(char *buf, char *targetname, char *ip, int port,
				idbm_t *db, void *data,
				int (* fn)(void *data, node_rec_t *rec));
extern int idbm_print_discovery(idbm_t *db, discovery_rec_t *rec, int show);
extern int idbm_print_all_discovery(idbm_t *db);
extern int idbm_delete_discovery(idbm_t *db, discovery_rec_t *rec);
extern void idbm_node_setup_defaults(node_rec_t *rec);
extern int idbm_delete_node(void *data, node_rec_t *rec);
extern int idbm_add_node(idbm_t *db, node_rec_t *newrec, discovery_rec_t *drec);
extern int idbm_add_nodes(idbm_t *db, node_rec_t *newrec,
			  discovery_rec_t *drec);
extern void idbm_new_discovery(idbm_t *db, discovery_rec_t *drec);
extern void idbm_sendtargets_defaults(idbm_t *db,
		      struct iscsi_sendtargets_config *cfg);
extern void idbm_slp_defaults(idbm_t *db, struct iscsi_slp_config *cfg);
extern int idbm_discovery_read(idbm_t *db, discovery_rec_t *rec, char *addr,
				int port);
extern int idbm_node_read(idbm_t *db, node_rec_t *out_rec, char *target_name,
			 char *addr, int port, char *iface);
extern int idbm_node_set_param(void *data, node_rec_t *rec);
#endif /* IDBM_H */
