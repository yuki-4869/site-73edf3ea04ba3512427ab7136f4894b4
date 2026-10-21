# ライブラリの使い方

各実装のAPI・引数と返り値・制約・計算量・使用例をまとめています。
[ACLの説明書](https://atcoder.github.io/ac-library/production/document_ja/scc.html)を参考に、API宣言と説明、必要な制約、計算量の順に記載しています。

- [共通設定・添字](common.md)
- [C++23での確認結果](validation.md)
- [優先して確認・修正する項目](../review-notes.md#優先して確認修正する項目)
- [リポジトリ](../../README.md)

## 分野から探す

| 分野 | ファイル数 |
| --- | ---: |
| [基本テンプレート](#category-1) | 1 |
| [テンプレートの別版](#category-2) | 3 |
| [基礎データ構造](#category-3) | 5 |
| [区間データ構造](#category-4) | 8 |
| [高度なデータ構造](#category-5) | 3 |
| [CHT・離散凸性](#category-6) | 3 |
| [グラフ](#category-7) | 7 |
| [木](#category-8) | 8 |
| [整数論](#category-9) | 8 |
| [組合せ](#category-10) | 2 |
| [線形代数](#category-11) | 3 |
| [畳み込み](#category-12) | 1 |
| [文字列](#category-13) | 9 |
| [幾何](#category-14) | 1 |
| [探索・オフライン処理](#category-15) | 1 |
| [穴埋め用スニペット](#category-16) | 3 |

<a id="category-1"></a>

## 基本テンプレート

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [基本テンプレート](template.md) | main の中身と閉じ括弧を追記して使用 | [コード](../../template.cpp) |

<a id="category-2"></a>

## テンプレートの別版

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [modintのみをincludeするテンプレート](Templates/modint_only.md) | 元の名前に反して atcoder/modint の依存が残る | [コード](../../Templates/modint_only.cpp) |
| [複数テストケース用テンプレート](Templates/multiple_test_cases.md) | solve の末尾を追記して使用 | [コード](../../Templates/multiple_test_cases.cpp) |
| [Codeforces用テンプレート](Templates/codeforces.md) | ACL modint の依存あり。Graph の型にも注意 | [コード](../../Templates/codeforces.cpp) |

<a id="category-3"></a>

## 基礎データ構造

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [区間集合](DataStructureBasic/range_set.md) | 半開区間の統合・追加・削除、mex、被覆数（改訂済み） | [コード](../../DataStructureBasic/range_set.cpp) |
| [重み付きUnion-Find](DataStructureBasic/weighted_union_find.md) | potential[b] - potential[a] を管理 | [コード](../../DataStructureBasic/weighted_union_find.cpp) |
| [Rollback Union-Find](DataStructureBasic/rollback_union_find.md) | 経路圧縮なし。直前のmerge呼び出しを取り消す | [コード](../../DataStructureBasic/rollback_union_find.cpp) |
| [座標圧縮](DataStructureBasic/coordinate_compression.md) | Zip・一次元／二次元の圧縮、厳密な検索・復元（改訂済み） | [コード](../../DataStructureBasic/coordinate_compression.cpp) |
| [Binary Trie](DataStructureBasic/binary_trie.md) | 整数集合の管理と全体XOR | [コード](../../DataStructureBasic/binary_trie.cpp) |

<a id="category-4"></a>

## 区間データ構造

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [Disjoint Sparse Table](DataStructureSegment/disjoint_sparse_table.md) | 静的な結合的区間積をO(1)で取得、非可換演算にも対応 | [コード](../../DataStructureSegment/disjoint_sparse_table.cpp) |
| [永続セグメント木](DataStructureSegment/persistent_segment_tree.md) | 過去の版から分岐、容量検査・遅延ノード確保（改訂済み） | [コード](../../DataStructureSegment/persistent_segment_tree.cpp) |
| [Wavelet Matrix](DataStructureSegment/wavelet_matrix.md) | 静的列。k番目の k は 0-indexed | [コード](../../DataStructureSegment/wavelet_matrix.cpp) |
| [動的重み付きWavelet Matrix](DataStructureSegment/dynamic_weighted_wavelet_matrix.md) | 変更に使う可能性のあるキーをuniverseに事前登録 | [コード](../../DataStructureSegment/dynamic_weighted_wavelet_matrix.cpp) |
| [Segment Tree Beats](DataStructureSegment/segment_tree_beats.md) | 区間のchmin/chmax・加算・代入など | [コード](../../DataStructureSegment/segment_tree_beats.cpp) |
| [Segment Tree Beats の拡張](DataStructureSegment/segment_tree_beats_extra.md) | 剰余・除算・平方根・AND/OR、境界検査・一括更新（改訂済み） | [コード](../../DataStructureSegment/segment_tree_beats_extra.cpp) |
| [ACL segtree の各種ラッパー](DataStructureSegment/segtree_wrappers.md) | 最小／最大／和などの型と演算 | [コード](../../DataStructureSegment/segtree_wrappers.cpp) |
| [ACL lazy_segtree の各種ラッパー](DataStructureSegment/lazy_segtree_wrappers.md) | 17種類の区間更新・取得、境界検査・全API一覧（改訂済み） | [コード](../../DataStructureSegment/lazy_segtree_wrappers.cpp) |

<a id="category-5"></a>

## 高度なデータ構造

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [動的連結性（オンライン）](DataStructureAdvanced/dynamic_connectivity.md) | 辺の追加・削除、連結判定、成分サイズ・集計 | [コード](../../DataStructureAdvanced/dynamic_connectivity.cpp) |
| [Implicit Treap](DataStructureAdvanced/implicit_treap.md) | 位置をキーにした列の操作 | [コード](../../DataStructureAdvanced/implicit_treap.cpp) |
| [Sorted Implicit Treap](DataStructureAdvanced/sorted_implicit_treap.md) | 昇順列の管理と集計 | [コード](../../DataStructureAdvanced/sorted_implicit_treap.cpp) |

<a id="category-6"></a>

## CHT・離散凸性

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [Li Chao Tree](DiscreteConvexity/li_chao_tree.md) | 直線・区間直線の最小値／最大値 | [コード](../../DiscreteConvexity/li_chao_tree.cpp) |
| [Convex Hull Trick](DiscreteConvexity/convex_hull_trick.md) | 傾きの追加順に制約。query の x の順序は任意 | [コード](../../DiscreteConvexity/convex_hull_trick.cpp) |
| [Deque CHT](DiscreteConvexity/deque_cht.md) | 傾きの追加順とqueryの x の順序に制約 | [コード](../../DiscreteConvexity/deque_cht.cpp) |

<a id="category-7"></a>

## グラフ

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [動的連結性（重み付き・オフライン）](Graph/offline_dynamic_connectivity_weighted.md) | Undo可能なUnion-Find と成分の重さの集計 | [コード](../../Graph/offline_dynamic_connectivity_weighted.cpp) |
| [SCCと縮約DAG](Graph/scc_condensation.md) | 元のファイル名と異なり、本文はSCC結果と縮約DAGの処理 | [コード](../../Graph/scc_condensation.cpp) |
| [Functional Graph](Graph/functional_graph.md) | サイクル情報、到達可能性、距離 | [コード](../../Graph/functional_graph.cpp) |
| [オイラー路・閉路](Graph/euler_trail.md) | 有向／無向グラフ。全辺を使えない場合は空vector | [コード](../../Graph/euler_trail.cpp) |
| [Binary Lifting](Graph/binary_lifting.md) | 遷移のダブリングと結合則を満たす集計 | [コード](../../Graph/binary_lifting.cpp) |
| [動的連結性（オフライン）](Graph/offline_dynamic_connectivity.md) | 辺の追加・削除・連結性クエリ | [コード](../../Graph/offline_dynamic_connectivity.cpp) |
| [LowLink](Graph/lowlink.md) | 橋・関節点などを計算 | [コード](../../Graph/lowlink.cpp) |

<a id="category-8"></a>

## 木

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [Cartesian Tree](Tree/cartesian_tree.md) | 最小値を根に持つ木を構築 | [コード](../../Tree/cartesian_tree.cpp) |
| [LCA](Tree/lca.md) | 最小共通祖先・距離・祖先判定 | [コード](../../Tree/lca.cpp) |
| [Heavy-Light Decomposition](Tree/hld.md) | パスを区間分解。非可換演算向けの順序にも対応 | [コード](../../Tree/hld.cpp) |
| [全方位木DP](Tree/rerooting.md) | merge・lift・add_vertex を指定 | [コード](../../Tree/rerooting.cpp) |
| [木の直径・最遠距離](Tree/tree_diameter.md) | 重みなし・非負重み付きの実装を収録 | [コード](../../Tree/tree_diameter.cpp) |
| [木の重心](Tree/tree_centroid.md) | 重みなし・頂点重み付きの重心情報 | [コード](../../Tree/tree_centroid.cpp) |
| [Virtual Tree](Tree/virtual_tree.md) | DFS順・深さ・LCA関数を与えて構築 | [コード](../../Tree/virtual_tree.cpp) |
| [DSU on Tree](Tree/dsu_on_tree.md) | 部分木クエリ用。再帰を使用 | [コード](../../Tree/dsu_on_tree.cpp) |

<a id="category-9"></a>

## 整数論

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [約数列挙](MathNumberTheory/divisors.md) | 正の整数の約数列挙。負数について確認事項あり | [コード](../../MathNumberTheory/divisors.cpp) |
| [mod累乗・逆元](MathNumberTheory/mod_pow_inverse.md) | modinv はフェルマーの小定理を利用。素数mod・逆元の存在が前提 | [コード](../../MathNumberTheory/mod_pow_inverse.cpp) |
| [10^6 以下の素数表](MathNumberTheory/primes_upto_1000000.md) | 78,498 個の素数データ | [コード](../../MathNumberTheory/primes_upto_1000000.txt) |
| [約数・倍数ゼータ／メビウス変換](MathNumberTheory/divisor_multiple_transforms.md) | 添字 1..N を使用し、f[0] は未使用 | [コード](../../MathNumberTheory/divisor_multiple_transforms.cpp) |
| [Miller-Rabin / Pollard Rho](MathNumberTheory/miller_rabin_pollard_rho.md) | uint64_t の素数判定と素因数分解 | [コード](../../MathNumberTheory/miller_rabin_pollard_rho.cpp) |
| [拡張ユークリッド互除法](MathNumberTheory/extended_euclid.md) | 一次不定方程式・合同式・逆元 | [コード](../../MathNumberTheory/extended_euclid.cpp) |
| [線形篩](MathNumberTheory/linear_sieve.md) | 素数・最小素因数・phi・mu | [コード](../../MathNumberTheory/linear_sieve.cpp) |
| [整数論のまとめ](MathNumberTheory/number_theory_bundle.md) | LinearSieve・素数関連・約数／倍数変換など | [コード](../../MathNumberTheory/number_theory_bundle.cpp) |

<a id="category-10"></a>

## 組合せ

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [第二種スターリング数](MathCombinatorics/stirling_second.md) | ACL dynamic_modint を使用 | [コード](../../MathCombinatorics/stirling_second.cpp) |
| [二項係数の累積和](MathCombinatorics/binomial_prefix_sum.md) | nCr・mint・rep に依存。状態の変更範囲に制約あり | [コード](../../MathCombinatorics/binomial_prefix_sum.cpp) |

<a id="category-11"></a>

## 線形代数

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [XOR基底](LinearAlgebra/xor_basis.md) | 符号なし整数型を使用 | [コード](../../LinearAlgebra/xor_basis.cpp) |
| [ガウス消去法](LinearAlgebra/gaussian_elimination.md) | modint などの体上の連立一次方程式 | [コード](../../LinearAlgebra/gaussian_elimination.cpp) |
| [GF(2) のガウス消去法](LinearAlgebra/gf2_gaussian_elimination.md) | bitset の最終列に右辺を格納 | [コード](../../LinearAlgebra/gf2_gaussian_elimination.cpp) |

<a id="category-12"></a>

## 畳み込み

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [XOR畳み込み（FWHT）](FPSandPolynomial/xor_convolution.md) | mod版の逆変換は gcd(N,mod)=1 が必要 | [コード](../../FPSandPolynomial/xor_convolution.cpp) |

<a id="category-13"></a>

## 文字列

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [回文半径](String/palindrome_radius.md) | 文字を中心とする奇数長回文の半径。関数内に貼り付けるスニペット | [コード](../../String/palindrome_radius.cpp) |
| [Aho-Corasick](String/aho_corasick.md) | 複数文字列の照合。文字列追加後にbuild | [コード](../../String/aho_corasick.cpp) |
| [静的Rolling Hash](String/static_rolling_hash.md) | 区間は [l,r)。二重のランダム化ハッシュ | [コード](../../String/static_rolling_hash.cpp) |
| [動的Rolling Hash](String/dynamic_rolling_hash.md) | 列の変更に対応するハッシュ | [コード](../../String/dynamic_rolling_hash.cpp) |
| [二進文字列の順序管理](String/binary_string_order_set.md) | 本文のクラス名は StringWaveletMatrix | [コード](../../String/binary_string_order_set.cpp) |
| [Trie（英小文字）](String/trie_lowercase.md) | 本文の文字写像は a..z | [コード](../../String/trie_lowercase.cpp) |
| [Trie（英小文字・英大文字）](String/trie_letters.md) | 本文のクラス名は TrieAzAZ | [コード](../../String/trie_letters.cpp) |
| [Trie（数字）](String/trie_digits.md) | 文字 0..9 | [コード](../../String/trie_digits.cpp) |
| [Trie（英小文字・数字）](String/trie_lowercase_digits.md) | 本文の文字写像は a..z と 0..9 | [コード](../../String/trie_lowercase_digits.cpp) |

<a id="category-14"></a>

## 幾何

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [偏角ソート](Geometry/argument_sort.md) | 整数座標の点を偏角順に整列 | [コード](../../Geometry/argument_sort.cpp) |

<a id="category-15"></a>

## 探索・オフライン処理

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [Mo法](Search/mo.md) | オフライン区間クエリ。add/remove/output を指定 | [コード](../../Search/mo.cpp) |

<a id="category-16"></a>

## 穴埋め用スニペット

| 説明ページ | 内容 | 実装 |
| --- | --- | --- |
| [二分探索の穴埋めテンプレート](Snippets/binary_search.md) | ok/ng と check を記入。std::midpoint はC++20 | [コード](../../Snippets/binary_search.cpp) |
| [ACL segtree の穴埋めテンプレート](Snippets/segtree_custom.md) | S・op・e を定義して使用 | [コード](../../Snippets/segtree_custom.cpp) |
| [ACL lazy_segtree の穴埋めテンプレート](Snippets/lazy_segtree_custom.md) | S・F・各演算を定義。LazySegBase に依存 | [コード](../../Snippets/lazy_segtree_custom.cpp) |

各ページにリンクした公開版の実装に基づきます。確認できた不備は各ページの「この実装の注意点」に記載しています。
