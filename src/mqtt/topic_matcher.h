/////////////////////////////////////////////////////////////////////////////
/// @file topic_matcher.h
/// Declaration of MQTT topic_matcher class
/// @date April 23, 2022
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2022 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_topic_matcher_h
#define __mqtt_topic_matcher_h

#include "mqtt/types.h"
#include "mqtt/topic.h"
#include <vector>
#include <map>
#include <forward_list>
#include <initializer_list>
#include <memory>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * This can be used to get an iterator to all items that have a filter that
 * matches a topic. To test against a single filter, see
 * [`TopicFilter`](crate::TopicFilter). This collection is more commonly
 * used when there are a number of filters and each needs to be associated
 * with a particular action or piece of data. Note, though, that a single
 * incoming topic could match against several items in the collection. For
 * example, the topic:
 *     data/temperature/engine
 *
 * Could match against the filters:
 *     data/temperature/# data/+/engine
 *
 * Thus, the collection gives an iterator for the items matching a topic.
 *
 * A common use for this would be to store callbacks to process incoming
 * messages based on topics.
 *
 * This code was adapted from the Eclipse Python `MQTTMatcher` class:
 *
   <https://github.com/eclipse/paho.mqtt.python/blob/master/src/paho/mqtt/matcher.py>
 *
 * which use a prefix tree (trie) to store the values.
 *
 * For example, if you had a `topic_mapper<int>` and you inserted:
 *	    insert({"some/random/topic", 42})
 *      insert({"some/#", 99})
 *      insert({"some/+/topic", 33})
 *
 * The collection would be built like:
 *     "some" -> <null>
 *     	   "random" -> <null>
 *     	       "topic" -> <42>
 * 	       "#" -> <99>
 * 	       "+" -> <null>
 *             "topic" -> <33>
 */
template <typename T>
class topic_matcher
{
public:
	using key_type = string;
	using mapped_type = T;
	using value_type = std::pair<key_type, mapped_type>;
	using reference = value_type;
	using const_reference = const value_type&;

private:
	/**
	 * The nodes of the collection.
	 */
	struct node
	{
		using ptr_t = std::unique_ptr<node>;
		using map_t = std::map<string, ptr_t>;

		/** The value that matches the topic at this node, if any */
		std::unique_ptr<value_type> content;
		/** Child nodes mapped by the next field of the topic */
		map_t children;

		static ptr_t create() {
            // TODO: When available (C++14,17) use
            // std::make_unique<node>();
			return std::unique_ptr<node>(new node{});
		}
	};
	using node_ptr = typename node::ptr_t;
	using node_map = typename node::map_t;

	/** The root node of the collection */
	node_ptr root_;

public:
	class iterator {
		/**
		 * Information about a node to search.
		 */
		struct search_node {
			/** The current node being searched. null means end. */
			node* node_;
			/** The fields of the topic still to be searched. */
			std::forward_list<string> syms_;

			search_node() : node_(nullptr) {}
			search_node(node* nd, const std::forward_list<string>& sy)
				: node_(nd), syms_(sy) {}
			search_node(node* nd, std::forward_list<string>&& sy)
				: node_(nd), syms_(std::move(sy)) {}
		};

		/** The last-found value */
		value_type* pval_;
		/** The current search node */
		search_node snode_;
		/** The nodes still to be checked */
		std::vector<search_node> nodes_;

		/**
		 * Move the next iterator to the next value, or to end(), if none
		 * left.
		 *
		 * This will keep recursing until it finds a matching node that
		 * contains a value or it reaches the end.
		 */
		void next() {
			pval_ = nullptr;

			// Can't move if we're already at the end
			if (!snode_.node_)
				return;

			if (snode_.syms_.empty()) {
				pval_ = snode_.node_->content.get();
			}
			else {
				typename node_map::iterator child;
				auto map_end = snode_.node_->children.end();
				auto sym = snode_.syms_.front();

				if ((child = snode_.node_->children.find(sym)) != map_end) {
					auto syms = snode_.syms_;
					syms.pop_front();
					nodes_.push_back({child->second.get(), std::move(syms)});
				}
				if ((child = snode_.node_->children.find("+")) != map_end) {
					auto syms = snode_.syms_;
					syms.pop_front();
					nodes_.push_back({child->second.get(), std::move(syms)});
				}
				if ((child = snode_.node_->children.find("#")) != map_end) {
					pval_ = child->second->content.get();
				}
			}

			if (!nodes_.empty()) {
				// TODO: List pop_front()?
				snode_ = nodes_.back();
				nodes_.pop_back();
				if (!pval_) {
					// Recurse
					return this->next();
				}
			}
			else {
				snode_ = search_node();
			}
		}

		friend class topic_matcher;

		iterator() : pval_(nullptr) {}
		iterator(value_type* pval) : pval_(pval) {}
		iterator(node* root, const string& topic) : pval_(nullptr) {
			auto v = topic::split(topic);
			std::forward_list<string> syms(v.begin(), v.end());
			snode_ = search_node(root, std::move(syms));
			next();
		}

