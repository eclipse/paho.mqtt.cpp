/////////////////////////////////////////////////////////////////////////////
/// @file buffer_ref.h
/// Buffer reference type for the Paho MQTT C++ library.
/// @date April 18, 2017
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2017 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_buffer_ref_h
#define __mqtt_buffer_ref_h

#include "mqtt/types.h"
#include <iostream>
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * A reference object for holding immutable data buffers, with cheap copy
 * semantics.
 *
 * Each object of this class contains a reference-counted pointer to an
 * immutable data buffer. Onjects can be copied freely and easily since all
 * instances promise not to modify the contents of the buffer.
 *
 * The buffer is immutable but the reference itself acts like a normal
 * variable. It can be reassigned to point to a different buffer.
 */
template <typename T>
class buffer_ref
{
public:
	/**
	 * The underlying type for the buffer.
	 * Normally byte-wide data (char or uint8_t) for Paho.
	 */
	using value_type = T;
	/**
	 * The type for the buffer.
	 * We use basic_string for compatibility with string data.
	 */
	using blob = std::basic_string<value_type>;
	/**
	 *  The pointer we use.
	 *  Note that it is a pointer to a _const_ blob.
	 */
	using pointer_type = std::shared_ptr<const blob>;

private:
	/** Our data is a shared pointer to a const buffer */
	pointer_type data_;

public:
	/**
	 * Copy constructor only copies a shared pointer.
	 * @param buf Another buffer reference.
	 */
	buffer_ref(const buffer_ref& buf) =default;
	/**
	 * Move constructor only moves a shared pointer.
	 * @param buf Another buffer reference.
	 */
	buffer_ref(buffer_ref&& buf) =default;
	/**
	 * Creates a reference to a new buffer by copying data.
	 * @param b A string from which to create a new buffer.
	 */
	buffer_ref(const blob& b) : data_{new blob(b)} {}
	/**
	 * Creates a reference to a new buffer by moving a string into the
	 * buffer.
	 * @param b A string from which to create a new buffer.
	 */
	buffer_ref(blob&& b) : data_{new blob(std::move(b))} {}
	/**
	 * Creates a reference to an existing buffer by copying the shared
	 * pointer.
	 * Note that it is up to the caller to insure that there are no mutable
	 * references to the buffer.
	 * @param p A shared pointer to a string.
	 */
	buffer_ref(const pointer_type& p) : data_(p) {}
	/**
	 * Creates a reference to an existing buffer by moving the shared
	 * pointer.
	 * Note that it is up to the caller to insure that there are no mutable
	 * references to the buffer.
	 * @param p A shared pointer to a string.
	 */
	buffer_ref(pointer_type&& p) : data_(std::move(p)) {}
	/**
	 * Creates a reference to a new buffer containing a copy of the data.
	 * @param buf The memory to copy
	 * @param n The number of bytes to copy.
	 */
	buffer_ref(const value_type* buf, size_t n) : data_{new blob(buf,n)} {}
	/**
	 * Creates a reference to a new buffer containing a copy of the
	 * NUL-terminated char array.
	 * @param buf A NUL-terminated char array (C string).
	 */
	buffer_ref(const char* buf) : buffer_ref(reinterpret_cast<const value_type*>(buf), std::strlen(buf)) {
		static_assert(sizeof(char) == sizeof(T), "can only use C arr with char or byte buffers");
	}

	/*
	  There are two ways we can handle default construction. One is that we
	  allow the data_ pointer to use its default and get a null value. That
	  would be quick and efficient, but leave us with several unsafe
	  operations that would require null/empty checks for us and/or the
	  user.
	  The other option is for the default constructor to create and point to
	  an empty string. Then all operations are safe and valid, at the
	  expense of some default construction performance.
	  It's a tough choice. The first sounds dangerous, but is analagous to
	  reference types in other languages that default to a "nil" of "null"
	  value. But the latter creates something closer to a drop-in
	  replacement for a C++ string
	*/
	#if 1
		buffer_ref() =default;
		explicit operator bool() const { return bool(data_); }
		bool empty() const { return !data_ || data_->empty(); }
	#else
		buffer_ref() : data_{new blob()} {}
		bool empty() const { return data_->empty(); }
	#endif

