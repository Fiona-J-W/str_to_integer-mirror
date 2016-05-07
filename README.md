String to Integer (sti)
=======================

This is a header-only library that provides functions to parse integers.
In contrast to the stdlib-functions it takes the target-type as a template
argument and guarantees to always throw an exception if the value cannot
be represented by it.

Advantages over `std::stoi`
-------------------------

“Don't reinvent the wheel, use the standard-library.” - Yes, I am aware
of this dogma and in fact I'm preaching it myself. This library is however
not a case of not-invented-here-syndrom but an answer to the lack of such
functionality in the standard-library. Yes, I am aware of functions like
`std::stoi`, `std::atoi` and `std::strtoi`: They are what motivated me
to write this library because they are fundamentally broken:

* They fail to signal some kinds of errors in a usefull way
* They are unusable in a generic context.
* The input decides the base which is very dangerous with leading zeros
* Their API makes them inefficient

As an example for the first: `std::stoul("-1")` will not result in
an exception, as it definitely should, but in `static_cast<unsigned long>(-1)`,
aka the largest value that `unsigned long` can hold.

By unusable in generic contexts I mean things like the following code:

```cpp
template<typename T>
T parse_and_double(const std::string& str) {
	// what am I supposed to put here?:
	return std::stoXXX(str) * 2;
}
```

The base-problem is that a string `0023` will be parsed in base 8,
even though there are perfectly legit reasons to use leading zeros
in decimal numbers. This behavior cannot really be changed but I really
don't think that it is necessary for me to point out how this can end
terrible.

Finally it is shocking how slow `std::stoi` and it's friends are. A
trivial implementation with fixed base can easily outperform them
even with complete error-checking and no optimisation. And I am not
just talking about a few percent here, I am talking about the trivial
implementation being twice as fast and a slightly less trivial one
being five times as fast. Note that I am not blaming the
implementation, for a large part this really is the fault of the overly
dynamic API. Which is why we should drop the API in favour of one that
solves all these problems, which is what this project attempts.



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
should work: `char`, `wchar_t`, `char16_t` and `char32_t`. This is under the
assumption that they are big-endian; if you are using little-endian you are
bad and you should feel bad. (Though that can also be said about anyone not
using [utf-8 everywhere](http://utf8everywhere.org/), but that is another
discussion).

If the passed string contains any invalid characters, a `sti::invalid_input_failure`
will be thrown. If the numeric value of the argument cannot be represented
an `sti::out_of_bounds_failure` will be thrown. Both classes inherit from
`sti::conversion_failure` which inherits from `std::runtime_error`.

Valid charachters in the argument are ‘+’ and ‘-’ as the first symbol
(optional) followed by digits and letters that are valid in the choosen
base: In base 8 that would be ‘0’ - ‘7’, in base 10 ‘0’ - ‘9’ and in base
16 ‘0’ - ‘9’, ‘a’ - ‘f’ and ‘A’ - ‘F’ (note that both upper and lower-case
letters work). All other characters (including whitespace!) will trigger an
exception.

One thing to note here is that unsigned integers may be prefixed by a ‘-’,
though everything except for zero will then trigger an `out_of_bounds_failure`
(though *not* an `invalid_input_failure`!).


Status
------

This library is fairly new and hasn't received a code-review yet (if you are
willing to participate in changing that, contact me). There is a sufficient
amount of unittests for me to believe that normal use should work. There might
however be weird behaviour in corner-cases and I am gratefull for bug-reports.


License
-------

This library is licensed as LGPL v2 or later versions.

If you use it in your software I would be very happy to learn about that,
so please let me know, for example by sending me an email. Knowing about other
people using my stuff, might motivate me to put more work into improoving
my software, so you may benefit as well. (Just to be clear: This is personal
request, not a legal requirement for use).



