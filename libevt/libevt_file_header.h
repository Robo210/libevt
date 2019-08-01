/*
 * File header functions
 *
 * Copyright (C) 2011-2019, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEVT_FILE_HEADER_H )
#define _LIBEVT_FILE_HEADER_H

#include <common.h>
#include <types.h>

#include "libevt_libbfio.h"
#include "libevt_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevt_file_header libevt_file_header_t;

struct libevt_file_header
{
	/* (header) size
	 */
	uint32_t size;

	/* Major format version
	 */
	uint32_t major_format_version;

	/* Minor format version
	 */
	uint32_t minor_format_version;

	/* First (oldest) record offset
	 */
	uint32_t first_record_offset;

	/* End of file record offset
	 */
	uint32_t end_of_file_record_offset;

	/* File flags
	 */
	uint32_t file_flags;

	/* Copy of (header) size
	 */
	uint32_t copy_of_size;
};

int libevt_file_header_initialize(
     libevt_file_header_t **file_header,
     libcerror_error_t **error );

int libevt_file_header_free(
     libevt_file_header_t **file_header,
     libcerror_error_t **error );

int libevt_file_header_read_data(
     libevt_file_header_t *file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libevt_file_header_read_file_io_handle(
     libevt_file_header_t *file_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_FILE_HEADER_H ) */
