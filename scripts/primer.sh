#!/bin/bash

cmake -B build -DCMAKE_BUILD_TYPE=Debug
cd build

make trie_test trie_store_test -j$(nproc)
make trie_noncopy_test trie_store_noncopy_test -j$(nproc)
./test/trie_test
./test/trie_noncopy_test
./test/trie_store_test
./test/trie_store_noncopy_test
