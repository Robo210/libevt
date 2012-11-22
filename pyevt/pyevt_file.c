/*
 * Python object definition of the libevt file
 *
 * Copyright (c) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyevt.h"
#include "pyevt_codepage.h"
#include "pyevt_file.h"
#include "pyevt_file_object_io_handle.h"
#include "pyevt_libbfio.h"
#include "pyevt_libcerror.h"
#include "pyevt_libclocale.h"
#include "pyevt_libcstring.h"
#include "pyevt_libevt.h"
#include "pyevt_python.h"
#include "pyevt_record.h"
#include "pyevt_records.h"

#if !defined( LIBEVT_HAVE_BFIO )
LIBEVT_EXTERN \
int libevt_file_open_file_io_handle(
     libevt_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevt_error_t **error );
#endif

PyMethodDef pyevt_file_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyevt_file_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the file to abort the current activity" },

	/* Functions to access the file */

	{ "open",
	  (PyCFunction) pyevt_file_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, access_flags) -> None\n"
	  "\n"
	  "Opens a file" },

	{ "open_file_object",
	  (PyCFunction) pyevt_file_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(file_object, access_flags) -> None\n"
	  "\n"
	  "Opens a file using a file-like object" },

	{ "close",
	  (PyCFunction) pyevt_file_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a file" },

	{ "get_ascii_codepage",
	  (PyCFunction) pyevt_file_get_ascii_codepage,
	  METH_NOARGS,
	  "get_ascii_codepage() -> String\n"
	  "\n"
	  "Returns the codepage used for ASCII strings in the file" },

	{ "set_ascii_codepage",
	  (PyCFunction) pyevt_file_set_ascii_codepage,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_ascii_codepage(codepage) -> None\n"
	  "\n"
	  "Set the codepage used for ASCII strings in the file\n"
	  "Expects the codepage to be a String containing a Python codec definition" },

	/* Functions to access the records */

	{ "get_number_of_records",
	  (PyCFunction) pyevt_file_get_number_of_records,
	  METH_NOARGS,
	  "get_number_of_records() -> Integer\n"
	  "\n"
	  "Retrieves the number of records" },

	{ "get_record",
	  (PyCFunction) pyevt_file_get_record,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_record(record_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific record" },

	{ "get_number_of_recovered_records",
	  (PyCFunction) pyevt_file_get_number_of_recovered_records,
	  METH_NOARGS,
	  "get_number_of_recovered_records() -> Integer\n"
	  "\n"
	  "Retrieves the number of recovered records" },

	{ "get_recovered_record",
	  (PyCFunction) pyevt_file_get_recovered_record,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_recovered_record(record_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific recovered record" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyevt_file_object_get_set_definitions[] = {

	{ "ascii_codepage",
	  (getter) pyevt_file_get_ascii_codepage,
	  (setter) pyevt_file_set_ascii_codepage,
	  "The codepage used for ASCII strings in the file",
	  NULL },

	{ "number_of_records",
	  (getter) pyevt_file_get_number_of_records,
	  (setter) 0,
	  "The number of records",
	  NULL },

	{ "records",
	  (getter) pyevt_file_get_records,
	  (setter) 0,
	  "The records",
	  NULL },

	{ "number_of_recovered_records",
	  (getter) pyevt_file_get_number_of_recovered_records,
	  (setter) 0,
	  "The number of records",
	  NULL },

	{ "recoverd_records",
	  (getter) pyevt_file_get_recovered_records,
	  (setter) 0,
	  "The recovered records",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyevt_file_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyevt.file",
	/* tp_basicsize */
	sizeof( pyevt_file_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyevt_file_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyevt file object (wraps libevt_file_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyevt_file_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyevt_file_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyevt_file_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new file object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_new(
           PyObject *self )
{
	pyevt_file_t *pyevt_file = NULL;
	static char *function      = "pyevt_file_new";

	pyevt_file = PyObject_New(
	              struct pyevt_file,
	              &pyevt_file_type_object );

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		return( NULL );
	}
	if( pyevt_file_init(
	     pyevt_file ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pyevt_file );

on_error:
	if( pyevt_file != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyevt_file );
	}
	return( NULL );
}

/* Creates a new file object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyevt_file = NULL;

	pyevt_file = pyevt_file_new(
	              self );

	pyevt_file_open(
	 (pyevt_file_t *) pyevt_file,
	 arguments,
	 keywords );

	return( pyevt_file );
}

/* Intializes a file object
 * Returns 0 if successful or -1 on error
 */
int pyevt_file_init(
     pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	static char *function    = "pyevt_file_init";
	libcerror_error_t *error = NULL;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	/* Make sure libevt file is set to NULL
	 */
	pyevt_file->file = NULL;

	if( libevt_file_initialize(
	     &( pyevt_file->file ),
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a file object
 */
void pyevt_file_free(
      pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevt_file_free";
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return;
	}
	if( pyevt_file->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file - missing ob_type.",
		 function );

		return;
	}
	if( pyevt_file->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyevt_file->file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file - missing libevt file.",
		 function );

		return;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_free(
	          &( pyevt_file->file ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free libevt file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free libevt file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );
	}
	pyevt_file->ob_type->tp_free(
	 (PyObject*) pyevt_file );
}

/* Signals the file to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_signal_abort(
           pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevt_file_signal_abort";
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_signal_abort(
	          pyevt_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to signal abort.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to signal abort.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_open(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	char *filename              = NULL;
	static char *keyword_list[] = { "filename", "access_flags", NULL };
	static char *function       = "pyevt_file_open";
	int access_flags            = 0;
	int result                  = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s|i",
	     keyword_list,
	     &filename,
	     &access_flags ) == 0 )
        {
                return( NULL );
        }
	/* Default to read-only if no access flags were provided
	 */
	if( access_flags == 0 )
	{
		access_flags = libevt_get_access_flags_read();
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_open(
	          pyevt_file->file,
                  filename,
                  access_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_open_file_object(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	static char *keyword_list[]      = { "file_object", "access_flags", NULL };
	static char *function            = "pyevt_file_open_file_object";
	int access_flags                 = 0;
	int result                       = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|i",
	     keyword_list,
	     &file_object,
	     &access_flags ) == 0 )
        {
                return( NULL );
        }
	/* Default to read-only if no access flags were provided
	 */
	if( access_flags == 0 )
	{
		access_flags = libevt_get_access_flags_read();
	}
	if( pyevt_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_open_file_io_handle(
	          pyevt_file->file,
                  file_io_handle,
                  access_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Closes a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_close(
           pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevt_file_close";
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_close(
	          pyevt_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_ascii_codepage(
           pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	const char *codepage_string = NULL;
	static char *function       = "pyevt_file_get_ascii_codepage";
	int ascii_codepage          = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libevt_file_get_ascii_codepage(
	     pyevt_file->file,
	     &ascii_codepage,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	codepage_string = pyevt_codepage_to_string(
	                   ascii_codepage );

	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported ASCII codepage: %d.",
		 function,
		 ascii_codepage );

		return( NULL );
	}
	string_object = PyString_FromString(
	                 codepage_string );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert codepage string into string object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_set_ascii_codepage(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error      = NULL;
	char *codepage_string         = NULL;
	static char *keyword_list[]   = { "codepage", NULL };
	static char *function         = "pyevt_file_set_ascii_codepage";
	size_t codepage_string_length = 0;
	uint32_t feature_flags        = 0;
	int ascii_codepage            = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &codepage_string ) == 0 )
        {
                return( NULL );
        }
	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid codepage string.",
		 function );

		return( NULL );
	}
	codepage_string_length = libcstring_narrow_string_length(
	                          codepage_string );

	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	if( libclocale_codepage_copy_from_string(
	     &ascii_codepage,
	     codepage_string,
	     codepage_string_length,
	     feature_flags,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( libevt_file_set_ascii_codepage(
	     pyevt_file->file,
	     ascii_codepage,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to set ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to set ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the number of records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_number_of_records(
           pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevt_file_get_number_of_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_get_number_of_records(
	          pyevt_file->file,
	          &number_of_records,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_records ) );
}

/* Retrieves a specific record by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_record_by_index(
           pyevt_file_t *pyevt_file,
           int record_index )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	libevt_record_t *record = NULL;
	PyObject *record_object  = NULL;
	static char *function    = "pyevt_file_get_record_by_index";
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_get_record(
	          pyevt_file->file,
	          record_index,
	          &record,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve record: %d.",
			 function,
			 record_index );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve record: %d.\n%s",
			 function,
			 record_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	record_object = pyevt_record_new(
	                 record,
	                 pyevt_file );

	if( record_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create record object.",
		 function );

		goto on_error;
	}
	return( record_object );

on_error:
	if( record != NULL )
	{
		libevt_record_free(
		 &record,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific record
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_record(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *record_object     = NULL;
	static char *keyword_list[] = { "record_index", NULL };
	static char *function       = "pyevt_file_get_record";
	int record_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &record_index ) == 0 )
        {
		return( NULL );
        }
	record_object = pyevt_file_get_record_by_index(
	                 pyevt_file,
	                 record_index );

	return( record_object );
}

/* Retrieves a records sequence and iterator object for the records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_records(
           pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	PyObject *records_object = NULL;
	static char *function    = "pyevt_file_get_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_get_number_of_records(
	          pyevt_file->file,
	          &number_of_records,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	records_object = pyevt_records_new(
	                  pyevt_file,
	                  &pyevt_file_get_record_by_index,
	                  number_of_records );

	if( records_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create records object.",
		 function );

		return( NULL );
	}
	return( records_object );
}

/* Retrieves the number of recovered records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_number_of_recovered_records(
           pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevt_file_get_number_of_recovered_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_get_number_of_recovered_records(
	          pyevt_file->file,
	          &number_of_records,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recovered records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recovered records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_records ) );
}

/* Retrieves a specific recovered record by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_recovered_record_by_index(
           pyevt_file_t *pyevt_file,
           int record_index )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	libevt_record_t *record = NULL;
	PyObject *record_object  = NULL;
	static char *function    = "pyevt_file_get_recovered_record_by_index";
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_get_recovered_record(
	          pyevt_file->file,
	          record_index,
	          &record,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve recovered record: %d.",
			 function,
			 record_index );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve recovered record: %d.\n%s",
			 function,
			 record_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	record_object = pyevt_record_new(
	                 record,
	                 pyevt_file );

	if( record_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create recovered record object.",
		 function );

		goto on_error;
	}
	return( record_object );

on_error:
	if( record != NULL )
	{
		libevt_record_free(
		 &record,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific recovered record
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_recovered_record(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *record_object     = NULL;
	static char *keyword_list[] = { "record_index", NULL };
	static char *function       = "pyevt_file_get_recovered_record";
	int record_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &record_index ) == 0 )
        {
		return( NULL );
        }
	record_object = pyevt_file_get_recovered_record_by_index(
	                 pyevt_file,
	                 record_index );

	return( record_object );
}

/* Retrieves a records sequence and iterator object for the recoverd records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_file_get_recovered_records(
           pyevt_file_t *pyevt_file )
{
	char error_string[ PYEVT_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	PyObject *records_object = NULL;
	static char *function    = "pyevt_file_get_recovered_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevt_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevt_file_get_number_of_recovered_records(
	          pyevt_file->file,
	          &number_of_records,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVT_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recovered records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recovered records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	records_object = pyevt_records_new(
	                  pyevt_file,
	                  &pyevt_file_get_recovered_record_by_index,
	                  number_of_records );

	if( records_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create records object.",
		 function );

		return( NULL );
	}
	return( records_object );
}

