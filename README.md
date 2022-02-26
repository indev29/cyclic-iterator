[![Standard](https://img.shields.io/badge/c%2B%2B-11/14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-BSL-blue.svg)](http://www.boost.org/LICENSE_1_0.txt)
[![CI](https://img.shields.io/github/workflow/status/indev29/cyclic-iterator/CI/master)](https://github.com/indev29/cyclic-iterator/actions/workflows/ci.yml)

# cyclic-iterator
C++ iterator adapter created from two iterators (`begin`, `end`) that jumps to `begin` when `end` is reached.

Works with C++11 and onward.

```c++
#include <idv/cyclic_iterator.h>

#include <cassert>
#include <vector>

int main()
{
  auto v = std::vector<int>{1, 2, 3};
  auto it = idv::cyclic_iterator(v.begin(), v.end());
  
  auto x1 = *it++; // 1
  auto x2 = *it++; // 2
  auto x3 = *it++; // 3
  auto x4 = *it++; // 1
  
  assert(x1 == x4); // true
  
  return 0;
}
```

`cyclic_iterator` requires base iterator to be at least `ForwardIterator` since its semantics assume multipass guarantee.  
It is implemented for every suitable iterator category and inherits the category (and operations) of base iterator.

## Usage

The simplest way is to copy `include/idv/cyclic_iterator.h` into the project and `#include` it.

Another way is to use it as a submodule.

```cmake
add_subdirectory(cyclic_iterator)

target_link_libraries(<target-name> cyclic_iterator)
```
