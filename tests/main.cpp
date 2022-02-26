#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <idv/cyclic_iterator.h>

#include <set>
#include <vector>

using namespace idv;

struct S
{
    int x;
};

bool operator<(const S & a, const S & b) { return a.x < b.x; }
bool operator==(const S & a, const S & b) { return a.x == b.x; }

TEST_CASE("forward", "[forward]")
{
    auto v = std::vector<S>{{1}, {2}, {3}};

    auto it = make_cyclic_iterator(v.begin(), v.end());

    CHECK(it.base() == v.begin());
    CHECK((*it).x == 1);
    CHECK(it->x == 1);

    const auto & r = ++it;

    CHECK(&r == &it);
    CHECK(it.base() == v.begin() + 1);
    CHECK((*it).x == 2);
    CHECK(it->x == 2);

    const auto & r2 = it++;

    CHECK(&r2 != &it);
    CHECK(it.base() == v.begin() + 2);
    CHECK((*it).x == 3);
    CHECK(it->x == 3);

    ++it;

    CHECK(it.base() == v.begin());
    CHECK((*it).x == 1);
    CHECK(it->x == 1);
}

TEST_CASE("forward.comparison", "[forward]")
{
    auto v = std::vector<S>{{1}, {2}, {3}};

    auto it = make_cyclic_iterator(v.begin(), v.end());
    auto it2 = make_cyclic_iterator(v.begin(), v.end());
    auto it3 = make_cyclic_iterator(v.begin() + 1, v.end());

    CHECK(it == it2);
    CHECK(it != it3);

    ++it;

    CHECK(it != it2);
    CHECK((it.base() == it3.base() && it2 != it3));

    ++it;

    CHECK(it != it2);
    CHECK(it != it3);

    ++it;

    CHECK(it == it2);
    CHECK(it != it3);
}

TEST_CASE("bidirectional", "[bidirectional]")
{
    auto v = std::set<S>{{1}, {2}, {3}};
    auto it = make_cyclic_iterator(v.begin(), v.end());

    CHECK(it.base() == v.begin());
    CHECK((*it).x == 1);
    CHECK(it->x == 1);

    const auto & r = --it;

    CHECK(&r == &it);
    CHECK(it.base() == std::next(v.begin(), 2));
    CHECK((*it).x == 3);
    CHECK(it->x == 3);

    const auto & r2 = it--;

    CHECK(&r2 != &it);
    CHECK(it.base() == std::next(v.begin(), 1));
    CHECK((*it).x == 2);
    CHECK(it->x == 2);

    --it;

    CHECK(it.base() == v.begin());
    CHECK((*it).x == 1);
    CHECK(it->x == 1);
}

TEST_CASE("random_access.empty", "[random_access]")
{
    auto v = std::vector<S>{};
    auto it = make_cyclic_iterator(v.begin(), v.begin());

    it += 0;
    CHECK(it.base() == v.begin());
    it += 1;
    CHECK(it.base() == v.begin());
    it += 2;
    CHECK(it.base() == v.begin());
    it -= 1;
    CHECK(it.base() == v.begin());
    it -= 2;
    CHECK(it.base() == v.begin());

    CHECK((it + 0).base() == v.begin());
    CHECK((it + 1).base() == v.begin());
    CHECK((it + 2).base() == v.begin());
    CHECK((it - 1).base() == v.begin());
    CHECK((it - 2).base() == v.begin());
}

TEST_CASE("random_access.single_element", "[random_access]")
{
    auto v = std::vector<S>{{1}};
    auto it = make_cyclic_iterator(v.begin(), v.end());

    it += 0;
    CHECK(it.base() == v.begin());
    it += 1;
    CHECK(it.base() == v.begin());
    it += 2;
    CHECK(it.base() == v.begin());
    it -= 1;
    CHECK(it.base() == v.begin());
    it -= 2;
    CHECK(it.base() == v.begin());

    CHECK((it + 0).base() == v.begin());
    CHECK((it + 1).base() == v.begin());
    CHECK((it + 2).base() == v.begin());
    CHECK((it - 1).base() == v.begin());
    CHECK((it - 2).base() == v.begin());
}

TEST_CASE("random_access", "[random_access]")
{
    auto v = std::vector<S>{{1}, {2}, {3}, {4}, {5}};
    auto it = make_cyclic_iterator(v.begin(), v.end());

    CHECK(it[0] == S{1});
    CHECK(it[1] == S{2});
    CHECK(it[2] == S{3});

    it += 0;
    CHECK(it.base() == v.begin());
    it += 5;
    CHECK(it.base() == v.begin());
    it += 6;
    CHECK(it.base() == v.begin() + 1);
    it += 13;
    CHECK(it.base() == v.begin() + 4);
    it += 0;
    CHECK(it.base() == v.begin() + 4);
    it += 5;
    CHECK(it.base() == v.begin() + 4);
    it += 3;
    CHECK(it.base() == v.begin() + 2);
    it += 0;
    CHECK(it.base() == v.begin() + 2);
    it += 5;
    CHECK(it.base() == v.begin() + 2);
    it += 3;
    CHECK(it.base() == v.begin());

    it -= 0;
    CHECK(it.base() == v.begin());
    it -= 5;
    CHECK(it.base() == v.begin());
    it -= 6;
    CHECK(it.base() == v.begin() + 4);
    it -= 13;
    CHECK(it.base() == v.begin() + 1);
    it -= 0;
    CHECK(it.base() == v.begin() + 1);
    it -= 5;
    CHECK(it.base() == v.begin() + 1);
    it -= 3;
    CHECK(it.base() == v.begin() + 3);
    it -= 0;
    CHECK(it.base() == v.begin() + 3);
    it -= 5;
    CHECK(it.base() == v.begin() + 3);
    it -= 3;
    CHECK(it.base() == v.begin());

    auto begin = make_cyclic_iterator(v.begin(), v.end());

    CHECK((begin + 0) == (0 + begin));
    CHECK((begin + 0).base() == v.begin());
    CHECK((begin + 5) == (5 + begin));
    CHECK((begin + 5).base() == v.begin());
    CHECK((begin + 3) == (3 + begin));
    CHECK((begin + 3).base() == v.begin() + 3);
    CHECK((begin + 6) == (6 + begin));
    CHECK((begin + 6).base() == v.begin() + 1);

    CHECK((begin - 0).base() == v.begin());
    CHECK((begin - 5).base() == v.begin());
    CHECK((begin - 3).base() == v.begin() + 2);
    CHECK((begin - 6).base() == v.begin() + 4);

    CHECK(begin - 1 > begin + 1);
    CHECK(begin + 2 > begin + 1);
    CHECK(begin + 2 < begin - 1);
    CHECK(begin - 2 < begin - 1);

    CHECK(((begin + 1) - (begin + 1)) ==  0);
    CHECK(((begin + 2) - (begin + 1)) == 1);
    CHECK(((begin + 2) - (begin + 4)) == -2);
    CHECK(((begin + 7) - (begin - 2)) == -1);
}
