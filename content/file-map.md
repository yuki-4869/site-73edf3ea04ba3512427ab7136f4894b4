# 元ファイルとの対応

初回整理時はファイル名・配置・拡張子のみを変更し、本文のバイト列を元ファイルと一致させました。その後の実装改訂は、各説明ページに記録しています。

[RangeSet](usage/DataStructureBasic/range_set.md)、[座標圧縮](usage/DataStructureBasic/coordinate_compression.md)、[遅延セグ木ラッパー](usage/DataStructureSegment/lazy_segtree_wrappers.md)、[Beats拡張](usage/DataStructureSegment/segment_tree_beats_extra.md)は2026-10-15に実装を改訂しています。file-manifest.jsonのbytes・sha256は初回整理時の記録で、改訂後の値は該当ファイルのcurrent_revisionに記録しています。
元の `.txt` をC++コードとして閲覧しやすい `.cpp` に変更しています。素数表は `.txt` のままです。

| 元ファイル名 | 整理後のファイル |
| --- | --- |
| 約数列挙.txt | [MathNumberTheory/divisors.cpp](../MathNumberTheory/divisors.cpp) |
| modの割り算・modの累乗.txt | [MathNumberTheory/mod_pow_inverse.cpp](../MathNumberTheory/mod_pow_inverse.cpp) |
| 素数10^6.txt | [MathNumberTheory/primes_upto_1000000.txt](../MathNumberTheory/primes_upto_1000000.txt) |
| 回文の半径を求める.txt | [String/palindrome_radius.cpp](../String/palindrome_radius.cpp) |
| 削除可能なUnionFind.txt | [DataStructureAdvanced/dynamic_connectivity.cpp](../DataStructureAdvanced/dynamic_connectivity.cpp) |
| 削除可能なオフラインUnionFind.txt | [Graph/offline_dynamic_connectivity_weighted.cpp](../Graph/offline_dynamic_connectivity_weighted.cpp) |
| RangeSet.txt | [DataStructureBasic/range_set.cpp](../DataStructureBasic/range_set.cpp) |
| CartesianTree.txt | [Tree/cartesian_tree.cpp](../Tree/cartesian_tree.cpp) |
| 永続セグメント木.txt | [DataStructureSegment/persistent_segment_tree.cpp](../DataStructureSegment/persistent_segment_tree.cpp) |
| 第二種スターリング数.txt | [MathCombinatorics/stirling_second.cpp](../MathCombinatorics/stirling_second.cpp) |
| 偏角ソート.txt | [Geometry/argument_sort.cpp](../Geometry/argument_sort.cpp) |
| ImplicitTreap.txt | [DataStructureAdvanced/implicit_treap.cpp](../DataStructureAdvanced/implicit_treap.cpp) |
| 二項係数の累積和.txt | [MathCombinatorics/binomial_prefix_sum.cpp](../MathCombinatorics/binomial_prefix_sum.cpp) |
| 約数倍数ゼータメビウス.txt | [MathNumberTheory/divisor_multiple_transforms.cpp](../MathNumberTheory/divisor_multiple_transforms.cpp) |
| SortedImplicitTreap.txt | [DataStructureAdvanced/sorted_implicit_treap.cpp](../DataStructureAdvanced/sorted_implicit_treap.cpp) |
| 重み付きUnionFind.txt | [DataStructureBasic/weighted_union_find.cpp](../DataStructureBasic/weighted_union_find.cpp) |
| LCA.txt | [Tree/lca.cpp](../Tree/lca.cpp) |
| HLD.txt | [Tree/hld.cpp](../Tree/hld.cpp) |
| RollbackUnionFind.txt | [DataStructureBasic/rollback_union_find.cpp](../DataStructureBasic/rollback_union_find.cpp) |
| LiChaoTree.txt | [DiscreteConvexity/li_chao_tree.cpp](../DiscreteConvexity/li_chao_tree.cpp) |
| ConvexHullTrick.txt | [DiscreteConvexity/convex_hull_trick.cpp](../DiscreteConvexity/convex_hull_trick.cpp) |
| DequeCHT.txt | [DiscreteConvexity/deque_cht.cpp](../DiscreteConvexity/deque_cht.cpp) |
| 全方位木DP（Rerooting）.txt | [Tree/rerooting.cpp](../Tree/rerooting.cpp) |
| AhoCorasick.txt | [String/aho_corasick.cpp](../String/aho_corasick.cpp) |
| Mo's Algorithm.txt | [Search/mo.cpp](../Search/mo.cpp) |
| XOR基底.txt | [LinearAlgebra/xor_basis.cpp](../LinearAlgebra/xor_basis.cpp) |
| Miller-RabinPollard Rho.txt | [MathNumberTheory/miller_rabin_pollard_rho.cpp](../MathNumberTheory/miller_rabin_pollard_rho.cpp) |
| トポロジカルソート.txt | [Graph/scc_condensation.cpp](../Graph/scc_condensation.cpp) |
| 木の直径・全ての頂点からの最遠点までの距離.txt | [Tree/tree_diameter.cpp](../Tree/tree_diameter.cpp) |
| StaticRollingHash.txt | [String/static_rolling_hash.cpp](../String/static_rolling_hash.cpp) |
| WaveletMatrix.txt | [DataStructureSegment/wavelet_matrix.cpp](../DataStructureSegment/wavelet_matrix.cpp) |
| DynamicWeightedWaveletMatrix.txt | [DataStructureSegment/dynamic_weighted_wavelet_matrix.cpp](../DataStructureSegment/dynamic_weighted_wavelet_matrix.cpp) |
| DynamicRollingHash.txt | [String/dynamic_rolling_hash.cpp](../String/dynamic_rolling_hash.cpp) |
| 木の重心.txt | [Tree/tree_centroid.cpp](../Tree/tree_centroid.cpp) |
| XOR畳み込み.txt | [FPSandPolynomial/xor_convolution.cpp](../FPSandPolynomial/xor_convolution.cpp) |
| SegmentTreeBeats!.txt | [DataStructureSegment/segment_tree_beats.cpp](../DataStructureSegment/segment_tree_beats.cpp) |
| Segment Tree Beats! Extra.txt | [DataStructureSegment/segment_tree_beats_extra.cpp](../DataStructureSegment/segment_tree_beats_extra.cpp) |
| 拡張ユークリッドの互除法.txt | [MathNumberTheory/extended_euclid.cpp](../MathNumberTheory/extended_euclid.cpp) |
| 座標圧縮.txt | [DataStructureBasic/coordinate_compression.cpp](../DataStructureBasic/coordinate_compression.cpp) |
| 二部探索テンプレ.txt | [Snippets/binary_search.cpp](../Snippets/binary_search.cpp) |
| コードatcoder抜き.txt | [Templates/modint_only.cpp](../Templates/modint_only.cpp) |
| FunctionalGraph.txt | [Graph/functional_graph.cpp](../Graph/functional_graph.cpp) |
| EulerTrail.txt | [Graph/euler_trail.cpp](../Graph/euler_trail.cpp) |
| LinearSieve.txt | [MathNumberTheory/linear_sieve.cpp](../MathNumberTheory/linear_sieve.cpp) |
| BinaryLifting.txt | [Graph/binary_lifting.cpp](../Graph/binary_lifting.cpp) |
| Gaussian Elimination.txt | [LinearAlgebra/gaussian_elimination.cpp](../LinearAlgebra/gaussian_elimination.cpp) |
| GF(2) Gaussian Elimination.txt | [LinearAlgebra/gf2_gaussian_elimination.cpp](../LinearAlgebra/gf2_gaussian_elimination.cpp) |
| VirtualTreeResult.txt | [Tree/virtual_tree.cpp](../Tree/virtual_tree.cpp) |
| DSUonTree.txt | [Tree/dsu_on_tree.cpp](../Tree/dsu_on_tree.cpp) |
| OfflineDynamicConnectivity.txt | [Graph/offline_dynamic_connectivity.cpp](../Graph/offline_dynamic_connectivity.cpp) |
| 01-StringWavelerMatrix.txt | [String/binary_string_order_set.cpp](../String/binary_string_order_set.cpp) |
| TrieAZ.txt | [String/trie_lowercase.cpp](../String/trie_lowercase.cpp) |
| TrieAzaz.txt | [String/trie_letters.cpp](../String/trie_letters.cpp) |
| Trie09.txt | [String/trie_digits.cpp](../String/trie_digits.cpp) |
| TrieAZ09.txt | [String/trie_lowercase_digits.cpp](../String/trie_lowercase_digits.cpp) |
| segtree セグ木テンプレ.txt | [DataStructureSegment/segtree_wrappers.cpp](../DataStructureSegment/segtree_wrappers.cpp) |
| SegXXX.txt | [Snippets/segtree_custom.cpp](../Snippets/segtree_custom.cpp) |
| lazy_segtree 遅延セグ木テンプレ.txt | [DataStructureSegment/lazy_segtree_wrappers.cpp](../DataStructureSegment/lazy_segtree_wrappers.cpp) |
| LazyXXX.txt | [Snippets/lazy_segtree_custom.cpp](../Snippets/lazy_segtree_custom.cpp) |
| codet.txt | [Templates/multiple_test_cases.cpp](../Templates/multiple_test_cases.cpp) |
| codecf.txt | [Templates/codeforces.cpp](../Templates/codeforces.cpp) |
| 素数判定.txt | [MathNumberTheory/number_theory_bundle.cpp](../MathNumberTheory/number_theory_bundle.cpp) |
| BinaryTrie.txt | [DataStructureBasic/binary_trie.cpp](../DataStructureBasic/binary_trie.cpp) |
| LowLink.txt | [Graph/lowlink.cpp](../Graph/lowlink.cpp) |
| コード.txt | [template.cpp](../template.cpp) |
