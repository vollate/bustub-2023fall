#!/bin/bash

cd build
make trie_test trie_store_test -j
./test/trie_test
./test/trie_store_test

