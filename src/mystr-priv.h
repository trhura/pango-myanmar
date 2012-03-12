/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * libburmese - Burmese Language Support Library
 * Copyright (C) 2011  Thura Hlaing <trhura@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * mystr-priv.h - private file for mystr.h
 * Created: 2011-01-08
 */

#ifndef BURMESE_MYSTR_PRIVATE_H
#define BURMESE_MYSTR_PRIVATE_H
#include <wchar.h>

struct _ClusterIter {
	wchar_t* string;
	wchar_t* position;
	int		 length;
};

#endif
