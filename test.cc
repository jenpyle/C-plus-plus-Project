#include "Gallery.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <regex>

using namespace std;

void show(const Enemy e) {          // pass by value to force a copy
    for (auto p : e) {
        assert(e[p.first] == p.second);
        cout << p.first << ": " << p.second << '\n';
    }
}

void show(const Gallery g) {        // pass by value to force a copy
    string gutter;
    for (const Enemy &e : g) {
        cout << gutter;
        gutter = "\n";
        show(e);
    }
}

void test_comparison_operators(const Gallery &g) {
    assert(g.size() >= 3);

    assert(g[0] == g[0]);
    assert(g[1] == g[1]);
    assert(!(g[1] == g[0]));
    assert(!(g[0] == g[1]));

    assert(!(g[0] != g[0]));
    assert(!(g[1] != g[1]));
    assert(g[1] != g[0]);
    assert(g[0] != g[1]);

    // Guarantee that all Enemies in this Gallery are distinct.
    assert(g[0] != g[2]);
    assert(g[1] != g[2]);
}

void test_gallery_iterators(const Gallery &g) {
    Gallery::iterator it1 = g.begin();
    auto a1 = it1++;            // iterator “pointing” to first element
    auto b1 = it1++;            // iterator “pointing” to second element
    auto c1 = it1++;            // iterator “pointing” to third element

    Gallery::iterator it2 = g.begin();
    const auto a2 = it2;        // iterator “pointing” to first element
    const auto b2 = ++it2;      // iterator “pointing” to second element
    const auto c2 = ++it2;      // iterator “pointing” to third element

    Gallery::iterator it3 = g.end();
    const auto c3 = --it3;      // iterator “pointing” to third element
    const auto b3 = --it3;      // iterator “pointing” to second element
    const auto a3 = --it3;      // iterator “pointing” to first element

    Gallery::iterator it4 = g.end();
    it4--;                      // now “points” to third element
    auto c4 = it4--;            // iterator “pointing” to third element
    auto b4 = it4--;            // iterator “pointing” to second element
    auto a4 = it4;              // iterator “pointing” to first element

    // All three must be distinct.
    assert(a1 != b1);
    assert(a1 != c1);
    assert(b1 != c1);

    // Assure that the iterators, acquired in various ways, match.
    assert(a1 == a2);
    assert(b1 == b2);
    assert(c1 == c2);

    assert(a1 == a3);
    assert(b1 == b3);
    assert(c1 == c3);

    assert(a1 == a4);
    assert(b1 == b4);
    assert(c1 == c4);

    // Instead of comparing iterators, now compare Enemies.
    assert(*a1 != *b1);
    assert(*a1 != *c1);
    assert(*b1 != *c1);

    assert(*a1 == *a2);
    assert(*b1 == *b2);
    assert(*c1 == *c2);

    assert(*a1 == *a3);
    assert(*b1 == *b3);
    assert(*c1 == *c3);

    assert(*a1 == *a4);
    assert(*b1 == *b4);
    assert(*c1 == *c4);
}

void test_enemy_iterators(const Gallery &g) {
    // Find an Enemy in this Gallery with five key/value pairs:
    const Enemy &e = g[0].size()==5 ? g[0] : g[1];
    assert(e.size() == 5);

    Enemy::iterator it1 = e.begin();
    auto a1 = it1++;            // iterator “pointing” to first element
    auto b1 = it1++;            // iterator “pointing” to second element
    auto c1 = it1++;            // iterator “pointing” to third element

    Enemy::iterator it2 = e.begin();
    const auto a2 = it2;        // iterator “pointing” to first element
    const auto b2 = ++it2;      // iterator “pointing” to second element
    const auto c2 = ++it2;      // iterator “pointing” to third element

    Enemy::iterator it3 = e.end();
    it3--; --it3;               // now “points” to fourth element
    const auto c3 = --it3;      // iterator “pointing” to third element
    const auto b3 = --it3;      // iterator “pointing” to second element
    const auto a3 = --it3;      // iterator “pointing” to first element

    Enemy::iterator it4 = e.end();
    --it4; it4--; --it4;        // now “points” to third element
    auto c4 = it4--;            // iterator “pointing” to third element
    auto b4 = it4--;            // iterator “pointing” to second element
    auto a4 = it4;              // iterator “pointing” to first element

    // All three must be distinct.
    assert(a1 != b1);
    assert(a1 != c1);
    assert(b1 != c1);

    // Assure that the iterators, acquired in various ways, match.
    assert(a1 == a2);
    assert(b1 == b2);
    assert(c1 == c2);

    assert(a1 == a3);
    assert(b1 == b3);
    assert(c1 == c3);

    assert(a1 == a4);
    assert(b1 == b4);
    assert(c1 == c4);

    // Instead of comparing iterators, now compare key/value pairs.
    assert(*a1 != *b1);
    assert(*a1 != *c1);
    assert(*b1 != *c1);

    assert(*a1 == *a2);
    assert(*b1 == *b2);
    assert(*c1 == *c2);

    assert(*a1 == *a3);
    assert(*b1 == *b3);
    assert(*c1 == *c3);

    assert(*a1 == *a4);
    assert(*b1 == *b4);
    assert(*c1 == *c4);
}

int main() {
/*    try {
        const Gallery vacant("oz-keys", "/dev/null");
        Gallery monsters("oz-villains", "oz-keys");

        assert(!vacant);
        assert(monsters);
        assert(monsters[0]);

        test_comparison_operators(monsters);
        test_gallery_iterators(monsters);
        test_enemy_iterators(monsters);

        show(vacant);
        show(monsters);
    }
    catch (const exception &e) {
        cerr << "Unexpected exception: " << e.what() << '\n';
        return 1;
    }
    catch (...) {
        cerr << "Caught something, but it’s not a std::exception‽\n";
        return 2;
    }

    cout << "*** Done! ***\n";
*/
const regex r("[a-fx0-9]");
cout << boolalpha << regex_search("123", r) << '\n';


    return 0;
}
