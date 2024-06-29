#!/bin/bash

cmake -B build -DCMAKE_BUILD_TYPE=Debug
cd build

make trie_test trie_store_test -j
make trie_noncopy_test trie_store_noncopy_test -j
make sqllogictest -j
./test/trie_test
./test/trie_noncopy_test
./test/trie_store_test
./test/trie_store_noncopy_test
./bin/bustub-sqllogictest ../test/sql/p0.01-lower-upper.slt --verbose
./bin/bustub-sqllogictest ../test/sql/p0.02-function-error.slt --verbose
./bin/bustub-sqllogictest ../test/sql/p0.03-string-scan.slt --verbose
