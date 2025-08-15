/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "varitensor/impl/Index.h"

#include <sstream>

#include "varitensor/impl/deny.h"

namespace varitensor {

// =================================================================================================
//                                                                                          c/dtor |
// =================================================================================================

Index::Index(const int size):
    m_size{size},
    m_id{s_next_id++}
{
    impl::deny(m_size < 2, "Cannot initialize unnamed index with size < 2");
}

Index::Index(std::string  name, const int size):
    m_size{size},
    m_id{s_next_id++},
    m_name{std::move(name)}
{
    impl::deny(m_size < 2, "Cannot initialize index with size < 2");
}

Index::Index(const Interval& interval): // NOLINT - we want implicit conversion
    m_size{interval.last - interval.first + 1},
    m_id{interval.origin.m_id},
    m_name{interval.origin.m_name},
    m_interval_start{interval.first}
{}

// =================================================================================================
//                                                                                      comparison |
// =================================================================================================

bool Index::operator==(const Index& other) const {
    if (m_name.empty()) return m_id == other.m_id && m_size == other.m_size;
    return m_name == other.m_name && m_size == other.m_size;
}

// =================================================================================================
//                                                                               getters / setters |
// =================================================================================================

const int& Index::size() const { return m_size; }

const int& Index::id() const { return m_id; }

std::string Index::name() const {
    std::ostringstream name;

    if (m_name.empty()) name << "idx" << m_id;
    else name << m_name;

    if (m_interval_start != -1) {
        name << std::string("(") << m_interval_start << '-' << (m_interval_start + m_size - 1) << ')';
    }

    return name.str();
}

void Index::set_name(const std::string& name) {
    m_name = name;
}

// =================================================================================================
//                                                                               interval creation |
// =================================================================================================

Interval Index::operator()(const int first, const int last/* = impl::MAX_INTERVAL */) const {
    return interval(first, last);
}

Interval Index::interval(const int first, int last/* = impl::MAX_INTERVAL */) const {
    if (last == impl::MAX_INTERVAL) last = m_size - 1;

    impl::deny(first < 0, "Interval cannot have negative start");
    impl::deny(first >= last, "Interval must end after it starts");
    impl::deny(last >= m_size, "Interval cannot overflow index size");
    return {*this, first, last};
}

} // namespace varitensor
