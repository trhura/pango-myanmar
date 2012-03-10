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
 * myctype.h - Burmese Unicode character classifications
 * Created: 2011-01-07
 */

#ifndef __MYCTYPE_H__
#define __MYCTYPE_H__

#include <glib.h>

/**
 * @file   myctype.h
 * @brief  Burmese Unicode character classifications
 */

int my_wcismyanmar (gunichar wc);

/* Burmese Character Classification */
int my_wcismyconsonant           (gunichar wc);
int my_wcismymedial              (gunichar wc);
int my_wcismyvowel               (gunichar wc);
int my_wcismytone                (gunichar wc);
int my_wcismydigit               (gunichar wc);
int my_wcismypunct               (gunichar wc);
int my_wcismydiac                (gunichar wc);
int my_wcismyindependvowel       (gunichar wc);
int my_wcismyindependsymbol      (gunichar wc);
int my_wcismyletter              (gunichar wc);
int my_wcismymark                (gunichar wc);

#endif  /* __MYCTYPE_H__ */