	/**
	 * Copy the reference to the buffer.
	 * @param rhs Another buffer
	 * @return A reference to this object
	 */
	buffer_ref& operator=(const buffer_ref& rhs) =default;
	/**
	 * Move a reference to a buffer.
	 * @param rhs The other reference to move.
	 * @return A reference to this object.
	 */
	buffer_ref& operator=(buffer_ref&& rhs) =default;
	/**
	 * Copy a string into this object, creating a new buffer.
	 * Modifies the reference for this object, pointing it to a
	 * newly-created buffer. Other references to the old object remain
	 * unchanges, so this follows copy-on-write semantics.
	 * @param b A new blob/string to copy.
	 * @return A reference to this object.
	 */
	buffer_ref& operator=(const blob& b) {
		data_.reset(new blob(b));
		return *this;
	}
	/**
	 * Move a string into this object, creating a new buffer.
	 * Modifies the reference for this object, pointing it to a
	 * newly-created buffer. Other references to the old object remain
	 * unchanges, so this follows copy-on-write semantics.
	 * @param b A new blob/string to move.
	 * @return A reference to this object.
	 */
	buffer_ref& operator=(blob&& b) {
		data_.reset(new blob(std::move(b)));
		return *this;
	}
	/**
	 * Copy a NUL-terminated C char array into a new buffer
	 * @param cstr A NUL-terminated C string.
	 * @return A reference to this object
	 */
	buffer_ref& operator=(const char* cstr) {
		static_assert(sizeof(char) == sizeof(T), "can only use C arr with char or byte buffers");
		data_.reset(new blob(reinterpret_cast<const value_type*>(cstr), strlen(cstr)));
		return *this;
	}
	/**
	 * Copy another type of buffer reference to this one.
	 * This can copy a buffer of different types, provided that the size of
	 * the data elements are the same. This is typically used to convert
	 * from char to byte, where the data is the same, but the interpretation
	 * is different. Note that this copies the underlying buffer.
	 * @param rhs A reference to a different type of buffer.
	 * @return A reference to this object.
	 */
	template <typename OT>
	buffer_ref& operator=(const buffer_ref<OT>& rhs) {
		static_assert(sizeof(OT) == sizeof(T), "Can only assign buffers if values the same size");
		data_.reset(new blob(reinterpret_cast<const value_type*>(rhs.data()), rhs.size()));
		return *this;
	}
	/**
	 * Clears the reference to nil.
	 */
	void reset() { data_.reset(); }
	/**
	 * Get a const pointer to the data buffer.
	 * @return A pointer to the data buffer.
	 */
	const value_type* data() const { return data_ ? data_->data() : nullptr; }
	/**
	 * Get the size of the data buffer.
	 * @return The size of the data buffer.
	 */
	size_t size() const { return data_ ? data_->size() : 0; }
	/**
	 * Get the size of the data buffer.
	 * @return The size of the data buffer.
	 */
	size_t length() const { return data_ ? data_->length() : 0; }
	/**
	 * Get the data buffer as a string.
	 * @return The data buffer as a string.
	 */
	//blob to_string() const { return data_ ? (*data_) : blob(); }
	const blob& to_string() const { return *data_; }
	/**
	 * Get the data buffer as NUL-terminated C string.
	 * Note that the reference must be set to call this function.
	 * @return The data buffer as a string.
	 */
	const char* c_str() const { return data_->c_str(); }
	/**
	 * Gets a shared pointer to the (const) data buffer.
	 * @return A shared pointer to the (const) data buffer.
	 */
	const pointer_type& ptr() const { return data_; }
	/**
	 * Gets elemental access to the data buffer (read only)
	 * @param i The index into the buffer.
	 * @return The value at the specified index.
	 */
	const value_type& operator[](size_t i) const { return (*data_)[i]; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const buffer_ref<T>& buf) {
	if (!buf.empty())
		os.write(buf.data(), buf.size());
	return os;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * A refernce to a text buffer.
 */
using string_ref = buffer_ref<char>;

/**
 * A reference to a binary buffer.
 * Note that we're using char for the underlying data type to allow
 * efficient moves to and from std::string's. Using a separate type
 * indicates that the data may be arbitrary binary.
 */
using binary_ref = buffer_ref<char>;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_buffer_ref_h

