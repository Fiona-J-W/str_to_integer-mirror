String to Integer (sti)
=======================

This is a header-only library that provides functions to parse integers.
In contrast to the stdlib-functions it takes the target-type as a template
argument and guarantees to always throw an exception if the value cannot
be represented by it.

Usage
-----

This library provides three overloads of a functiontemplate `str_to` with the
following signatures:

```cpp
template <typename Integer, unsigned Base = 10u, typename Iterator>
Integer str_to(Iterator first, Iterator last);

template <typename Integer, unsigned Base = 10u, typename String>
Integer str_to(const String& str);

template <typename Integer, unsigned Base = 10u, typename Char>
Integer str_to(const Char* str);
```

The first template-argument `Integer` is the desired integer-type.
The second one (`Base`) is the base in which the the parsing should be done;
note that prefixes to literals won't be able to change that: „023“ will not
set the base to 8 and this is considered to be a feature.
The third template-argument is not to be passed explicitly.

The second and the third overload will forward their arguments to the first.
The second will use the `begin`- and `end`-functions provided either via ADL
or the ones in the stdlib.
The third one will use `std::char_traits<Char>::length()` to find out the
length of the string and forward a pair of pointers.

The implementation is written so that all of the following character-types
should work, though at this point not all have tests: `char`, `wchar_t`,
`char16_t` and `char32_t`. This is under the assumption that they are
big-endian; if you are using little-endian you are bad and you should feel bad.
(Though that can also be said about anyone not using utf-8 everywhere, but that
is another discussion).


License
-------

This library is licensed as LGPL v2 or later versions.

