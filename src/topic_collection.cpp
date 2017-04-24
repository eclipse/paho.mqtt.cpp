// topic_collection.cpp

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


#include "mqtt/topic_collection.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

topic_collection::topic_collection(const string& str) : coll_{ str }
{
	update_c_arr();
}

topic_collection::topic_collection(string&& str) : coll_{ std::move(str) }
{
	update_c_arr();
}

topic_collection::topic_collection(const collection_type& vec) : coll_{ vec }
{
	update_c_arr();
}

topic_collection::topic_collection(collection_type&& vec) : coll_{ std::move(vec) }
{
	update_c_arr();
}

topic_collection::topic_collection(const topic_collection& coll) : coll_ { coll.coll_ }
{
	update_c_arr();
}

topic_collection::topic_collection(std::initializer_list<string> sl)
{
	for (const auto& s : sl)
		push_back(s);
}

topic_collection::topic_collection(std::initializer_list<const char*> sl)
{
	for (const auto& s : sl)
		push_back(string(s));
}

void topic_collection::update_c_arr()
{
	cArr_.clear();
	cArr_.reserve(coll_.size());
	for (const auto& s : coll_)
		cArr_.push_back(const_cast<char*>(s.c_str()));
}

topic_collection& topic_collection::operator=(const topic_collection& coll)
{
	coll_ = coll.coll_;
	update_c_arr();
	return *this;
}

void topic_collection::push_back(const string& str)
{
	size_t i = coll_.size();
	coll_.push_back(str);
	cArr_.push_back(const_cast<char*>(coll_[i].c_str()));
}


void topic_collection::push_back(string&& str)
{
	size_t i = coll_.size();
	coll_.push_back(str);
	cArr_.push_back(const_cast<char*>(coll_[i].c_str()));
}


void topic_collection::clear()
{
	coll_.clear();
	cArr_.clear();
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