	public:
		/**
		 * Gets a reference to the current value.
		 * @return A reference to the current value.
		 */
		reference operator*() noexcept {
			return *pval_;
		}
		/**
		 * Gets a const reference to the current value.
		 * @return A const reference to the current value.
		 */
		const_reference operator*() const noexcept {
			return *pval_;
		}
		/**
		 * Get a pointer to the current value.
		 * @return A pointer to the current value.
		 */
		value_type* operator->() noexcept {
			return pval_;
		}
		/**
		 * Get a const pointer to the current value.
		 * @return A const pointer to the current value.
		 */
		const value_type* operator->() const noexcept {
			return pval_;
		}
		/**
		 * Postfix increment operator.
		 * @return An iterator pointing to the previous matching item.
		 */
		iterator operator++(int) noexcept {
			auto tmp = *this;
			this->next();
			return tmp;
		}
		/**
		 * Prefix increment operator.
		 * @return An iterator pointing to the next matching item.
		 */
		iterator& operator++() noexcept {
			this->next();
			return *this;
		}
		/**
		 * Compares two iterators to see if they don't refer to the same
		 * node.
		 *
		 * @param other The other iterator to compare against this one.
		 * @return @em true if they don't match, @em false if they do
		 */
		bool operator!=(const iterator& other) const noexcept {
			// TODO: Is this sufficient in the long run?
			return pval_ != other.pval_ || snode_.node_ != other.snode_.node_;
		}
	};

	/**
	 * A const iterator.
	 */
	class const_iterator : public iterator {
		using base = iterator;

		friend class topic_matcher;
		const_iterator(iterator it) : base(it) {}

	public:
		/**
		 * Gets a const reference to the current value.
		 * @return A const reference to the current value.
		 */
		const_reference operator*() const noexcept {
			return base::operator*();
		}
		/**
		 * Get a const pointer to the current value.
		 * @return A const pointer to the current value.
		 */
		const value_type* operator->() const noexcept {
			return base::operator->();
		}
	};

	/**
	 * Creates  new, empty collection.
	 */
	topic_matcher()
		: root_(node::create()) {}
	/**
	 * Creates a new collection with a list of key/value pairs.
	 *
	 * This can be used to create a connection from a table of entries, as
	 * key/value pairs, like:
	 *
	 *     topic_matcher<int> matcher {
	 *  	   { "#", -1 },
	 *  	   { "some/random/topic", 42 },
	 *  	   { "some/#", 99 }
	 *     }
	 *
	 * @param lst The list of key/value pairs to populate the collection.
	 */
	topic_matcher(std::initializer_list<value_type> lst)
			: root_(node::create()) {
		for (const auto& v : lst) {
			insert(v);
		}
	}
	/**
	 * Inserts a new key/value pair into the collection.
	 * @param val The value to place in the collection.
	 */
	void insert(value_type&& val) {
		auto nd = root_.get();
		auto fields = topic::split(val.first);

		for (auto& field : fields) {
			auto it = nd->children.find(field);
			if (it == nd->children.end()) {
				nd->children[field] = node::create();
				it = nd->children.find(field);
			}
			nd = it->second.get();
		}
        // TODO: When available (C++14,17) use:
        //   std::make_unique<value_type>(std::move(val));
		nd->content = std::unique_ptr<value_type>(new value_type{std::move(val)});
	}
	/**
	 * Inserts a new value into the collection.
	 * @param key The topic/filter entry
	 * @param val The value to associate with that entry.
	 */
	void insert(const value_type& val) {
		value_type v { val };
		this->insert(std::move(v));
	}
	/**
	 * Gets a pointer to the value at the requested key.
	 * @param key The topic/filter entry to find.
	 * @return An iterator to the value if found, @em end() if not found.
	 */
	iterator find(const key_type& key) {
		auto nd = root_.get();
		auto fields = topic::split(key);

		for (auto& field : fields) {
			auto it = nd->children.find(field);
			if (it == nd->children.end())
				return end();

			nd = it->second.get();
		}
		return iterator{ nd->content.get() };
	}
	/**
	 * Gets a const pointer to the value at the requested key.
	 * @param key The topic/filter entry to find.
	 * @return A const pointer to the value if found, @em nullptr if not
	 *  	   found.
	 */
	const_iterator find(const key_type& key) const {
		return const_cast<topic_matcher*>(this)->find(key);
	}
	/**
	 * Gets an iterator that can find the matches to the topic.
	 * @param topic The topic to search for matches.
	 * @return An iterator that can find the matches to the topic.
	 */
	iterator matches(const string& topic) {
		return iterator(root_.get(), topic);
	}
	/**
	 * Gets a const iterator that can find the matches to the topic.
	 * @param topic The topic to search for matches.
	 * @return A const iterator that can find the matches to the topic.
	 */
	const_iterator matches(const string& topic) const {
		return iterator(root_.get(), topic);
	}
	/**
	 * Gets an iterator for the end of the collection.
	 *
	 * This simply returns an empty/null iterator which we can use to signal
	 * the end of the collection.
	 *
	 * @return An empty/null iterator indicating the end of the collection.
	 */
	const_iterator end() const noexcept {
		return iterator {};
	}
	/**
	 * Gets an iterator for the end of the collection.
	 *
	 * This simply returns an empty/null iterator which we can use to signal
	 * the end of the collection.
	 *
	 * @return An empty/null iterator indicating the end of the collection.
	 */
	const_iterator cend() const noexcept {
		return iterator {};
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_topic_matcher_h

