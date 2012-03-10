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
 * myctype.c - Burmese Unicode character classifications
 * Created: 2011-01-07
 */

#include "myanmar.h"
#include "myctype.h"

/**
 * @brief       Determines whether a unicode character is Burmese character.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese character
 */
int my_wcismyanmar (gunichar wc)
{
    return (wc >= MYANMAR_LETTER_KA && wc <= MYANMAR_SYMBOL_GENITIVE);
}

/**
 * @brief       Determines whether a unicode character is Burmese consonant.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese consonant
 */
int my_wcismyconsonant	(gunichar wc)
{
    return ((wc >= MYANMAR_LETTER_KA) && (wc <= MYANMAR_LETTER_A));
//            wc == MYANMAR_LETTER_GREAT_SA);
}

/**
 * @brief       Determines whether a unicode character is Burmese medial.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese medial
 */
int my_wcismymedial     (gunichar wc)
{
    return (wc >= MYANMAR_CONSONANT_SIGN_MEDIAL_YA) &&
        (wc <= MYANMAR_CONSONANT_SIGN_MEDIAL_HA);
}

/**
 * @brief       Determines whether a unicode character is Burmese vowel.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese vowel
 */
int my_wcismyvowel      (gunichar wc)
{
    return ((wc >= MYANMAR_VOWEL_SIGN_TALL_AA) && (wc <= MYANMAR_VOWEL_SIGN_AI));
}

/**
 * @brief       Determines whether a unicode character is Burmese tone.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese tone
 */
int my_wcismytone       (gunichar wc)
{
    return (wc == MYANMAR_SIGN_DOT_BELOW || wc == MYANMAR_SIGN_VISARGA);
}

/**
 * @brief       Determines whether a unicode character is Burmese digit.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese digit
 */
int my_wcismydiac       (gunichar wc)
{
    return (my_wcismyvowel (wc) || my_wcismymedial (wc) || my_wcismytone (wc) ||
            wc == MYANMAR_SIGN_ANUSVARA || wc == MYANMAR_SIGN_ASAT);
}

/**
 * @brief       Determines whether a unicode character is Burmese digit.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese digit
 */
int my_wcismydigit      (gunichar wc)
{
    return (wc >= MYANMAR_DIGIT_ZERO  && wc <= MYANMAR_DIGIT_NINE);
}

/**
 * @brief       Determines whether a unicode character is Burmese punctuation.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese punctuation
 */
int my_wcismypunct      (gunichar wc)
{
    return (wc == MYANMAR_SIGN_LITTLE_SECTION || wc == MYANMAR_SIGN_SECTION);
}

/**
 * @brief       Determines whether a unicode character is Burmese independent vowel.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese independent vowel
 */
int my_wcismyindependvowel  (gunichar wc)
{
    return (wc >= MYANMAR_LETTER_I && wc <= MYANMAR_LETTER_E) || wc == MYANMAR_LETTER_O || wc == MYANMAR_LETTER_AU;
}

/**
 * @brief       Determines whether a unicode character is Burmese independent symbol.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese independent symbol
 */
int my_wcismyindependsymbol (gunichar wc)
{
    return (wc >= MYANMAR_SYMBOL_LOCATIVE && wc <= MYANMAR_SYMBOL_GENITIVE);
}

/**
 * @brief       Determines whether a unicode character is Burmese letter.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese letter.
 */
int my_wcismyletter (gunichar wc)
{
    return (my_wcismyconsonant (wc) ||
            my_wcismyindependvowel (wc) ||
            wc == MYANMAR_SYMBOL_AFOREMENTIONED);
}

/**
 * @brief       Determines whether a unicode character is Burmese mark.
 * @param       wc : a unicode character
 * @returns     0 when wc is not a Burmese mark
 */
int my_wcismymark (gunichar wc)
{
    return (my_wcismymedial (wc) ||
            my_wcismyvowel  (wc) ||
            (wc >= MYANMAR_SIGN_ANUSVARA && wc<= MYANMAR_SIGN_ASAT));
}
/*
vi:ts=4:ai:expandtab
*/
