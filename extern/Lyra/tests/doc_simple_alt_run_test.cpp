/*
Copyright 2020 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "doc_simple_alt.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "doc_simple_alt", "-x", "1");
		TEST_MAIN(test, "doc_simple_alt", "-x", "1", "-y", "2");
		TEST_MAIN(test, "doc_simple_alt", "-x", "1", "-y", "2", "3");
		TEST_MAIN_FAIL(test, "doc_simple_alt", "-z", "1");
		TEST_MAIN_FAIL(test, "doc_simple_alt", "-x");
	}
	return test;
}
